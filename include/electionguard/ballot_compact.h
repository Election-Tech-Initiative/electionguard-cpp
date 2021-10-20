/// @file ballot_compact.h
#ifndef __ELECTIONGUARD_CPP_BALLOT_COMPACT_H_INCLUDED__
#define __ELECTIONGUARD_CPP_BALLOT_COMPACT_H_INCLUDED__

#include "ballot.h"
#include "chaum_pedersen.h"
#include "elgamal.h"
#include "export.h"
#include "group.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CompactPlaintextBallot

struct eg_compact_plaintext_ballot_s;

/*
* A CompactPlaintextBallot is a representation of a PlaintextBallot
* that includes only the minimal data necessary to interact with electionguard
*
* This class is space optimized to serve specific use cases on systems where storage
* is a concern.  This object expects an array of slection value votes that are ordered
* correctly according to the sequence order of all of the contests and selections on a ballot
* The CompactPlaintextBallot can be expanded into a PlaintextBallot by traversing the
* selections array and rebuilding the selections from the vote values.
*
* Extended data is included as a map where the key value of the map is the selection id
* corresponding to the selection that includes any extended data.
*
* Don't make this directly. Use `make` instead.
*/
typedef struct eg_compact_plaintext_ballot_s eg_compact_plaintext_ballot_t;

EG_API eg_electionguard_status_t
eg_compact_plaintext_ballot_free(eg_compact_plaintext_ballot_t *handle);

// TODO: other methods

EG_API eg_electionguard_status_t eg_compact_plaintext_ballot_from_msgpack(
  uint8_t *in_data, uint64_t in_length, eg_compact_plaintext_ballot_t **out_handle);

EG_API eg_electionguard_status_t eg_compact_plaintext_ballot_to_msgpack(
  eg_compact_plaintext_ballot_t *handle, uint8_t **out_data, uint64_t *out_size);

EG_API eg_electionguard_status_t eg_compact_plaintext_ballot_msgpack_free(uint8_t *in_data);

#endif

#ifndef CompactCiphertextBallot

struct eg_compact_ciphertext_ballot_s;

/*
* A CompactCiphertextBallot is a CompactPlaintextBallot that includes the encryption parameters
* to properly re-encrypt the same ballot.
*
* This class is space optimized to serve specific use cases where an encrypted ballot is used
* to verify that plaintext selections have not been tampered with.
*
* Don't make this directly. Use `make` instead.
*/
typedef struct eg_ciphertext_ballot_s eg_compact_ciphertext_ballot_t;

// no constructors defined.  use `eg_encrypt_compact_ballot` in encrypt.h

EG_API eg_electionguard_status_t
eg_compact_ciphertext_ballot_free(eg_compact_ciphertext_ballot_t *handle);

// TODO: other methods

EG_API eg_electionguard_status_t eg_compact_ciphertext_ballot_get_object_id(
  eg_compact_ciphertext_ballot_t *handle, char **out_object_id);

EG_API eg_electionguard_status_t eg_compact_ciphertext_ballot_from_msgpack(
  uint8_t *in_data, uint64_t in_length, eg_compact_ciphertext_ballot_t **out_handle);

EG_API eg_electionguard_status_t eg_compact_ciphertext_ballot_to_msgpack(
  eg_compact_ciphertext_ballot_t *handle, uint8_t **out_data, uint64_t *out_size);

EG_API eg_electionguard_status_t eg_compact_ciphertext_ballot_msgpack_free(uint8_t *in_data);

#endif

#ifndef Compress Functions

// TODO: implement

#endif

#ifndef Expand Functions

// TODO: implement

#endif

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CPP_BALLOT_H_INCLUDED__ */
