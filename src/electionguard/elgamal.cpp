#include "electionguard/elgamal.hpp"

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

    ElGamalCiphertext *elgamalEncrypt(uint64_t m, ElementModQ *nonce, ElementModP *publicKey)
    {
        if (m > 0) {
            // just bypass compiler error
        }
        if (nonce->get() != nullptr) {
            // just bypass compiler error
        }
        return new ElGamalCiphertext(publicKey, publicKey);
    }
} // namespace electionguard