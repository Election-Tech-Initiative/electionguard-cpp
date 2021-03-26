#include "ballot.hpp"

#include "../../../src/electionguard/log.hpp"
#include "../../../src/electionguard/variant_cast.hpp"

#include <electionguard/ballot.hpp>

extern "C" {
#include "ballot.h"
}

using namespace electionguard;
using namespace std;

eg_electionguard_status_t
eg_test_ballot_mocks_get_simple_ballot_from_file(eg_plaintext_ballot_t **out_handle)
{
    try {
        auto election = electionguard::test::mocks::BallotGenerator::getSimpleBallotFromFile();
        *out_handle = AS_TYPE(eg_plaintext_ballot_t, election.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        electionguard::Log::error(": eg_test_election_mocks_get_simple_election_from_file", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}
