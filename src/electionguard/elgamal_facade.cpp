#include "electionguard/elgamal.hpp"
#include "electionguard/group.hpp"

extern "C" {
#include "electionguard/elgamal.h"
}

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)

using namespace electionguard;

// ElGamalCiphertext
EG_API eg_elgamal_ciphertext_t *eg_elgamal_ciphertext_new(eg_element_mod_p_t *pad,
                                                          eg_element_mod_p_t *data)
{
    auto *pad_ = AS_TYPE(ElementModP, pad);
    auto *data_ = AS_TYPE(ElementModP, data);
    return AS_TYPE(eg_elgamal_ciphertext_t, new ElGamalCiphertext(pad_, data_));
}
EG_API void eg_elgamal_ciphertext_free(eg_elgamal_ciphertext_t *ciphertext)
{
    if (ciphertext == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(ElGamalCiphertext, ciphertext);
}

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
    auto *cryptoHash = AS_TYPE(ElGamalCiphertext, ciphertext)->crypto_hash();
    return AS_TYPE(eg_element_mod_q_t, cryptoHash);
}

// ElgamalEncrypt

EG_API eg_elgamal_ciphertext_t *eg_elgamal_encrypt(uint64_t m, eg_element_mod_q_t *nonce,
                                                   eg_element_mod_p_t *public_key)
{
    auto *nonce_ = AS_TYPE(ElementModQ, nonce);
    auto *publicKey = AS_TYPE(ElementModP, public_key);
    return AS_TYPE(eg_elgamal_ciphertext_t, elgamalEncrypt(m, nonce_, publicKey));
}