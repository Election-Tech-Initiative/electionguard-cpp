#include "electionguard/election.hpp"

#include "../log.hpp"
#include "convert.hpp"
#include "variant_cast.hpp"

#include <cstring>
#include <memory>
#include <string>
#include <vector>

extern "C" {

#include "electionguard/election.h"
}

using electionguard::CiphertextElectionContext;
using electionguard::dynamicCopy;
using electionguard::ElectionDescription;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::InternalElectionDescription;
using electionguard::Log;
using electionguard::SelectionDescription;

using std::make_unique;
using std::string;
using std::unique_ptr;
using std::vector;

#pragma region SelectionDescription

eg_electionguard_status_t eg_selection_description_new(const char *in_object_id,
                                                       const char *in_candidate_id,
                                                       uint64_t in_sequence_order,
                                                       eg_selection_description_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto candidateId = string(in_candidate_id);
        auto description =
          make_unique<SelectionDescription>(objectId, candidateId, in_sequence_order);

        *out_handle = AS_TYPE(eg_selection_description_t, description.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_selection_description_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_selection_description_free(eg_selection_description_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(SelectionDescription, handle);
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_selection_description_get_object_id(eg_selection_description_t *handle,
                                                                 const char **out_object_id)
{
    try {
        auto objectId = AS_TYPE(SelectionDescription, handle)->getObjectId();
        *out_object_id = dynamicCopy(objectId);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_selection_description_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_selection_description_get_candidate_id(eg_selection_description_t *handle,
                                          const char **out_candidate_id)
{
    try {
        auto candidateId = AS_TYPE(SelectionDescription, handle)->getCandidateId();
        *out_candidate_id = dynamicCopy(candidateId);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_selection_description_get_candidate_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_selection_description_get_sequence_order(eg_selection_description_t *handle,
                                            uint64_t *out_sequence_order)
{
    *out_sequence_order = AS_TYPE(SelectionDescription, handle)->getSequenceOrder();
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_selection_description_crypto_hash(eg_selection_description_t *handle,
                                                               eg_element_mod_q_t **out_crypto_hash)
{
    auto cryptoHash = AS_TYPE(SelectionDescription, handle)->crypto_hash();
    *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, cryptoHash.release());
    return ELECTIONGUARD_STATUS_SUCCESS;
}

#pragma endregion

#pragma region ElectionDescription

eg_electionguard_status_t eg_election_description_free(eg_election_description_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ElectionDescription, handle);
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_election_description_get_election_scope_id(eg_election_description_t *handle,
                                              char **out_election_scope_id)
{
    try {
        auto scopeId = AS_TYPE(ElectionDescription, handle)->getElectionScopeId();
        *out_election_scope_id = dynamicCopy(scopeId);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_description_get_election_scope_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_description_crypto_hash(eg_election_description_t *handle,
                                                              eg_element_mod_q_t **out_owned_hash)
{
    try {
        auto cryptoHash = AS_TYPE(ElectionDescription, handle)->crypto_hash();
        *out_owned_hash = AS_TYPE(eg_element_mod_q_t, cryptoHash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_description_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_description_from_json(char *in_data,
                                                            eg_election_description_t **out_handle)
{
    try {
        auto data = string(in_data);
        auto deserialized = ElectionDescription::fromJson(data);

        *out_handle = AS_TYPE(eg_election_description_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_description_from_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_description_from_bson(uint8_t *in_data, uint64_t in_length,
                                                            eg_election_description_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = ElectionDescription::fromBson(data_bytes);

        *out_handle = AS_TYPE(eg_election_description_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_description_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_description_to_json(eg_election_description_t *handle,
                                                          char **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(ElectionDescription, handle);
        auto data_string = domain_type->toJson();

        size_t size = 0;
        *out_data = dynamicCopy(data_string, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_description_to_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_description_to_bson(eg_election_description_t *handle,
                                                          uint8_t **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(ElectionDescription, handle);
        auto data_bytes = domain_type->toBson();

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_election_description_to_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region InternalElectionDescription

eg_electionguard_status_t
eg_internal_election_description_new(eg_election_description_t *in_election_description,
                                     eg_internal_election_description_t **out_handle)
{
    try {
        auto *domain_type = AS_TYPE(ElectionDescription, in_election_description);
        auto pointer = make_unique<InternalElectionDescription>(*domain_type);

        *out_handle = AS_TYPE(eg_internal_election_description_t, pointer.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_election_description_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_internal_election_description_free(eg_internal_election_description_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(InternalElectionDescription, handle);
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_internal_election_description_get_description_hash(eg_internal_election_description_t *handle,
                                                      eg_element_mod_q_t **out_description_hash)
{
    const auto *description = AS_TYPE(InternalElectionDescription, handle)->getDescriptionHash();
    *out_description_hash = AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(description));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_internal_election_description_from_json(char *in_data,
                                           eg_internal_election_description_t **out_handle)
{
    try {
        auto data = string(in_data);
        auto deserialized = InternalElectionDescription::fromJson(data);

        *out_handle = AS_TYPE(eg_internal_election_description_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_election_description_from_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_internal_election_description_from_bson(uint8_t *in_data, uint64_t in_length,
                                           eg_internal_election_description_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = InternalElectionDescription::fromBson(data_bytes);

        *out_handle = AS_TYPE(eg_internal_election_description_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_election_description_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_internal_election_description_to_json(eg_internal_election_description_t *handle,
                                         char **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(InternalElectionDescription, handle);
        auto data_string = domain_type->toJson();

        size_t size = 0;
        *out_data = dynamicCopy(data_string, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_election_description_to_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_internal_election_description_to_bson(eg_internal_election_description_t *handle,
                                         uint8_t **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(InternalElectionDescription, handle);
        auto data_bytes = domain_type->toBson();

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_election_description_to_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region CiphertextElectionContext

eg_electionguard_status_t
eg_ciphertext_election_context_free(eg_ciphertext_election_context_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(CiphertextElectionContext, handle);
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_election_context_get_elgamal_public_key(eg_ciphertext_election_context_t *handle,
                                                      eg_element_mod_p_t **out_elgamal_public_key)
{
    const auto *pointer = AS_TYPE(CiphertextElectionContext, handle)->getElGamalPublicKey();
    *out_elgamal_public_key = AS_TYPE(eg_element_mod_p_t, const_cast<ElementModP *>(pointer));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_election_context_get_description_hash(eg_ciphertext_election_context_t *handle,
                                                    eg_element_mod_q_t **out_description_hash)
{
    const auto *pointer = AS_TYPE(CiphertextElectionContext, handle)->getDescriptionHash();
    *out_description_hash = AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(pointer));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_election_context_get_crypto_base_hash(eg_ciphertext_election_context_t *handle,
                                                    eg_element_mod_q_t **out_crypto_base_hash)
{
    const auto *pointer = AS_TYPE(CiphertextElectionContext, handle)->getCryptoBaseHash();
    *out_crypto_base_hash = AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(pointer));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_ciphertext_election_context_get_crypto_extended_base_hash(
  eg_ciphertext_election_context_t *handle, eg_element_mod_q_t **out_crypto_extended_base_hash)
{
    const auto *pointer = AS_TYPE(CiphertextElectionContext, handle)->getCryptoExtendedBaseHash();
    *out_crypto_extended_base_hash =
      AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(pointer));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_ciphertext_election_context_make(
  uint64_t in_number_of_guardians, uint64_t in_quorum, eg_element_mod_p_t *in_elgamal_public_key,
  eg_element_mod_q_t *in_description_hash, eg_ciphertext_election_context_t **out_handle)
{
    try {
        auto *publicKeyPtr = AS_TYPE(ElementModP, in_elgamal_public_key);
        auto *hashPtr = AS_TYPE(ElementModQ, in_description_hash);

        unique_ptr<ElementModP> elGamalPublicKey{publicKeyPtr};
        unique_ptr<ElementModQ> descriptionHash{hashPtr};

        auto context = CiphertextElectionContext::make(
          in_number_of_guardians, in_quorum, move(elGamalPublicKey), move(descriptionHash));

        *out_handle = AS_TYPE(eg_ciphertext_election_context_t, context.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_make", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_election_context_make_from_hex(
  uint64_t in_number_of_guardians, uint64_t in_quorum, const char *in_elgamal_public_key,
  const char *in_description_hash, eg_ciphertext_election_context_t **out_handle)
{
    try {
        auto elGamalPublicKey = string(in_elgamal_public_key);
        auto descriptionHash = string(in_description_hash);

        auto context = CiphertextElectionContext::make(in_number_of_guardians, in_quorum,
                                                       elGamalPublicKey, descriptionHash);
        *out_handle = AS_TYPE(eg_ciphertext_election_context_t, context.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_make_from_hex", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_election_context_from_json(char *in_data,
                                         eg_ciphertext_election_context_t **out_handle)
{
    try {
        auto data = string(in_data);
        auto deserialized = CiphertextElectionContext::fromJson(data);
        *out_handle = AS_TYPE(eg_ciphertext_election_context_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_from_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_election_context_from_bson(uint8_t *in_data, uint64_t in_length,
                                         eg_ciphertext_election_context_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto deserialized = CiphertextElectionContext::fromBson(data_bytes);

        *out_handle = AS_TYPE(eg_ciphertext_election_context_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_election_context_to_json(eg_ciphertext_election_context_t *handle, char **out_data,
                                       size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CiphertextElectionContext, handle);
        auto data_string = domain_type->toJson();

        size_t size = 0;
        *out_data = dynamicCopy(data_string, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_to_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_election_context_to_bson(eg_ciphertext_election_context_t *handle, uint8_t **out_data,
                                       size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CiphertextElectionContext, handle);
        auto data_bytes = domain_type->toBson();

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_to_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion
