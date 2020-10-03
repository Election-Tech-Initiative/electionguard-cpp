#include <assert.h>
#include <electionguard/ballot.h>
#include <electionguard/chaum_pedersen.h>
#include <electionguard/constants.h>
#include <electionguard/election.h>
#include <electionguard/elgamal.h>
#include <electionguard/encrypt.h>
#include <electionguard/group.h>
#include <electionguard/hash.h>
#include <electionguard/tracker.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool strings_are_equal(char *expected, char *actual);

static bool test_encrypt_ballot();
static bool test_generate_tracking_code();

int main()
{
    // TODO: currently these tests just mimick the c++ tests
    // however this class file should instead execute an
    // end to end test to demonstrate usage
    assert(test_encrypt_ballot() == true);
    assert(test_generate_tracking_code() == true);

    printf("TEST STATUS SUCCESS!");
}

bool test_encrypt_ballot()
{
    // Arrange
    char *json =
      "{\"ballot_style\":\"ballot-style-1\",\"contests\":[{\"ballot_selections\":[{\"object_id\":"
      "\"contest-1-selection-1-id\",\"vote\":\"1\"},{\"object_id\":\"contest-1-selection-2-id\","
      "\"vote\":\"0\"},{\"object_id\":\"contest-1-selection-3-id\",\"vote\":\"0\"}],\"object_id\":"
      "\"contest-1-id\"},{\"ballot_selections\":[{\"object_id\":\"contest-2-selection-1-id\","
      "\"vote\":\"1\"},{\"object_id\":\"contest-2-selection-2-id\",\"vote\":\"0\"}],\"object_id\":"
      "\"contest-2-id\"}],\"object_id\":\"ballot-id-123\"}";

    eg_plaintext_ballot_t *fromJson = eg_plaintext_ballot_from_json(json);

    char *derived;
    uint64_t size = eg_plaintext_ballot_to_json(fromJson, &derived);

    assert(strings_are_equal(json, derived) == true);
    free(derived);
    return true;
}

bool test_generate_tracking_code()
{
    // Arrange
    eg_element_mod_q_t *device_hash = eg_get_hash_for_device(1234, "some-location-string");

    uint64_t first_hash[4] = {1, 2, 3, 4};
    eg_element_mod_q_t *first_ballot_hash = NULL;
    if (eg_element_mod_q_new(first_hash, &first_ballot_hash)) {
        assert(false);
    }

    uint64_t second_hash[4] = {2, 3, 4, 5};
    eg_element_mod_q_t *second_ballot_hash = NULL;
    if (eg_element_mod_q_new(second_hash, &second_ballot_hash)) {
        assert(false);
    }

    // Act
    eg_element_mod_q_t *rotating_hash_1 =
      eg_get_rotating_tracker_hash(device_hash, 1000, first_ballot_hash);
    eg_element_mod_q_t *rotating_hash_2 =
      eg_get_rotating_tracker_hash(rotating_hash_1, 1001, second_ballot_hash);

    // Assert
    assert(device_hash != NULL);
    assert(rotating_hash_1 != NULL);
    assert(rotating_hash_2 != NULL);

    uint64_t *device_hash_data = NULL;
    size_t device_hash_size;
    if (eg_element_mod_q_get_data(device_hash, &device_hash_data, &device_hash_size)) {
        assert(false);
    }

    uint64_t *rotating_hash_1_data = NULL;
    size_t rotating_hash_1_size;
    if (eg_element_mod_q_get_data(rotating_hash_1, &rotating_hash_1_data, &rotating_hash_1_size)) {
        assert(false);
    }

    uint64_t *rotating_hash_2_data = NULL;
    size_t rotating_hash_2_size;
    if (eg_element_mod_q_get_data(rotating_hash_2, &rotating_hash_2_data, &rotating_hash_2_size)) {
        assert(false);
    }

    assert(device_hash_data != NULL);
    assert(rotating_hash_1_data != NULL);
    assert(rotating_hash_2_data != NULL);

    assert(rotating_hash_1_data != device_hash_data);
    assert(rotating_hash_2_data != device_hash_data);
    assert(rotating_hash_1_data != rotating_hash_2_data);

    assert(rotating_hash_1_data[0] != device_hash_data[0]);

    // Clean Up
    if (eg_element_mod_q_free(first_ballot_hash)) {
        assert(false);
    }

    if (eg_element_mod_q_free(second_ballot_hash)) {
        assert(false);
    }

    return true;
}

bool strings_are_equal(char *expected, char *actual)
{
    while (*expected == *actual) {
        if (*expected == '\0' || *actual == '\0') {
            break;
        }
        expected++;
        actual++;
    }

    if (*expected == '\0' && *actual == '\0') {
        return true;
    } else {
        return false;
    }
}
