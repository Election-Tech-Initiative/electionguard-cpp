#include "electionguard/elgamal.hpp"

#include "../log.hpp"
#include "electionguard/group.hpp"
#include "electionguard/status.h"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/elgamal.h"
}

using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::ElGamalCiphertext;
using electionguard::ElGamalKeyPair;
using electionguard::Log;

#pragma region ElGamalKeyPair

eg_electionguard_status_t eg_elgamal_keypair_from_secret_new(eg_element_mod_q_t *in_secret_key,
                                                             eg_elgamal_keypair_t **out_handle)
{
    if (in_secret_key == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    try {
        auto *secretKey = AS_TYPE(ElementModQ, in_secret_key);
        auto keyPair = ElGamalKeyPair::fromSecret(*secretKey);

        *out_handle = AS_TYPE(eg_elgamal_keypair_t, keyPair.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_elgamal_keypair_from_secret_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_elgamal_keypair_free(eg_elgamal_keypair_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ElGamalKeyPair, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_elgamal_keypair_get_secret_key(eg_elgamal_keypair_t *handle,
                                                            eg_element_mod_q_t **out_secret_key)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }
    auto *keyPair = AS_TYPE(ElGamalKeyPair, handle);
    *out_secret_key = AS_TYPE(eg_element_mod_q_t, keyPair->getSecretKey());
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_elgamal_keypair_get_public_key(eg_elgamal_keypair_t *handle,
                                                            eg_element_mod_p_t **out_public_key)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }
    auto *keyPair = AS_TYPE(ElGamalKeyPair, handle);
    *out_public_key = AS_TYPE(eg_element_mod_p_t, keyPair->getPublicKey());
    return ELECTIONGUARD_STATUS_SUCCESS;
}

#pragma endregion

#pragma region ElGamalCiphertext

eg_electionguard_status_t eg_elgamal_ciphertext_free(eg_elgamal_ciphertext_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(ElGamalCiphertext, handle); // NOLINT(cppcoreguidelines-owning-memory)
    handle = nullptr;
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_elgamal_ciphertext_get_pad(eg_elgamal_ciphertext_t *handle,
                                                        eg_element_mod_p_t **out_pad)
{
    auto *pad = AS_TYPE(ElGamalCiphertext, handle)->getPad();
    *out_pad = AS_TYPE(eg_element_mod_p_t, pad);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_elgamal_ciphertext_get_data(eg_elgamal_ciphertext_t *handle,
                                                         eg_element_mod_p_t **out_data)
{
    auto *data = AS_TYPE(ElGamalCiphertext, handle)->getData();
    *out_data = AS_TYPE(eg_element_mod_p_t, data);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_elgamal_ciphertext_crypto_hash(eg_elgamal_ciphertext_t *handle,
                                                            eg_element_mod_q_t **out_crypto_hash)
{
    try {
        auto cryptoHash = AS_TYPE(ElGamalCiphertext, handle)->crypto_hash();
        *out_crypto_hash = AS_TYPE(eg_element_mod_q_t, cryptoHash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_elgamal_ciphertext_crypto_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_elgamal_ciphertext_decrypt_with_secret(
  eg_elgamal_ciphertext_t *handle, eg_element_mod_q_t *in_secret_key, uint64_t *out_plaintext)
{
    try {
        auto *secretKey = AS_TYPE(ElementModQ, in_secret_key);
        *out_plaintext = AS_TYPE(ElGamalCiphertext, handle)->decrypt(*secretKey);
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_elgamal_ciphertext_decrypt_with_secret", e);
        return ELECTIONGUARD_STATUS_ERROR_RUNTIME_ERROR;
    }
}

#pragma endregion

#pragma region ElgamalEncrypt

eg_electionguard_status_t eg_elgamal_encrypt(uint64_t in_plaintext, eg_element_mod_q_t *in_nonce,
                                             eg_element_mod_p_t *in_public_key,
                                             eg_elgamal_ciphertext_t **out_ciphertext)
{
    try {
        auto *nonce = AS_TYPE(ElementModQ, in_nonce);
        auto *publicKey = AS_TYPE(ElementModP, in_public_key);
        auto ciphertext = elgamalEncrypt(in_plaintext, *nonce, *publicKey);
        *out_ciphertext = AS_TYPE(eg_elgamal_ciphertext_t, ciphertext.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(":eg_elgamal_encrypt", e);
        return ELECTIONGUARD_STATUS_ERROR_RUNTIME_ERROR;
    }
}

#pragma endregion
