/// @file manifest.h
#ifndef __ELECTIONGUARD_CPP_TEST_MOCKS_MAANIFEST_H_INCLUDED__
#define __ELECTIONGUARD_CPP_TEST_MOCKS_MAANIFEST_H_INCLUDED__

#include <electionguard/export.h>
#include <electionguard/manifest.h>

#ifdef __cplusplus
extern "C" {
#endif

EG_API eg_electionguard_status_t
eg_test_election_mocks_get_simple_election_from_file(eg_election_manifest_t **out_handle);

#ifdef __cplusplus
}
#endif

#endif /* __ELECTIONGUARD_CPP_TEST_MOCKS_MAANIFEST_H_INCLUDED__ */
