#include "election.hpp"

#include "../../../src/electionguard/log.hpp"
#include "../../../src/electionguard/variant_cast.hpp"

#include <electionguard/election.hpp>

extern "C" {
#include "election.h"
}

using namespace electionguard;
using namespace std;

eg_electionguard_status_t
eg_test_election_generator_get_simple_election_from_file(eg_election_description_t **out_handle)
{
    try {
        auto election =
          electionguard::test::generators::ElectionGenerator::getSimpleElectionFromFile();
        *out_handle = AS_TYPE(eg_election_description_t, election.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        electionguard::Log::error(": eg_test_election_generator_get_simple_election_from_file", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_test_election_generator_get_fake_ciphertext_election(
  eg_election_description_t *in_description, eg_element_mod_p_t *in_public_key,
  eg_internal_election_description_t **out_metadata, eg_ciphertext_election_context_t **out_context)
{
    try {
        auto description = AS_TYPE(ElectionDescription, in_description);
        auto publicKey = AS_TYPE(ElementModP, in_public_key);
        auto [metadata, context] =
          electionguard::test::generators::ElectionGenerator::getFakeCiphertextElection(
            *description, *publicKey);
        *out_metadata = AS_TYPE(eg_internal_election_description_t, metadata.release());
        *out_context = AS_TYPE(eg_ciphertext_election_context_t, context.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        electionguard::Log::error(": eg_test_election_generator_get_simple_election_from_file", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}