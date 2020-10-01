#ifndef __ELECTIONGUARD_CORE_ELECTION_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_ELECTION_HPP_INCLUDED__

#include "crypto_hashable.hpp"
#include "election_object_base.hpp"
#include "export.h"
#include "group.hpp"

#include <memory>
#include <string>

namespace electionguard
{

    /// <summary>
    /// Data entity for the ballot selections in a contest,
    /// for example linking candidates and parties to their vote counts.
    /// See: https://developers.google.com/elections-data/reference/ballot-selection
    /// Note: The ElectionGuard Data Spec deviates from the NIST model in that
    /// there is no difference for different types of selections.
    /// The ElectionGuard Data Spec deviates from the NIST model in that
    /// `sequence_order` is a required field since it is used for ordering selections
    /// in a contest to ensure various encryption primitives are deterministic.
    /// For a given election, the sequence of selections displayed to a user may be different
    /// however that information is not captured by default when encrypting a specific ballot.
    /// </summary>
    class EG_API SelectionDescription : public CryptoHashable
    {
      public:
        SelectionDescription(const SelectionDescription &other);
        SelectionDescription(const SelectionDescription &&other);
        SelectionDescription(const string &objectId, const string &candidateId,
                             const uint64_t sequenceOrder);
        ~SelectionDescription();

        SelectionDescription &operator=(SelectionDescription other);
        SelectionDescription &operator=(SelectionDescription &&other);

        string getObjectId() const;
        string getCandidateId() const;
        uint64_t getSequenceOrder() const;

        virtual unique_ptr<ElementModQ> crypto_hash() override;
        virtual unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
    };

    // Note currently does not support placeholders
    class EG_API ContestDescription : public CryptoHashable
    {
      public:
        ContestDescription(const ContestDescription &other);
        ContestDescription(const ContestDescription &&other);
        ContestDescription(const string &objectId, const string &electoralDistrictId,
                           const uint64_t sequenceOrder, const string &voteVariation,
                           const uint64_t numberElected, const uint64_t votesAllowed,
                           const string &name, const string &ballotTitle,
                           const string &ballotSubtitle,
                           vector<unique_ptr<SelectionDescription>> selections);
        ~ContestDescription();

        ContestDescription &operator=(ContestDescription other);
        ContestDescription &operator=(ContestDescription &&other);

        string getObjectId() const;
        string getElectoralDistrictId() const;
        uint64_t getSequenceOrder() const;
        string getVoteVariation() const; // TODO: domain type enum
        uint64_t getNumberElected() const;
        uint64_t getVotesAllowed() const;
        string getName() const;

        string getBallotTitle() const; // TODO: domain type
        string getBallotSubtitle() const;

        vector<reference_wrapper<SelectionDescription>> getSelections() const;

        virtual unique_ptr<ElementModQ> crypto_hash() override;
        virtual unique_ptr<ElementModQ> crypto_hash() const override;

        // TODO: isValid() const;

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
    };

    // TODO: complete implementation
    class EG_API InternalElectionDescription
    {
      public:
        InternalElectionDescription(const InternalElectionDescription &other);
        InternalElectionDescription(const InternalElectionDescription &&other);
        InternalElectionDescription(const ElementModQ &descriptionHash,
                                    vector<unique_ptr<ContestDescription>> contests);
        ~InternalElectionDescription();

        InternalElectionDescription &operator=(InternalElectionDescription other);
        InternalElectionDescription &operator=(InternalElectionDescription &&other);

        // TODO: If the Election Manifest is used to populate, calculate this value
        // temporarily, it may be null.
        // Use CiphertextElectionContext.getDescriptionHash()
        const ElementModQ &getDescriptionHash() const;
        vector<reference_wrapper<ContestDescription>> getContests() const;

        vector<uint8_t> toBson() const;
        string toJson();
        string toJson() const;
        // can accept either the InternalElectionDescription or the Election Manifest
        static unique_ptr<InternalElectionDescription> fromJson(string data);
        static unique_ptr<InternalElectionDescription> fromBson(vector<uint8_t> data);

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
    };

    class EG_API CiphertextElectionContext
    {
      public:
        CiphertextElectionContext(const CiphertextElectionContext &other);
        CiphertextElectionContext(const CiphertextElectionContext &&other);
        CiphertextElectionContext(const uint64_t numberOfGuardians, const uint64_t quorum,
                                  unique_ptr<ElementModP> elGamalPublicKey,
                                  unique_ptr<ElementModQ> descriptionHash,
                                  unique_ptr<ElementModQ> cryptoBaseHash,
                                  unique_ptr<ElementModQ> cryptoExtendedBaseHash);
        ~CiphertextElectionContext();

        CiphertextElectionContext &operator=(CiphertextElectionContext other);
        CiphertextElectionContext &operator=(CiphertextElectionContext &&other);

        uint64_t getNumberOfGuardians() const;
        uint64_t getQuorum() const;
        const ElementModP *getElGamalPublicKey() const;
        const ElementModQ *getDescriptionHash() const;
        const ElementModQ *getCryptoBaseHash() const;
        const ElementModQ *getCryptoExtendedBaseHash() const;

        static unique_ptr<CiphertextElectionContext> make(const uint64_t numberOfGuardians,
                                                          const uint64_t quorum,
                                                          unique_ptr<ElementModP> elGamalPublicKey,
                                                          unique_ptr<ElementModQ> descriptionHash);

        static unique_ptr<CiphertextElectionContext> make(const uint64_t numberOfGuardians,
                                                          const uint64_t quorum,
                                                          const string &elGamalPublicKeyInHex,
                                                          const string &descriptionHashInHex);

        vector<uint8_t> toBson() const;
        string toJson() const;
        static unique_ptr<CiphertextElectionContext> fromJson(string data);
        static unique_ptr<CiphertextElectionContext> fromBson(vector<uint8_t> data);

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_ELECTION_HPP_INCLUDED__ */