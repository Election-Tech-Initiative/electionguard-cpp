#include "electionguard/tracker.hpp"

#include "../log.hpp"
#include "convert.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/tracker.h"
}

#include <cstring>

using electionguard::dynamicCopy;
using electionguard::ElementModQ;
using electionguard::Log;
using electionguard::Tracker;

eg_electionguard_status_t eg_get_hash_for_device(uint64_t in_uuid, const char *in_location,
                                                 eg_element_mod_q_t **out_device_hash)
{
    try {
        auto hash = Tracker::getHashForDevice(in_uuid, string(in_location));
        *out_device_hash = AS_TYPE(eg_element_mod_q_t, hash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_get_hash_for_device", e);
        return ELECTIONGUARD_STATUS_ERROR_RUNTIME_ERROR;
    }
}

eg_electionguard_status_t eg_get_rotating_tracker_hash(eg_element_mod_q_t *in_previous,
                                                       uint64_t in_timestamp,
                                                       eg_element_mod_q_t *in_ballot_hash,
                                                       eg_element_mod_q_t **out_tracker_hash)
{
    if (in_previous == nullptr || in_ballot_hash == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    try {
        auto *previousHash = AS_TYPE(ElementModQ, in_previous);
        auto *ballotHash = AS_TYPE(ElementModQ, in_ballot_hash);

        auto hash = Tracker::getRotatingTrackerHash(*previousHash, in_timestamp, *ballotHash);
        *out_tracker_hash = AS_TYPE(eg_element_mod_q_t, hash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_get_rotating_tracker_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_RUNTIME_ERROR;
    }
}

eg_electionguard_status_t
eg_hash_to_words_with_default_separator(eg_element_mod_q_t *in_tracker_hash, char **out_hash_words)
{
    return eg_hash_to_words(in_tracker_hash, " ", out_hash_words);
}

eg_electionguard_status_t eg_hash_to_words(eg_element_mod_q_t *in_tracker_hash,
                                           const char *in_separator, char **out_hash_words)
{
    try {
        auto *trackerHash = AS_TYPE(ElementModQ, in_tracker_hash);
        auto words = Tracker::hashToWords(*trackerHash, in_separator);
        *out_hash_words = dynamicCopy(words);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_element_mod_q_to_hex", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}
