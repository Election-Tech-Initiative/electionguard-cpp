/// @file election.h
#ifndef __ELECTIONGUARD_CPP_TEST_MOCKS_ELECTION_H_INCLUDED__
#define __ELECTIONGUARD_CPP_TEST_MOCKS_ELECTION_H_INCLUDED__

#include <electionguard/election.h>

#ifdef __cplusplus
extern "C" {
#endif

eg_electionguard_status_t
eg_test_election_mocks_get_simple_election_from_file(eg_election_manifest_t **out_handle);

eg_electionguard_status_t eg_test_election_mocks_get_fake_ciphertext_election(
  eg_election_manifest_t *in_manifest, eg_element_mod_p_t *in_public_key,
  eg_internal_manifest_t **out_manifest, eg_ciphertext_election_context_t **out_context);

#ifdef __cplusplus
}
#endif

#endif /* __ELECTIONGUARD_CPP_TEST_MOCKS_ELECTION_H_INCLUDED__ */
