#include "electionguard/hash.hpp"

#include "../log.hpp"
#include "electionguard/group.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/hash.h"
}

using electionguard::ElementModQ;
using electionguard::hash_elems;
using electionguard::Log;
using std::vector;

eg_electionguard_status_t eg_hash_elems_string(const char *a, eg_element_mod_q_t **out_handle)
{
    try {
        auto hash = hash_elems(a);

        *out_handle = AS_TYPE(eg_element_mod_q_t, hash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_hash_elems_string", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_hash_elems_strings(const char **a, size_t in_length,
                                                eg_element_mod_q_t **out_handle)
{
    try {
        vector<string> v(a, a + in_length);
        auto hash = hash_elems(v);

        *out_handle = AS_TYPE(eg_element_mod_q_t, hash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_hash_elems_strings", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_hash_elems_int(const uint64_t a, eg_element_mod_q_t **out_handle)
{
    try {
        auto hash = hash_elems(a);

        *out_handle = AS_TYPE(eg_element_mod_q_t, hash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_hash_elems_int", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}
