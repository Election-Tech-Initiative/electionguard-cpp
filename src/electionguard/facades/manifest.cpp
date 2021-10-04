#include "electionguard/manifest.hpp"

#include "../log.hpp"
#include "convert.hpp"
#include "variant_cast.hpp"

#include <cstring>
#include <memory>
#include <string>
#include <vector>

extern "C" {

#include "electionguard/manifest.h"
}

using electionguard::AnnotatedString;
using electionguard::dynamicCopy;
using electionguard::ElementModQ;
using electionguard::InternalManifest;
using electionguard::InternationalizedText;
using electionguard::Language;
using electionguard::Log;
using electionguard::Manifest;
using electionguard::SelectionDescription;

using std::make_unique;
using std::string;
using std::unique_ptr;
using std::vector;

#pragma region AnnotatedString

eg_electionguard_status_t eg_annotated_string_new(const char *in_annotation, const char *in_value,
                                                  eg_annotated_string_t **out_handle)
{
    try {
        auto annotation = string(in_annotation);
        auto value = string(in_value);
        auto result = make_unique<AnnotatedString>(annotation, value);

        *out_handle = AS_TYPE(eg_annotated_string_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_annotated_string_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_annotated_string_free(eg_annotated_string_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(AnnotatedString, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_annotated_string_get_annotation(eg_annotated_string_t *handle,
                                                             const char **out_annotation)
{
    try {
        auto result = AS_TYPE(AnnotatedString, handle)->getAnnotation();
        *out_annotation = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_annotated_string_get_annotation", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_annotated_string_get_value(eg_annotated_string_t *handle,
                                                        const char **out_value)
{
    try {
        auto result = AS_TYPE(AnnotatedString, handle)->getValue();
        *out_value = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_annotated_string_get_value", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_annotated_string_crypto_hash(eg_annotated_string_t *handle,
                                                          eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto result = AS_TYPE(AnnotatedString, handle)->crypto_hash();
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_annotated_string_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region Language

eg_electionguard_status_t eg_language_new(const char *in_value, const char *in_language,
                                          eg_language_t **out_handle)
{
    try {
        auto language = string(in_language);
        auto value = string(in_value);
        auto result = make_unique<Language>(value, language);

        *out_handle = AS_TYPE(eg_language_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_language_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_language_free(eg_language_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(Language, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_language_get_language(eg_language_t *handle, char **out_annotation)
{
    try {
        auto result = AS_TYPE(Language, handle)->getLanguage();
        *out_annotation = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_language_get_annotation", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_language_get_value(eg_language_t *handle, char **out_value)
{
    try {
        auto result = AS_TYPE(Language, handle)->getValue();
        *out_value = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_language_get_value", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_language_crypto_hash(eg_language_t *handle,
                                                  eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto result = AS_TYPE(Language, handle)->crypto_hash();
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_language_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region InternationalizedText

eg_electionguard_status_t eg_internationalized_text_new(eg_language_t *in_text[],
                                                        const size_t in_text_size,
                                                        eg_internationalized_text_t **out_handle)
{
    try {
        vector<unique_ptr<Language>> collection;
        collection.reserve(in_text_size);
        for (auto i = 0; i < in_text_size; i++) {
            auto *element = AS_TYPE(Language, in_text[i]);
            unique_ptr<Language> lang(new Language(std::move(*element)));
            collection.push_back(std::move(lang));
            //collection.push_back(make_unique<Language>(std::move(*element)));
        }

        auto result = make_unique<InternationalizedText>(move(collection));
        *out_handle = AS_TYPE(eg_internationalized_text_t, result.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_language_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_internationalized_text_free(eg_internationalized_text_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(InternationalizedText, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

size_t eg_internationalized_text_get_text_size(eg_internationalized_text_t *handle)
{
    return AS_TYPE(InternationalizedText, handle)->getText().size();
}

eg_electionguard_status_t
eg_internationalized_text_get_text_at_index(eg_internationalized_text_t *handle, size_t in_index,
                                            eg_language_t **out_text_ref)
{
    try {
        auto collection = AS_TYPE(InternationalizedText, handle)->getText();
        auto *reference = &collection.at(in_index).get();

        *out_text_ref = AS_TYPE(eg_language_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_internationalized_text_get_text_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_intertnationalized_text_crypto_hash(eg_language_t *handle, eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto result = AS_TYPE(InternationalizedText, handle)->crypto_hash();
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_intertnationalized_text_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region SelectionDescription

eg_electionguard_status_t eg_selection_description_new(const char *in_object_id,
                                                       const char *in_candidate_id,
                                                       uint64_t in_sequence_order,
                                                       eg_selection_description_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto candidateId = string(in_candidate_id);
        auto result = make_unique<SelectionDescription>(objectId, candidateId, in_sequence_order);

        *out_handle = AS_TYPE(eg_selection_description_t, result.release());
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

    delete AS_TYPE(SelectionDescription, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_selection_description_get_object_id(eg_selection_description_t *handle,
                                                                 const char **out_object_id)
{
    try {
        auto result = AS_TYPE(SelectionDescription, handle)->getObjectId();
        *out_object_id = dynamicCopy(result);

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
        auto result = AS_TYPE(SelectionDescription, handle)->getCandidateId();
        *out_candidate_id = dynamicCopy(result);

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
    try {
        auto result = AS_TYPE(SelectionDescription, handle)->crypto_hash();
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_selection_description_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region Manifest

eg_electionguard_status_t eg_election_manifest_free(eg_election_manifest_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(Manifest, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_election_manifest_get_election_scope_id(eg_election_manifest_t *handle,
                                                                     char **out_election_scope_id)
{
    try {
        auto result = AS_TYPE(Manifest, handle)->getElectionScopeId();
        *out_election_scope_id = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_manifest_get_election_scope_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_manifest_crypto_hash(eg_election_manifest_t *handle,
                                                           eg_element_mod_q_t **out_owned_hash)
{
    try {
        auto result = AS_TYPE(Manifest, handle)->crypto_hash();
        *out_owned_hash = AS_TYPE(eg_element_mod_q_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_manifest_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_manifest_from_json(char *in_data,
                                                         eg_election_manifest_t **out_handle)
{
    try {
        auto data = string(in_data);
        auto deserialized = Manifest::fromJson(data);

        *out_handle = AS_TYPE(eg_election_manifest_t, deserialized.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_manifest_from_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_manifest_from_bson(uint8_t *in_data, uint64_t in_length,
                                                         eg_election_manifest_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto result = Manifest::fromBson(data_bytes);

        *out_handle = AS_TYPE(eg_election_manifest_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_manifest_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_manifest_to_json(eg_election_manifest_t *handle,
                                                       char **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(Manifest, handle);
        auto result = domain_type->toJson();

        size_t size = 0;
        *out_data = dynamicCopy(result, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_manifest_to_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_manifest_to_bson(eg_election_manifest_t *handle,
                                                       uint8_t **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(Manifest, handle);
        auto result = domain_type->toBson();

        size_t size = 0;
        *out_data = dynamicCopy(result, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_manifest_to_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region InternalManifest

eg_electionguard_status_t eg_internal_manifest_new(eg_election_manifest_t *in_manifest,
                                                   eg_internal_manifest_t **out_handle)
{
    try {
        auto *domain_type = AS_TYPE(Manifest, in_manifest);
        auto result = make_unique<InternalManifest>(*domain_type);

        *out_handle = AS_TYPE(eg_internal_manifest_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_manifest_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_internal_manifest_free(eg_internal_manifest_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(InternalManifest, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_internal_manifest_get_manifest_hash(eg_internal_manifest_t *handle,
                                       eg_element_mod_q_t **out_manifest_hash_ref)
{
    const auto *reference = AS_TYPE(InternalManifest, handle)->getManifestHash();
    *out_manifest_hash_ref = AS_TYPE(eg_element_mod_q_t, const_cast<ElementModQ *>(reference));
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_internal_manifest_from_json(char *in_data,
                                                         eg_internal_manifest_t **out_handle)
{
    try {
        auto data = string(in_data);
        auto result = InternalManifest::fromJson(data);

        *out_handle = AS_TYPE(eg_internal_manifest_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_manifest_from_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_internal_manifest_from_bson(uint8_t *in_data, uint64_t in_length,
                                                         eg_internal_manifest_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto result = InternalManifest::fromBson(data_bytes);

        *out_handle = AS_TYPE(eg_internal_manifest_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_manifest_from_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_internal_manifest_to_json(eg_internal_manifest_t *handle,
                                                       char **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(InternalManifest, handle);
        auto result = domain_type->toJson();

        size_t size = 0;
        *out_data = dynamicCopy(result, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_manifest_to_json", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_internal_manifest_to_bson(eg_internal_manifest_t *handle,
                                                       uint8_t **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(InternalManifest, handle);
        auto result = domain_type->toBson();

        size_t size = 0;
        *out_data = dynamicCopy(result, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_manifest_to_bson", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion
