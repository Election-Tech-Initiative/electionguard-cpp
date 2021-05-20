#include "manifest.hpp"

#include "../../../src/electionguard/log.hpp"
#include "../../../src/electionguard/variant_cast.hpp"

#include <electionguard/manifest.hpp>

extern "C" {
#include "manifest.h"
}

using namespace electionguard;
using namespace std;

EG_API eg_electionguard_status_t
eg_test_election_mocks_get_simple_election_from_file(eg_election_manifest_t **out_handle)
{
    try {
        auto election = electionguard::test::mocks::ManifestGenerator::
          getJeffersonCountryManifest_multipleBallotStyle_fromFile();
        *out_handle = AS_TYPE(eg_election_manifest_t, election.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        electionguard::Log::error(": eg_test_election_mocks_get_simple_election_from_file", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}
