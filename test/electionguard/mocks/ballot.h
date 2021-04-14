/// @file ballot.h
#ifndef __ELECTIONGUARD_CPP_TEST_MOCKS_BALLOT_H_INCLUDED__
#define __ELECTIONGUARD_CPP_TEST_MOCKS_BALLOT_H_INCLUDED__

#include <electionguard/ballot.h>
#include <electionguard/export.h>

#ifdef __cplusplus
extern "C" {
#endif

EG_API eg_electionguard_status_t
eg_test_ballot_mocks_get_simple_ballot_from_file(eg_plaintext_ballot_t **out_handle);

#ifdef __cplusplus
}
#endif

#endif /* __ELECTIONGUARD_CPP_TEST_MOCKS_BALLOT_H_INCLUDED__ */
