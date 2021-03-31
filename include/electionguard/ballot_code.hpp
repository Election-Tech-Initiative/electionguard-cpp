#ifndef __ELECTIONGUARD_CPP_BALLOT_CODE_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_BALLOT_CODE_HPP_INCLUDED__

#include "export.h"
#include "group.hpp"

#include <memory>
#include <string>

namespace electionguard
{
    class EG_API BallotCode
    {
      public:
        /// <summary>
        /// Get a hash for a specific device.  The hash includes several components
        /// to guarantee the uniqueness of the device, the session instance, and the election context
        ///
        /// It is up to the consuming application to convey meaning to these fields.
        ///
        /// <param name="deviceUuid">Unique identifier of device such as a hardware Id</param>
        /// <param name="sessionUuid">Unique identifier of the application's session instance,
        ///                           can be randomly generated on startup</param>
        /// <param name="launchCode">Runtime launch code associated wit ha specific election</param>
        /// <param name="location">location of device, as an arbitrary string that is meaningful
        ///                        to the external system</param>
        /// <returns>A hash of device</returns>
        /// </summary>
        static std::unique_ptr<ElementModQ> getHashForDevice(uint64_t deviceUuid,
                                                             uint64_t sessionUuid,
                                                             uint64_t launchCode,
                                                             const std::string &location);

        /// <summary>
        /// Get a ballot code based on a seed.  Useful for chaining ballots or defining higher order link strcutures.
        ///
        /// Though not required by the spec, it can be useful for some use cases to chain ballots together
        /// And prove an unbroken dataset for a specific encryption device. Typically the rotating ballot code
        /// Is seeded with the device hash and then each subsequent call consumes the ballot code of the
        /// previous encrypted ballot creating a linked hash chain between the ballots that can be traversed.
        /// Other liniking paradigms exists (such as tree strctures) and are beyond the scope of this implementation.
        ///
        /// <param name="seed">a seed such as the previous ballot code or starting hash from device</param>
        /// <param name="timestamp">Timestamp or other incrementing integer value</param>
        /// <param name="ballotCode">Hash of the ballot</param>
        /// <returns>Code</returns>
        /// </summary>
        static std::unique_ptr<ElementModQ>
        getBallotCode(const ElementModQ &seed, uint64_t timestamp, const ElementModQ &ballotCode);
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_BALLOT_CODE_HPP_INCLUDED__ */
