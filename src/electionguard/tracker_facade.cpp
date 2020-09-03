#include "electionguard/tracker.hpp"

extern "C" {
#include "electionguard/tracker.h"
}

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)

using namespace electionguard;

EG_API eg_element_mod_q_t *eg_get_hash_for_device(uint64_t uuid, const char *location)
{
    auto deviceHash = Tracker::getHashForDevice(uuid, string(location));
    return AS_TYPE(eg_element_mod_q_t, deviceHash);
}

EG_API eg_element_mod_q_t *eg_get_rotating_tracker_hash(eg_element_mod_q_t *previous,
                                                        uint64_t timestamp,
                                                        eg_element_mod_q_t *ballot_hash)
{
    if (previous == nullptr) {
        return nullptr;
    }
    // TODO: safety
    auto previousHash = AS_TYPE(ElementModQ, previous);
    auto ballotHash = AS_TYPE(ElementModQ, ballot_hash);

    auto rotating_hash = Tracker::getRotatingTrackerHash(previousHash, timestamp, ballotHash);
    return AS_TYPE(eg_element_mod_q_t, rotating_hash);
}

EG_API const char *eg_hash_to_words_with_default_separator(eg_element_mod_q_t *tracker_hash)
{
    return eg_hash_to_words(tracker_hash, " ");
}

EG_API const char *eg_hash_to_words(eg_element_mod_q_t *tracker_hash, const char *separator)
{

    auto *trackerHash = AS_TYPE(ElementModQ, tracker_hash);
    auto words = Tracker::hashToWords(trackerHash, separator);
    return words.c_str();
}