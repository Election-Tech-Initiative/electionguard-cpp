#include "electionguard/election.hpp"

#include "electionguard/hash.hpp"
#include "log.hpp"

#include <cstring>

using std::string;

namespace electionguard
{
    SelectionDescription::SelectionDescription(const string &object_id, const string &candidate_id,
                                               uint64_t sequence_order)
    {
        data.object_id = object_id;

        auto _csize = candidate_id.size() + 1;
        candidate_id.copy(data.candidate_id, _csize, 0UL);
        data.sequence_order = sequence_order;
    }

    SelectionDescription::~SelectionDescription() { data = {}; }

    string SelectionDescription::getObjectId() const { return data.object_id; }
    char *SelectionDescription::getCandidateId() { return data.candidate_id; }
    uint64_t SelectionDescription::getSequenceOrder() const { return data.sequence_order; }

    unique_ptr<ElementModQ> SelectionDescription::crypto_hash() const
    {
        return hash_elems({data.object_id, data.sequence_order, string(data.candidate_id)});
    }

} // namespace electionguard
