#ifndef __ELECTIONGUARD_CORE_TRACKER_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_TRACKER_HPP_INCLUDED__

#include "export.h"
#include "group.hpp"

#include <string>

namespace electionguard
{
    class EG_API Tracker
    {
      public:
        static ElementModQ *getHashForDevice(uint64_t uuid, const string location);
        static ElementModQ *getRotatingTrackerHash(ElementModQ *previous, uint64_t timestamp,
                                                   ElementModQ *ballotHash);

        static string hashToWords(ElementModQ *trackerHash, char *separator = " ");
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__ */