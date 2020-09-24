#include "electionguard/tracker.hpp"

#include "memory_cache.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/tracker.h"
}

using electionguard::Cache;
using electionguard::ElementModQ;
using electionguard::Tracker;

// TODO: safe initialization
static Cache<ElementModQ> cache_element_mod_q;

EG_API eg_element_mod_q_t *eg_get_hash_for_device(uint64_t uuid, const char *location)
{
    auto hash = Tracker::getHashForDevice(uuid, string(location));
    auto *reference = cache_element_mod_q.retain(move(hash));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

EG_API eg_element_mod_q_t *eg_get_rotating_tracker_hash(eg_element_mod_q_t *previous,
                                                        uint64_t timestamp,
                                                        eg_element_mod_q_t *ballot_hash)
{
    if (previous == nullptr || ballot_hash == nullptr) {
        return nullptr;
    }

    auto *previousHash = AS_TYPE(ElementModQ, previous);
    auto *ballotHash = AS_TYPE(ElementModQ, ballot_hash);

    auto hash = Tracker::getRotatingTrackerHash(*previousHash, timestamp, *ballotHash);
    auto *reference = cache_element_mod_q.retain(move(hash));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

EG_API const char *eg_hash_to_words_with_default_separator(eg_element_mod_q_t *tracker_hash)
{
    return eg_hash_to_words(tracker_hash, " ");
}

EG_API const char *eg_hash_to_words(eg_element_mod_q_t *tracker_hash, const char *separator)
{

    auto *trackerHash = AS_TYPE(ElementModQ, tracker_hash);
    auto words = Tracker::hashToWords(*trackerHash, separator);
    const auto *words_ = words.c_str();
    return words_;
}