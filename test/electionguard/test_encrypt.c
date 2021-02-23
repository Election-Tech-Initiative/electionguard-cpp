#include "utils/utils.h"

#include <assert.h>
#include <electionguard/encrypt.h>
#include <stdlib.h>

bool strings_are_equal(char *expected, char *actual);

static bool test_encrypt_selection(void);
static bool test_encrypt_contest(void);
static bool test_encrypt_ballot(void);

bool test_encrypt(void)
{
    printf("\n -------- test_encrypt.c --------- \n");
    return test_encrypt_selection() && test_encrypt_contest() && test_encrypt_ballot();
}

bool test_encrypt_selection(void)
{
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

bool test_encrypt_contest() { return true; }

bool test_encrypt_ballot()
{
    // TODO
    return true;
}
