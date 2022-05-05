#ifndef __ELECTIONGUARD_CPP_TOOLS_GENERATORS_MANIFEST_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_TOOLS_GENERATORS_MANIFEST_HPP_INCLUDED__

#include <electionguard/election.hpp>
#include <electionguard/export.h>
#include <electionguard/manifest.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace electionguard;
using namespace std;

#ifndef TEST_SPEC_VERSION
#    define TEST_SPEC_VERSION "0.95.0"
#endif

#ifndef TEST_USE_SAMPLE
#    define TEST_USE_SAMPLE "hamilton-general"
#endif

#ifndef TEST_USE_FULL_SAMPLE
#    define TEST_USE_FULL_SAMPLE "full"
#endif

namespace electionguard::tools::generators
{
    class ManifestGenerator
    {
      public:
        static unique_ptr<Manifest> getJeffersonCountryManifest_multipleBallotStyle_fromFile()
        {
            return getManifestFromFile("election_manifest_jefferson_county.json");
        }
        static unique_ptr<Manifest> getManifestFromFile(const string &version, const string &sample)
        {
            return getManifestFromFile(version + "/sample/" + sample +
                                       "/election_record/manifest.json");
        }
        static unique_ptr<Manifest> getManifestFromFile(const string &filePath)
        {

            ifstream file;
            file.open("data/" + filePath);
            if (!file) {
                //Log::debug(filePath);
                throw invalid_argument("could not find file");
            }

            stringstream stream;
            stream << file.rdbuf();
            file.close();

            return Manifest::fromJson(stream.str());
        }
        static unique_ptr<BallotStyle> getFakeBallotStyle(string objectId, string gpUnitId)
        {
            vector<string> gpunitIds{gpUnitId};
            return make_unique<BallotStyle>(objectId, gpunitIds);
        }

        static unique_ptr<ContestDescription>
        getFakeReferendumContestDescription(string objectId, string electoralDistrictId,
                                            uint64_t sequenceOrder, string name)
        {
            vector<unique_ptr<SelectionDescription>> selections;
            selections.push_back(
              make_unique<SelectionDescription>("referendum-pineapple-affirmative-selection",
                                                "referendum-pineapple-affirmative", 0UL));
            selections.push_back(make_unique<SelectionDescription>(
              "referendum-pineapple-negative-selection", "referendum-pineapple-negative", 1UL));
            auto numberElected = 1UL;
            return make_unique<ContestDescription>(objectId, electoralDistrictId, sequenceOrder,
                                                   VoteVariationType::one_of_m, numberElected, name,
                                                   move(selections));
        }

        static unique_ptr<ContestDescription>
        getFakeCandidateContestDescription(string objectId, string electoralDistrictId,
                                           uint64_t sequenceOrder, uint64_t numberElected,
                                           string name)
        {
            vector<unique_ptr<SelectionDescription>> selections;
            selections.push_back(make_unique<SelectionDescription>("benjamin-franklin-selection",
                                                                   "benjamin-franklin", 0UL));
            selections.push_back(
              make_unique<SelectionDescription>("john-adams-selection", "john-adams", 1UL));
            selections.push_back(
              make_unique<SelectionDescription>("write-in-selection", "write-in", 2UL));

            return make_unique<ContestDescription>(objectId, electoralDistrictId, sequenceOrder,
                                                   VoteVariationType::one_of_m, numberElected, name,
                                                   move(selections));
        }
        // TODO: rename these
        static unique_ptr<Manifest> getJeffersonCountyManifest_Minimal()
        {
            vector<unique_ptr<GeopoliticalUnit>> gpUnits;
            gpUnits.push_back(make_unique<GeopoliticalUnit>("jefferson-county", "jefferson-county",
                                                            ReportingUnitType::county));

            vector<unique_ptr<Party>> parties = {};
            parties.push_back(make_unique<Party>("whig"));
            parties.push_back(make_unique<Party>("federalist"));

            vector<unique_ptr<Candidate>> candidates;
            candidates.push_back(make_unique<Candidate>("benjamin-franklin", false));
            candidates.push_back(make_unique<Candidate>("john-adams", false));
            candidates.push_back(make_unique<Candidate>("write-in", true));

            vector<unique_ptr<ContestDescription>> contests;
            contests.push_back(getFakeCandidateContestDescription(
              "justice-supreme-court", gpUnits.at(0).get()->getObjectId(), 0UL, 1UL,
              "Justice of the Supreme Court"));

            vector<unique_ptr<BallotStyle>> ballotStyles;
            vector<string> gpunitIds{gpUnits.at(0).get()->getObjectId()};
            ballotStyles.push_back(
              make_unique<BallotStyle>("jefferson-county-ballot-style", gpunitIds));

            return make_unique<Manifest>(
              "jefferson-county-open-primary", ElectionType::primary,
              std::chrono::system_clock::now(), std::chrono::system_clock::now(), move(gpUnits),
              move(parties), move(candidates), move(contests), move(ballotStyles));
        }

        static unique_ptr<Manifest> getJeffersonCountyManifest_MultipleBallotStyles()
        {
            vector<unique_ptr<GeopoliticalUnit>> gpUnits;
            gpUnits.push_back(make_unique<GeopoliticalUnit>("jefferson-county", "jefferson-county",
                                                            ReportingUnitType::county));
            gpUnits.push_back(make_unique<GeopoliticalUnit>("jefferson-county-school-district-1",
                                                            "Jefferson County School District 1",
                                                            ReportingUnitType::school));

            vector<unique_ptr<Party>> parties;
            parties.push_back(make_unique<Party>("whig"));
            parties.push_back(make_unique<Party>("federalist"));

            vector<unique_ptr<Candidate>> candidates;
            candidates.push_back(make_unique<Candidate>("benjamin-franklin", false));
            candidates.push_back(make_unique<Candidate>("john-adams", false));
            candidates.push_back(make_unique<Candidate>("write-in", true));
            candidates.push_back(make_unique<Candidate>("referendum-pineapple-affirmative", false));
            candidates.push_back(make_unique<Candidate>("referendum-pineapple-negative", false));

            vector<unique_ptr<ContestDescription>> contests;
            contests.push_back(getFakeCandidateContestDescription(
              "justice-supreme-court", gpUnits.at(0).get()->getObjectId(), 0UL, 1UL,
              "Justice of the Supreme Court"));
            contests.push_back(getFakeCandidateContestDescription(
              "referendum-pineapple", gpUnits.at(1).get()->getObjectId(), 1UL, 1UL,
              "The Pineapple Question"));

            vector<unique_ptr<BallotStyle>> ballotStyles;
            vector<string> jeffersonGPUnitIds{gpUnits.at(0).get()->getObjectId()};
            ballotStyles.push_back(
              make_unique<BallotStyle>("jefferson-county-ballot-style", jeffersonGPUnitIds));
            vector<string> schoolGPUnitIds{gpUnits.at(0).get()->getObjectId(),
                                           gpUnits.at(1).get()->getObjectId()};
            ballotStyles.push_back(
              make_unique<BallotStyle>("jefferson-county-school-district-1", schoolGPUnitIds));

            return make_unique<Manifest>(
              "jefferson-county-open-primary", ElectionType::primary,
              std::chrono::system_clock::now(), std::chrono::system_clock::now(), move(gpUnits),
              move(parties), move(candidates), move(contests), move(ballotStyles));
        }
    };
} // namespace electionguard::tools::generators

#endif /* __ELECTIONGUARD_CPP_TOOLS_GENERATORS_MANIFEST_HPP_INCLUDED__ */
