#include "electionguard/election.hpp"

#include "hash.hpp"
#include "log.hpp"

#include <cstdlib>
using std::string;

namespace electionguard
{
    SelectionDescription::SelectionDescription(const std::string object_id,
                                               std::string candidate_id, uint64_t sequence_order)
        : data()
    {
        auto _osize = object_id.size() + 1;
        object_id.copy(data.object_id, _osize, 0UL);

        auto _csize = candidate_id.size() + 1;
        candidate_id.copy(data.candidate_id, _csize, 0UL);

        data.sequence_order = sequence_order;
    }

    SelectionDescription::SelectionDescription(const char *object_id, char *candidate_id,
                                               uint64_t sequence_order)
        : data()
    {
        size_t _osize = strlen(object_id) + 1;
        strlcpy(data.object_id, object_id, _osize);

        size_t _csize = strlen(candidate_id) + 1;
        strlcpy(data.candidate_id, candidate_id, _csize);

        data.sequence_order = sequence_order;
    }

    SelectionDescription::~SelectionDescription() { data = {}; }

    char *SelectionDescription::getObjectId() { return data.object_id; }
    char *SelectionDescription::getCandidateId() { return data.candidate_id; }
    uint64_t SelectionDescription::getSequenceOrder() { return data.sequence_order; }

    ElementModQ *SelectionDescription::crypto_hash()
    {
        return hash_elems({string(data.object_id), data.sequence_order, string(data.candidate_id)});
    }

} // namespace electionguard