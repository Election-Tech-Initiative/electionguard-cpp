#include "electionguard/elgamal.hpp"

#include "electionguard/hash.hpp"
#include "log.hpp"

namespace electionguard
{
    // ElGamalKeyPair

    ElGamalKeyPair::ElGamalKeyPair(ElementModQ *secretKey, ElementModP *publicKey) : data()
    {
        data.secretKey = secretKey;
        data.publicKey = publicKey;
    }

    ElGamalKeyPair::~ElGamalKeyPair() {}

    ElGamalKeyPair *ElGamalKeyPair::fromSecret(ElementModQ *a)
    {
        if (*a < *TWO_MOD_Q()) {
            Log::debug(" ElGamal secret key needs to be in [2,Q).");
            return nullptr;
        }

        return new ElGamalKeyPair(a, g_pow_p(a->toElementModP()));
    }

    ElementModQ *ElGamalKeyPair::getSecretKey() { return data.secretKey; }

    ElementModP *ElGamalKeyPair::getPublicKey() { return data.publicKey; }

    // ElGamalCiphertext

    ElGamalCiphertext::ElGamalCiphertext(ElementModP *pad, ElementModP *data) : data()
    {
        this->data.pad = pad;
        this->data.data = data;
    }

    ElGamalCiphertext::~ElGamalCiphertext() {}

    ElementModP *ElGamalCiphertext::getPad() { return data.pad; }
    ElementModP *ElGamalCiphertext::getData() { return data.data; }

    ElementModQ *ElGamalCiphertext::crypto_hash() { return hash_elems({data.pad, data.data}); }

    ElGamalCiphertext *elgamalEncrypt(uint64_t m, ElementModQ *nonce, ElementModP *publicKey)
    {
        if ((*nonce == *ZERO_MOD_Q())) {
            Log::debug("ElGamal encryption requires a non-zero nonce");
            return nullptr;
        }

        return new ElGamalCiphertext(
          g_pow_p(nonce->toElementModP()),
          mul_mod_p(g_pow_p(uint64_to_p(m)), pow_mod_p(publicKey, nonce->toElementModP())));
    }

} // namespace electionguard