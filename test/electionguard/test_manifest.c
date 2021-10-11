#include "mocks/election.h"
#include "mocks/manifest.h"
#include "utils/utils.h"

#include <assert.h>
#include <electionguard/election.h>
#include <stdlib.h>

bool strings_are_equal(char *expected, char *actual);

bool test_simple_manifest_is_valid(void);
bool test_can_construct_ballot_style(void);
bool test_can_construct_internationalized_text(void);
static bool test_can_deserialize_election_description(void);
static bool test_can_construct_internal_election_description_from_election_description(void);
static bool test_can_deserlialze_internal_election_description(void);

bool test_manifest(void)
{
    printf("\n -------- test_manifest.c --------- \n");
    return test_simple_manifest_is_valid() && test_can_construct_internationalized_text() &&
           test_can_construct_ballot_style() && test_can_deserialize_election_description() &&
           test_can_construct_internal_election_description_from_election_description() &&
           test_can_deserlialze_internal_election_description();
}

// A dump of an Manifest from test_manifest.cpp
char *election_json =
  "{\"ballot_styles\":[{\"geopolitical_unit_ids\":[\"some-geopoltical-unit-id\"],\"object_id\":"
  "\"some-ballot-style-id\"}],\"candidates\":[{\"object_id\":\"some-candidate-id-1\"},{"
  "\"object_id\":\"some-candidate-id-2\"},{\"object_id\":\"some-candidate-id-3\"}],"
  "\"contests\":[{\"ballot_selections\":[{\"candidate_id\":\"some-candidate-id-1\",\"object_"
  "id\":\"some-object-id-affirmative\",\"sequence_order\":0},{\"candidate_id\":\"some-"
  "candidate-id-2\",\"object_id\":\"some-object-id-negative\",\"sequence_order\":1}],"
  "\"electoral_district_id\":\"some-geopoltical-unit-id\",\"name\":\"some-referendum-contest-"
  "name\",\"number_elected\":1,\"object_id\":\"some-referendum-contest-object-id\",\"sequence_"
  "order\":0,\"vote_variation\":\"one_of_m\"},{\"ballot_selections\":[{\"candidate_id\":\"some-"
  "candidate-id-1\",\"object_id\":\"some-object-id-candidate-1\",\"sequence_order\":0},{"
  "\"candidate_id\":\"some-candidate-id-2\",\"object_id\":\"some-object-id-candidate-2\","
  "\"sequence_order\":1},{\"candidate_id\":\"some-candidate-id-3\",\"object_id\":\"some-object-"
  "id-candidate-3\",\"sequence_order\":2}],\"electoral_district_id\":\"some-geopoltical-unit-"
  "id\",\"name\":\"some-candidate-contest-name\",\"number_elected\":2,\"object_id\":\"some-"
  "candidate-contest-object-id\",\"sequence_order\":1,\"vote_variation\":\"one_of_m\"}],"
  "\"election_scope_id\":\"some-scope-id\",\"end_date\":\"2021-02-04T13:30:10Z\","
  "\"geopolitical_units\":[{\"name\":\"some-gp-unit-name\",\"object_id\":\"some-geopoltical-"
  "unit-id\",\"type\":\"unknown\"}],\"parties\":[{\"object_id\":\"some-party-id-1\"},{\"object_"
  "id\":\"some-party-id-2\"}],\"start_date\":\"2021-02-04T13:30:10Z\",\"type\":\"unknown\"}";

bool test_simple_manifest_is_valid(void)
{
    printf("\n -------- test_simple_manifest_is_valid -------- \n");
    // Act
    eg_election_manifest_t *result = NULL;
    if (eg_test_election_mocks_get_simple_election_from_file(&result)) {
        assert(false);
    }

    // Assert
    char *election_scope_id;
    if (eg_election_manifest_get_election_scope_id(result, &election_scope_id)) {
        assert(false);
    }

    assert(strings_are_equal("jefferson-county-primary", election_scope_id) == true);

    // Clean Up
    eg_election_manifest_free(result);

    free(election_scope_id);

    return true;
}

bool test_can_deserialize_election_description(void)
{
    printf("\n -------- test_can_deserialize_election_description -------- \n");
    // Act
    eg_election_manifest_t *result = NULL;
    if (eg_election_manifest_from_json(election_json, &result)) {
        assert(false);
    }

    // Assert
    char *election_scope_id;
    if (eg_election_manifest_get_election_scope_id(result, &election_scope_id)) {
        assert(false);
    }
    assert(strings_are_equal("some-scope-id", election_scope_id) == true);

    // Clean Up
    eg_election_manifest_free(result);

    free(election_scope_id);

    return true;
}

bool test_can_construct_internationalized_text(void)
{
    printf("\n -------- test_can_construct_internationalized_text -------- \n");
    // Arrange
    eg_language_t *language_1 = NULL;
    if (eg_language_new("some words", "en", &language_1)) {
        assert(false);
    }

    eg_language_t *language_2 = NULL;
    if (eg_language_new("algunas palabras", "es", &language_2)) {
        assert(false);
    }

    eg_language_t *languages[2];
    languages[0] = language_1;
    languages[1] = language_2;

    // Act
    eg_internationalized_text_t *text = NULL;
    if (eg_internationalized_text_new(languages, 2, &text)) {
        assert(false);
    }

    // Assert
    eg_language_t *actual = NULL;
    if (eg_internationalized_text_get_text_at_index(text, 1, &actual)) {
        assert(false);
    }

    char *text_string = NULL;
    if (eg_language_get_value(actual, &text_string)) {
        assert(false);
    }

    assert(strings_are_equal(text_string, "algunas palabras") == true);

    // We dont need to free language_1 and language_2
    // because they are owned by internationalized_text

    free(text_string);
    //eg_language_free(language_2);

    eg_internationalized_text_free(text);

    return true;
}

bool test_can_construct_ballot_style(void)
{
    printf("\n -------- test_can_construct_ballot_style -------- \n");
    // Arrange
    char *gpUnits[2];
    gpUnits[0] = "gp-unit-1";
    gpUnits[1] = "gp-unit-2";

    // Act
    eg_ballot_style_t *actual = NULL;
    if (eg_ballot_style_new("some-object-id", gpUnits, 2, &actual)) {
        assert(false);
    }

    // Assert
    char *text_string = NULL;
    if (eg_ballot_style_get_geopolitical_unit_id_at_index(actual, 0, &text_string)) {
        assert(false);
    }

    assert(strings_are_equal(text_string, gpUnits[0]) == true);

    // We dont need to free gpUnits 1 and gpUnits 2
    // because they are owned by ballot_style

    free(text_string);
    eg_ballot_style_free(actual);

    return true;
}

bool test_can_construct_internal_election_description_from_election_description(void)
{
    printf("\n -------- test_can_construct_internal_election_description_from_election_description "
           "-------- \n");
    // Arrange
    eg_election_manifest_t *data = NULL;
    if (eg_election_manifest_from_json(election_json, &data)) {
        assert(false);
    }

    // Act
    eg_internal_manifest_t *result = NULL;
    if (eg_internal_manifest_new(data, &result)) {
        assert(false);
    }

    // Assert
    eg_element_mod_q_t *expected = NULL;
    if (eg_election_manifest_crypto_hash(data, &expected)) {
        assert(false);
    }

    eg_element_mod_q_t *actual = NULL;
    if (eg_internal_manifest_get_manifest_hash(result, &actual)) {
        assert(false);
    }

    uint64_t *expected_hash_data = NULL;
    size_t expected_hash_data_size;
    if (eg_element_mod_q_get_data(expected, &expected_hash_data, &expected_hash_data_size)) {
        assert(false);
    }

    uint64_t *actual_hash_data = NULL;
    size_t actual_hash_data_size;
    if (eg_element_mod_q_get_data(actual, &actual_hash_data, &actual_hash_data_size)) {
        assert(false);
    }

    assert(expected_hash_data_size == 4);
    assert(actual_hash_data_size == 4);
    assert(expected_hash_data[0] == actual_hash_data[0]);

    // Clean Up
    eg_element_mod_q_free(expected);
    eg_election_manifest_free(data);
    eg_internal_manifest_free(result);

    return true;
}

bool test_can_deserlialze_internal_election_description(void)
{
    printf("\n -------- test_can_deserlialze_internal_election_description -------- \n");
    // Arrange
    // A dump of an InternalManifest from test_election.cpp
    char *internal_json =
      "{\"ballot_styles\":[{\"geopolitical_unit_ids\":[\"geopolitical-unit-1\"],\"image_uri\":"
      "\"some-uri\",\"object_id\":\"some-ballot-style-id\",\"party_ids\":[\"party-1\"]}],"
      "\"contests\":[{\"ballot_placeholders\":[{\"candidate_id\":\"candidate-4-id\",\"object_id\":"
      "\"contest-1-placeholder-selection-4-id\",\"sequence_order\":4}],\"ballot_selections\":[{"
      "\"candidate_id\":\"candidate-1-id\",\"object_id\":\"contest-1-selection-1-id\",\"sequence_"
      "order\":1},{\"candidate_id\":\"candidate-2-id\",\"object_id\":\"contest-1-selection-2-id\","
      "\"sequence_order\":2},{\"candidate_id\":\"candidate-3-id\",\"object_id\":\"contest-1-"
      "selection-3-id\",\"sequence_order\":3}],\"ballot_subtitle\":{\"text\":[{\"language\":\"en\","
      "\"value\":\"some-title-string\"},{\"language\":\"es\",\"value\":\"some-title-string-es\"}]},"
      "\"ballot_title\":{\"text\":[{\"language\":\"en\",\"value\":\"some-title-string\"},{"
      "\"language\":\"es\",\"value\":\"some-title-string-es\"}]},\"electoral_district_id\":"
      "\"geopolitical-unit-1\",\"name\":\"contest-1-name\",\"number_elected\":2,\"object_id\":"
      "\"contest-1-id\",\"sequence_order\":1,\"vote_variation\":\"n_of_m\",\"votes_allowed\":2},{"
      "\"ballot_placeholders\":[{\"candidate_id\":\"candidate-3-id\",\"object_id\":\"contest-2-"
      "placeholder-selection-3-id\",\"sequence_order\":3}],\"ballot_selections\":[{\"candidate_"
      "id\":\"candidate-1-id\",\"object_id\":\"contest-2-selection-1-id\",\"sequence_order\":1},{"
      "\"candidate_id\":\"candidate-2-id\",\"object_id\":\"contest-2-selection-2-id\",\"sequence_"
      "order\":2}],\"ballot_subtitle\":{\"text\":[{\"language\":\"en\",\"value\":\"some-title-"
      "string\"},{\"language\":\"es\",\"value\":\"some-title-string-es\"}]},\"ballot_title\":{"
      "\"text\":[{\"language\":\"en\",\"value\":\"some-title-string\"},{\"language\":\"es\","
      "\"value\":\"some-title-string-es\"}]},\"electoral_district_id\":\"geopolitical-unit-1\","
      "\"name\":\"contest-2-name\",\"number_elected\":2,\"object_id\":\"contest-2-id\",\"sequence_"
      "order\":2,\"vote_variation\":\"n_of_m\",\"votes_allowed\":2}],\"manifest_hash\":\"02\","
      "\"geopolitical_units\":[{\"contact_information\":{\"address_line\":[\"some-street\",\"some-"
      "city\",\"some-whatever\"],\"name\":\"gp-unit-contact-info\"},\"name\":\"district-1-id\","
      "\"object_id\":\"geopolitical-unit-1\",\"type\":\"city\"}]}";

    // Act
    eg_internal_manifest_t *result = NULL;
    if (eg_internal_manifest_from_json(internal_json, &result)) {
        assert(false);
    }

    // Assert
    eg_element_mod_q_t *actual = NULL;
    if (eg_internal_manifest_get_manifest_hash(result, &actual)) {
        assert(false);
    }

    uint64_t *actual_hash_data = NULL;
    size_t actual_hash_data_size;
    if (eg_element_mod_q_get_data(actual, &actual_hash_data, &actual_hash_data_size)) {
        assert(false);
    }

    assert(actual_hash_data_size == 4);
    assert(actual_hash_data[0] == 02);

    // Clean Up
    eg_internal_manifest_free(result);

    return true;
}
