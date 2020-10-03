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

static bool test_crypto_hashing();
static bool test_constant_chaum_pedersen_proof();
static bool test_encrypt_selection();
static bool test_encrypt_ballot();
static bool test_generate_tracking_code();

int main()
{
    // TODO: currently these tests just mimick the c++ tests
    // however this class file should instead execute an
    // end to end test to demonstrate usage
    assert(test_crypto_hashing() == true);
    assert(test_constant_chaum_pedersen_proof() == true);
    assert(test_encrypt_selection() == true);
    assert(test_encrypt_ballot() == true);
    assert(test_generate_tracking_code() == true);

    printf("TEST STATUS SUCCESS!");
}

bool test_crypto_hashing()
{
    eg_element_mod_q_t *string_hash = eg_hash_elems_string("test");
    uint64_t *string_hash_data = NULL;
    size_t string_hash_data_size;
    if (eg_element_mod_q_get_data(string_hash, &string_hash_data, &string_hash_data_size)) {
        assert(false);
    }

    printf("string hash %llu", string_hash_data[0]);

    // TODO: check the actual hash string
    assert(string_hash != NULL);
    assert(string_hash != 0);
    assert(string_hash_data_size == 4);
    assert(string_hash_data[0] > 0);

    const char *strings[] = {"test", "strings"};
    eg_element_mod_q_t *strings_hash = eg_hash_elems_strings(strings, 2);
    uint64_t *strings_hash_data = NULL;
    size_t strings_hash_data_size;
    if (eg_element_mod_q_get_data(string_hash, &strings_hash_data, &strings_hash_data_size)) {
        assert(false);
    }

    // TODO: check the actual hash string
    assert(strings_hash != NULL);
    assert(strings_hash != 0);
    assert(strings_hash_data_size == 4);
    assert(strings_hash_data[0] > 0);

    eg_element_mod_q_t *int_hash = eg_hash_elems_int(1234);
    uint64_t *int_hash_data = NULL;
    size_t int_hash_data_size;
    if (eg_element_mod_q_get_data(string_hash, &int_hash_data, &int_hash_data_size)) {
        assert(false);
    }

    // TODO: check the actual hash string
    assert(int_hash != NULL);
    assert(int_hash != 0);
    assert(int_hash_data_size == 4);
    assert(int_hash_data[0] > 0);

    return true;
}

bool test_constant_chaum_pedersen_proof()
{
    eg_element_mod_q_t *one_mod_q = NULL;
    if (eg_element_mod_q_new(ONE_MOD_Q_ARRAY, &one_mod_q)) {
        assert(false);
    }

    eg_element_mod_q_t *two_mod_q = NULL;
    if (eg_element_mod_q_new(TWO_MOD_Q_ARRAY, &two_mod_q)) {
        assert(false);
    }

    eg_elgamal_keypair_t *key_pair = eg_elgamal_keypair_from_secret(two_mod_q);

    eg_element_mod_q_t *nonce = NULL;
    if (eg_element_mod_q_new(ONE_MOD_Q_ARRAY, &nonce)) {
        assert(false);
    }

    eg_element_mod_q_t *seed = NULL;
    if (eg_element_mod_q_new(TWO_MOD_Q_ARRAY, &seed)) {
        assert(false);
    }

    eg_element_mod_p_t *public_key = eg_elgamal_keypair_get_public_key(key_pair);

    eg_elgamal_ciphertext_t *message = eg_elgamal_encrypt(0UL, nonce, public_key);
    eg_constant_chaum_pedersen_proof_t *proof =
      eg_constant_chaum_pedersen_proof_make(message, nonce, public_key, seed, one_mod_q, 0UL);
    eg_constant_chaum_pedersen_proof_t *bad_proof =
      eg_constant_chaum_pedersen_proof_make(message, nonce, public_key, seed, one_mod_q, 1UL);
    assert(eg_constant_chaum_pedersen_proof_is_valid(proof, message, public_key, one_mod_q) ==
           true);
    assert(eg_constant_chaum_pedersen_proof_is_valid(bad_proof, message, public_key, one_mod_q) ==
           false);

    // Clean Up
    if (eg_element_mod_q_free(one_mod_q)) {
        assert(false);
    }
    if (eg_element_mod_q_free(two_mod_q)) {
        assert(false);
    }
    if (eg_element_mod_q_free(nonce)) {
        assert(false);
    }
    if (eg_element_mod_q_free(seed)) {
        assert(false);
    }

    return true;
}

bool test_encrypt_selection()
{
    // Arrange
    const char *candidate_id = "some-candidate-id";
    eg_element_mod_q_t *one_mod_q = NULL;
    if (eg_element_mod_q_new(ONE_MOD_Q_ARRAY, &one_mod_q)) {
        assert(false);
    }

    eg_element_mod_q_t *two_mod_q = NULL;
    if (eg_element_mod_q_new(TWO_MOD_Q_ARRAY, &two_mod_q)) {
        assert(false);
    }

    eg_elgamal_keypair_t *key_pair = eg_elgamal_keypair_from_secret(two_mod_q);

    eg_element_mod_q_t *nonce = NULL;
    if (eg_element_mod_q_new_rand_q(&nonce)) {
        assert(false);
    }

    eg_selection_description_t *metadata =
      eg_selection_description_create("some-selection-object_id", candidate_id, 1UL);

    eg_element_mod_q_t *hash_context = eg_selection_description_crypto_hash(metadata);
    eg_plaintext_ballot_selection_t *plaintext =
      eg_plaintext_ballot_selection_create(candidate_id, "1");

    eg_element_mod_p_t *public_key = eg_elgamal_keypair_get_public_key(key_pair);

    // Act
    eg_ciphertext_ballot_selection_t *result =
      eg_encrypt_selection(plaintext, metadata, public_key, one_mod_q, nonce, false, true);

    // Assert
    assert(result != NULL);

    eg_elgamal_ciphertext_t *ciphertext = eg_ciphertext_ballot_selection_get_ciphertext(result);
    eg_disjunctive_chaum_pedersen_proof_t *proof = eg_ciphertext_ballot_selection_get_proof(result);

    assert(eg_ciphertext_ballot_selection_is_valid_encryption(result, hash_context, public_key,
                                                              one_mod_q) == true);

    assert(eg_disjunctive_chaum_pedersen_proof_is_valid(proof, ciphertext, public_key, one_mod_q) ==
           true);

    // Clean Up
    if (eg_element_mod_q_free(one_mod_q)) {
        assert(false);
    }
    if (eg_element_mod_q_free(two_mod_q)) {
        assert(false);
    }
    if (eg_element_mod_q_free(nonce)) {
        assert(false);
    }

    return true;
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
