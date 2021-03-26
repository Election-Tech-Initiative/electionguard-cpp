#include "electionguard/ballot_compact.hpp"

#include "../log.hpp"
#include "convert.hpp"
#include "electionguard/group.hpp"
#include "variant_cast.hpp"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>

extern "C" {
#include "electionguard/ballot_compact.h"
}

using electionguard::CompactCiphertextBallot;
using electionguard::CompactPlaintextBallot;
using electionguard::dynamicCopy;
using electionguard::Log;

#pragma region CompactPlaintextBallot

eg_electionguard_status_t eg_compact_plaintext_ballot_free(eg_compact_plaintext_ballot_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(CompactPlaintextBallot, handle);
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_compact_plaintext_ballot_from_msgpack(uint8_t *in_data, uint64_t in_length,
                                         eg_compact_plaintext_ballot_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = CompactPlaintextBallot::fromMsgPack(data_bytes);

        *out_handle = AS_TYPE(eg_compact_plaintext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_compact_plaintext_ballot_from_msgpack", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_compact_plaintext_ballot_to_msgpack(eg_compact_plaintext_ballot_t *handle, uint8_t **out_data,
                                       size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CompactPlaintextBallot, handle);
        auto data_bytes = domain_type->toMsgPack();

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_compact_plaintext_ballot_to_msgpack", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_compact_plaintext_ballot_msgpack_free(uint8_t *in_data)
{
    if (in_data == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete[] in_data;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

#pragma endregion

#pragma region CompactCiphertextBallot

eg_electionguard_status_t eg_compact_ciphertext_ballot_free(eg_compact_ciphertext_ballot_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(CompactCiphertextBallot, handle);
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_compact_ciphertext_ballot_get_object_id(eg_compact_ciphertext_ballot_t *handle,
                                           char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(CompactCiphertextBallot, handle)->getObjectId();
        *out_object_id = dynamicCopy(objectId);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_compact_ciphertext_ballot_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_compact_ciphertext_ballot_from_msgpack(uint8_t *in_data, uint64_t in_length,
                                          eg_compact_ciphertext_ballot_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = CompactCiphertextBallot::fromMsgPack(data_bytes);

        *out_handle = AS_TYPE(eg_compact_ciphertext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_compact_ciphertext_ballot_from_msgpack", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_compact_ciphertext_ballot_to_msgpack(eg_compact_ciphertext_ballot_t *handle, uint8_t **out_data,
                                        size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CompactCiphertextBallot, handle);
        auto data_bytes = domain_type->toMsgPack();

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_compact_ciphertext_ballot_to_msgpack", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_compact_ciphertext_ballot_msgpack_free(uint8_t *in_data)
{
    if (in_data == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete[] in_data;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

#pragma endregion

#pragma region Compress Functions

// TODO: implement

#pragma endregion

#pragma region Expand Functions

// TODO: implement

#pragma endregion
