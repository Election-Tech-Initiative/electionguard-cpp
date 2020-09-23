#include "electionguard/tracker.hpp"

#include "electionguard/hash.hpp"
#include "words.hpp"

#include <algorithm>
#include <string>
#include <vector>

using std::distance;
using std::find;
using std::string;
using std::vector;

namespace electionguard
{
    unique_ptr<ElementModQ> Tracker::getHashForDevice(uint64_t uuid, const string &location)
    {
        return hash_elems({uuid, location});
    }

    unique_ptr<ElementModQ> Tracker::getRotatingTrackerHash(const ElementModQ &previous,
                                                            uint64_t timestamp,
                                                            const ElementModQ &ballotHash)
    {
        return hash_elems({&const_cast<ElementModQ &>(previous), timestamp,
                           &const_cast<ElementModQ &>(ballotHash)});
    }

    string Tracker::hashToWords(const ElementModQ &trackerHash, const char *separator)
    {
        // TODO: implement properly
        return "Doggo ipsum long woofer pats blep thicc shooberino, sub woofer pupper";
    }
} // namespace electionguard
