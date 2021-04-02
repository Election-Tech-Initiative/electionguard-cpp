#include "election.hpp"

#include "../../../src/electionguard/log.hpp"
#include "../../../src/electionguard/variant_cast.hpp"

#include <electionguard/election.hpp>
#include <electionguard/manifest.hpp>

extern "C" {
#include "election.h"
}

using namespace electionguard;
using namespace std;

eg_electionguard_status_t
eg_test_election_mocks_get_simple_election_from_file(eg_election_manifest_t **out_handle)
{
    try {
        auto election = electionguard::test::mocks::ElectionGenerator::getSimpleElectionFromFile();
        *out_handle = AS_TYPE(eg_election_manifest_t, election.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        electionguard::Log::error(": eg_test_election_mocks_get_simple_election_from_file", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_test_election_mocks_get_fake_ciphertext_election(
  eg_election_manifest_t *in_manifest, eg_element_mod_p_t *in_public_key,
  eg_internal_manifest_t **out_manifest, eg_ciphertext_election_context_t **out_context)
{
    try {
        auto *description = AS_TYPE(Manifest, in_manifest);
        auto *publicKey = AS_TYPE(ElementModP, in_public_key);
        auto [manifest, context] =
          electionguard::test::mocks::ElectionGenerator::getFakeCiphertextElection(*description,
                                                                                   *publicKey);
        *out_manifest = AS_TYPE(eg_internal_manifest_t, manifest.release());
        *out_context = AS_TYPE(eg_ciphertext_election_context_t, context.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        electionguard::Log::error(": eg_test_election_mocks_get_simple_election_from_file", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}