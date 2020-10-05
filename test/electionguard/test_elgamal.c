#include <assert.h>
#include <electionguard/elgamal.h>

static bool test_elgamal_encrypt_simple(void);

bool test_elgamal(void) { return test_elgamal_encrypt_simple(); }

bool test_elgamal_encrypt_simple(void)
{
    // Arrange
    eg_element_mod_q_t *nonce = NULL;
    if (eg_element_mod_q_new(ONE_MOD_Q_ARRAY, &nonce)) {
        assert(false);
    }

    eg_element_mod_q_t *secret = NULL;
    if (eg_element_mod_q_new(TWO_MOD_Q_ARRAY, &secret)) {
        assert(false);
    }

    eg_elgamal_keypair_t *key_pair = NULL;
    if (eg_elgamal_keypair_from_secret_new(secret, &key_pair)) {
        assert(false);
    }

    eg_element_mod_p_t *public_key = NULL;
    if (eg_elgamal_keypair_get_public_key(key_pair, &public_key)) {
        assert(false);
    }

    // Act
    eg_elgamal_ciphertext_t *ciphertext = NULL;
    if (eg_elgamal_encrypt(1UL, nonce, public_key, &ciphertext)) {
        assert(false);
    }

    uint64_t plaintext = 0;
    if (eg_elgamal_ciphertext_decrypt_with_secret(ciphertext, secret, &plaintext)) {
        assert(false);
    }

    // Assert
    assert(plaintext == 1UL);

    // Clean Up
    if (eg_element_mod_q_free(nonce)) {
        assert(false);
    }
    if (eg_element_mod_q_free(secret)) {
        assert(false);
    }
    if (eg_elgamal_keypair_free(key_pair)) {
        assert(false);
    }
    if (eg_elgamal_ciphertext_free(ciphertext)) {
        assert(false);
    }

    // Don't call free, we don't own it.
    public_key = NULL;

    return true;
}
