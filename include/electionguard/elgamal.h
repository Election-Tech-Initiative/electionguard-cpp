#ifndef __ELECTIONGUARD_ELGAMAL_H_INCLUDED__
#define __ELECTIONGUARD_ELGAMAL_H_INCLUDED__

#include "export.h"
#include "group.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ElgamalKeyPair

struct eg_elgamal_keypair_s;
typedef struct eg_elgamal_keypair_s eg_elgamal_keypair_t;

// no constructors defined.  use `eg_elgamal_keypair_from_secret_new`

EG_API eg_electionguard_status_t eg_elgamal_keypair_from_secret_new(
  eg_element_mod_q_t *in_secret_key, eg_elgamal_keypair_t **out_handle);

EG_API eg_electionguard_status_t eg_elgamal_keypair_free(eg_elgamal_keypair_t *handle);

EG_API eg_electionguard_status_t eg_elgamal_keypair_get_public_key(
  eg_elgamal_keypair_t *handle, eg_element_mod_p_t **out_public_key);

EG_API eg_electionguard_status_t eg_elgamal_keypair_get_secret_key(
  eg_elgamal_keypair_t *handle, eg_element_mod_q_t **out_secret_key);

#endif

#ifndef ElGamalCiphertext

struct eg_elgamal_ciphertext_s;
typedef struct eg_elgamal_ciphertext_s eg_elgamal_ciphertext_t;

// no constructors defined.  use `eg_elgamal_encrypt`

EG_API eg_electionguard_status_t eg_elgamal_ciphertext_free(eg_elgamal_ciphertext_t *handle);

EG_API eg_electionguard_status_t eg_elgamal_ciphertext_get_pad(eg_elgamal_ciphertext_t *handle,
                                                               eg_element_mod_p_t **out_pad);
EG_API eg_electionguard_status_t eg_elgamal_ciphertext_get_data(eg_elgamal_ciphertext_t *handle,
                                                                eg_element_mod_p_t **out_data);

EG_API eg_electionguard_status_t eg_elgamal_ciphertext_crypto_hash(
  eg_elgamal_ciphertext_t *handle, eg_element_mod_q_t **out_crypto_hash);

EG_API eg_electionguard_status_t eg_elgamal_ciphertext_decrypt_with_secret(
  eg_elgamal_ciphertext_t *handle, eg_element_mod_q_t *in_secret_key, uint64_t *out_plaintext);

#endif

#ifndef ElgamalEncrypt

EG_API eg_electionguard_status_t eg_elgamal_encrypt(uint64_t in_plaintext,
                                                    eg_element_mod_q_t *in_nonce,
                                                    eg_element_mod_p_t *in_public_key,
                                                    eg_elgamal_ciphertext_t **out_ciphertext);

#endif

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_ELGAMAL_H_INCLUDED__ */
