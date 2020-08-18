#ifndef __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__

#include "export.h"

extern "C" {
#include "election_object_base.h"
}

#include <string>

namespace electionguard
{
    struct PlaintextBallotSelectionData : public ElectionObjectBase {
        char vote[256];
        bool isPlaceholderSelection;
    };

    class EG_API PlaintextBallotSelection
    {
      public:
        PlaintextBallotSelection(const std::string object_id, std::string vote);
        PlaintextBallotSelection(const char *object_id, char *vote);
        ~PlaintextBallotSelection();

        int toInt();

      private:
        PlaintextBallotSelectionData data;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__ */