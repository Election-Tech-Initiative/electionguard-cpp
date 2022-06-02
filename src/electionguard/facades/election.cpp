#include "electionguard/election.hpp"

#include "../log.hpp"
#include "convert.hpp"
#include "electionguard/collections.h"
#include "variant_cast.hpp"

#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

extern "C" {
#include "electionguard/election.h"
}

using electionguard::CiphertextElectionContext;
using electionguard::ContextConfiguration;
using electionguard::dynamicCopy;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::Log;

using std::make_unique;
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

#pragma region CiphertextElectionContextConfiguration

EG_API eg_electionguard_status_t eg_ciphertext_election_context_config_get_allowed_overvotes(
  eg_context_configuration_t *handle, bool *out_allowed_overvotes)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    auto config = AS_TYPE(ContextConfiguration, handle);
    *out_allowed_overvotes = config->getAllowOverVotes();
    return ELECTIONGUARD_STATUS_SUCCESS;
}

EG_API eg_electionguard_status_t eg_ciphertext_election_context_config_get_max_ballots(
  eg_context_configuration_t *handle, uint64_t *out_max_ballots)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    auto config = AS_TYPE(ContextConfiguration, handle);
    *out_max_ballots = config->getMaxNumberOfBallots();
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_election_context_config_free(eg_context_configuration_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ContextConfiguration, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_election_context_config_make(bool in_allow_overvotes, uint64_t in_number_of_guardians,
                                           eg_context_configuration_t **out_handle)
{
    try {
        auto context = ContextConfiguration::make(in_allow_overvotes, in_number_of_guardians);

        *out_handle = AS_TYPE(eg_context_configuration_t, context.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_config_make", e);
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

    delete AS_TYPE(CiphertextElectionContext, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_ciphertext_election_context_get_elgamal_public_key(
  eg_ciphertext_election_context_t *handle, eg_element_mod_p_t **out_elgamal_public_key_ref)
{
    const auto *pointer = AS_TYPE(CiphertextElectionContext, handle)->getElGamalPublicKey();
    *out_elgamal_public_key_ref = AS_TYPE(eg_element_mod_p_t, const_cast<ElementModP *>(pointer));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_election_context_get_configuration(eg_ciphertext_election_context_t *handle,
                                                 eg_context_configuration_t **out_config)
{
    const auto pointer = AS_TYPE(CiphertextElectionContext, handle)->getConfiguration();
    *out_config = AS_TYPE(eg_context_configuration_t, const_cast<ContextConfiguration *>(pointer));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_election_context_get_commitment_hash(eg_ciphertext_election_context_t *handle,
                                                   eg_element_mod_q_t **out_commitment_hash_ref)
{
    const auto *pointer = AS_TYPE(CiphertextElectionContext, handle)->getCommitmentHash();
    *out_commitment_hash_ref = AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(pointer));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_election_context_get_manifest_hash(eg_ciphertext_election_context_t *handle,
                                                 eg_element_mod_q_t **out_manifest_hash_ref)
{
    const auto *pointer = AS_TYPE(CiphertextElectionContext, handle)->getManifestHash();
    *out_manifest_hash_ref = AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(pointer));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_election_context_get_crypto_base_hash(eg_ciphertext_election_context_t *handle,
                                                    eg_element_mod_q_t **out_crypto_base_hash_ref)
{
    const auto *pointer = AS_TYPE(CiphertextElectionContext, handle)->getCryptoBaseHash();
    *out_crypto_base_hash_ref = AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(pointer));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_ciphertext_election_context_get_crypto_extended_base_hash(
  eg_ciphertext_election_context_t *handle, eg_element_mod_q_t **out_crypto_extended_base_hash_ref)
{
    const auto *pointer = AS_TYPE(CiphertextElectionContext, handle)->getCryptoExtendedBaseHash();
    *out_crypto_extended_base_hash_ref =
      AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(pointer));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_ciphertext_election_context_get_extended_data(eg_ciphertext_election_context_t *handle,
                                                 eg_linked_list_t **out_extended_data)
{
    auto extendedData = AS_TYPE(CiphertextElectionContext, handle)->getExtendedData();

    eg_linked_list_t *extended_data = NULL;
    if (eg_linked_list_new(&extended_data)) {
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }

    unordered_map<string, string>::iterator iterator;
    for (iterator = extendedData.begin(); iterator != extendedData.end(); iterator++) {
        eg_linked_list_append(extended_data, const_cast<char *>(iterator->first.c_str()),
                              const_cast<char *>(iterator->second.c_str()));
    }
    *out_extended_data = extended_data;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_ciphertext_election_context_make(
  uint64_t in_number_of_guardians, uint64_t in_quorum, eg_element_mod_p_t *in_elgamal_public_key,
  eg_element_mod_q_t *in_commitment_hash, eg_element_mod_q_t *in_manifest_hash,
  eg_ciphertext_election_context_t **out_handle)
{
    try {
        auto *publicKeyPtr = AS_TYPE(ElementModP, in_elgamal_public_key);
        auto *commitmentHashPtr = AS_TYPE(ElementModQ, in_commitment_hash);
        auto *manifestHashPtr = AS_TYPE(ElementModQ, in_manifest_hash);

        auto context =
          CiphertextElectionContext::make(in_number_of_guardians, in_quorum, publicKeyPtr->clone(),
                                          commitmentHashPtr->clone(), manifestHashPtr->clone());

        *out_handle = AS_TYPE(eg_ciphertext_election_context_t, context.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_make", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_election_context_make_with_extended_data(
  uint64_t in_number_of_guardians, uint64_t in_quorum, eg_element_mod_p_t *in_elgamal_public_key,
  eg_element_mod_q_t *in_commitment_hash, eg_element_mod_q_t *in_manifest_hash,
  eg_linked_list_t *in_extended_data, eg_ciphertext_election_context_t **out_handle)
{
    try {
        auto *publicKeyPtr = AS_TYPE(ElementModP, in_elgamal_public_key);
        auto *commitmentHashPtr = AS_TYPE(ElementModQ, in_commitment_hash);
        auto *manifestHashPtr = AS_TYPE(ElementModQ, in_manifest_hash);

        unordered_map<string, string> extendedData = {};
        for (uint64_t i = 0; i < eg_linked_list_get_count(in_extended_data); i++) {
            char *edKey = NULL;
            char *edValue = NULL;
            if (eg_linked_list_get_element_at(in_extended_data, i, &edKey, &edValue)) {
                return ELECTIONGUARD_STATUS_ERROR_BAD_ACCESS;
            }
            extendedData[string(edKey)] = string(edValue);
        }

        auto context = CiphertextElectionContext::make(
          in_number_of_guardians, in_quorum, publicKeyPtr->clone(), commitmentHashPtr->clone(),
          manifestHashPtr->clone(), move(extendedData));

        *out_handle = AS_TYPE(eg_ciphertext_election_context_t, context.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_make_with_extended_data", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_election_context_make_from_hex(
  uint64_t in_number_of_guardians, uint64_t in_quorum, const char *in_elgamal_public_key,
  const char *in_commitment_hash, const char *in_manifest_hash,
  eg_ciphertext_election_context_t **out_handle)
{
    try {
        auto elGamalPublicKey = string(in_elgamal_public_key);
        auto commitmentHash = string(in_commitment_hash);
        auto manifestHash = string(in_manifest_hash);

        auto context = CiphertextElectionContext::make(
          in_number_of_guardians, in_quorum, elGamalPublicKey, commitmentHash, manifestHash);
        *out_handle = AS_TYPE(eg_ciphertext_election_context_t, context.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_make_from_hex", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ciphertext_election_context_make_from_hex_with_extended_data(
  uint64_t in_number_of_guardians, uint64_t in_quorum, const char *in_elgamal_public_key,
  const char *in_commitment_hash, const char *in_manifest_hash, eg_linked_list_t *in_extended_data,
  eg_ciphertext_election_context_t **out_handle)
{
    try {
        auto elGamalPublicKey = string(in_elgamal_public_key);
        auto commitmentHash = string(in_commitment_hash);
        auto manifestHash = string(in_manifest_hash);

        unordered_map<string, string> extendedData = {};
        for (uint64_t i = 0; i < eg_linked_list_get_count(in_extended_data); i++) {
            char *edKey = NULL;
            char *edValue = NULL;
            if (eg_linked_list_get_element_at(in_extended_data, i, &edKey, &edValue)) {
                return ELECTIONGUARD_STATUS_ERROR_BAD_ACCESS;
            }
            extendedData[string(edKey)] = string(edValue);
        }

        auto context = CiphertextElectionContext::make(
          in_number_of_guardians, in_quorum, elGamalPublicKey, commitmentHash, manifestHash);
        *out_handle = AS_TYPE(eg_ciphertext_election_context_t, context.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_make_from_hex_with_extended_data", e);
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
                                       uint64_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CiphertextElectionContext, handle);
        auto data_string = domain_type->toJson();

        size_t size = 0;
        *out_data = dynamicCopy(data_string, &size);
        *out_size = (uint64_t)size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_to_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ciphertext_election_context_to_bson(eg_ciphertext_election_context_t *handle, uint8_t **out_data,
                                       uint64_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(CiphertextElectionContext, handle);
        auto data_bytes = domain_type->toBson();

        size_t size = 0;
        *out_data = dynamicCopy(data_bytes, &size);
        *out_size = (uint64_t)size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ciphertext_election_context_to_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion
