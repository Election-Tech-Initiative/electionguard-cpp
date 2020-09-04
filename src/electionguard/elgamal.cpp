#include "electionguard/elgamal.hpp"

#include "electionguard/hash.hpp"

namespace electionguard
{

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
        if (m > 0) {
            // just bypass compiler error
        }
        if (nonce->get() != nullptr) {
            // just bypass compiler error
        }

        return new ElGamalCiphertext(
          g_pow_p(nonce->toP()),
          mul_mod_p(g_pow_p(uint64_to_p(m)), pow_mod_p(publicKey, nonce->toP())));
    }

} // namespace electionguard