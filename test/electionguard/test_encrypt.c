#include <assert.h>
#include <electionguard/encrypt.h>

static bool test_encrypt_selection(void);

bool test_encrypt(void) { return test_encrypt_selection(); }

bool test_encrypt_selection(void)
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

    eg_elgamal_keypair_t *key_pair = NULL;
    if (eg_elgamal_keypair_from_secret_new(two_mod_q, &key_pair)) {
        assert(false);
    }

    eg_element_mod_q_t *nonce = NULL;
    if (eg_element_mod_q_rand_q_new(&nonce)) {
        assert(false);
    }

    eg_selection_description_t *metadata =
      eg_selection_description_create("some-selection-object_id", candidate_id, 1UL);

    eg_element_mod_q_t *hash_context = eg_selection_description_crypto_hash(metadata);
    eg_plaintext_ballot_selection_t *plaintext =
      eg_plaintext_ballot_selection_create(candidate_id, "1");

    eg_element_mod_p_t *public_key = NULL;
    if (eg_elgamal_keypair_get_public_key(key_pair, &public_key)) {
        assert(false);
    }

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
    if (eg_elgamal_keypair_free(key_pair)) {
        assert(false);
    }
    if (eg_element_mod_q_free(nonce)) {
        assert(false);
    }

    public_key = NULL;

    return true;
}
