#ifndef __ELECTIONGUARD_ELGAMAL_H_INCLUDED__
#define __ELECTIONGUARD_ELGAMAL_H_INCLUDED__

#include "export.h"
#include "group.h"

#ifdef __cplusplus
extern "C" {
#endif

// ElGamalCiphertext
struct eg_elgamal_ciphertext_s;
typedef struct eg_elgamal_ciphertext_s eg_elgamal_ciphertext_t;

EG_API eg_elgamal_ciphertext_t *eg_elgamal_ciphertext_new(eg_element_mod_p_t *pad,
                                                          eg_element_mod_p_t *data);
EG_API void eg_elgamal_ciphertext_free(eg_elgamal_ciphertext_t *ciphertext);

eg_element_mod_p_t *eg_elgamal_ciphertext_get_pad(eg_elgamal_ciphertext_t *ciphertext);
eg_element_mod_p_t *eg_elgamal_ciphertext_get_data(eg_elgamal_ciphertext_t *ciphertext);

eg_element_mod_q_t *eg_elgamal_ciphertext_crypto_hash(eg_elgamal_ciphertext_t *ciphertext);

// ElgamalEncrypt

EG_API eg_elgamal_ciphertext_t *eg_elgamal_encrypt(uint64_t m, eg_element_mod_q_t *nonce,
                                                   eg_element_mod_p_t *public_key);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_ELGAMAL_H_INCLUDED__ */
