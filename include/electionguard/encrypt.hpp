#ifndef __ELECTIONGUARD_CPP_ENCRYPT_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_ENCRYPT_HPP_INCLUDED__

#include "electionguard/ballot.hpp"
#include "electionguard/election.hpp"
#include "electionguard/group.hpp"
#include "export.h"

#include <memory>

namespace electionguard
{
    /// <summary>
    /// Metadata for encryption device
    ///
    /// The encryption device is a stateful container that represents abstract hardware
    /// authorized to participate in a specific election.
    ///
    /// <param name="deviceUuid">a unique identifier tied to the device hardware</param>
    /// <param name="sessionUuid">a unique identifier tied to the runtime session</param>
    /// <param name="launchCode">a unique identifer tied to the election</param>
    /// <param name="location">an arbitrary string meaningful to the external system
    ///                        such as a friendly name, description, or some other value</param>
    /// </summary>
    class EG_API EncryptionDevice
    {
      public:
        EncryptionDevice(const EncryptionDevice &other);
        EncryptionDevice(const EncryptionDevice &&other);
        EncryptionDevice(const uint64_t deviceUuid, const uint64_t sessionUuid,
                         const uint64_t launchCode, const std::string &location);
        ~EncryptionDevice();

        EncryptionDevice &operator=(EncryptionDevice other);
        EncryptionDevice &operator=(EncryptionDevice &&other);

        std::unique_ptr<ElementModQ> getHash() const;
        uint64_t getTimestamp() const;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// An object for caching election and encryption state.
    ///
    /// the encryption mediator composes ballots by querying the encryption device
    /// for a hash of its metadata and incremental timestamps/
    ///
    /// this is a convenience wrapper around the encrypt methods
    /// and may not be suitable for all use cases.
    /// </summary>
    class EG_API EncryptionMediator
    {
      public:
        EncryptionMediator(const EncryptionMediator &other);
        EncryptionMediator(const EncryptionMediator &&other);
        EncryptionMediator(const InternalElectionDescription &metadata,
                           const CiphertextElectionContext &context,
                           const EncryptionDevice &encryptionDevice);
        ~EncryptionMediator();

        EncryptionMediator &operator=(EncryptionMediator other);
        EncryptionMediator &operator=(EncryptionMediator &&other);

        std::unique_ptr<CiphertextBallot> encrypt(const PlaintextBallot &ballot,
                                                  bool shouldVerifyProofs = true) const;

        std::unique_ptr<CompactCiphertextBallot>
        compactEncrypt(const PlaintextBallot &ballot, bool shouldVerifyProofs = true) const;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// Encrypt a specific `BallotSelection` in the context of a specific `BallotContest`
    ///
    /// <param name="plaintext">the selection in the valid input form</param>
    /// <param name="description">the `SelectionDescription` from the `ContestDescription`
    ///                           which defines this selection's structure</param>
    /// <param name="elgamalPublicKey">the public key (K) used to encrypt the ballot</param>
    /// <param name="cryptoExtendedBaseHash">the extended base hash of the election</param>
    /// <param name="nonceSeed">an `ElementModQ` used as a header to seed the `Nonce` generated
    ///                          for this selection. this value can be (or derived from) the
    ///                          BallotContest nonce, but no relationship is required</param>
    /// <param name="isPlaceholder">specifies if this is a placeholder selection</param>
    /// <param name="shouldVerifyProofs">specify if the proofs should be verified prior to returning (default True)</param>
    /// <returns>A `CiphertextBallotSelection`</returns>
    /// </summary>
    EG_API std::unique_ptr<CiphertextBallotSelection>
    encryptSelection(const PlaintextBallotSelection &selection,
                     const SelectionDescription &description, const ElementModP &elgamalPublicKey,
                     const ElementModQ &cryptoExtendedBaseHash, const ElementModQ &nonceSeed,
                     bool isPlaceholder = false, bool shouldVerifyProofs = true);

    /// <summary>
    /// Encrypt a specific `BallotContest` in the context of a specific `Ballot`
    ///
    /// This method accepts a contest representation that only includes `True` selections.
    /// It will fill missing selections for a contest with `False` values, and generate `placeholder`
    /// selections to represent the number of seats available for a given contest.  By adding `placeholder`
    /// votes
    /// <param name="plaintext">the selection in the valid input form</param>
    /// <param name="description">the `ContestDescriptionWithPlaceholders` from the `ContestDescription`
    ///                           which defines this contest's structure</param>
    /// <param name="elgamalPublicKey">the public key (K) used to encrypt the ballot</param>
    /// <param name="cryptoExtendedBaseHash">the extended base hash of the election</param>
    /// <param name="nonceSeed">an `ElementModQ` used as a header to seed the `Nonce` generated
    ///                          for this contest. this value can be (or derived from) the
    ///                          BallotContest nonce, but no relationship is required</param>
    /// <param name="shouldVerifyProofs">specify if the proofs should be verified prior to returning (default True)</param>
    /// <returns>A `CiphertextBallotContest`</returns>
    /// </summary>
    EG_API std::unique_ptr<CiphertextBallotContest>
    encryptContest(const PlaintextBallotContest &contest,
                   const ContestDescriptionWithPlaceholders &description,
                   const ElementModP &elgamalPublicKey, const ElementModQ &cryptoExtendedBaseHash,
                   const ElementModQ &nonceSeed, bool shouldVerifyProofs = true);

    /// <summary>
    /// Encrypt a specific `Ballot` in the context of a specific `CiphertextElectionContext`
    ///
    /// This method accepts a ballot representation that only includes `True` selections.
    /// It will fill missing selections for a contest with `False` values, and generate `placeholder`
    /// selections to represent the number of seats available for a given contest.  By adding `placeholder`
    /// votes
    ///
    /// This method also allows for ballots to exclude passing contests for which the voter made no selections.
    /// It will fill missing contests with `False` selections and generate `placeholder` selections that are marked `True`.
    ///
    /// <param name="plaintext">the selection in the valid input form</param>
    /// <param name="metadata">the `InternalElectionDescription` which defines this ballot's structure</param>
    /// <param name="context">all the cryptographic context for the election</param>
    /// <param name="seedHash">Hash from previous ballot or starting hash from device</param>
    /// <param name="nonce">an optional `int` used to seed the `Nonce` generated for this contest
    ///                     if this value is not provided, the secret generating mechanism of the OS provides its own</param>
    /// <param name="shouldVerifyProofs">specify if the proofs should be verified prior to returning (default True)</param>
    /// <returns>A `CiphertextBallot`</returns>
    /// </summary>
    EG_API std::unique_ptr<CiphertextBallot>
    encryptBallot(const PlaintextBallot &ballot, const InternalElectionDescription &metadata,
                  const CiphertextElectionContext &context, const ElementModQ &seedHash,
                  std::unique_ptr<ElementModQ> nonce = nullptr, uint64_t timestamp = 0,
                  bool shouldVerifyProofs = true);

    /// <summary>
    /// Encrypt a specific `Ballot` in the context of a specific `CiphertextElectionContext`
    ///
    /// This method accepts a ballot representation that only includes `True` selections.
    /// It will fill missing selections for a contest with `False` values, and generate `placeholder`
    /// selections to represent the number of seats available for a given contest.  By adding `placeholder`
    /// votes
    ///
    /// This method also allows for ballots to exclude passing contests for which the voter made no selections.
    /// It will fill missing contests with `False` selections and generate `placeholder` selections that are marked `True`.
    ///
    /// This version of the encrypt method returns a `compact` version of the ballot that includes a minimal representation
    /// of the plaintext ballot along with the crypto parameters that are required to expand the ballot
    ///
    /// <param name="plaintext">the selection in the valid input form</param>
    /// <param name="metadata">the `InternalElectionDescription` which defines this ballot's structure</param>
    /// <param name="context">all the cryptographic context for the election</param>
    /// <param name="seedHash">Hash from previous ballot or starting hash from device</param>
    /// <param name="nonce">an optional `int` used to seed the `Nonce` generated for this contest
    ///                     if this value is not provided, the secret generating mechanism of the OS provides its own</param>
    /// <param name="shouldVerifyProofs">specify if the proofs should be verified prior to returning (default True)</param>
    /// <returns>A `CiphertextBallot`</returns>
    /// </summary>
    EG_API std::unique_ptr<CompactCiphertextBallot>
    encryptCompactBallot(const PlaintextBallot &ballot, const InternalElectionDescription &metadata,
                         const CiphertextElectionContext &context, const ElementModQ &seedHash,
                         std::unique_ptr<ElementModQ> nonce = nullptr, uint64_t timestamp = 0,
                         bool shouldVerifyProofs = true);

    std::unique_ptr<PlaintextBallot>
    expandCompactPlaintextBallot(const CompactPlaintextBallot &compactBallot,
                                 const InternalElectionDescription &description);

    std::unique_ptr<CiphertextBallot>
    expandCompactCiphertextBallot(const CompactCiphertextBallot &compactCiphertext,
                                  const InternalElectionDescription &metadata,
                                  const CiphertextElectionContext &context);

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_ENCRYPT_HPP_INCLUDED__ */
