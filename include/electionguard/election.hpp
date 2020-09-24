#ifndef __ELECTIONGUARD_CORE_ELECTION_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_ELECTION_HPP_INCLUDED__

#include "crypto_hashable.hpp"
#include "election_object_base.hpp"
#include "export.h"
#include "group.hpp"

#include <string>

namespace electionguard
{
    struct SelectionDescriptionData : public ElectionObjectBase {
        char candidate_id[MAX_IDENIFIER_LENGTH];
        uint64_t sequence_order;
    };

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
        SelectionDescription(const string &object_id, const string &candidate_id,
                             uint64_t sequence_order);
        ~SelectionDescription();

        // TODO: to hex?
        int toInt();

        string getObjectId() const;
        char *getCandidateId();
        uint64_t getSequenceOrder() const;

        virtual unique_ptr<ElementModQ> crypto_hash() const;

      private:
        SelectionDescriptionData data;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_ELECTION_HPP_INCLUDED__ */