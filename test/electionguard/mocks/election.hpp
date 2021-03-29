#ifndef __ELECTIONGUARD_CPP_TEST_MOCKS_ELECTION_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_TEST_MOCKS_ELECTION_HPP_INCLUDED__

#include <electionguard/election.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace electionguard;
using namespace std;

namespace electionguard::test::mocks
{
    class ElectionGenerator
    {
      public:
        static unique_ptr<ElectionDescription> getSimpleElectionFromFile()
        {
            return getSimpleElectionFromFile("election_manifest_simple.json");
        }
        static unique_ptr<ElectionDescription> getSimpleElectionFromFile(const string &filename)
        {

            ifstream file;
            file.open("data/" + filename);
            if (!file) {
                throw invalid_argument("could not find file");
            }

            stringstream stream;
            stream << file.rdbuf();
            file.close();

            return ElectionDescription::fromJson(stream.str());
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
            selections.push_back(make_unique<SelectionDescription>("some-object-id-affirmative",
                                                                   "some-candidate-id-1", 0UL));
            selections.push_back(make_unique<SelectionDescription>("some-object-id-negative",
                                                                   "some-candidate-id-2", 1UL));
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
            selections.push_back(make_unique<SelectionDescription>("some-object-id-candidate-1",
                                                                   "some-candidate-id-1", 0UL));
            selections.push_back(make_unique<SelectionDescription>("some-object-id-candidate-2",
                                                                   "some-candidate-id-2", 1UL));
            selections.push_back(make_unique<SelectionDescription>("some-object-id-candidate-3",
                                                                   "some-candidate-id-3", 2UL));

            return make_unique<ContestDescription>(objectId, electoralDistrictId, sequenceOrder,
                                                   VoteVariationType::one_of_m, numberElected, name,
                                                   move(selections));
        }

        static unique_ptr<ElectionDescription> getFakeElection()
        {
            vector<unique_ptr<GeopoliticalUnit>> gpUnits;
            gpUnits.push_back(make_unique<GeopoliticalUnit>(
              "some-geopoltical-unit-id", "some-gp-unit-name", ReportingUnitType::unknown));

            vector<unique_ptr<Party>> parties;
            parties.push_back(make_unique<Party>("some-party-id-1"));
            parties.push_back(make_unique<Party>("some-party-id-2"));

            vector<unique_ptr<Candidate>> candidates;
            candidates.push_back(make_unique<Candidate>("some-candidate-id-1"));
            candidates.push_back(make_unique<Candidate>("some-candidate-id-2"));
            candidates.push_back(make_unique<Candidate>("some-candidate-id-3"));

            vector<unique_ptr<BallotStyle>> ballotStyles;
            ballotStyles.push_back(
              getFakeBallotStyle("some-ballot-style-id", gpUnits.at(0).get()->getObjectId()));

            vector<unique_ptr<ContestDescription>> contests;
            contests.push_back(getFakeReferendumContestDescription(
              "some-referendum-contest-object-id", gpUnits.at(0).get()->getObjectId(), 0UL,
              "some-referendum-contest-name"));
            contests.push_back(getFakeCandidateContestDescription(
              "some-candidate-contest-object-id", gpUnits.at(0).get()->getObjectId(), 1UL, 2UL,
              "some-candidate-contest-name"));

            return make_unique<ElectionDescription>(
              "some-scope-id", ElectionType::unknown, std::chrono::system_clock::now(),
              std::chrono::system_clock::now(), move(gpUnits), move(parties), move(candidates),
              move(contests), move(ballotStyles));
        }

        static unique_ptr<InternalElectionDescription>
        getFakeMetadata(const ElementModQ &descriptionHash)
        {
            vector<unique_ptr<SelectionDescription>> selections1;
            selections1.push_back(
              make_unique<SelectionDescription>("contest-1-selection-1-id", "candidate-1-id", 1UL));
            selections1.push_back(
              make_unique<SelectionDescription>("contest-1-selection-2-id", "candidate-2-id", 2UL));
            selections1.push_back(
              make_unique<SelectionDescription>("contest-1-selection-3-id", "candidate-3-id", 3UL));

            vector<unique_ptr<SelectionDescription>> placeholderSelections1;
            placeholderSelections1.push_back(make_unique<SelectionDescription>(
              "contest-1-placeholder-selection-4-id", "candidate-4-id", 4UL));

            vector<unique_ptr<SelectionDescription>> selections2;
            selections2.push_back(
              make_unique<SelectionDescription>("contest-2-selection-1-id", "candidate-1-id", 1UL));
            selections2.push_back(
              make_unique<SelectionDescription>("contest-2-selection-2-id", "candidate-2-id", 2UL));

            vector<unique_ptr<SelectionDescription>> placeholderSelections2;
            placeholderSelections2.push_back(make_unique<SelectionDescription>(
              "contest-2-placeholder-selection-3-id", "candidate-3-id", 3UL));

            vector<unique_ptr<Language>> contest1TitleText;
            contest1TitleText.push_back(make_unique<Language>("some-title-string", "en"));
            contest1TitleText.push_back(make_unique<Language>("some-title-string-es", "es"));

            vector<unique_ptr<Language>> contest1SubTitleText;
            contest1SubTitleText.push_back(make_unique<Language>("some-title-string", "en"));
            contest1SubTitleText.push_back(make_unique<Language>("some-title-string-es", "es"));

            auto title1 = make_unique<InternationalizedText>(move(contest1TitleText));
            auto subtitle1 = make_unique<InternationalizedText>(move(contest1SubTitleText));

            vector<unique_ptr<Language>> contest2TitleText;
            contest2TitleText.push_back(make_unique<Language>("some-title-string", "en"));
            contest2TitleText.push_back(make_unique<Language>("some-title-string-es", "es"));
            vector<unique_ptr<Language>> contest2SubTitleText;
            contest2SubTitleText.push_back(make_unique<Language>("some-title-string", "en"));
            contest2SubTitleText.push_back(make_unique<Language>("some-title-string-es", "es"));

            auto title2 = make_unique<InternationalizedText>(move(contest2TitleText));
            auto subtitle2 = make_unique<InternationalizedText>(move(contest2SubTitleText));

            vector<string> addressLine{"some-street", "some-city", "some-whatever"};
            auto gpUnit_contactInfo =
              make_unique<ContactInformation>(addressLine, "gp-unit-contact-info");

            vector<unique_ptr<GeopoliticalUnit>> gpUnits;
            gpUnits.push_back(make_unique<GeopoliticalUnit>("geopolitical-unit-1", "district-1-id",
                                                            ReportingUnitType::city,
                                                            move(gpUnit_contactInfo)));

            vector<unique_ptr<Party>> parties;
            vector<unique_ptr<Language>> party1Name;
            party1Name.push_back(make_unique<Language>("some-party-1-string", "en"));
            auto partyName = make_unique<InternationalizedText>(move(party1Name));
            parties.push_back(make_unique<Party>("party-1", move(partyName), "party1", "some-color",
                                                 "some-logo-uri"));

            vector<unique_ptr<ContestDescriptionWithPlaceholders>> contests;
            const auto numElected = 2UL;
            contests.push_back(make_unique<ContestDescriptionWithPlaceholders>(
              "contest-1-id", gpUnits.at(0).get()->getObjectId(), 1UL, VoteVariationType::n_of_m,
              numElected, numElected, "contest-1-name", move(title1), move(subtitle1),
              move(selections1), move(placeholderSelections1)));

            contests.push_back(make_unique<ContestDescriptionWithPlaceholders>(
              "contest-2-id", gpUnits.at(0).get()->getObjectId(), 2UL, VoteVariationType::n_of_m,
              numElected, numElected, "contest-2-name", move(title2), move(subtitle2),
              move(selections2), move(placeholderSelections2)));

            vector<unique_ptr<BallotStyle>> ballotStyles;
            vector<string> gpunitIds{gpUnits.at(0).get()->getObjectId()};
            vector<string> partyIds{parties.at(0).get()->getObjectId()};
            ballotStyles.push_back(
              make_unique<BallotStyle>("some-ballot-style-id", gpunitIds, partyIds, "some-uri"));

            auto metadata = make_unique<InternalElectionDescription>(
              move(gpUnits), move(contests), move(ballotStyles), descriptionHash);
            return metadata;
        }

        static tuple<unique_ptr<InternalElectionDescription>, unique_ptr<CiphertextElectionContext>>
        getFakeCiphertextElection(const ElectionDescription &description,
                                  const ElementModP &publicKey)
        {
            auto metadata = make_unique<InternalElectionDescription>(description);
            auto context = getFakeContext(*metadata, publicKey);

            return make_tuple(move(metadata), move(context));
        }

        static unique_ptr<CiphertextElectionContext>
        getFakeContext(const InternalElectionDescription &metadata, const ElementModP &publicKey)
        {
            auto context = CiphertextElectionContext::make(
              1UL, 1UL, make_unique<ElementModP>(publicKey), make_unique<ElementModQ>(TWO_MOD_Q()),
              make_unique<ElementModQ>(*metadata.getDescriptionHash()));
            return context;
        }
    };
} // namespace electionguard::test::mocks

#endif /* __ELECTIONGUARD_CPP_TEST_MOCKS_ELECTION_HPP_INCLUDED__ */
