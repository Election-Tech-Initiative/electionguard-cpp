#ifndef __ELECTIONGUARD_CPP_TEST_MOCKS_BALLOT_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_TEST_MOCKS_BALLOT_HPP_INCLUDED__

#include "../../../src/electionguard/random.hpp"

#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>
#include <electionguard/export.h>
#include <electionguard/manifest.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace electionguard;
using namespace std;

namespace electionguard::test::mocks
{
    class BallotGenerator
    {
      public:
        static unique_ptr<PlaintextBallot> getSimpleBallotFromFile()
        {
            return getSimpleBallotFromFile("ballot_in_simple.json");
        }
        static unique_ptr<PlaintextBallot> getSimpleBallotFromFile(const string &filename)
        {

            ifstream file;
            file.open("data/" + filename);
            if (!file) {
                throw invalid_argument("could not find file");
            }

            stringstream stream;
            stream << file.rdbuf();
            file.close();

            return PlaintextBallot::fromJson(stream.str());
        }

        static unique_ptr<PlaintextBallotSelection>
        randomSelectionFrom(const SelectionDescription &description)
        {
            auto random = Random::getBytes(ByteSize::CHAR);
            auto choice = random.at(0) % 2UL;

            return make_unique<PlaintextBallotSelection>(description.getObjectId(), choice);
        }

        static unique_ptr<PlaintextBallotSelection>
        selectionFrom(const SelectionDescription &description, bool isAffirmative = false,
                      bool isPlaceholder = false)
        {
            return make_unique<PlaintextBallotSelection>(description.getObjectId(),
                                                         isAffirmative ? 1UL : 0UL, isPlaceholder);
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
                    Log::debug(" " + description.get().getObjectId() + " Adding Selection: TRUE");
                    selections.push_back(selectionFrom(description.get(), true));
                } else {
                    Log::debug(" " + description.get().getObjectId() + " Adding Selection: FALSE");
                    selections.push_back(selectionFrom(description.get(), false));
                }
            }

            return make_unique<PlaintextBallotContest>(contest.getObjectId(), move(selections));
        }

        static unique_ptr<PlaintextBallot> getFakeBallot(const InternalManifest &manifest,
                                                         uint64_t maxChoices = 1)
        {
            string ballotId =
              "ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123ballot-"
              "id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-"
              "123-ballot-id-123-ballot-id-123-ballot-id-123";

            vector<unique_ptr<PlaintextBallotContest>> contests;
            for (const auto &contest : manifest.getContests()) {
                contests.push_back(contestFrom(contest.get(), maxChoices));
            }
            return make_unique<PlaintextBallot>(
              ballotId, manifest.getBallotStyles().at(0).get().getObjectId(), move(contests));
        }
    };
} // namespace electionguard::test::mocks

#endif /* __ELECTIONGUARD_CPP_TEST_MOCKS_BALLOT_HPP_INCLUDED__ */
