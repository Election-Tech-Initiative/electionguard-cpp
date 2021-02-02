#ifndef __ELECTIONGUARD_CORE_TEST_GENERATORS_BALLOT_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_TEST_GENERATORS_BALLOT_HPP_INCLUDED__

#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>

using namespace electionguard;
using namespace std;

namespace electionguard::test::generators
{
    class BallotGenerator
    {
      public:
        static unique_ptr<PlaintextBallotSelection>
        selectionFrom(const SelectionDescription &description, bool choice)
        {
            return make_unique<PlaintextBallotSelection>(description.getObjectId(),
                                                         choice ? "1" : "0");
        }

        static unique_ptr<PlaintextBallotContest> contestFrom(const ContestDescription &contest,
                                                              uint64_t maxChoices = 1)
        {

            uint64_t choices = 0;
            vector<unique_ptr<PlaintextBallotSelection>> selections;

            // jsut loop through and select the necessary amount of trues
            for (const auto &description : contest.getSelections()) {
                if (maxChoices > contest.getNumberElected()) {
                    maxChoices = contest.getNumberElected();
                }
                if (choices < maxChoices) {
                    ++choices;
                    Log::debug(" Adding Selection: TRUE");
                    selections.push_back(selectionFrom(description.get(), true));
                } else {
                    Log::debug(" Adding Selection: FALSE");
                    selections.push_back(selectionFrom(description.get(), false));
                }
            }

            return make_unique<PlaintextBallotContest>(contest.getObjectId(), move(selections));
        }

        static unique_ptr<PlaintextBallot>
        getFakeBallot(const InternalElectionDescription &metadata, uint64_t maxChoices = 1)
        {
            string ballotId =
              "ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123ballot-"
              "id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-"
              "123-ballot-id-123-ballot-id-123-ballot-id-123";

            vector<unique_ptr<PlaintextBallotContest>> contests;
            for (const auto &contest : metadata.getContests()) {
                contests.push_back(contestFrom(contest.get()));
            }
            return make_unique<PlaintextBallot>(
              ballotId, metadata.getBallotStyles().at(0).get().getObjectId(), move(contests));
        }
    };
} // namespace electionguard::test::generators

#endif /* __ELECTIONGUARD_CORE_TEST_GENERATORS_BALLOT_HPP_INCLUDED__ */