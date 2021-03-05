#include "generators/ballot.h"
#include "generators/election.h"
#include "utils/utils.h"

#include <assert.h>
#include <electionguard/encrypt.h>
#include <stdlib.h>

bool strings_are_equal(char *expected, char *actual);

static bool test_encrypt_selection(void);
static bool test_encrypt_contest(void);
static bool test_encrypt_ballot_simple_succeeds(void);

bool test_encrypt(void)
{
    printf("\n -------- test_encrypt.c --------- \n");
    return test_encrypt_selection() && test_encrypt_contest() &&
           test_encrypt_ballot_simple_succeeds();
}

bool test_encrypt_selection(void)
{
    printf("\n -------- test_encrypt_selection -------- \n");
    // Arrange
    const char *candidate_id = "some-candidate-id";
    const char *selection_id = "some-selection-object_id";
    eg_element_mod_q_t *one_mod_q = NULL;
    if (eg_element_mod_q_new(ONE_MOD_Q_ARRAY, &one_mod_q)) {
        assert(false);
    }

    eg_element_mod_q_t *two_mod_q = NULL;
    if (eg_element_mod_q_new(TWO_MOD_Q_ARRAY, &two_mod_q)) {
        assert(false);
    }

    eg_elgamal_keypair_t *key_pair = NULL;
    if (eg_elgamal_keypair_from_secret_new(two_mod_q, &key_pair)) {
        assert(false);
    }

    eg_element_mod_q_t *nonce = NULL;
    if (eg_element_mod_q_rand_q_new(&nonce)) {
        assert(false);
    }

    eg_selection_description_t *metadata = NULL;
    if (eg_selection_description_new(selection_id, candidate_id, 1UL, &metadata)) {
        assert(false);
    }

    eg_element_mod_q_t *hash_context = NULL;
    if (eg_selection_description_crypto_hash(metadata, &hash_context)) {
        assert(false);
    }

    eg_plaintext_ballot_selection_t *plaintext = NULL;
    if (eg_plaintext_ballot_selection_new(selection_id, 1UL, false, &plaintext)) {
        assert(false);
    }

    eg_element_mod_p_t *public_key = NULL;
    if (eg_elgamal_keypair_get_public_key(key_pair, &public_key)) {
        assert(false);
    }

    // Act
    eg_ciphertext_ballot_selection_t *result = NULL;
    if (eg_encrypt_selection(plaintext, metadata, public_key, one_mod_q, nonce, false, true,
                             &result)) {
        assert(false);
    }

    // Assert
    assert(result != NULL);

    eg_elgamal_ciphertext_t *ciphertext = NULL;
    if (eg_ciphertext_ballot_selection_get_ciphertext(result, &ciphertext)) {
        assert(false);
    }

    eg_disjunctive_chaum_pedersen_proof_t *proof = NULL;
    if (eg_ciphertext_ballot_selection_get_proof(result, &proof)) {
        assert(false);
    }

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
    if (eg_elgamal_keypair_free(key_pair)) {
        assert(false);
    }
    if (eg_element_mod_q_free(nonce)) {
        assert(false);
    }
    if (eg_element_mod_q_free(hash_context)) {
        assert(false);
    }
    if (eg_plaintext_ballot_selection_free(plaintext)) {
        assert(false);
    }
    if (eg_ciphertext_ballot_selection_free(result)) {
        assert(false);
    }
    if (eg_selection_description_free(metadata)) {
        assert(false);
    }

    public_key = NULL;
    ciphertext = NULL;
    proof = NULL;

    return true;
}

bool test_encrypt_contest()
{
    // TODO
    return true;
}

bool test_encrypt_ballot_simple_succeeds(void)
{
    printf("\n -------- test_encrypt_ballot_simple_succeeds -------- \n");

    // Arrange

    eg_element_mod_q_t *two_mod_q = NULL;
    if (eg_element_mod_q_new(TWO_MOD_Q_ARRAY, &two_mod_q)) {
        assert(false);
    }

    eg_elgamal_keypair_t *key_pair = NULL;
    if (eg_elgamal_keypair_from_secret_new(two_mod_q, &key_pair)) {
        assert(false);
    }

    eg_element_mod_p_t *public_key = NULL;
    if (eg_elgamal_keypair_get_public_key(key_pair, &public_key)) {
        assert(false);
    }

    eg_election_description_t *description = NULL;
    if (eg_test_election_generator_get_simple_election_from_file(&description)) {
        assert(false);
    }

    eg_internal_election_description_t *metadata = NULL;
    eg_ciphertext_election_context_t *context = NULL;
    if (eg_test_election_generator_get_fake_ciphertext_election(description, public_key, &metadata,
                                                                &context)) {
        assert(false);
    }

    eg_encryption_device_t *device = NULL;
    if (eg_encryption_device_new(12345UL, 23456UL, 34567UL, "Location", &device)) {
        assert(false);
    }

    eg_plaintext_ballot_t *ballot = NULL;
    if (eg_test_ballot_generator_get_simple_ballot_from_file(&ballot)) {
        assert(false);
    }

    eg_element_mod_q_t *device_hash = NULL;
    if (eg_encryption_device_get_hash(device, &device_hash)) {
        assert(false);
    }

    // Act

    eg_ciphertext_ballot_t *ciphertext = NULL;
    if (eg_encrypt_ballot(ballot, metadata, context, device_hash, true, &ciphertext)) {
        assert(false);
    }

    eg_compact_ciphertext_ballot_t *compact = NULL;
    if (eg_encrypt_compact_ballot(ballot, metadata, context, device_hash, true, &compact)) {
        assert(false);
    }

    uint8_t *compact_data = NULL;
    size_t compact_data_size;
    if (eg_compact_ciphertext_ballot_to_msgpack(compact, &compact_data, &compact_data_size)) {
        assert(false);
    }

    assert(compact_data != NULL);
    assert(compact_data[0] != 0);
    assert(compact_data_size > 0);

    eg_compact_ciphertext_ballot_t *compact_deserialized = NULL;
    if (eg_compact_ciphertext_ballot_from_msgpack(compact_data, compact_data_size,
                                                  &compact_deserialized)) {
        assert(false);
    }

    // Assert
    eg_element_mod_q_t *description_hash = NULL;
    if (eg_ciphertext_election_context_get_description_hash(context, &description_hash)) {
        assert(false);
    }

    eg_element_mod_q_t *extended_base_hash = NULL;
    if (eg_ciphertext_election_context_get_crypto_extended_base_hash(context,
                                                                     &extended_base_hash)) {
        assert(false);
    }

    assert(eg_ciphertext_ballot_is_valid_encryption(ciphertext, description_hash, public_key,
                                                    extended_base_hash) == true);

    // check the serialized compact ballot
    char *compact_object_id;
    if (eg_compact_ciphertext_ballot_get_object_id(compact, &compact_object_id)) {
        assert(false);
    }

    char *compact_deserialized_object_id;
    if (eg_compact_ciphertext_ballot_get_object_id(compact_deserialized,
                                                   &compact_deserialized_object_id)) {
        assert(false);
    }
    assert(strings_are_equal(compact_object_id, compact_deserialized_object_id) == true);

    // Clean Up
    free(compact_deserialized_object_id);
    free(compact_object_id);
    free(compact_data);
    eg_compact_ciphertext_ballot_free(compact_deserialized);
    eg_compact_ciphertext_ballot_free(compact);
    eg_ciphertext_ballot_free(ciphertext);
    eg_plaintext_ballot_free(ballot);
    eg_element_mod_q_free(device_hash);
    eg_encryption_device_free(device);
    eg_ciphertext_election_context_free(context);
    eg_internal_election_description_free(metadata);
    eg_election_description_free(description);
    eg_elgamal_keypair_free(key_pair);
    eg_element_mod_q_free(two_mod_q);

    return true;
}
