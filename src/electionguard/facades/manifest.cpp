#include "electionguard/manifest.hpp"

#include "../log.hpp"
#include "convert.hpp"
#include "variant_cast.hpp"

#include <chrono>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

extern "C" {

#include "electionguard/manifest.h"
}

using electionguard::AnnotatedString;
using electionguard::BallotStyle;
using electionguard::Candidate;
using electionguard::ContactInformation;
using electionguard::ContestDescription;
using electionguard::ContestDescriptionWithPlaceholders;
using electionguard::dynamicCopy;
using electionguard::ElectionType;
using electionguard::ElementModQ;
using electionguard::GeopoliticalUnit;
using electionguard::InternalManifest;
using electionguard::InternationalizedText;
using electionguard::Language;
using electionguard::Log;
using electionguard::Manifest;
using electionguard::Party;
using electionguard::ReportingUnitType;
using electionguard::SelectionDescription;
using electionguard::VoteVariationType;

using std::make_unique;
using std::string;
using std::unique_ptr;
using std::vector;
using std::chrono::system_clock;

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
        auto value = string(in_value);
        auto language = string(in_language);
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

eg_electionguard_status_t eg_language_get_language(eg_language_t *handle, char **out_language)
{
    try {
        auto result = AS_TYPE(Language, handle)->getLanguage();
        *out_language = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_language_get_language", e);
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

#pragma region ContactInformation

eg_electionguard_status_t eg_contact_information_new(char *in_name,
                                                     eg_contact_information_t **out_handle)
{
    try {
        auto name = string(in_name);
        auto result = make_unique<ContactInformation>(name);

        *out_handle = AS_TYPE(eg_contact_information_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contact_information_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contact_information_new_with_collections(
  char *in_name, char *in_address_line[], const size_t in_address_line_size,
  eg_annotated_string_t *in_email_line[], const size_t in_email_line_size,
  eg_annotated_string_t *in_phone_line[], const size_t in_phone_line_size,
  eg_contact_information_t **out_handle)
{
    try {
        vector<string> address;
        address.reserve(in_address_line_size);
        for (auto i = 0; i < in_address_line_size; i++) {
            address.push_back(in_address_line[i]);
        }

        vector<unique_ptr<AnnotatedString>> email;
        email.reserve(in_email_line_size);
        for (auto i = 0; i < in_email_line_size; i++) {
            auto *element = AS_TYPE(AnnotatedString, in_email_line[i]);
            unique_ptr<AnnotatedString> annotated(new AnnotatedString(std::move(*element)));
            email.push_back(std::move(annotated));
        }

        vector<unique_ptr<AnnotatedString>> phone;
        email.reserve(in_phone_line_size);
        for (auto i = 0; i < in_phone_line_size; i++) {
            auto *element = AS_TYPE(AnnotatedString, in_phone_line[i]);
            unique_ptr<AnnotatedString> annotated(new AnnotatedString(std::move(*element)));
            phone.push_back(std::move(annotated));
        }

        auto result =
          make_unique<ContactInformation>(move(address), move(email), move(phone), in_name);
        *out_handle = AS_TYPE(eg_contact_information_t, result.release());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contact_information_new_with_collections", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contact_information_free(eg_contact_information_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ContactInformation, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

size_t eg_contact_information_get_address_line_size(eg_contact_information_t *handle)
{
    return AS_TYPE(ContactInformation, handle)->getAddressLine().size();
}

eg_electionguard_status_t
eg_contact_information_get_address_line_at_index(eg_contact_information_t *handle, size_t in_index,
                                                 char **out_address)
{
    try {
        auto collection = AS_TYPE(ContactInformation, handle)->getAddressLine();
        auto reference = collection.at(in_index);

        *out_address = dynamicCopy(reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_contact_information_get_phone_line_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_contact_information_get_email_line_size(eg_contact_information_t *handle)
{
    return AS_TYPE(ContactInformation, handle)->getEmail().size();
}

eg_electionguard_status_t
eg_contact_information_get_email_line_at_index(eg_contact_information_t *handle, size_t in_index,
                                               eg_internationalized_text_t **out_address_ref)
{
    try {
        auto collection = AS_TYPE(ContactInformation, handle)->getEmail();
        auto *reference = &collection.at(in_index).get();

        *out_address_ref = AS_TYPE(eg_internationalized_text_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_contact_information_get_email_line_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_contact_information_get_phone_line_size(eg_contact_information_t *handle)
{
    return AS_TYPE(ContactInformation, handle)->getPhone().size();
}

eg_electionguard_status_t
eg_contact_information_get_phone_line_at_index(eg_contact_information_t *handle, size_t in_index,
                                               eg_internationalized_text_t **out_phone_ref)
{
    try {
        auto collection = AS_TYPE(ContactInformation, handle)->getPhone();
        auto *reference = &collection.at(in_index).get();

        *out_phone_ref = AS_TYPE(eg_internationalized_text_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_contact_information_get_phone_line_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contact_information_get_name(eg_contact_information_t *handle,
                                                          const char **out_name)
{
    try {
        auto result = AS_TYPE(GeopoliticalUnit, handle)->getName();
        *out_name = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contact_information_get_name", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contact_information_crypto_hash(eg_contact_information_t *handle,
                                                             eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto result = AS_TYPE(InternationalizedText, handle)->crypto_hash();
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contact_information_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region GeopoliticalUnit

eg_electionguard_status_t eg_geopolitical_unit_new(char *in_object_id, char *in_name,
                                                   eg_reporting_unit_type_t in_reporting_unit_type,
                                                   eg_geopolitical_unit_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto name = string(in_name);

        auto result =
          make_unique<GeopoliticalUnit>(objectId, name, (ReportingUnitType)in_reporting_unit_type);

        *out_handle = AS_TYPE(eg_geopolitical_unit_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_geopolitical_unit_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_geopolitical_unit_new_with_contact_info(
  char *in_object_id, char *in_name, eg_reporting_unit_type_t in_reporting_unit_type,
  eg_contact_information_t *in_contact_information, eg_geopolitical_unit_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto name = string(in_name);
        auto contact = AS_TYPE(ContactInformation, in_contact_information);

        auto result =
          make_unique<GeopoliticalUnit>(objectId, name, (ReportingUnitType)in_reporting_unit_type,
                                        make_unique<ContactInformation>(std::move(*contact)));

        *out_handle = AS_TYPE(eg_geopolitical_unit_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_geopolitical_unit_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_geopolitical_unit_free(eg_geopolitical_unit_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(GeopoliticalUnit, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_geopolitical_unit_get_object_id(eg_geopolitical_unit_t *handle,
                                                             const char **out_object_id)
{
    try {
        auto result = AS_TYPE(GeopoliticalUnit, handle)->getObjectId();
        *out_object_id = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_geopolitical_unit_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_geopolitical_unit_get_name(eg_geopolitical_unit_t *handle,
                                                        const char **out_name)
{
    try {
        auto result = AS_TYPE(GeopoliticalUnit, handle)->getName();
        *out_name = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_geopolitical_unit_get_name", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_reporting_unit_type_t get_geopolitical_unit_get_type(eg_geopolitical_unit_t *handle)
{
    return (eg_reporting_unit_type_t)AS_TYPE(GeopoliticalUnit, handle)->getType();
}

eg_electionguard_status_t eg_geopolitical_unit_crypto_hash(eg_geopolitical_unit_t *handle,
                                                           eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto result = AS_TYPE(GeopoliticalUnit, handle)->crypto_hash();
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_geopolitical_unit_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region BallotStyle

eg_electionguard_status_t eg_ballot_style_new(char *in_object_id, char *in_geopolitical_unit_ids[],
                                              const size_t in_geopolitical_unit_ids_size,
                                              eg_ballot_style_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        vector<string> gpUnitIds;
        gpUnitIds.reserve(in_geopolitical_unit_ids_size);
        for (auto i = 0; i < in_geopolitical_unit_ids_size; i++) {
            gpUnitIds.push_back(in_geopolitical_unit_ids[i]);
        }

        auto result = make_unique<BallotStyle>(objectId, move(gpUnitIds));

        *out_handle = AS_TYPE(eg_ballot_style_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ballot_style_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_ballot_style_new_with_parties(char *in_object_id, char *in_geopolitical_unit_ids[],
                                 const size_t in_geopolitical_unit_ids_size, char *in_party_ids[],
                                 const size_t in_party_ids_size, char *in_image_uri,
                                 eg_ballot_style_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto imageUri = string(in_image_uri);
        vector<string> gpUnitIds;
        gpUnitIds.reserve(in_geopolitical_unit_ids_size);
        for (auto i = 0; i < in_geopolitical_unit_ids_size; i++) {
            gpUnitIds.push_back(in_geopolitical_unit_ids[i]);
        }

        vector<string> partyIds;
        gpUnitIds.reserve(in_party_ids_size);
        for (auto i = 0; i < in_party_ids_size; i++) {
            partyIds.push_back(in_party_ids[i]);
        }

        auto result = make_unique<BallotStyle>(objectId, move(gpUnitIds), move(partyIds), imageUri);

        *out_handle = AS_TYPE(eg_ballot_style_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ballot_style_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ballot_style_free(eg_ballot_style_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(BallotStyle, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_ballot_style_get_object_id(eg_ballot_style_t *handle,
                                                        const char **out_object_id)
{
    try {
        auto result = AS_TYPE(BallotStyle, handle)->getObjectId();
        *out_object_id = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ballot_style_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_ballot_style_get_geopolitical_unit_ids_size(eg_ballot_style_t *handle)
{
    return AS_TYPE(BallotStyle, handle)->getGeopoliticalUnitIds().size();
}

eg_electionguard_status_t
eg_ballot_style_get_geopolitical_unit_id_at_index(eg_ballot_style_t *handle, size_t in_index,
                                                  char **out_geopolitical_unit_id)
{
    try {
        auto collection = AS_TYPE(BallotStyle, handle)->getGeopoliticalUnitIds();
        auto reference = collection.at(in_index);
        *out_geopolitical_unit_id = dynamicCopy(reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ballot_style_get_geopolitical_unit_id_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_ballot_style_get_party_ids_size(eg_ballot_style_t *handle)
{
    return AS_TYPE(BallotStyle, handle)->getPartyIds().size();
}

eg_electionguard_status_t eg_ballot_style_get_party_id_at_index(eg_ballot_style_t *handle,
                                                                size_t in_index,
                                                                char **out_party_id)
{
    try {
        auto collection = AS_TYPE(BallotStyle, handle)->getPartyIds();
        auto reference = collection.at(in_index);
        *out_party_id = dynamicCopy(reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ballot_style_get_party_id_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ballot_style_get_image_uri(eg_ballot_style_t *handle,
                                                        const char **out_image_uri)
{
    try {
        auto result = AS_TYPE(BallotStyle, handle)->getImageUri();
        *out_image_uri = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ballot_style_get_image_uri", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_ballot_style_crypto_hash(eg_ballot_style_t *handle,
                                                      eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto result = AS_TYPE(BallotStyle, handle)->crypto_hash();
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_ballot_style_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region Party

eg_electionguard_status_t eg_party_new(char *in_object_id, eg_party_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);

        auto result = make_unique<Party>(objectId);

        *out_handle = AS_TYPE(eg_party_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_party_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

// TODO: string in params should all be const i think

eg_electionguard_status_t eg_party_new_with_extras(char *in_object_id,
                                                   eg_internationalized_text_t *in_name,
                                                   char *in_abbreviation, char *in_color,
                                                   char *in_logo_uri, eg_party_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto name = AS_TYPE(InternationalizedText, in_name);

        auto abbreviation = string(in_abbreviation);
        auto color = string(in_color);
        auto logo = in_logo_uri;

        auto result =
          make_unique<Party>(objectId, make_unique<InternationalizedText>(std::move(*name)),
                             abbreviation, color, logo);

        *out_handle = AS_TYPE(eg_party_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_party_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_party_free(eg_party_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(Party, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_party_get_object_id(eg_party_t *handle, const char **out_object_id)
{
    try {
        auto result = AS_TYPE(Party, handle)->getObjectId();
        *out_object_id = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_party_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_party_get_abbreviation(eg_party_t *handle,
                                                    const char **out_abbreviation)
{
    try {
        auto result = AS_TYPE(Party, handle)->getAbbreviation();
        *out_abbreviation = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_party_get_abbreviation", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_party_get_name(eg_party_t *handle,
                                            eg_internationalized_text_t **out_name_ref)
{
    try {
        auto result = AS_TYPE(Party, handle)->getName();
        *out_name_ref = AS_TYPE(eg_internationalized_text_t, result);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_party_get_name", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_party_get_color(eg_party_t *handle, const char **out_color)
{
    try {
        auto result = AS_TYPE(Party, handle)->getColor();
        *out_color = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_party_get_color", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_party_get_logo_uri(eg_party_t *handle, const char **out_logo_uri)
{
    try {
        auto result = AS_TYPE(Party, handle)->getLogoUri();
        *out_logo_uri = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_party_get_logo_uri", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}
eg_electionguard_status_t eg_party_crypto_hash(eg_party_t *handle,
                                               eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto result = AS_TYPE(Party, handle)->crypto_hash();
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_party_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region Candidate

eg_electionguard_status_t eg_candidate_new(char *in_object_id, bool in_is_write_in,
                                           eg_candidate_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);

        auto result = make_unique<Candidate>(objectId, in_is_write_in);

        *out_handle = AS_TYPE(eg_candidate_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_candidate_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

// TODO: string in params should all be const i think

eg_electionguard_status_t eg_candidate_new_with_extras(char *in_object_id,
                                                       eg_internationalized_text_t *in_name,
                                                       char *in_party_id, char *in_image_uri,
                                                       bool in_is_write_in,
                                                       eg_candidate_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto name = AS_TYPE(InternationalizedText, in_name);

        auto partyId = string(in_party_id);
        auto imageUri = string(in_image_uri);

        auto result =
          make_unique<Candidate>(objectId, make_unique<InternationalizedText>(std::move(*name)),
                                 partyId, imageUri, in_is_write_in);

        *out_handle = AS_TYPE(eg_candidate_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_candidate_new_with_extras", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_candidate_free(eg_candidate_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(Candidate, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_candidate_get_object_id(eg_candidate_t *handle,
                                                     const char **out_object_id)
{
    try {
        auto result = AS_TYPE(Candidate, handle)->getObjectId();
        *out_object_id = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_candidate_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_candidate_get_candidate_id(eg_candidate_t *handle,
                                                        const char **out_candidate_id)
{
    try {
        auto result = AS_TYPE(Candidate, handle)->getCandidateId();
        *out_candidate_id = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_candidate_get_candidate_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_candidate_get_name(eg_candidate_t *handle,
                                                eg_internationalized_text_t **out_name_ref)
{
    try {
        auto result = AS_TYPE(Candidate, handle)->getName();
        *out_name_ref = AS_TYPE(eg_internationalized_text_t, result);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_candidate_get_name", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_candidate_get_party_id(eg_candidate_t *handle,
                                                    const char **out_party_id)
{
    try {
        auto result = AS_TYPE(Candidate, handle)->getPartyId();
        *out_party_id = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_candidate_get_party_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_candidate_get_image_uri(eg_candidate_t *handle,
                                                     const char **out_image_uri)
{
    try {
        auto result = AS_TYPE(Candidate, handle)->getImageUri();
        *out_image_uri = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_candidate_get_image_uri", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

bool eg_candidate_get_is_write_in(eg_candidate_t *handle)
{
    try {
        return AS_TYPE(Candidate, handle)->isWriteIn();
    } catch (const exception &e) {
        Log::error(":eg_candidate_get_is_write_in", e);
        return false;
    }
}

eg_electionguard_status_t eg_candidate_crypto_hash(eg_candidate_t *handle,
                                                   eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto result = AS_TYPE(Party, handle)->crypto_hash();
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_candidate_crypto_hash", e);
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

uint64_t eg_selection_description_get_sequence_order(eg_selection_description_t *handle)
{
    try {
        return AS_TYPE(SelectionDescription, handle)->getSequenceOrder();
    } catch (const exception &e) {
        Log::error(":eg_selection_description_get_sequence_order", e);
        return 0;
    }
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

#pragma region ContestDescription

eg_electionguard_status_t
eg_contest_description_new(const char *in_object_id, char *in_electoral_district_id,
                           uint64_t in_sequence_order, eg_vote_variation_type_t in_vote_variation,
                           uint64_t in_number_elected, char *in_name,
                           eg_selection_description_t *in_selections[], uint64_t in_selections_size,
                           eg_contest_description_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto electoraldistrictId = string(in_electoral_district_id);
        auto name = string(in_name);

        vector<unique_ptr<SelectionDescription>> selections;
        selections.reserve(in_selections_size);
        for (auto i = 0; i < in_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            selections.push_back(std::move(annotated));
        }

        auto result = make_unique<ContestDescription>(
          objectId, electoraldistrictId, in_sequence_order, (VoteVariationType)in_vote_variation,
          in_number_elected, name, move(selections));

        *out_handle = AS_TYPE(eg_contest_description_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contest_description_new_with_title(
  const char *in_object_id, char *in_electoral_district_id, uint64_t in_sequence_order,
  eg_vote_variation_type_t in_vote_variation, uint64_t in_number_elected, uint64_t in_votes_allowed,
  char *in_name, eg_internationalized_text_t *in_ballot_title,
  eg_internationalized_text_t *in_ballot_subtitle, eg_selection_description_t *in_selections[],
  uint64_t in_selections_size, eg_contest_description_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto electoraldistrictId = string(in_electoral_district_id);
        auto name = string(in_name);

        auto ballotTitle = AS_TYPE(InternationalizedText, in_ballot_title);
        auto ballotSubTitle = AS_TYPE(InternationalizedText, in_ballot_subtitle);

        vector<unique_ptr<SelectionDescription>> selections;
        selections.reserve(in_selections_size);
        for (auto i = 0; i < in_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            selections.push_back(std::move(annotated));
        }

        auto result = make_unique<ContestDescription>(
          objectId, electoraldistrictId, in_sequence_order, (VoteVariationType)in_vote_variation,
          in_number_elected, in_votes_allowed, name,
          make_unique<InternationalizedText>(std::move(*ballotTitle)),
          make_unique<InternationalizedText>(std::move(*ballotSubTitle)), move(selections));

        *out_handle = AS_TYPE(eg_contest_description_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contest_description_new_with_parties(
  const char *in_object_id, char *in_electoral_district_id, uint64_t in_sequence_order,
  eg_vote_variation_type_t in_vote_variation, uint64_t in_number_elected, char *in_name,
  eg_selection_description_t *in_selections[], uint64_t in_selections_size,
  char *in_primary_party_ids[], uint64_t in_primary_party_ids_size,
  eg_contest_description_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto electoraldistrictId = string(in_electoral_district_id);
        auto name = string(in_name);

        vector<unique_ptr<SelectionDescription>> selections;
        selections.reserve(in_selections_size);
        for (auto i = 0; i < in_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            selections.push_back(std::move(annotated));
        }

        vector<string> partyIds;
        partyIds.reserve(in_primary_party_ids_size);
        for (auto i = 0; i < in_primary_party_ids_size; i++) {
            partyIds.push_back(in_primary_party_ids[i]);
        }

        auto result = make_unique<ContestDescription>(
          objectId, electoraldistrictId, in_sequence_order, (VoteVariationType)in_vote_variation,
          in_number_elected, name, move(selections), move(partyIds));

        *out_handle = AS_TYPE(eg_contest_description_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contest_description_new_with_title_and_parties(
  const char *in_object_id, char *in_electoral_district_id, uint64_t in_sequence_order,
  eg_vote_variation_type_t in_vote_variation, uint64_t in_number_elected, uint64_t in_votes_allowed,
  char *in_name, eg_internationalized_text_t *in_ballot_title,
  eg_internationalized_text_t *in_ballot_subtitle, eg_selection_description_t *in_selections[],
  uint64_t in_selections_size, char *in_primary_party_ids[], uint64_t in_primary_party_ids_size,
  eg_contest_description_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto electoraldistrictId = string(in_electoral_district_id);
        auto name = string(in_name);

        auto ballotTitle = AS_TYPE(InternationalizedText, in_ballot_title);
        auto ballotSubTitle = AS_TYPE(InternationalizedText, in_ballot_subtitle);

        vector<unique_ptr<SelectionDescription>> selections;
        selections.reserve(in_selections_size);
        for (auto i = 0; i < in_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            selections.push_back(std::move(annotated));
        }

        vector<string> partyIds;
        partyIds.reserve(in_primary_party_ids_size);
        for (auto i = 0; i < in_primary_party_ids_size; i++) {
            partyIds.push_back(in_primary_party_ids[i]);
        }

        auto result = make_unique<ContestDescription>(
          objectId, electoraldistrictId, in_sequence_order, (VoteVariationType)in_vote_variation,
          in_number_elected, in_votes_allowed, name,
          make_unique<InternationalizedText>(std::move(*ballotTitle)),
          make_unique<InternationalizedText>(std::move(*ballotSubTitle)), move(selections),
          move(partyIds));

        *out_handle = AS_TYPE(eg_contest_description_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contest_description_free(eg_contest_description_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ContestDescription,
                   handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_contest_description_get_object_id(eg_contest_description_t *handle,
                                                               const char **out_object_id)
{
    try {
        auto result = AS_TYPE(ContestDescription, handle)->getObjectId();
        *out_object_id = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_get_object_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_contest_description_get_electoral_district_id(eg_contest_description_t *handle,
                                                 const char **out_electoral_district_id)
{
    try {
        auto result = AS_TYPE(ContestDescription, handle)->getElectoralDistrictId();
        *out_electoral_district_id = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_get_electoral_district_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

uint64_t eg_contest_description_get_sequence_order(eg_contest_description_t *handle)
{
    return AS_TYPE(ContestDescription, handle)->getSequenceOrder();
}

eg_vote_variation_type_t eg_contest_description_get_vote_variation(eg_contest_description_t *handle)
{
    return (eg_vote_variation_type_t)AS_TYPE(ContestDescription, handle)->getVoteVariation();
}

uint64_t eg_contest_description_get_number_elected(eg_contest_description_t *handle)
{
    return AS_TYPE(ContestDescription, handle)->getNumberElected();
}

uint64_t eg_contest_description_get_votes_allowed(eg_contest_description_t *handle)
{
    return AS_TYPE(ContestDescription, handle)->getVotesAllowed();
}

eg_electionguard_status_t eg_contest_description_get_name(eg_contest_description_t *handle,
                                                          const char **out_name)
{
    try {
        auto result = AS_TYPE(ContestDescription, handle)->getName();
        *out_name = dynamicCopy(result);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_get_name", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_contest_description_get_ballot_title(eg_contest_description_t *handle,
                                        eg_internationalized_text_t **out_ballot_title_ref)
{
    try {
        auto result = AS_TYPE(ContestDescription, handle)->getBallotTitle();
        *out_ballot_title_ref = AS_TYPE(eg_internationalized_text_t, result);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_get_ballot_title", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_contest_description_get_ballot_subtitle(eg_contest_description_t *handle,
                                           eg_internationalized_text_t **out_ballot_subtitle_ref)
{
    try {
        auto result = AS_TYPE(ContestDescription, handle)->getBallotSubtitle();
        *out_ballot_subtitle_ref = AS_TYPE(eg_internationalized_text_t, result);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_get_ballot_subtitle", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_contest_description_get_selections_size(eg_contest_description_t *handle)
{
    return AS_TYPE(ContestDescription, handle)->getSelections().size();
}

eg_electionguard_status_t
eg_contest_description_get_selection_at_index(eg_contest_description_t *handle, size_t in_index,
                                              eg_selection_description_t **out_selection_ref)
{
    try {
        auto collection = AS_TYPE(ContestDescription, handle)->getSelections();
        auto *reference = &collection.at(in_index).get();

        *out_selection_ref = AS_TYPE(eg_selection_description_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_contest_description_get_selection_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contest_description_crypto_hash(eg_contest_description_t *handle,
                                                             eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto result = AS_TYPE(ContestDescription, handle)->crypto_hash();
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region ContestDescriptionWithPlaceholders

eg_electionguard_status_t eg_contest_description_with_placeholders_new(
  const char *in_object_id, char *in_electoral_district_id, uint64_t in_sequence_order,
  eg_vote_variation_type_t in_vote_variation, uint64_t in_number_elected, char *in_name,
  eg_selection_description_t *in_selections[], uint64_t in_selections_size,
  eg_selection_description_t *in_placeholder_selections[], uint64_t in_placeholder_selections_size,
  eg_contest_description_with_placeholders_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto electoraldistrictId = string(in_electoral_district_id);
        auto name = string(in_name);

        vector<unique_ptr<SelectionDescription>> selections;
        selections.reserve(in_selections_size);
        for (auto i = 0; i < in_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            selections.push_back(std::move(annotated));
        }

        vector<unique_ptr<SelectionDescription>> placeholders;
        placeholders.reserve(in_placeholder_selections_size);
        for (auto i = 0; i < in_placeholder_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_placeholder_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            placeholders.push_back(std::move(annotated));
        }

        auto result = make_unique<ContestDescriptionWithPlaceholders>(
          objectId, electoraldistrictId, in_sequence_order, (VoteVariationType)in_vote_variation,
          in_number_elected, name, move(selections), move(placeholders));

        *out_handle = AS_TYPE(eg_contest_description_with_placeholders_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_with_placeholders_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contest_description_with_placeholders_new_with_title(
  const char *in_object_id, char *in_electoral_district_id, uint64_t in_sequence_order,
  eg_vote_variation_type_t in_vote_variation, uint64_t in_number_elected, uint64_t in_votes_allowed,
  char *in_name, eg_internationalized_text_t *in_ballot_title,
  eg_internationalized_text_t *in_ballot_subtitle, eg_selection_description_t *in_selections[],
  uint64_t in_selections_size, eg_selection_description_t *in_placeholder_selections[],
  uint64_t in_placeholder_selections_size, eg_contest_description_with_placeholders_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto electoraldistrictId = string(in_electoral_district_id);
        auto name = string(in_name);

        auto ballotTitle = AS_TYPE(InternationalizedText, in_ballot_title);
        auto ballotSubTitle = AS_TYPE(InternationalizedText, in_ballot_subtitle);

        vector<unique_ptr<SelectionDescription>> selections;
        selections.reserve(in_selections_size);
        for (auto i = 0; i < in_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            selections.push_back(std::move(annotated));
        }

        vector<unique_ptr<SelectionDescription>> placeholders;
        placeholders.reserve(in_placeholder_selections_size);
        for (auto i = 0; i < in_placeholder_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_placeholder_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            placeholders.push_back(std::move(annotated));
        }

        auto result = make_unique<ContestDescriptionWithPlaceholders>(
          objectId, electoraldistrictId, in_sequence_order, (VoteVariationType)in_vote_variation,
          in_number_elected, in_votes_allowed, name,
          make_unique<InternationalizedText>(std::move(*ballotTitle)),
          make_unique<InternationalizedText>(std::move(*ballotSubTitle)), move(selections),
          move(placeholders));

        *out_handle = AS_TYPE(eg_contest_description_with_placeholders_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_with_placeholders_new_with_title", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contest_description_with_placeholders_new_with_parties(
  const char *in_object_id, char *in_electoral_district_id, uint64_t in_sequence_order,
  eg_vote_variation_type_t in_vote_variation, uint64_t in_number_elected, char *in_name,
  eg_selection_description_t *in_selections[], uint64_t in_selections_size,
  char *in_primary_party_ids[], uint64_t in_primary_party_ids_size,
  eg_selection_description_t *in_placeholder_selections[], uint64_t in_placeholder_selections_size,
  eg_contest_description_with_placeholders_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto electoraldistrictId = string(in_electoral_district_id);
        auto name = string(in_name);

        vector<unique_ptr<SelectionDescription>> selections;
        selections.reserve(in_selections_size);
        for (auto i = 0; i < in_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            selections.push_back(std::move(annotated));
        }

        vector<unique_ptr<SelectionDescription>> placeholders;
        placeholders.reserve(in_placeholder_selections_size);
        for (auto i = 0; i < in_placeholder_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_placeholder_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            placeholders.push_back(std::move(annotated));
        }

        vector<string> partyIds;
        partyIds.reserve(in_primary_party_ids_size);
        for (auto i = 0; i < in_primary_party_ids_size; i++) {
            partyIds.push_back(in_primary_party_ids[i]);
        }

        auto result = make_unique<ContestDescriptionWithPlaceholders>(
          objectId, electoraldistrictId, in_sequence_order, (VoteVariationType)in_vote_variation,
          in_number_elected, name, move(selections), move(partyIds), move(placeholders));

        *out_handle = AS_TYPE(eg_contest_description_with_placeholders_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_with_placeholders_new_with_title_and_parties", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_contest_description_with_placeholders_new_with_title_and_parties(
  const char *in_object_id, char *in_electoral_district_id, uint64_t in_sequence_order,
  eg_vote_variation_type_t in_vote_variation, uint64_t in_number_elected, uint64_t in_votes_allowed,
  char *in_name, eg_internationalized_text_t *in_ballot_title,
  eg_internationalized_text_t *in_ballot_subtitle, eg_selection_description_t *in_selections[],
  uint64_t in_selections_size, char *in_primary_party_ids[], uint64_t in_primary_party_ids_size,
  eg_selection_description_t *in_placeholder_selections[], uint64_t in_placeholder_selections_size,
  eg_contest_description_with_placeholders_t **out_handle)
{
    try {
        auto objectId = string(in_object_id);
        auto electoraldistrictId = string(in_electoral_district_id);
        auto name = string(in_name);

        auto ballotTitle = AS_TYPE(InternationalizedText, in_ballot_title);
        auto ballotSubTitle = AS_TYPE(InternationalizedText, in_ballot_subtitle);

        vector<unique_ptr<SelectionDescription>> selections;
        selections.reserve(in_selections_size);
        for (auto i = 0; i < in_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            selections.push_back(std::move(annotated));
        }

        vector<unique_ptr<SelectionDescription>> placeholders;
        placeholders.reserve(in_placeholder_selections_size);
        for (auto i = 0; i < in_placeholder_selections_size; i++) {
            auto *element = AS_TYPE(SelectionDescription, in_placeholder_selections[i]);
            unique_ptr<SelectionDescription> annotated(
              new SelectionDescription(std::move(*element)));
            placeholders.push_back(std::move(annotated));
        }

        vector<string> partyIds;
        partyIds.reserve(in_primary_party_ids_size);
        for (auto i = 0; i < in_primary_party_ids_size; i++) {
            partyIds.push_back(in_primary_party_ids[i]);
        }

        auto result = make_unique<ContestDescriptionWithPlaceholders>(
          objectId, electoraldistrictId, in_sequence_order, (VoteVariationType)in_vote_variation,
          in_number_elected, in_votes_allowed, name,
          make_unique<InternationalizedText>(std::move(*ballotTitle)),
          make_unique<InternationalizedText>(std::move(*ballotSubTitle)), move(selections),
          move(partyIds), move(placeholders));

        *out_handle = AS_TYPE(eg_contest_description_with_placeholders_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_contest_description_with_placeholders_new_with_title_and_parties", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_contest_description_with_placeholders_free(eg_contest_description_with_placeholders_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ContestDescriptionWithPlaceholders,
                   handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

size_t eg_contest_description_with_placeholders_get_placeholders_size(
  eg_contest_description_with_placeholders_t *handle)
{
    return AS_TYPE(ContestDescriptionWithPlaceholders, handle)->getPlaceholders().size();
}

eg_electionguard_status_t eg_contest_description_with_placeholders_get_placeholder_at_index(
  eg_contest_description_with_placeholders_t *handle, size_t in_index,
  eg_contest_description_with_placeholders_t **out_placeholder_ref)
{
    try {
        auto collection = AS_TYPE(ContestDescriptionWithPlaceholders, handle)->getPlaceholders();
        auto *reference = &collection.at(in_index).get();

        *out_placeholder_ref = AS_TYPE(eg_contest_description_with_placeholders_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_contest_description_with_placeholders_get_placeholder_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

bool eg_contest_description_with_placeholders_is_placeholder(
  eg_contest_description_with_placeholders_t *handle, eg_selection_description_t *in_selection)
{
    auto selection = AS_TYPE(SelectionDescription, in_selection);
    return AS_TYPE(ContestDescriptionWithPlaceholders, handle)->IsPlaceholder(*selection);
}

eg_electionguard_status_t eg_contest_description_with_placeholders_selection_for_id(
  eg_contest_description_with_placeholders_t *handle, char *in_selection_id,
  eg_selection_description_t **out_selection_ref)
{
    try {
        auto selectionId = string(in_selection_id);
        auto element =
          AS_TYPE(ContestDescriptionWithPlaceholders, handle)->selectionFor(selectionId);

        *out_selection_ref = AS_TYPE(eg_selection_description_t, &element.get());

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_contest_description_with_placeholders_selection_for_id", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region Manifest

eg_electionguard_status_t
eg_election_manifest_new(char *in_election_scope_id, eg_election_type_t in_election_type,
                         uint64_t in_start_date, uint64_t in_end_date,
                         eg_geopolitical_unit_t *in_geopolitical_units[],
                         uint64_t in_geopolitical_units_size, eg_party_t *in_parties[],
                         uint64_t in_parties_size, eg_candidate_t *in_candidates[],
                         uint64_t in_candidates_size, eg_contest_description_t *in_contests[],
                         uint64_t in_contests_size, eg_ballot_style_t *in_ballot_styles[],
                         uint64_t in_ballot_styles_size, eg_election_manifest_t **out_handle)
{
    try {
        auto scopeId = string(in_election_scope_id);

        std::chrono::system_clock::time_point startDate(std::chrono::milliseconds{in_start_date});
        std::chrono::system_clock::time_point endDate(std::chrono::milliseconds{in_end_date});

        vector<unique_ptr<GeopoliticalUnit>> gpUnits;
        gpUnits.reserve(in_geopolitical_units_size);
        for (auto i = 0; i < in_geopolitical_units_size; i++) {
            auto *element = AS_TYPE(GeopoliticalUnit, in_geopolitical_units[i]);
            unique_ptr<GeopoliticalUnit> annotated(new GeopoliticalUnit(std::move(*element)));
            gpUnits.push_back(std::move(annotated));
        }

        vector<unique_ptr<Party>> parties;
        parties.reserve(in_parties_size);
        for (auto i = 0; i < in_parties_size; i++) {
            auto *element = AS_TYPE(Party, in_parties[i]);
            unique_ptr<Party> annotated(new Party(std::move(*element)));
            parties.push_back(std::move(annotated));
        }

        vector<unique_ptr<Candidate>> candidates;
        candidates.reserve(in_candidates_size);
        for (auto i = 0; i < in_candidates_size; i++) {
            auto *element = AS_TYPE(Candidate, in_candidates[i]);
            unique_ptr<Candidate> annotated(new Candidate(std::move(*element)));
            candidates.push_back(std::move(annotated));
        }

        vector<unique_ptr<ContestDescription>> contests;
        contests.reserve(in_contests_size);
        for (auto i = 0; i < in_contests_size; i++) {
            auto *element = AS_TYPE(ContestDescription, in_contests[i]);
            unique_ptr<ContestDescription> annotated(new ContestDescription(std::move(*element)));
            contests.push_back(std::move(annotated));
        }

        vector<unique_ptr<BallotStyle>> ballotStyles;
        ballotStyles.reserve(in_ballot_styles_size);
        for (auto i = 0; i < in_ballot_styles_size; i++) {
            auto *element = AS_TYPE(BallotStyle, in_ballot_styles[i]);
            unique_ptr<BallotStyle> annotated(new BallotStyle(std::move(*element)));
            ballotStyles.push_back(std::move(annotated));
        }

        auto result = make_unique<Manifest>(scopeId, (ElectionType)in_election_type, startDate,
                                            endDate, move(gpUnits), move(parties), move(candidates),
                                            move(contests), move(ballotStyles));

        *out_handle = AS_TYPE(eg_election_manifest_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_manifest_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_manifest_new_with_contact(
  char *in_election_scope_id, eg_election_type_t in_election_type, uint64_t in_start_date,
  uint64_t in_end_date, eg_geopolitical_unit_t *in_geopolitical_units[],
  uint64_t in_geopolitical_units_size, eg_party_t *in_parties[], uint64_t in_parties_size,
  eg_candidate_t *in_candidates[], uint64_t in_candidates_size,
  eg_contest_description_t *in_contests[], uint64_t in_contests_size,
  eg_ballot_style_t *in_ballot_styles[], uint64_t in_ballot_styles_size,
  eg_internationalized_text_t *in_name, eg_contact_information_t *in_contact_info,
  eg_election_manifest_t **out_handle)
{
    try {
        auto scopeId = string(in_election_scope_id);
        std::chrono::system_clock::time_point startDate(std::chrono::milliseconds{in_start_date});
        std::chrono::system_clock::time_point endDate(std::chrono::milliseconds{in_end_date});

        vector<unique_ptr<GeopoliticalUnit>> gpUnits;
        gpUnits.reserve(in_geopolitical_units_size);
        for (auto i = 0; i < in_geopolitical_units_size; i++) {
            auto *element = AS_TYPE(GeopoliticalUnit, in_geopolitical_units[i]);
            unique_ptr<GeopoliticalUnit> annotated(new GeopoliticalUnit(std::move(*element)));
            gpUnits.push_back(std::move(annotated));
        }

        vector<unique_ptr<Party>> parties;
        parties.reserve(in_parties_size);
        for (auto i = 0; i < in_parties_size; i++) {
            auto *element = AS_TYPE(Party, in_parties[i]);
            unique_ptr<Party> annotated(new Party(std::move(*element)));
            parties.push_back(std::move(annotated));
        }

        vector<unique_ptr<Candidate>> candidates;
        candidates.reserve(in_candidates_size);
        for (auto i = 0; i < in_candidates_size; i++) {
            auto *element = AS_TYPE(Candidate, in_candidates[i]);
            unique_ptr<Candidate> annotated(new Candidate(std::move(*element)));
            candidates.push_back(std::move(annotated));
        }

        vector<unique_ptr<ContestDescription>> contests;
        contests.reserve(in_contests_size);
        for (auto i = 0; i < in_contests_size; i++) {
            auto *element = AS_TYPE(ContestDescription, in_contests[i]);
            unique_ptr<ContestDescription> annotated(new ContestDescription(std::move(*element)));
            contests.push_back(std::move(annotated));
        }

        vector<unique_ptr<BallotStyle>> ballotStyles;
        ballotStyles.reserve(in_ballot_styles_size);
        for (auto i = 0; i < in_ballot_styles_size; i++) {
            auto *element = AS_TYPE(BallotStyle, in_ballot_styles[i]);
            unique_ptr<BallotStyle> annotated(new BallotStyle(std::move(*element)));
            ballotStyles.push_back(std::move(annotated));
        }

        auto name = AS_TYPE(InternationalizedText, in_name);
        auto contactInfo = AS_TYPE(ContactInformation, in_contact_info);

        auto result = make_unique<Manifest>(
          scopeId, (ElectionType)in_election_type, startDate, endDate, move(gpUnits), move(parties),
          move(candidates), move(contests), move(ballotStyles),
          make_unique<InternationalizedText>(std::move(*name)),
          make_unique<ContactInformation>(std::move(*contactInfo)));

        *out_handle = AS_TYPE(eg_election_manifest_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_manifest_new_with_contact", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

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

eg_election_type_t eg_election_manifest_get_type(eg_election_manifest_t *handle)
{
    return (eg_election_type_t)AS_TYPE(Manifest, handle)->getElectionType();
}

uint64_t eg_election_manifest_get_start_date(eg_election_manifest_t *handle)
{
    // TODO:
    return 0;
}

uint64_t eg_election_manifest_get_end_date(eg_election_manifest_t *handle)
{
    // TODO:
    return 0;
}

size_t eg_election_manifest_get_geopolitical_units_size(eg_election_manifest_t *handle)
{
    return AS_TYPE(Manifest, handle)->getGeopoliticalUnits().size();
}

eg_electionguard_status_t
eg_election_manifest_get_geopolitical_unit_at_index(eg_election_manifest_t *handle, size_t in_index,
                                                    eg_geopolitical_unit_t **out_gp_unit_ref)
{
    try {
        auto collection = AS_TYPE(Manifest, handle)->getGeopoliticalUnits();
        auto *reference = &collection.at(in_index).get();

        *out_gp_unit_ref = AS_TYPE(eg_geopolitical_unit_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_election_manifest_get_geopolitical_unit_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_election_manifest_get_parties_size(eg_election_manifest_t *handle)
{
    return AS_TYPE(Manifest, handle)->getParties().size();
}

eg_electionguard_status_t eg_election_manifest_get_party_at_index(eg_election_manifest_t *handle,
                                                                  size_t in_index,
                                                                  eg_party_t **out_party_ref)
{
    try {
        auto collection = AS_TYPE(Manifest, handle)->getParties();
        auto *reference = &collection.at(in_index).get();

        *out_party_ref = AS_TYPE(eg_party_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_election_manifest_get_party_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_election_manifest_get_candidates_size(eg_election_manifest_t *handle)
{
    return AS_TYPE(Manifest, handle)->getCandidates().size();
}

eg_electionguard_status_t
eg_election_manifest_get_candidate_at_index(eg_election_manifest_t *handle, size_t in_index,
                                            eg_candidate_t **out_candidate_ref)
{
    try {
        auto collection = AS_TYPE(Manifest, handle)->getCandidates();
        auto *reference = &collection.at(in_index).get();

        *out_candidate_ref = AS_TYPE(eg_candidate_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_election_manifest_get_candidate_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_election_manifest_get_contests_size(eg_election_manifest_t *handle)
{
    return AS_TYPE(Manifest, handle)->getContests().size();
}

eg_electionguard_status_t
eg_election_manifest_get_contest_at_index(eg_election_manifest_t *handle, size_t in_index,
                                          eg_contest_description_t **out_contest_ref)
{
    try {
        auto collection = AS_TYPE(Manifest, handle)->getContests();
        auto *reference = &collection.at(in_index).get();

        *out_contest_ref = AS_TYPE(eg_contest_description_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_election_manifest_get_contest_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_election_manifest_get_ballot_styles_size(eg_election_manifest_t *handle)
{
    return AS_TYPE(Manifest, handle)->getBallotStyles().size();
}

eg_electionguard_status_t
eg_election_manifest_get_ballot_style_at_index(eg_election_manifest_t *handle, size_t in_index,
                                               eg_ballot_style_t **out_ballot_style_ref)
{
    try {
        auto collection = AS_TYPE(Manifest, handle)->getBallotStyles();
        auto *reference = &collection.at(in_index).get();

        *out_ballot_style_ref = AS_TYPE(eg_ballot_style_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_election_manifest_get_ballot_style_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_election_manifest_get_name(eg_election_manifest_t *handle,
                                                        eg_internationalized_text_t **out_name_ref)
{
    try {
        auto element = AS_TYPE(Manifest, handle)->getName();

        *out_name_ref = AS_TYPE(eg_internationalized_text_t, element);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_election_manifest_get_name", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_election_manifest_get_contact_info(eg_election_manifest_t *handle,
                                      eg_contact_information_t **out_contact_info_ref)
{
    try {
        auto element = AS_TYPE(Manifest, handle)->getContactInformation();

        *out_contact_info_ref = AS_TYPE(eg_contact_information_t, element);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_election_manifest_get_contact_info", e);
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

bool eg_election_manifest_is_valid(eg_election_manifest_t *handle)
{
    try {
        auto result = AS_TYPE(Manifest, handle)->isValid();
        return result;
    } catch (const exception &e) {
        Log::error(":eg_election_manifest_is_valid", e);
        return false;
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

eg_electionguard_status_t eg_election_manifest_from_msgpack(uint8_t *in_data, uint64_t in_length,
                                                            eg_election_manifest_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto result = Manifest::fromMsgPack(data_bytes);

        *out_handle = AS_TYPE(eg_election_manifest_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_manifest_from_msgpack", e);
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

eg_electionguard_status_t eg_election_manifest_to_msgpack(eg_election_manifest_t *handle,
                                                          uint8_t **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(Manifest, handle);
        auto result = domain_type->toMsgPack();

        size_t size = 0;
        *out_data = dynamicCopy(result, &size);
        *out_size = size;

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_election_manifest_to_msgpack", e);
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

size_t eg_internal_manifest_get_geopolitical_units_size(eg_internal_manifest_t *handle)
{
    return AS_TYPE(InternalManifest, handle)->getGeopoliticalUnits().size();
}

eg_electionguard_status_t
eg_internal_manifest_get_geopolitical_unit_at_index(eg_internal_manifest_t *handle, size_t in_index,
                                                    eg_geopolitical_unit_t **out_gp_unit_ref)
{
    try {
        auto collection = AS_TYPE(InternalManifest, handle)->getGeopoliticalUnits();
        auto *reference = &collection.at(in_index).get();

        *out_gp_unit_ref = AS_TYPE(eg_geopolitical_unit_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_internal_manifest_get_geopolitical_unit_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_internal_manifest_get_contests_size(eg_internal_manifest_t *handle)
{
    return AS_TYPE(InternalManifest, handle)->getContests().size();
}

eg_electionguard_status_t eg_internal_manifest_get_contest_at_index(
  eg_internal_manifest_t *handle, size_t in_index,
  eg_contest_description_with_placeholders_t **out_contest_ref)
{
    try {
        auto collection = AS_TYPE(InternalManifest, handle)->getContests();
        auto *reference = &collection.at(in_index).get();

        *out_contest_ref = AS_TYPE(eg_contest_description_with_placeholders_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_internal_manifest_get_contest_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

size_t eg_internal_manifest_get_ballot_styles_size(eg_internal_manifest_t *handle)
{
    return AS_TYPE(InternalManifest, handle)->getBallotStyles().size();
}

eg_electionguard_status_t
eg_internal_manifest_get_ballot_style_at_index(eg_internal_manifest_t *handle, size_t in_index,
                                               eg_ballot_style_t **out_ballot_style_ref)
{
    try {
        auto collection = AS_TYPE(InternalManifest, handle)->getBallotStyles();
        auto *reference = &collection.at(in_index).get();

        *out_ballot_style_ref = AS_TYPE(eg_ballot_style_t, reference);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_internal_manifest_get_ballot_style_at_index", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t
eg_internal_manifest_get_ballot_style(eg_internal_manifest_t *handle, char *in_ballot_style_id,
                                      eg_ballot_style_t **out_ballot_style_ref)
{
    try {
        auto element =
          AS_TYPE(InternalManifest, handle)->getBallotStyle(string(in_ballot_style_id));

        *out_ballot_style_ref = AS_TYPE(eg_ballot_style_t, element);

        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error("eg_internal_manifest_get_ballot_style", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
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

eg_electionguard_status_t eg_internal_manifest_from_msgpack(uint8_t *in_data, uint64_t in_length,
                                                            eg_internal_manifest_t **out_handle)
{
    try {
        auto data_bytes = vector<uint8_t>(in_data, in_data + in_length);
        auto result = InternalManifest::fromMsgPack(data_bytes);

        *out_handle = AS_TYPE(eg_internal_manifest_t, result.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_internal_manifest_from_msgpack", e);
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

eg_electionguard_status_t eg_internal_manifest_to_msgpack(eg_internal_manifest_t *handle,
                                                          uint8_t **out_data, size_t *out_size)
{
    try {
        auto *domain_type = AS_TYPE(InternalManifest, handle);
        auto result = domain_type->toMsgPack();

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
