/// @file ballot.h
#ifndef __ELECTIONGUARD_CPP_TEST_GENERATORS_BALLOT_H_INCLUDED__
#define __ELECTIONGUARD_CPP_TEST_GENERATORS_BALLOT_H_INCLUDED__

#include <electionguard/ballot.h>

#ifdef __cplusplus
extern "C" {
#endif

eg_electionguard_status_t
eg_test_ballot_generator_get_simple_ballot_from_file(eg_plaintext_ballot_t **out_handle);

#ifdef __cplusplus
}
#endif

#endif /* __ELECTIONGUARD_CPP_TEST_GENERATORS_BALLOT_H_INCLUDED__ */
