#include "electionguard/ballot_code.hpp"

#include "../log.hpp"
#include "convert.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/ballot_code.h"
}

#include <cstring>

using electionguard::BallotCode;
using electionguard::dynamicCopy;
using electionguard::ElementModQ;
using electionguard::Log;

eg_electionguard_status_t eg_ballot_code_get_hash_for_device(uint64_t in_device_uuid,
                                                             uint64_t in_session_uuid,
                                                             uint64_t in_launch_code,
                                                             const char *in_location,
                                                             eg_element_mod_q_t **out_device_hash)
{
    try {
        auto hash = BallotCode::getHashForDevice(in_device_uuid, in_session_uuid, in_launch_code,
                                                 string(in_location));
        *out_device_hash = AS_TYPE(eg_element_mod_q_t, hash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ballot_code_get_hash_for_device", e);
        return ELECTIONGUARD_STATUS_ERROR_RUNTIME_ERROR;
    }
}

eg_electionguard_status_t
eg_ballot_code_get_rotating_ballot_code(eg_element_mod_q_t *in_previous, uint64_t in_timestamp,
                                        eg_element_mod_q_t *in_ballot_hash,
                                        eg_element_mod_q_t **out_ballot_code)
{
    if (in_previous == nullptr || in_ballot_hash == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    try {
        auto *previousHash = AS_TYPE(ElementModQ, in_previous);
        auto *ballotHash = AS_TYPE(ElementModQ, in_ballot_hash);

        auto hash = BallotCode::getRotatingBallotCode(*previousHash, in_timestamp, *ballotHash);
        *out_ballot_code = AS_TYPE(eg_element_mod_q_t, hash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ballot_code_get_rotating_ballot_code", e);
        return ELECTIONGUARD_STATUS_ERROR_RUNTIME_ERROR;
    }
}
