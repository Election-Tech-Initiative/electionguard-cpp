#include "electionguard/group.hpp"

#include "../log.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/group.h"
}

#include <cstring>

using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::Log;
using electionguard::rand_q;

using std::string;

#pragma region ElementModP

eg_electionguard_status_t eg_element_mod_p_new(const uint64_t in_data[MAX_P_LEN],
                                               eg_element_mod_p_t **out_handle)
{
    try {
        uint64_t data[MAX_P_LEN] = {};
        memcpy(static_cast<uint64_t *>(data), in_data, MAX_P_SIZE);

        auto element = make_unique<ElementModP>(data);
        *out_handle = AS_TYPE(eg_element_mod_p_t, element.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_p_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_element_mod_p_new_unchecked(const uint64_t in_data[MAX_P_LEN],
                                                         eg_element_mod_p_t **out_handle)
{
    try {
        uint64_t data[MAX_P_LEN] = {};
        memcpy(static_cast<uint64_t *>(data), in_data, MAX_P_SIZE);

        auto element = make_unique<ElementModP>(data, true);
        *out_handle = AS_TYPE(eg_element_mod_p_t, element.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_p_new_unchecked", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

EG_API eg_electionguard_status_t eg_element_mod_p_free(eg_element_mod_p_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    Log::debug("enter2");
    delete AS_TYPE(ElementModP, handle);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_p_get_data(eg_element_mod_p_t *handle, uint64_t **out_data,
                                                    size_t out_size)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    auto *element = AS_TYPE(ElementModP, handle);
    *out_data = element->get();
    out_size = MAX_P_LEN;

    return ELECTIONGUARD_STATUS_SUCCESS;
}

#pragma endregion

#pragma region ElementModQ

eg_electionguard_status_t eg_element_mod_q_new(const uint64_t in_data[MAX_Q_LEN],
                                               eg_element_mod_q_t **out_handle)
{
    try {
        uint64_t data[MAX_Q_LEN] = {};
        memcpy(static_cast<uint64_t *>(data), in_data, MAX_Q_SIZE);

        auto element = make_unique<ElementModQ>(data);
        *out_handle = AS_TYPE(eg_element_mod_q_t, element.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_q_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_element_mod_q_new_unchecked(const uint64_t in_data[MAX_Q_LEN],
                                                         eg_element_mod_q_t **out_handle)
{
    try {
        uint64_t data[MAX_Q_LEN] = {};
        memcpy(static_cast<uint64_t *>(data), in_data, MAX_Q_SIZE);

        auto element = make_unique<ElementModQ>(data, true);
        *out_handle = AS_TYPE(eg_element_mod_q_t, element.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_q_new_unchecked", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

EG_API eg_electionguard_status_t eg_element_mod_q_free(eg_element_mod_q_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ElementModQ, handle);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_element_mod_q_get_data(eg_element_mod_q_t *handle, uint64_t **out_data,
                                                    size_t *out_size)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    auto *element = AS_TYPE(ElementModQ, handle);
    *out_data = element->get();
    *out_size = (size_t)MAX_Q_LEN;

    return ELECTIONGUARD_STATUS_SUCCESS;
}

#pragma endregion

#pragma region ElementModQ Functions

eg_electionguard_status_t eg_element_mod_q_rand_q_new(eg_element_mod_q_t **out_handle)
{
    try {
        auto random = rand_q();
        *out_handle = AS_TYPE(eg_element_mod_q_t, random.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_element_mod_q_rand_q_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion
