#ifndef __ELECTIONGUARD_CPP_TRACKER_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_TRACKER_HPP_INCLUDED__

#include "export.h"
#include "group.hpp"

#include <memory>
#include <string>

namespace electionguard
{
    class EG_API Tracker
    {
      public:
        static std::unique_ptr<ElementModQ> getHashForDevice(uint64_t deviceUuid,
                                                             uint64_t sessionUuid,
                                                             uint64_t launchCode,
                                                             const std::string &location);

        // TODO: use explicit time type
        static std::unique_ptr<ElementModQ> getRotatingTrackerHash(const ElementModQ &previous,
                                                                   uint64_t timestamp,
                                                                   const ElementModQ &ballotHash);
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_ENCRYPT_HPP_INCLUDED__ */