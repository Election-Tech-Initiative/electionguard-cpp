#include "electionguard/tracker.hpp"

#include "electionguard/hash.hpp"
#include "log.hpp"
#include "words.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::unique_ptr;
using std::uppercase;

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
        auto bytes = trackerHash.toBytes();
        stringstream stream;

        for (size_t i = 0; (i + 3) < bytes.size(); i += 4) {
            // Select 4 bytes for the segment
            auto first = bytes.at(i);
            auto second = bytes.at(i + 1);
            auto third = bytes.at(i + 2);
            auto fourth = bytes.at(i + 3);

            // word is byte(1) + 1/2 of byte(2)
            auto wordPart = getWord(static_cast<uint16_t>((first * 16 + (second >> 4))));
            // hex is other 1/2 of byte(2) + byte(3) + byte(4)
            auto hexPart = {(second & 0x0f) >> 0, (third & 0xf0) >> 4, (third & 0x0f) >> 0,
                            (fourth & 0xf0) >> 4, (fourth & 0x0f) >> 0};

            stream << wordPart << separator;

            for (auto i : hexPart) {
                stream << hex << uppercase << setw(1) << (uint32_t)i;
            }

            if (fourth == bytes.back()) {
                separator = "";
            }

            stream << separator;
        }

        return stream.str();
    }
} // namespace electionguard
