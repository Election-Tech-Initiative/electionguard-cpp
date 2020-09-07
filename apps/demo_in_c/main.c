#include <assert.h>
#include <electionguard/ballot.h>
#include <electionguard/election.h>
#include <electionguard/encrypt.h>
#include <electionguard/hash.h>
#include <electionguard/tracker.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool strings_are_equal(char *expected, char *actual);

static bool test_crypto_hashing();
static bool test_encrypt_selection();
static bool test_generate_tracking_code();

int main()
{
    assert(test_crypto_hashing() == true);
    assert(test_encrypt_selection() == true);
    assert(test_generate_tracking_code() == true);
}

bool test_encrypt_selection()
{
    // insantiate the stateful mediator
    eg_encryption_mediator_t *encrypter = eg_encryption_mediator_new();
    assert(encrypter != NULL);

    char *candidate_id = "some-candidate-id";

    // instantiate a selection description
    eg_selection_description_t *description =
      eg_selection_description_new("some-description-object-id", candidate_id, (uint64_t)1);

    // instantiate a fake public key, hash, and nonce
    uint64_t pub[64] = {10};
    eg_element_mod_p_t *public_key = eg_element_mod_p_new(pub);

    uint64_t hash[4] = {9};
    eg_element_mod_q_t *extended_base_hash = eg_element_mod_q_new(hash);

    uint64_t seed[4] = {4};
    eg_element_mod_q_t *nonce_seed = eg_element_mod_q_new(seed);

    // instantiate a fake selection on a ballot
    eg_plaintext_ballot_selection_t *plaintext =
      eg_plaintext_ballot_selection_new(candidate_id, "1");

    // execute the encryption
    eg_ciphertext_ballot_selection_t *result = eg_encrypt_selection(
      plaintext, description, public_key, extended_base_hash, nonce_seed, false, false);
    assert(result != NULL);

    // check the object id by accessing the property getter
    char *plaintext_object_id = eg_plaintext_ballot_selection_get_object_id(plaintext);
    char *ciphertext_object_id = eg_ciphertext_ballot_selection_get_object_id(result);
    assert(strings_are_equal(plaintext_object_id, ciphertext_object_id) == true);
    assert(strings_are_equal(ciphertext_object_id, candidate_id) == true);

    // get out one of the ElementModQ hashes
    eg_element_mod_q_t *description_hash =
      eg_ciphertext_ballot_selection_get_description_hash(result);

    uint64_t *description_hash_data;
    uint8_t size = eg_element_mod_q_get(description_hash, &description_hash_data);
    assert(size == 4);

    // TODO: checks similar to the cpp test

    eg_encryption_mediator_free(encrypter);
    eg_plaintext_ballot_selection_free(plaintext);
    eg_ciphertext_ballot_selection_free(result);

    return true;
}

bool test_crypto_hashing()
{
    eg_element_mod_q_t *string_hash = eg_hash_elems_string("test");
    uint64_t *string_hash_data;
    uint8_t size = eg_element_mod_q_get(string_hash, &string_hash_data);

    assert(string_hash != NULL);
    assert(string_hash != 0);
    assert(size == 4);
    assert(string_hash_data[0] > 0);

    const char *strings[] = {"test", "strings"};
    eg_element_mod_q_t *strings_hash = eg_hash_elems_strings(strings, 2);
    uint64_t *strings_hash_data;
    size = eg_element_mod_q_get(strings_hash, &strings_hash_data);

    assert(strings_hash != NULL);
    assert(strings_hash != 0);
    assert(size == 4);
    assert(strings_hash_data[0] > 0);

    eg_element_mod_q_t *int_hash = eg_hash_elems_int(1234);
    uint64_t *int_hash_data;
    size = eg_element_mod_q_get(int_hash, &int_hash_data);

    assert(int_hash != NULL);
    assert(int_hash != 0);
    assert(size == 4);
    assert(int_hash_data[0] > 0);

    eg_element_mod_q_free(string_hash);
    eg_element_mod_q_free(strings_hash);
    eg_element_mod_q_free(int_hash);

    return true;
}

bool test_generate_tracking_code()
{
    // Arrange
    eg_element_mod_q_t *device_hash = eg_get_hash_for_device(1234, "some-location-string");
    uint64_t first_hash[4] = {1, 2, 3, 4};
    eg_element_mod_q_t *first_ballot_hash = eg_element_mod_q_new(first_hash);
    uint64_t second_hash[4] = {2, 3, 4, 5};
    eg_element_mod_q_t *second_ballot_hash = eg_element_mod_q_new(second_hash);

    // Act
    eg_element_mod_q_t *rotating_hash_1 =
      eg_get_rotating_tracker_hash(device_hash, 1000, first_ballot_hash);
    eg_element_mod_q_t *rotating_hash_2 =
      eg_get_rotating_tracker_hash(rotating_hash_1, 1001, second_ballot_hash);

    // Assert
    assert(device_hash != NULL);
    assert(rotating_hash_1 != NULL);
    assert(rotating_hash_2 != NULL);

    uint64_t *device_hash_data;
    uint8_t device_hash_size = eg_element_mod_q_get(device_hash, &device_hash_data);

    uint64_t *rotating_hash_1_data;
    uint8_t rotating_hash_1_size = eg_element_mod_q_get(rotating_hash_1, &rotating_hash_1_data);

    uint64_t *rotating_hash_2_data;
    uint8_t rotating_hash_2_size = eg_element_mod_q_get(rotating_hash_2, &rotating_hash_2_data);

    assert(device_hash_data != NULL);
    assert(rotating_hash_1_data != NULL);
    assert(rotating_hash_2_data != NULL);

    assert(rotating_hash_1_data != device_hash_data);
    assert(rotating_hash_2_data != device_hash_data);
    assert(rotating_hash_1_data != rotating_hash_2_data);

    assert(rotating_hash_1_data[0] != device_hash_data[0]);

    eg_element_mod_q_free(device_hash);
    eg_element_mod_q_free(first_ballot_hash);
    eg_element_mod_q_free(second_ballot_hash);
    eg_element_mod_q_free(rotating_hash_1);
    eg_element_mod_q_free(rotating_hash_2);

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
        printf("compare_string: failed!\n");
        return false;
    }
}
