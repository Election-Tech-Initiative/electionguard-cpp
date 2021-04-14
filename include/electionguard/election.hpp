#ifndef __ELECTIONGUARD_CPP_ELECTION_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_ELECTION_HPP_INCLUDED__

#include "crypto_hashable.hpp"
#include "election_object_base.hpp"
#include "export.h"
#include "group.hpp"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace electionguard
{
    /// <summary>
    /// `CiphertextElectionContext` is the ElectionGuard representation of a specific election
    /// Note: The ElectionGuard Data Spec deviates from the NIST model in that
    /// this object includes fields that are populated in the course of encrypting an election
    /// Specifically, `crypto_base_hash`, `crypto_extended_base_hash` and `elgamal_public_key`
    /// are populated with election-specific information necessary for encrypting the election.
    /// Refer to the [Electionguard Specification](https://github.com/microsoft/electionguard) for more information.
    ///
    /// To make an instance of this class, don't construct it directly. Use
    /// `make_ciphertext_election_context` instead.
    /// </summary>
    class EG_API CiphertextElectionContext
    {
      public:
        CiphertextElectionContext(const CiphertextElectionContext &other);
        CiphertextElectionContext(const CiphertextElectionContext &&other);
        explicit CiphertextElectionContext(const uint64_t numberOfGuardians, const uint64_t quorum,
                                           std::unique_ptr<ElementModP> elGamalPublicKey,
                                           std::unique_ptr<ElementModQ> commitmentHash,
                                           std::unique_ptr<ElementModQ> manifestHash,
                                           std::unique_ptr<ElementModQ> cryptoBaseHash,
                                           std::unique_ptr<ElementModQ> cryptoExtendedBaseHash);
        ~CiphertextElectionContext();

        CiphertextElectionContext &operator=(CiphertextElectionContext other);
        CiphertextElectionContext &operator=(CiphertextElectionContext &&other);

        /// <summary>
        /// The number of guardians necessary to generate the public key
        /// </summary>
        uint64_t getNumberOfGuardians() const;

        /// <summary>
        /// The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians`
        /// </summary>
        uint64_t getQuorum() const;

        /// <summary>
        /// the `joint public key (K)` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        const ElementModP *getElGamalPublicKey() const;

        /// <summary>
        /// the `commitment hash H(K 1,0 , K 2,0 ... , K n,0 )` of the public commitments
        /// guardians make to each other in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        const ElementModQ *getCommitmentHash() const;

        /// <summary>
        /// The hash of the election metadata
        /// </summary>
        const ElementModQ *getManifestHash() const;

        /// <summary>
        /// the `base hash code (𝑄)` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        const ElementModQ *getCryptoBaseHash() const;

        /// <summary>
        /// the `extended base hash code (𝑄')` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        const ElementModQ *getCryptoExtendedBaseHash() const;

        /// <summary>
        ///  Makes a CiphertextElectionContext object.
        ///
        /// <param name="number_of_guardians"> The number of guardians necessary to generate the public key </param>
        /// <param name="quorum"> The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians` </param>
        /// <param name="elGamalPublicKey"> the public key of the election </param>
        /// <param name="commitmentHash"> the hash of the commitments the guardians make to each other </param>
        /// <param name="manifestHash"> the hash of the election metadata </param>
        /// </summary>
        static std::unique_ptr<CiphertextElectionContext>
        make(const uint64_t numberOfGuardians, const uint64_t quorum,
             std::unique_ptr<ElementModP> elGamalPublicKey,
             std::unique_ptr<ElementModQ> commitmentHash,
             std::unique_ptr<ElementModQ> manifestHash);

        /// <summary>
        ///  Makes a CiphertextElectionContext object from hex string representations.
        ///
        /// <param name="number_of_guardians"> The number of guardians necessary to generate the public key </param>
        /// <param name="quorum"> The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians` </param>
        /// <param name="elGamalPublicKeyInHex"> the public key of the election </param>
        /// <param name="commitmentHashInHex"> the hash of the commitments the guardians make to each other </param>
        /// <param name="manifestHashInHex"> the hash of the election metadata </param>
        /// </summary>
        static std::unique_ptr<CiphertextElectionContext>
        make(const uint64_t numberOfGuardians, const uint64_t quorum,
             const std::string &elGamalPublicKeyInHex, const std::string &commitmentHashInHex,
             const std::string &manifestHashInHex);

        std::vector<uint8_t> toBson() const;
        std::string toJson() const;
        static std::unique_ptr<CiphertextElectionContext> fromJson(std::string data);
        static std::unique_ptr<CiphertextElectionContext> fromBson(std::vector<uint8_t> data);

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_ELECTION_HPP_INCLUDED__ */
