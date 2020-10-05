#include "electionguard/ballot.hpp"

#include "../log.hpp"
#include "electionguard/group.hpp"
#include "variant_cast.hpp"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>

extern "C" {
#include "electionguard/ballot.h"
}

using electionguard::CiphertextBallot;
using electionguard::CiphertextBallotSelection;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::Log;
using electionguard::PlaintextBallot;
using electionguard::PlaintextBallotSelection;

#pragma region PlaintextBallotSelection

eg_electionguard_status_t
eg_plaintext_ballot_selection_new(const char *in_object_id, const char *in_vote,
                                  eg_plaintext_ballot_selection_t **out_handle)
{
    try {
        auto selection = make_unique<PlaintextBallotSelection>(in_object_id, in_vote);
        *out_handle = AS_TYPE(eg_plaintext_ballot_selection_t, selection.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_plaintext_ballot_selection_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_plaintext_ballot_selection_free(eg_plaintext_ballot_selection_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(PlaintextBallotSelection, handle);
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_plaintext_ballot_selection_get_object_id(eg_plaintext_ballot_selection_t *handle,
                                            const char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(PlaintextBallotSelection, handle)->getObjectId();
        auto data_size = objectId.length() + 1;
        auto *data_array = (char *)malloc(data_size);
        strncpy(data_array, objectId.c_str(), data_size);
        *out_object_id = data_array;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_plaintext_ballot_selection_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region CiphertextBallotSelection

eg_electionguard_status_t
eg_ciphertext_ballot_selection_free(eg_ciphertext_ballot_selection_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(CiphertextBallotSelection, handle);
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_get_object_id(eg_ciphertext_ballot_selection_t *handle,
                                             const char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(CiphertextBallotSelection, handle)->getObjectId();
        auto data_size = objectId.length() + 1;
        auto *data_array = (char *)malloc(data_size);
        strncpy(data_array, objectId.c_str(), data_size);
        *out_object_id = data_array;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_ciphertext_ballot_selection_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_get_description_hash(eg_ciphertext_ballot_selection_t *handle,
                                                    eg_element_mod_q_t **out_hash)
{
    auto *descriptionHash = AS_TYPE(CiphertextBallotSelection, handle)->getDescriptionHash();
    *out_hash = AS_TYPE(eg_element_mod_q_t, descriptionHash);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_get_ciphertext(eg_ciphertext_ballot_selection_t *handle,
                                              eg_elgamal_ciphertext_t **out_ciphertext)
{
    auto *ciphertext = AS_TYPE(CiphertextBallotSelection, handle)->getCiphertext();
    *out_ciphertext = AS_TYPE(eg_elgamal_ciphertext_t, ciphertext);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_get_crypto_hash(eg_ciphertext_ballot_selection_t *handle,
                                               eg_element_mod_q_t **out_hash)
{
    auto *hash = AS_TYPE(CiphertextBallotSelection, handle)->getCryptoHash();
    *out_hash = AS_TYPE(eg_element_mod_q_t, hash);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_ballot_selection_get_proof(eg_ciphertext_ballot_selection_t *handle,
                                         eg_disjunctive_chaum_pedersen_proof_t **out_proof)
{
    auto *proof = AS_TYPE(CiphertextBallotSelection, handle)->getProof();
    *out_proof = AS_TYPE(eg_disjunctive_chaum_pedersen_proof_t, proof);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

bool eg_ciphertext_ballot_selection_is_valid_encryption(
  eg_ciphertext_ballot_selection_t *handle, eg_element_mod_q_t *in_seed_hash,
  eg_element_mod_p_t *in_public_key, eg_element_mod_q_t *in_crypto_extended_base_hash)
{
    auto *seedHash = AS_TYPE(ElementModQ, in_seed_hash);
    auto *publicKey = AS_TYPE(ElementModP, in_public_key);
    auto *cryptoExtendedBaseHash = AS_TYPE(ElementModQ, in_crypto_extended_base_hash);
    return AS_TYPE(CiphertextBallotSelection, handle)
      ->isValidEncryption(*seedHash, *publicKey, *cryptoExtendedBaseHash);
}

#pragma endregion

#pragma region PlaintextBallotContest

#pragma endregion

#pragma region CiphertextBallotContest

#pragma endregion

#pragma region PlaintextBallot

eg_electionguard_status_t eg_plaintext_ballot_free(eg_plaintext_ballot_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(PlaintextBallot, handle);
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_plaintext_ballot_get_object_id(eg_plaintext_ballot_t *handle,
                                                            const char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(PlaintextBallot, handle)->getObjectId();
        auto data_size = objectId.length() + 1;
        auto *data_array = (char *)malloc(data_size);
        strncpy(data_array, objectId.c_str(), data_size);
        *out_object_id = data_array;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_plaintext_ballot_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_from_json(char *in_data,
                                                        eg_plaintext_ballot_t **out_handle)
{
    try {
        auto data = string(in_data);
        auto deserialized = PlaintextBallot::fromJson(data);

        *out_handle = AS_TYPE(eg_plaintext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_plaintext_ballot_from_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_from_bson(uint8_t *in_data, uint64_t in_length,
                                                        eg_plaintext_ballot_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = PlaintextBallot::fromBson(data_bytes);

        *out_handle = AS_TYPE(eg_plaintext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_plaintext_ballot_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_to_json(eg_plaintext_ballot_t *handle,
                                                      char **out_data, size_t *out_size)
{
    try {
        auto domain_type = AS_TYPE(PlaintextBallot, handle);
        auto data_string = domain_type->toJson();

        auto data_size = data_string.length() + 1;
        auto *data_array = (char *)malloc(data_size);
        strncpy(data_array, data_string.c_str(), data_size);
        *out_data = data_array;
        *out_size = data_size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_plaintext_ballot_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_plaintext_ballot_to_bson(eg_plaintext_ballot_t *handle,
                                                      uint8_t **out_data, size_t *out_size)
{
    try {
        auto domain_type = AS_TYPE(PlaintextBallot, handle);
        auto data_bytes = domain_type->toBson();

        auto *data_array = new uint8_t[data_bytes.size()];
        copy(data_bytes.begin(), data_bytes.end(), data_array);
        *out_data = data_array;
        *out_size = data_bytes.size();

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_plaintext_ballot_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region CiphertextBallot

eg_electionguard_status_t eg_ciphertext_ballot_free(eg_ciphertext_ballot_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(CiphertextBallot, handle);
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_ciphertext_ballot_get_object_id(eg_ciphertext_ballot_t *handle,
                                                             const char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(CiphertextBallot, handle)->getObjectId();
        auto data_size = objectId.length() + 1;
        auto *data_array = (char *)malloc(data_size);
        strncpy(data_array, objectId.c_str(), data_size);
        *out_object_id = data_array;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_ballot_get_tracking_hash(eg_ciphertext_ballot_t *handle,
                                       eg_element_mod_q_t **out_tracking_hash)
{
    try {
        auto hash = AS_TYPE(CiphertextBallot, handle)->getTrackingHash();
        *out_tracking_hash = AS_TYPE(eg_element_mod_q_t, hash);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_get_tracking_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_get_tracking_code(eg_ciphertext_ballot_t *handle,
                                                                 const char **out_tracking_code)
{
    try {
        auto code = AS_TYPE(CiphertextBallot, handle)->getTrackingCode();
        auto data_size = code.length() + 1;
        auto *data_array = (char *)malloc(data_size);
        strncpy(data_array, code.c_str(), data_size);
        *out_tracking_code = data_array;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_get_tracking_code", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

uint64_t eg_ciphertext_ballot_get_timestamp(eg_ciphertext_ballot_t *ciphertext)
{
    auto timestamp = AS_TYPE(CiphertextBallot, ciphertext)->getTimestamp();
    return timestamp;
}

bool eg_ciphertext_ballot_is_valid_encryption(eg_ciphertext_ballot_t *handle,
                                              eg_element_mod_q_t *in_seed_hash,
                                              eg_element_mod_p_t *in_public_key,
                                              eg_element_mod_q_t *in_crypto_extended_base_hash)
{
    auto ciphertext = AS_TYPE(CiphertextBallot, handle);
    auto seed_hash = AS_TYPE(ElementModQ, in_seed_hash);
    auto public_key = AS_TYPE(ElementModP, in_public_key);
    auto extended_hash = AS_TYPE(ElementModQ, in_crypto_extended_base_hash);

    return ciphertext->isValidEncryption(*seed_hash, *public_key, *extended_hash);
}

eg_electionguard_status_t eg_ciphertext_ballot_from_json(char *in_data,
                                                         eg_ciphertext_ballot_t **out_handle)
{
    try {
        auto data = string(in_data);
        auto deserialized = CiphertextBallot::fromJson(data);

        *out_handle = AS_TYPE(eg_ciphertext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_from_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_from_bson(uint8_t *in_data, uint64_t in_length,
                                                         eg_ciphertext_ballot_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = CiphertextBallot::fromBson(data_bytes);

        *out_handle = AS_TYPE(eg_ciphertext_ballot_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_to_json(eg_ciphertext_ballot_t *handle,
                                                       char **out_data, uint64_t *out_size)
{
    try {
        auto domain_type = AS_TYPE(CiphertextBallot, handle);
        auto data_string = domain_type->toJson();

        auto data_size = data_string.length() + 1;
        auto *data_array = (char *)malloc(data_size);
        strncpy(data_array, data_string.c_str(), data_size);
        *out_data = data_array;
        *out_size = data_size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_to_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_ballot_to_bson(eg_ciphertext_ballot_t *handle,
                                                       uint8_t **out_data, uint64_t *out_size)
{
    try {
        auto domain_type = AS_TYPE(CiphertextBallot, handle);
        auto data_bytes = domain_type->toBson();

        auto *data_array = new uint8_t[data_bytes.size()];
        copy(data_bytes.begin(), data_bytes.end(), data_array);
        *out_data = data_array;
        *out_size = data_bytes.size();

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_ballot_to_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion