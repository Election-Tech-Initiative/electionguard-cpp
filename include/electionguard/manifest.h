/// @file election.h
#ifndef __ELECTIONGUARD_CPP_MANIFEST_H_INCLUDED__
#define __ELECTIONGUARD_CPP_MANIFEST_H_INCLUDED__

#include "export.h"
#include "group.h"
#include "status.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ElectionType

/**
 * Enumeration for the type of ElectionType
 * see: https://developers.google.com/elections-data/reference/election-type
 */
typedef enum eg_election_type_e {
    ELECTIONGUARD_ELECTION_TYPE_UNKNOWN = 0,
    ELECTIONGUARD_ELECTION_TYPE_GENERAL = 1,
    ELECTIONGUARD_ELECTION_TYPE_PARTISAN_PRIMARY_CLOSED = 2,
    ELECTIONGUARD_ELECTION_TYPE_PARTISAN_PRIMARY_OPEN = 3,
    ELECTIONGUARD_ELECTION_TYPE_PRIMARY = 4,
    ELECTIONGUARD_ELECTION_TYPE_RUNOFF = 5,
    ELECTIONGUARD_ELECTION_TYPE_SPECIAL = 6,
    ELECTIONGUARD_ELECTION_TYPE_OTHER = 7,
} eg_election_type_t;

#endif

#ifndef ReportingUnitType

/**
 * Enumeration for the type of geopolitical unit
 * see: https://developers.google.com/elections-data/reference/reporting-unit-type
 */
typedef enum eg_reporting_unit_type_e {
    ELECTIONGUARD_REPORTING_UNIT_TYPE_UNKNOWN = 0,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_BALLOT_BATCH = 1,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_BALLOT_STYLE_AREA = 2,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_BOROUGH = 3,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_CITY = 4,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_CITY_COUNCIL = 5,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_COMBINED_PRECINCT = 6,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_CONGRESSIONAL = 7,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_COUNTRY = 8,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_COUNTY = 9,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_COUNTY_COUNCIL = 10,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_DROP_BOX = 11,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_JUDICIAL = 12,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_MUNICIPALITY = 13,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_POLLING_PLACE = 14,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_PRECINCT = 15,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_SCHOOL = 16,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_SPECIAL = 17,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_SPLIT_PRECINCT = 18,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_STATE = 19,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_STATE_HOUSE = 20,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_STATE_SENATE = 21,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_TOWNSHIP = 22,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_UTILITY = 23,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_VILLAGE = 24,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_VOTE_CENTER = 25,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_WARD = 26,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_WATER = 27,
    ELECTIONGUARD_REPORTING_UNIT_TYPE_OTHER = 28
} eg_reporting_unit_type_t;

#endif

#ifndef VoteVariationType

/**
 * Enumeration for the type of VoteVariationType
 * see: https://developers.google.com/elections-data/reference/vote-variation
 */
typedef enum eg_vote_variation_type_e {
    ELECTIONGUARD_VOTE_VARIATION_TYPE_UNKNOWN = 0,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_ONE_OF_M = 1,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_APPROVAL = 2,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_BORDA = 3,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_CUMULATIVE = 4,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_MAJORITY = 5,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_N_OF_M = 6,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_PLURALITY = 7,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_PROPORTIONAL = 8,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_RANGE = 9,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_RCV = 10,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_SUPER_MAJORITY = 11,
    ELECTIONGUARD_VOTE_VARIATION_TYPE_OTHER = 12
} eg_vote_variation_type_t;

#endif

#ifndef AnnotatedString

struct eg_annotated_string_s;

/**
 * Use this as a type for character strings.
 * See: https://developers.google.com/elections-data/reference/annotated-string
 */
typedef struct eg_annotated_string_s eg_annotated_string_t;

EG_API eg_electionguard_status_t eg_annotated_string_new(const char *in_annotation,
                                                         const char *in_value,
                                                         eg_annotated_string_t **out_handle);

EG_API eg_electionguard_status_t eg_annotated_string_free(eg_annotated_string_t *handle);

/**
 * An annotation of up to 16 characters that's associated with a character string.
 */
EG_API eg_electionguard_status_t eg_annotated_string_get_annotation(eg_annotated_string_t *handle,
                                                                    const char **out_annotation);

/**
 * The character string
 */
EG_API eg_electionguard_status_t eg_annotated_string_get_value(eg_annotated_string_t *handle,
                                                               const char **out_value);

/**
 * A hash representation of the object. Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_annotated_string_crypto_hash(
  eg_annotated_string_t *handle, eg_element_mod_q_t **out_crypto_hash);

#endif

#ifndef Language

struct eg_language_s;

/**
 * The ISO-639 language
 * see: https://en.wikipedia.org/wiki/ISO_639
 * see: https://developers.google.com/civics-data/reference/internationalized-text#language-string
 */
typedef struct eg_language_s eg_language_t;

EG_API eg_electionguard_status_t eg_language_new(const char *in_value, const char *in_language,
                                                 eg_language_t **out_handle);

EG_API eg_electionguard_status_t eg_language_free(eg_language_t *handle);

/**
 * Identifies the language
 */
EG_API eg_electionguard_status_t eg_language_get_language(eg_language_t *handle,
                                                          char **out_annotation);

/**
 * the value
 */
EG_API eg_electionguard_status_t eg_language_get_value(eg_language_t *handle, char **out_value);

/**
 * A hash representation of the object.  Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_language_crypto_hash(eg_language_t *handle,
                                                         eg_element_mod_q_t **out_crypto_hash);

#endif

#ifndef InternationalizedText

struct eg_internationalized_text_s;

/**
 * Data entity used to represent multi-national text. Use when text on a ballot contains multi-national text.
 * See: https://developers.google.com/elections-data/reference/internationalized-text
 */
typedef struct eg_internationalized_text_s eg_internationalized_text_t;

EG_API eg_electionguard_status_t eg_internationalized_text_new(
  eg_language_t *in_text[], const size_t in_text_size, eg_internationalized_text_t **out_handle);

EG_API eg_electionguard_status_t
eg_internationalized_text_free(eg_internationalized_text_t *handle);

/**
 * Get the Size of the text collection
 */
EG_API size_t eg_internationalized_text_get_text_size(eg_internationalized_text_t *handle);

/**
 * Get a text at a specific index.
 * 
 * @param[in] in_index The index of the text
 * @param[out] out_text_ref An opaque pointer to the text.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_internationalized_text_get_text_at_index(
  eg_internationalized_text_t *handle, size_t in_index, eg_language_t **out_text_ref);

/**
 * A hash representation of the object. Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t
eg_intertnationalized_text_crypto_hash(eg_language_t *handle, eg_element_mod_q_t **out_crypto_hash);

#endif

#ifndef ContactInformation

struct eg_contact_information_s;

/**
* For defining contact information about objects such as persons, boards of authorities, and organizations.
*
* Contact Information values are not used internally by ElectionGuard when encrypting ballots
* but are included for checking the validity of a supplied manifest.
*
* See: https://developers.google.com/elections-data/reference/contact-information
*/
typedef struct eg_contact_information_s eg_contact_information_t;

EG_API eg_electionguard_status_t eg_contact_information_new(char *in_name,
                                                            eg_contact_information_t **out_handle);

EG_API eg_electionguard_status_t eg_contact_information_new_with_collections(
  char *in_name, char *in_address_line[], const size_t in_address_line_size,
  eg_annotated_string_t *in_email_line[], const size_t in_email_line_size,
  eg_annotated_string_t *in_phone_line[], const size_t in_phone_line_size,
  eg_contact_information_t **out_handle);

EG_API eg_electionguard_status_t eg_contact_information_free(eg_contact_information_t *handle);

/**
 * Get the Size of the address collection
 */
EG_API size_t eg_contact_information_get_address_line_size(eg_contact_information_t *handle);

/**
 * Get an address line at a specific index.
 * 
 *  Associates an address with the contact.
 * AddressLine needs to contain the lines that someone would
 * enter into a web mapping service to find the address on a map.
 * That is, the value of the field needs to geocode the contact location.
 * 
 * @param[in] in_index The index of the text
 * @param[out] out_text_ref An opaque pointer to the text.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_contact_information_get_address_line_at_index(
  eg_contact_information_t *handle, size_t in_index, char **out_address);

/**
 * Get the Size of the email line collection
 */
EG_API size_t eg_contact_information_get_email_line_size(eg_contact_information_t *handle);

/**
 * Get an email line at a specific index.
 * 
 *  Associates an address with the contact.
 * AddressLine needs to contain the lines that someone would
 * enter into a web mapping service to find the address on a map.
 * That is, the value of the field needs to geocode the contact location.
 * 
 * @param[in] in_index The index of the text
 * @param[out] out_text_ref An opaque pointer to the text.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_contact_information_get_email_line_at_index(
  eg_contact_information_t *handle, size_t in_index, eg_internationalized_text_t **out_address_ref);

/**
 * Get the Size of the phone line collection
 */
EG_API size_t eg_contact_information_get_phone_line_size(eg_contact_information_t *handle);

/**
 * Get a phone line at a specific index.
 * 
 *  Associates an address with the contact.
 * AddressLine needs to contain the lines that someone would
 * enter into a web mapping service to find the address on a map.
 * That is, the value of the field needs to geocode the contact location.
 * 
 * @param[in] in_index The index of the text
 * @param[out] out_text_ref An opaque pointer to the text.  
 *                          The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_contact_information_get_phone_line_at_index(
  eg_contact_information_t *handle, size_t in_index, eg_internationalized_text_t **out_phone_ref);

EG_API eg_electionguard_status_t eg_contact_information_get_name(eg_contact_information_t *handle,
                                                                 const char **out_name);

/**
 * A hash representation of the object. Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_contact_information_crypto_hash(
  eg_contact_information_t *handle, eg_element_mod_q_t **out_crypto_hash);

#endif

#ifndef GeopoliticalUnit

struct eg_geopolitical_unit_s;

/**
* A geopolitical unit describes any physical or
* virtual unit of representation or vote/seat aggregation.
* Use this entity for defining geopolitical units such as
* cities, districts, jurisdictions, or precincts,
* for the purpose of associating contests, offices, vote counts,
* or other information with the geographies.
*
* Geopolitical Units are not used when encrypting ballots but are required by
* ElectionGuard to determine the validity of ballot styles.
*
* See: https://developers.google.com/elections-data/reference/gp-unit
*/
typedef struct eg_geopolitical_unit_s eg_geopolitical_unit_t;

EG_API eg_electionguard_status_t eg_geopolitical_unit_new(
  char *in_object_id, char *in_name, eg_reporting_unit_type_t in_reporting_unit_type,
  eg_geopolitical_unit_t **out_handle);

EG_API eg_electionguard_status_t eg_geopolitical_unit_new_with_contact_info(
  char *in_object_id, char *in_name, eg_reporting_unit_type_t in_reporting_unit_type,
  eg_contact_information_t *in_contact_information, eg_geopolitical_unit_t **out_handle);

EG_API eg_electionguard_status_t eg_geopolitical_unit_free(eg_geopolitical_unit_t *handle);

EG_API eg_electionguard_status_t eg_geopolitical_unit_get_object_id(eg_geopolitical_unit_t *handle,
                                                                    const char **out_object_id);

EG_API eg_electionguard_status_t eg_geopolitical_unit_get_name(eg_geopolitical_unit_t *handle,
                                                               const char **out_name);

EG_API eg_reporting_unit_type_t get_geopolitical_unit_get_type(eg_geopolitical_unit_t *handle);

/**
 * A hash representation of the object. Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_geopolitical_unit_crypto_hash(
  eg_geopolitical_unit_t *handle, eg_element_mod_q_t **out_crypto_hash);

#endif

#ifndef BallotStyle

struct eg_ballot_style_s;

/*
* A BallotStyle works as a key to uniquely specify a set of contests. See also `ContestDescription`.
*
* For ElectionGuard, each contest is associated with a specific geopolitical unit,
* and each ballot style is associated with at least one geopolitical unit.
*
* It is up to the consuming application to determine how to interpreit the relationships
* between these entity types.
*/
typedef struct eg_ballot_style_s eg_ballot_style_t;

EG_API eg_electionguard_status_t eg_ballot_style_new(char *in_object_id,
                                                     char *in_geopolitical_unit_ids[],
                                                     const size_t in_geopolitical_unit_ids_size,
                                                     eg_ballot_style_t **out_handle);

EG_API eg_electionguard_status_t eg_ballot_style_new_with_parties(
  char *in_object_id, char *in_geopolitical_unit_ids[], const size_t in_geopolitical_unit_ids_size,
  char *in_party_ids[], const size_t in_party_ids_size, char *in_image_uri,
  eg_ballot_style_t **out_handle);

EG_API eg_electionguard_status_t eg_ballot_style_free(eg_ballot_style_t *handle);

EG_API eg_electionguard_status_t eg_ballot_style_get_object_id(eg_ballot_style_t *handle,
                                                               const char **out_object_id);

EG_API size_t eg_ballot_style_get_geopolitical_unit_ids_size(eg_ballot_style_t *handle);

EG_API eg_electionguard_status_t eg_ballot_style_get_geopolitical_unit_id_at_index(
  eg_ballot_style_t *handle, size_t in_index, char **out_geopolitical_unit_id);

EG_API size_t eg_ballot_style_get_party_ids_size(eg_ballot_style_t *handle);

EG_API eg_electionguard_status_t eg_ballot_style_get_party_id_at_index(eg_ballot_style_t *handle,
                                                                       size_t in_index,
                                                                       char **out_party_id);

EG_API eg_electionguard_status_t eg_ballot_style_get_image_uri(eg_ballot_style_t *handle,
                                                               const char **out_image_uri);

/**
 * A hash representation of the object. Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_ballot_style_crypto_hash(eg_ballot_style_t *handle,
                                                             eg_element_mod_q_t **out_crypto_hash);

#endif

#ifndef Party

struct eg_party_s;

/*
* Use this entity to describe a political party that can then be referenced from other entities.
*
* It is not required to define a party for Electionguard.
*
* See: https://developers.google.com/elections-data/reference/party
*/
typedef struct eg_party_s eg_party_t;

EG_API eg_electionguard_status_t eg_party_new(char *in_object_id, eg_party_t **out_handle);

// TODO: string in params should all be const i think

EG_API eg_electionguard_status_t eg_party_new_with_extras(char *in_object_id,
                                                          eg_internationalized_text_t *in_name,
                                                          char *in_abbreviation, char *in_color,
                                                          char *in_logo_uri,
                                                          eg_party_t **out_handle);

EG_API eg_electionguard_status_t eg_party_free(eg_party_t *handle);

EG_API eg_electionguard_status_t eg_party_get_object_id(eg_party_t *handle,
                                                        const char **out_object_id);

EG_API eg_electionguard_status_t eg_party_get_abbreviation(eg_party_t *handle,
                                                           const char **out_abbreviation);

EG_API eg_electionguard_status_t eg_party_get_name(eg_party_t *handle,
                                                   eg_internationalized_text_t **out_name_ref);

EG_API eg_electionguard_status_t eg_party_get_color(eg_party_t *handle, const char **out_color);

EG_API eg_electionguard_status_t eg_party_get_logo_uri(eg_party_t *handle,
                                                       const char **out_logo_uri);

/**
 * A hash representation of the object. Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_party_crypto_hash(eg_party_t *handle,
                                                      eg_element_mod_q_t **out_crypto_hash);

#endif

#ifndef Candidate

struct eg_candidate_s;

/*
* Entity describing information about a candidate in a contest.
* See: https://developers.google.com/elections-data/reference/candidate
*
* Note: The ElectionGuard Data Spec deviates from the NIST model in that
* selections for any contest type are considered a "candidate".
* for instance, on a yes-no referendum contest, two `candidate` objects
* would be included in the model to represent the `affirmative` and `negative`
* selections for the contest.  See the wiki, readme's, and tests in this repo for more info.
*/
typedef struct eg_candidate_s eg_candidate_t;

EG_API eg_electionguard_status_t eg_candidate_new(char *in_object_id, bool in_is_write_in,
                                                  eg_candidate_t **out_handle);

// TODO: string in params should all be const i think

EG_API eg_electionguard_status_t eg_candidate_new_with_extras(char *in_object_id,
                                                              eg_internationalized_text_t *in_name,
                                                              char *in_party_id, char *in_image_uri,
                                                              bool in_is_write_in,
                                                              eg_candidate_t **out_handle);

EG_API eg_electionguard_status_t eg_candidate_free(eg_candidate_t *handle);

EG_API eg_electionguard_status_t eg_candidate_get_object_id(eg_candidate_t *handle,
                                                            const char **out_object_id);

EG_API eg_electionguard_status_t eg_candidate_get_candidate_id(eg_candidate_t *handle,
                                                               const char **out_candidate_id);

EG_API eg_electionguard_status_t eg_candidate_get_name(eg_candidate_t *handle,
                                                       eg_internationalized_text_t **out_name_ref);

EG_API eg_electionguard_status_t eg_candidate_get_party_id(eg_candidate_t *handle,
                                                           const char **out_party_id);

EG_API eg_electionguard_status_t eg_candidate_get_image_uri(eg_candidate_t *handle,
                                                            const char **out_image_uri);

EG_API bool eg_candidate_get_is_write_in(eg_candidate_t *handle);

/**
 * A hash representation of the object. Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_candidate_crypto_hash(eg_candidate_t *handle,
                                                          eg_element_mod_q_t **out_crypto_hash);

#endif

#ifndef SelectionDescription

struct eg_selection_description_s;
typedef struct eg_selection_description_s eg_selection_description_t;

EG_API eg_electionguard_status_t
eg_selection_description_new(const char *in_object_id, const char *in_candidate_id,
                             uint64_t in_sequence_order, eg_selection_description_t **out_handle);

EG_API eg_electionguard_status_t eg_selection_description_free(eg_selection_description_t *handle);

EG_API eg_electionguard_status_t eg_selection_description_get_object_id(
  eg_selection_description_t *handle, const char **out_object_id);

EG_API eg_electionguard_status_t eg_selection_description_get_candidate_id(
  eg_selection_description_t *handle, const char **out_candidate_id);

EG_API eg_electionguard_status_t eg_selection_description_get_sequence_order(
  eg_selection_description_t *handle, uint64_t *out_sequence_order);

EG_API eg_electionguard_status_t eg_selection_description_crypto_hash(
  eg_selection_description_t *handle, eg_element_mod_q_t **out_crypto_hash);

#endif

#ifndef ConstestDescription

struct eg_contest_description_s;
typedef struct eg_contest_description_s eg_contest_description_t;

// TODO: ISSUE #129: implement

#endif

#ifndef ConstestDescriptionWithPlaceholders

struct eg_contest_description_with_placeholders_s;
typedef struct eg_contest_description_with_placeholders_s
  eg_contest_description_with_placeholders_t;

// TODO: ISSUE #129: implement

#endif

#ifndef Manifest

struct eg_election_manifest_s;

/**
* Use this entity for defining the structure of the election and associated
* information such as candidates, contests, and vote counts.  This class is
* based on the NIST Election Common Standard Data Specification.  Some deviations
* from the standard exist.
*
* This structure is considered an immutable input object and should not be changed
* through the course of an election, as it's hash representation is the basis for all
* other hash representations within an ElectionGuard election context.
*
* @see https://developers.google.com/elections-data/reference/election
*/
typedef struct eg_election_manifest_s eg_election_manifest_t;

// no constructors defined.  use `eg_election_manifest_from_json` or `eg_election_manifest_from_bson`

EG_API eg_electionguard_status_t eg_election_manifest_free(eg_election_manifest_t *handle);

EG_API eg_electionguard_status_t eg_election_manifest_get_election_scope_id(
  eg_election_manifest_t *handle, char **out_election_scope_id);

// TODO: ISSUE #129: implement missing members

/**
 * Get a hash of the metadata components of the `Manifest`
 * 
 * @param[in] handle the `eg_election_manifest_t`
 * @param[out] out_owned_hash the `eg_element_mod_q_t` crypto hash.  
 *                            Caller is responsible fo the lifecycle and 
 *                            must call `eg_element_mod_q_free`
 */
EG_API eg_electionguard_status_t eg_election_manifest_crypto_hash(
  eg_election_manifest_t *handle, eg_element_mod_q_t **out_owned_hash);

EG_API eg_electionguard_status_t
eg_election_manifest_from_json(char *in_data, eg_election_manifest_t **out_handle);

EG_API eg_electionguard_status_t eg_election_manifest_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_election_manifest_t **out_handle);

EG_API eg_electionguard_status_t eg_election_manifest_to_json(eg_election_manifest_t *handle,
                                                              char **out_data, size_t *out_size);

// returns the size and fills out_data, caller is responsible for freeing the out_data
EG_API eg_electionguard_status_t eg_election_manifest_to_bson(eg_election_manifest_t *handle,
                                                              uint8_t **out_data, size_t *out_size);

#endif

#ifndef InternalManifest

struct eg_internal_manifest_s;

/**
* `InternalManifest` is a subset of the `Manifest` structure that specifies
* the components that ElectionGuard uses for conducting an election.  The key component is the
* `contests` collection, which applies placeholder selections to the `Manifest` contests
*/
typedef struct eg_internal_manifest_s eg_internal_manifest_t;

/**
 * Constructs an `InternalManifest` by passing the `Manifest` to the
 * constructor
 * 
 * @param[in] in_manifest the `eg_election_manifest_t`
 * @param[out] out_handle An opaque pointer to the object.  Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_internal_manifest_new(eg_election_manifest_t *in_manifest,
                                                          eg_internal_manifest_t **out_handle);

EG_API eg_electionguard_status_t eg_internal_manifest_free(eg_internal_manifest_t *handle);

/**
 * The hash of the election metadata
 * 
 * @param[out] out_manifest_hash_ref An opaque pointer to the description hash.  
 *                                      The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_internal_manifest_get_manifest_hash(
  eg_internal_manifest_t *handle, eg_element_mod_q_t **out_manifest_hash_ref);

// TODO: ISSUE #129: implement missing members

EG_API eg_electionguard_status_t
eg_internal_manifest_from_json(char *in_data, eg_internal_manifest_t **out_handle);

EG_API eg_electionguard_status_t eg_internal_manifest_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_internal_manifest_t **out_handle);

EG_API eg_electionguard_status_t eg_internal_manifest_to_json(eg_internal_manifest_t *handle,
                                                              char **out_data, size_t *out_size);

// returns the size and fills out_data, caller is responsible for freeing the out_data
EG_API eg_electionguard_status_t eg_internal_manifest_to_bson(eg_internal_manifest_t *handle,
                                                              uint8_t **out_data, size_t *out_size);

#endif

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CPP_MANIFEST_H_INCLUDED__ */
