#include "electionguard/ballot_code.hpp"

#include "electionguard/hash.hpp"
#include "log.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::unique_ptr;

namespace electionguard
{
    unique_ptr<ElementModQ> BallotCode::getHashForDevice(uint64_t deviceUuid, uint64_t sessionUuid,
                                                         uint64_t launchCode,
                                                         const string &location)
    {
        return hash_elems({deviceUuid, sessionUuid, launchCode, location});
    }

    unique_ptr<ElementModQ> BallotCode::getBallotCode(const ElementModQ &seed, uint64_t timestamp,
                                                      const ElementModQ &ballotCode)
    {
        return hash_elems(
          {&const_cast<ElementModQ &>(seed), timestamp, &const_cast<ElementModQ &>(ballotCode)});
    }

} // namespace electionguard
