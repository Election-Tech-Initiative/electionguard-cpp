#include "electionguard/tracker.hpp"

#include "hash.hpp"
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
    ElementModQ *Tracker::getHashForDevice(uint64_t uuid, const string location)
    {
        return hash_elems({uuid, location});
    }
    ElementModQ *Tracker::getRotatingTrackerHash(ElementModQ *previous, uint64_t timestamp,
                                                 ElementModQ *ballotHash)
    {
        return hash_elems({previous, timestamp, ballotHash});
    }

    string Tracker::hashToWords(ElementModQ *trackerHash, char *separator)
    {
        // TODO: implement properly
        return "Doggo ipsum long woofer pats blep thicc shooberino, sub woofer pupper";
    }
} // namespace electionguard
