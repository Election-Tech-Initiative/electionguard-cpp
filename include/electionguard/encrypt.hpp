#ifndef __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__

#include "electionguard/ballot.hpp"
#include "electionguard/election.hpp"
#include "electionguard/group.hpp"
#include "export.h"

#include <memory>

namespace electionguard
{
    class EG_API EncryptionDevice
    {
      public:
        EncryptionDevice(const EncryptionDevice &other);
        EncryptionDevice(const EncryptionDevice &&other);
        EncryptionDevice(const uint64_t uuid, const string &location);
        ~EncryptionDevice();

        EncryptionDevice &operator=(EncryptionDevice other);
        EncryptionDevice &operator=(EncryptionDevice &&other);

        unique_ptr<ElementModQ> getHash() const;

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
    };

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

        unique_ptr<CiphertextBallot> encrypt(const PlaintextBallot &ballot,
                                             bool shouldVerifyProofs = true) const;

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
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
    EG_API unique_ptr<CiphertextBallotSelection>
    encryptSelection(const PlaintextBallotSelection &plaintext,
                     const SelectionDescription &description, const ElementModP &elgamalPublicKey,
                     const ElementModQ &cryptoExtendedBaseHash, const ElementModQ &nonceSeed,
                     bool isPlaceholder = false, bool shouldVerifyProofs = true);

    EG_API unique_ptr<CiphertextBallotContest>
    encryptContest(const PlaintextBallotContest &plaintext, const ContestDescription &description,
                   const ElementModP &elgamalPublicKey, const ElementModQ &cryptoExtendedBaseHash,
                   const ElementModQ &nonceSeed, bool shouldVerifyProofs = true);

    EG_API unique_ptr<CiphertextBallot>
    encryptBallot(const PlaintextBallot &plaintext, const InternalElectionDescription &metadata,
                  const CiphertextElectionContext &context, const ElementModQ &seedHash,
                  unique_ptr<ElementModQ> nonce = nullptr, bool shouldVerifyProofs = true);

} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__ */