#ifndef __ELECTIONGUARD_ELGAMAL_H_INCLUDED__
#define __ELECTIONGUARD_ELGAMAL_H_INCLUDED__

#include "export.h"
#include "group.h"

#ifdef __cplusplus
extern "C" {
#endif

// ElgamalKeyPair

struct eg_elgamal_keypair_s;
typedef struct eg_elgamal_keypair_s eg_elgamal_keypair_t;

// no constructors defined.  use `eg_elgamal_keypair_from_secret`

EG_API eg_elgamal_keypair_t *eg_elgamal_keypair_from_secret(eg_element_mod_q_t *secret_key);

EG_API eg_element_mod_q_t *eg_elgamal_keypair_get_secret_key(eg_elgamal_keypair_t *keypair);
EG_API eg_element_mod_p_t *eg_elgamal_keypair_get_public_key(eg_elgamal_keypair_t *keypair);

// ElGamalCiphertext

struct eg_elgamal_ciphertext_s;
typedef struct eg_elgamal_ciphertext_s eg_elgamal_ciphertext_t;

// no constructors defined.  use `eg_elgamal_encrypt`

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
