#include <assert.h>
#include <electionguard/chaum_pedersen.h>

static bool test_constant_chaum_pedersen_proof(void);

bool test_chaum_pedersen_proof(void) { return test_constant_chaum_pedersen_proof(); }
bool test_constant_chaum_pedersen_proof(void)
{
    // Arrange
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
    if (eg_element_mod_q_new(ONE_MOD_Q_ARRAY, &nonce)) {
        assert(false);
    }

    eg_element_mod_q_t *seed = NULL;
    if (eg_element_mod_q_new(TWO_MOD_Q_ARRAY, &seed)) {
        assert(false);
    }

    eg_element_mod_p_t *public_key = NULL;
    if (eg_elgamal_keypair_get_public_key(key_pair, &public_key)) {
        assert(false);
    }

    eg_elgamal_ciphertext_t *message = NULL;
    if (eg_elgamal_encrypt(0UL, nonce, public_key, &message)) {
        assert(false);
    }

    // Act
    eg_constant_chaum_pedersen_proof_t *proof = NULL;
    if (eg_constant_chaum_pedersen_proof_make(message, nonce, public_key, seed, one_mod_q, 0UL,
                                              &proof)) {
        assert(false);
    }
    eg_constant_chaum_pedersen_proof_t *bad_proof = NULL;
    if (eg_constant_chaum_pedersen_proof_make(message, nonce, public_key, seed, one_mod_q, 1UL,
                                              &bad_proof)) {
        assert(false);
    }

    // Assert
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
    if (eg_elgamal_keypair_free(key_pair)) {
        assert(false);
    }
    if (eg_elgamal_ciphertext_free(message)) {
        assert(false);
    }
    if (eg_constant_chaum_pedersen_proof_free(proof)) {
        assert(false);
    }
    if (eg_constant_chaum_pedersen_proof_free(bad_proof)) {
        assert(false);
    }

    // Don't call free since this is a property handle
    // and we don't own the object
    public_key = NULL;

    return true;
}
