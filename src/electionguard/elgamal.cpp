#include "electionguard/elgamal.hpp"

#include "electionguard/hash.hpp"
#include "log.hpp"

namespace electionguard
{
#pragma region ElgamalKeyPair

    // Lifecycle Methods

    ElGamalKeyPair::ElGamalKeyPair(ElementModQ *secretKey, ElementModP *publicKey) : data()
    {
        data.secretKey = secretKey;
        data.publicKey = publicKey;
    }

    ElGamalKeyPair::~ElGamalKeyPair()
    {
        delete data.secretKey;
        delete data.publicKey;
    }

    // Property Getters

    ElementModQ *ElGamalKeyPair::getSecretKey() { return data.secretKey; }

    ElementModP *ElGamalKeyPair::getPublicKey() { return data.publicKey; }

    // Public Members

    ElGamalKeyPair *ElGamalKeyPair::fromSecret(ElementModQ *a)
    {
        if (*a < *TWO_MOD_Q()) {
            Log::debug(" ElGamal secret key needs to be in [2,Q).");
            return nullptr;
        }

        return new ElGamalKeyPair(a, g_pow_p(a->toElementModP()));
    }

#pragma endregion

#pragma region ElGamalCiphertext

    ElGamalCiphertext::ElGamalCiphertext(ElementModP *pad, ElementModP *data) : data()
    {
        this->data.pad = pad;
        this->data.data = data;
    }

    ElGamalCiphertext::~ElGamalCiphertext() {}

    ElementModP *ElGamalCiphertext::getPad() { return data.pad; }
    ElementModP *ElGamalCiphertext::getData() { return data.data; }

    uint64_t ElGamalCiphertext::decrypt(ElementModP *product) { return 0; }
    uint64_t ElGamalCiphertext::decrypt(ElementModQ *secretKey)
    {
        auto *result = pow_mod_p(data.pad, secretKey->toElementModP());
        return result->get()[0];
    }
    uint64_t ElGamalCiphertext::decrypt(ElementModP *publicKey, ElementModQ *nonce) { return 0; }

    ElementModQ *ElGamalCiphertext::crypto_hash() { return hash_elems({data.pad, data.data}); }

    ElGamalCiphertext *elgamalEncrypt(uint64_t m, ElementModQ *nonce, ElementModP *publicKey)
    {
        if ((*nonce == *ZERO_MOD_Q())) {
            Log::debug("ElGamal encryption requires a non-zero nonce");
            return nullptr;
        }

        auto *pad = g_pow_p(nonce);
        auto *e = ElementModP::fromUint64(m);
        auto *gpowp_m = g_pow_p(e);
        auto *pubkey_pow_n = pow_mod_p(publicKey, nonce->toElementModP());
        auto *data = mul_mod_p(gpowp_m, pubkey_pow_n);

        // Log::debugHex(" e->get: ", e->toHex());
        // Log::debugHex(" gpowp_m->get: ", gpowp_m->toHex());
        // Log::debugHex(" pubkey_pow_n->get: ", pubkey_pow_n->toHex());
        // Log::debugHex(" data->get: ", data->toHex());

        auto *ciphertext = new ElGamalCiphertext(pad, data);

        delete pubkey_pow_n;
        delete gpowp_m;
        delete e;

        return ciphertext;
    }

#pragma endregion

} // namespace electionguard