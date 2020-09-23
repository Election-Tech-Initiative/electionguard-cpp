#ifndef __ELECTIONGUARD_CORE_TRACKER_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_TRACKER_HPP_INCLUDED__

#include "export.h"
#include "group.hpp"

#include <memory>
#include <string>

namespace electionguard
{
    class EG_API Tracker
    {
      public:
        static unique_ptr<ElementModQ> getHashForDevice(uint64_t uuid, const string &location);

        // TODO: use explicit time type
        static unique_ptr<ElementModQ> getRotatingTrackerHash(const ElementModQ &previous,
                                                              uint64_t timestamp,
                                                              const ElementModQ &ballotHash);

        static string hashToWords(const ElementModQ &trackerHash, const char *separator = " ");
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__ */