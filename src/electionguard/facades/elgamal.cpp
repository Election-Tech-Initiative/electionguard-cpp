#include "electionguard/elgamal.hpp"

#include "electionguard/group.hpp"
#include "memory_cache.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/elgamal.h"
}

using electionguard::Cache;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::ElGamalCiphertext;
using electionguard::ElGamalKeyPair;

#pragma region ElGamalKeyPair

// TODO: safe initialization
static Cache<ElGamalCiphertext> cache_ciphertext;
static Cache<ElGamalKeyPair> cache_keypair;
static Cache<ElementModQ> cache_element_mod_q;

eg_elgamal_keypair_t *eg_elgamal_keypair_from_secret(eg_element_mod_q_t *secret_key)
{
    auto *secretKey = AS_TYPE(ElementModQ, secret_key);
    auto keyPair = ElGamalKeyPair::fromSecret(*secretKey);
    auto *reference = cache_keypair.retain(move(keyPair));
    return AS_TYPE(eg_elgamal_keypair_t, reference);
}

eg_element_mod_q_t *eg_elgamal_keypair_get_secret_key(eg_elgamal_keypair_t *keypair)
{
    auto *keyPair_ = AS_TYPE(ElGamalKeyPair, keypair);
    return AS_TYPE(eg_element_mod_q_t, keyPair_->getSecretKey());
}

eg_element_mod_p_t *eg_elgamal_keypair_get_public_key(eg_elgamal_keypair_t *keypair)
{
    auto *keyPair_ = AS_TYPE(ElGamalKeyPair, keypair);
    return AS_TYPE(eg_element_mod_p_t, keyPair_->getPublicKey());
}

#pragma endregion

#pragma region ElGamalCiphertext

eg_element_mod_p_t *eg_elgamal_ciphertext_get_pad(eg_elgamal_ciphertext_t *ciphertext)
{
    auto *pad = AS_TYPE(ElGamalCiphertext, ciphertext)->getPad();
    return AS_TYPE(eg_element_mod_p_t, pad);
}
eg_element_mod_p_t *eg_elgamal_ciphertext_get_data(eg_elgamal_ciphertext_t *ciphertext)
{
    auto *data = AS_TYPE(ElGamalCiphertext, ciphertext)->getData();
    return AS_TYPE(eg_element_mod_p_t, data);
}

eg_element_mod_q_t *eg_elgamal_ciphertext_crypto_hash(eg_elgamal_ciphertext_t *ciphertext)
{
    auto cryptoHash = AS_TYPE(ElGamalCiphertext, ciphertext)->crypto_hash();
    auto *reference = cache_element_mod_q.retain(move(cryptoHash));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

#pragma endregion

#pragma region ElgamalEncrypt

eg_elgamal_ciphertext_t *eg_elgamal_encrypt(uint64_t m, eg_element_mod_q_t *nonce,
                                            eg_element_mod_p_t *public_key)
{
    auto *nonce_ = AS_TYPE(ElementModQ, nonce);
    auto *publicKey = AS_TYPE(ElementModP, public_key);
    auto ciphertext = elgamalEncrypt(m, *nonce_, *publicKey);
    auto *reference = cache_ciphertext.retain(move(ciphertext));
    return AS_TYPE(eg_elgamal_ciphertext_t, reference);
}

#pragma endregion
