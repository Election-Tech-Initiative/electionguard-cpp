/// @file election.h
#ifndef __ELECTIONGUARD_CPP_MANIFEST_H_INCLUDED__
#define __ELECTIONGUARD_CPP_MANIFEST_H_INCLUDED__

#include "export.h"
#include "group.h"
#include "status.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SelectionDescription

struct eg_selection_description_s;
typedef struct eg_selection_description_s eg_selection_description_t;

EG_API eg_electionguard_status_t
eg_selection_description_new(const char *in_object_id, const char *in_candidate_id,
                             uint64_t in_sequence_order, eg_selection_description_t **out_handle);

EG_API eg_electionguard_status_t eg_selection_description_free(eg_selection_description_t *handle);

EG_API eg_electionguard_status_t eg_selection_description_get_object_id(
  eg_selection_description_t *handle, const char **out_object_id);

EG_API eg_electionguard_status_t eg_selection_description_get_candidate_id(
  eg_selection_description_t *handle, const char **out_candidate_id);

EG_API eg_electionguard_status_t eg_selection_description_get_sequence_order(
  eg_selection_description_t *handle, uint64_t *out_sequence_order);

EG_API eg_electionguard_status_t eg_selection_description_crypto_hash(
  eg_selection_description_t *handle, eg_element_mod_q_t **out_crypto_hash);

#endif

#ifndef ConstestDescription

struct eg_contest_description_s;
typedef struct eg_contest_description_s eg_contest_description_t;

// TODO: ISSUE #129: implement

#endif

#ifndef ConstestDescriptionWithPlaceholders

struct eg_contest_description_with_placeholders_s;
typedef struct eg_contest_description_with_placeholders_s
  eg_contest_description_with_placeholders_t;

// TODO: ISSUE #129: implement

#endif

#ifndef Manifest

struct eg_election_manifest_s;

/**
* Use this entity for defining the structure of the election and associated
* information such as candidates, contests, and vote counts.  This class is
* based on the NIST Election Common Standard Data Specification.  Some deviations
* from the standard exist.
*
* This structure is considered an immutable input object and should not be changed
* through the course of an election, as it's hash representation is the basis for all
* other hash representations within an ElectionGuard election context.
*
* @see https://developers.google.com/elections-data/reference/election
*/
typedef struct eg_election_manifest_s eg_election_manifest_t;

// no constructors defined.  use `eg_election_manifest_from_json` or `eg_election_manifest_from_bson`

EG_API eg_electionguard_status_t eg_election_manifest_free(eg_election_manifest_t *handle);

EG_API eg_electionguard_status_t eg_election_manifest_get_election_scope_id(
  eg_election_manifest_t *handle, char **out_election_scope_id);

// TODO: ISSUE #129: implement missing members

/**
 * Get a hash of the metadata components of the `Manifest`
 * 
 * @param[in] handle the `eg_election_manifest_t`
 * @param[out] out_owned_hash the `eg_element_mod_q_t` crypto hash.  
 *                            Caller is responsible fo the lifecycle and 
 *                            must call `eg_element_mod_q_free`
 */
EG_API eg_electionguard_status_t eg_election_manifest_crypto_hash(
  eg_election_manifest_t *handle, eg_element_mod_q_t **out_owned_hash);

EG_API eg_electionguard_status_t
eg_election_manifest_from_json(char *in_data, eg_election_manifest_t **out_handle);

EG_API eg_electionguard_status_t eg_election_manifest_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_election_manifest_t **out_handle);

EG_API eg_electionguard_status_t eg_election_manifest_to_json(eg_election_manifest_t *handle,
                                                              char **out_data, size_t *out_size);

// returns the size and fills out_data, caller is responsible for freeing the out_data
EG_API eg_electionguard_status_t eg_election_manifest_to_bson(eg_election_manifest_t *handle,
                                                              uint8_t **out_data, size_t *out_size);

#endif

#ifndef InternalManifest

struct eg_internal_manifest_s;

/**
* `InternalManifest` is a subset of the `Manifest` structure that specifies
* the components that ElectionGuard uses for conducting an election.  The key component is the
* `contests` collection, which applies placeholder selections to the `Manifest` contests
*/
typedef struct eg_internal_manifest_s eg_internal_manifest_t;

/**
 * Constructs an `InternalManifest` by passing the `Manifest` to the
 * constructor
 * 
 * @param[in] in_manifest the `eg_election_manifest_t`
 * @param[out] out_handle An opaque pointer to the object.  Caller is responsible for lifecycle.
 */
EG_API eg_electionguard_status_t eg_internal_manifest_new(eg_election_manifest_t *in_manifest,
                                                          eg_internal_manifest_t **out_handle);

EG_API eg_electionguard_status_t eg_internal_manifest_free(eg_internal_manifest_t *handle);

/**
 * The hash of the election metadata
 * 
 * @param[out] out_manifest_hash_ref An opaque pointer to the description hash.  
 *                                      The value is a reference and is not owned by the caller
 */
EG_API eg_electionguard_status_t eg_internal_manifest_get_manifest_hash(
  eg_internal_manifest_t *handle, eg_element_mod_q_t **out_manifest_hash_ref);

// TODO: ISSUE #129: implement missing members

EG_API eg_electionguard_status_t
eg_internal_manifest_from_json(char *in_data, eg_internal_manifest_t **out_handle);

EG_API eg_electionguard_status_t eg_internal_manifest_from_bson(
  uint8_t *in_data, uint64_t in_length, eg_internal_manifest_t **out_handle);

EG_API eg_electionguard_status_t eg_internal_manifest_to_json(eg_internal_manifest_t *handle,
                                                              char **out_data, size_t *out_size);

// returns the size and fills out_data, caller is responsible for freeing the out_data
EG_API eg_electionguard_status_t eg_internal_manifest_to_bson(eg_internal_manifest_t *handle,
                                                              uint8_t **out_data, size_t *out_size);

#endif

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CPP_MANIFEST_H_INCLUDED__ */
