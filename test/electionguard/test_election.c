#include "utils/utils.h"

#include <assert.h>
#include <electionguard/election.h>
#include <stdlib.h>

bool strings_are_equal(char *expected, char *actual);

static bool test_can_deserialize_election_description(void);
static bool test_can_construct_internal_election_description_from_election_description(void);
static bool test_can_deserlialze_internal_election_description(void);
static bool test_can_deserialize_ciphertext_election_context(void);

bool test_election(void)
{
    printf("\n -------- test_election.c --------- \n");
    return test_can_deserialize_election_description() &&
           test_can_construct_internal_election_description_from_election_description() &&
           test_can_deserlialze_internal_election_description() &&
           test_can_deserialize_ciphertext_election_context();
}

// A dump of an ElectionDescription from test_election.cpp
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

bool test_can_deserialize_election_description(void)
{
    printf("\n -------- test_can_deserialize_election_description \n");
    // Act
    eg_election_description_t *result = NULL;
    if (eg_election_description_from_json(election_json, &result)) {
        assert(false);
    }

    // Assert
    char *election_scope_id;
    if (eg_election_description_get_election_scope_id(result, &election_scope_id)) {
        assert(false);
    }
    assert(strings_are_equal("some-scope-id", election_scope_id) == true);

    // Clean Up
    eg_election_description_free(result);

    free(election_scope_id);

    return true;
}

bool test_can_construct_internal_election_description_from_election_description(void)
{
    printf(
      "\n -------- test_can_construct_internal_election_description_from_election_description \n");
    // Arrange
    eg_election_description_t *data = NULL;
    if (eg_election_description_from_json(election_json, &data)) {
        assert(false);
    }

    // Act
    eg_internal_election_description_t *result = NULL;
    if (eg_internal_election_description_new(data, &result)) {
        assert(false);
    }

    // Assert
    eg_element_mod_q_t *expected = NULL;
    if (eg_election_description_crypto_hash(data, &expected)) {
        assert(false);
    }

    eg_element_mod_q_t *actual = NULL;
    if (eg_internal_election_description_get_description_hash(result, &actual)) {
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
    eg_election_description_free(data);
    eg_internal_election_description_free(result);

    return true;
}

bool test_can_deserlialze_internal_election_description(void)
{
    printf("\n -------- test_can_deserlialze_internal_election_description \n");
    // Arrange
    // A dump of an InternalElectionDescription from test_election.cpp
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
      "order\":2,\"vote_variation\":\"n_of_m\",\"votes_allowed\":2}],\"description_hash\":\"02\","
      "\"geopolitical_units\":[{\"contact_information\":{\"address_line\":[\"some-street\",\"some-"
      "city\",\"some-whatever\"],\"name\":\"gp-unit-contact-info\"},\"name\":\"district-1-id\","
      "\"object_id\":\"geopolitical-unit-1\",\"type\":\"city\"}]}";

    // Act
    eg_internal_election_description_t *result = NULL;
    if (eg_internal_election_description_from_json(internal_json, &result)) {
        assert(false);
    }

    // Assert
    eg_element_mod_q_t *actual = NULL;
    if (eg_internal_election_description_get_description_hash(result, &actual)) {
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
    eg_internal_election_description_free(result);

    return true;
}

bool test_can_deserialize_ciphertext_election_context(void)
{
    printf("\n -------- test_can_deserialize_ciphertext_election_context \n");
    char *context_json =
      "{\"crypto_base_hash\":\"B8CF9A8915BDB19C681AFBCDD1797F2CF360F723843D977D0E1B280CA2B24245\","
      "\"crypto_extended_base_hash\":"
      "\"731052175CAE2EF7CFE0E827C65BB585CD8A9CB243320EB34C09B5DF3F75F982\",\"description_hash\":"
      "\"02\",\"elgamal_public_key\":"
      "\"F258E409B1A130E00A3793555E0EAB2F560AA12CC01A3CB6B357035C6E734256B4D67877C018CB57AF150DDBBD"
      "0AC22B9D74C0B15C1AC80953086FDDFAAB7FC503022B61BE8C6E4FECD02136F4AFC68B51390D0E7E90661763455B"
      "8B1D79DDCBA82C7FFBA5BC7C28A323B2624FE7A72E693E45B0B88ACF1EBC4F2743AC0FBB9D3414070A37A7CD52B1"
      "C1443B69982B86DDCFEA3137C383A4C10705B7B2A244C93CD0179BD1203E5DCE19A078452C0525AA577D6214D1A8"
      "49AF5BCE02581F381A872436497DA0245F10245604069562F98BE4F424227331252D955E8E77B42B20D3AB85530A"
      "C824FA4D5A8EDC4A6C7E310019BED8B8062B9A95B63CC126EFCC74D7512095183E2A2A99386B40210A9B1E9B2433"
      "F1C00D04DCCDA4CAE17E27456C6BD7D55C45C5F98514400EDA58D93A82E0D456C209087ED41106190F969B1DA484"
      "1F8EA26907FF4D29CAF05A4C5159AED129F980A06351B18CC88FB0954E958984070C0C8999D043BC68E1BFB8151F"
      "061103C9D89D9FDF709F747F816275463568FA6E18BAAD0C21A6C204C3795383F91EEECE312A4FCFD71B541718FF"
      "21F7D408D3A2572C889A7D25957BD7D206041452B60F513B24604CD8336C351D6B7A70BD39BEDBAD0B910D329230"
      "F4A3228DF1398CBE7EE7EEF49CB22C94BE32ED6C3F51B4F2C43A40BC5C217106CD7A0550BA12BB3D84643F90976E"
      "CFEF79614F2C46\",\"number_of_guardians\":1,\"quorum\":1}";

    eg_ciphertext_election_context_t *result = NULL;
    if (eg_ciphertext_election_context_from_json(context_json, &result)) {
        assert(false);
    }

    eg_element_mod_q_t *crypto_base_hash = NULL;
    if (eg_ciphertext_election_context_get_crypto_base_hash(result, &crypto_base_hash)) {
        assert(false);
    }

    char *crypto_base_hash_hex;
    if (eg_element_mod_q_to_hex(crypto_base_hash, &crypto_base_hash_hex)) {
        assert(false);
    }

    assert(strings_are_equal("B8CF9A8915BDB19C681AFBCDD1797F2CF360F723843D977D0E1B280CA2B24245",
                             crypto_base_hash_hex) == true);

    // Clean Up
    eg_ciphertext_election_context_free(result);
    free(crypto_base_hash_hex);

    return true;
}
