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

//static bool strings_are_equal(char *expected, char *actual);

static bool test_crypto_hashing();
static bool test_encrypt_selection();
static bool test_generate_tracking_code();

int main()
{
    // TODO: currently these tests just mimick the c++ tests
    // however this class file should instead execute an
    // end to end test to demonstrate usage
    assert(test_crypto_hashing() == true);
    assert(test_encrypt_selection() == true);
    assert(test_generate_tracking_code() == true);

    printf("TEST STATUS SUCCESS!");
}

bool test_encrypt_selection()
{
    // Arrange
    const char *candidate_id = "some-candidate-id";
    eg_element_mod_q_t *one_mod_q = eg_element_mod_q_create(ONE_MOD_Q_ARRAY);
    eg_element_mod_q_t *two_mod_q = eg_element_mod_q_create(TWO_MOD_Q_ARRAY);
    eg_elgamal_keypair_t *key_pair = eg_elgamal_keypair_from_secret(two_mod_q);

    eg_element_mod_q_t *nonce = eg_rand_q();
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

    return true;
}

bool test_crypto_hashing()
{
    eg_element_mod_q_t *string_hash = eg_hash_elems_string("test");
    uint64_t *string_hash_data;
    uint8_t size = eg_element_mod_q_get(string_hash, &string_hash_data);

    // TODO: check the actual hash string
    assert(string_hash != NULL);
    assert(string_hash != 0);
    assert(size == 4);
    assert(string_hash_data[0] > 0);

    const char *strings[] = {"test", "strings"};
    eg_element_mod_q_t *strings_hash = eg_hash_elems_strings(strings, 2);
    uint64_t *strings_hash_data;
    size = eg_element_mod_q_get(strings_hash, &strings_hash_data);

    // TODO: check the actual hash string
    assert(strings_hash != NULL);
    assert(strings_hash != 0);
    assert(size == 4);
    assert(strings_hash_data[0] > 0);

    eg_element_mod_q_t *int_hash = eg_hash_elems_int(1234);
    uint64_t *int_hash_data;
    size = eg_element_mod_q_get(int_hash, &int_hash_data);

    // TODO: check the actual hash string
    assert(int_hash != NULL);
    assert(int_hash != 0);
    assert(size == 4);
    assert(int_hash_data[0] > 0);

    return true;
}

bool test_generate_tracking_code()
{
    // Arrange
    eg_element_mod_q_t *device_hash = eg_get_hash_for_device(1234, "some-location-string");
    uint64_t first_hash[4] = {1, 2, 3, 4};
    eg_element_mod_q_t *first_ballot_hash = eg_element_mod_q_create(first_hash);
    uint64_t second_hash[4] = {2, 3, 4, 5};
    eg_element_mod_q_t *second_ballot_hash = eg_element_mod_q_create(second_hash);

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

    return true;
}

// bool strings_are_equal(char *expected, char *actual)
// {
//     while (*expected == *actual) {
//         if (*expected == '\0' || *actual == '\0') {
//             break;
//         }
//         expected++;
//         actual++;
//     }

//     if (*expected == '\0' && *actual == '\0') {
//         return true;
//     } else {
//         printf("compare_string: failed!\n");
//         return false;
//     }
// }
