#ifndef __ELECTIONGUARD_CPP_SERIALIZE_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_SERIALIZE_HPP_INCLUDED__

#include "convert.hpp"
#include "electionguard/ballot.hpp"
#include "electionguard/ballot_compact.hpp"
#include "electionguard/election.hpp"
#include "electionguard/export.h"
#include "electionguard/group.hpp"
#include "log.hpp"

#include <cstdint>
#include <cstring>
#include <memory>
#include <nlohmann/json.hpp>

using std::make_unique;
using std::reference_wrapper;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

using nlohmann::json;

namespace electionguard
{
    // TODO: input sanitization and safety

    static json annotatedStringToJson(const AnnotatedString &serializable)
    {
        return {{"annotation", serializable.getAnnotation()}, {"value", serializable.getValue()}};
    }

    static unique_ptr<AnnotatedString> annotatedStringFromJson(const json &j)
    {
        auto value = j["value"].get<string>();
        auto annotation = j["annotation"].get<string>();
        return make_unique<AnnotatedString>(annotation, value);
    }

    static json languageToJson(const Language &serializable)
    {
        return {{"value", serializable.getValue()}, {"language", serializable.getLanguage()}};
    }

    static unique_ptr<Language> languageFromJson(const json &j)
    {
        auto value = j["value"].get<string>();
        auto language = j["language"].get<string>();
        return make_unique<Language>(value, language);
    }

    static json internationalizedTextToJson(const InternationalizedText &serializable)
    {
        json serialized;
        for (const auto &element : serializable.getText()) {
            serialized.push_back(languageToJson(element.get()));
        }
        return serialized;
    }

    static unique_ptr<InternationalizedText> internationalizedTextFromJson(const json &j)
    {
        vector<unique_ptr<Language>> text;
        for (const auto &i : j["text"]) {
            text.push_back(languageFromJson(i));
        }
        return make_unique<InternationalizedText>(move(text));
    }

    static json contactInformationToJson(const ContactInformation &serializable)
    {
        json serialized = {};

        if (!serializable.getName().empty()) {
            serialized["name"] = serializable.getName();
        }

        if (!serializable.getAddressLine().empty()) {
            json addressLine;
            for (const auto &i : serializable.getAddressLine()) {
                addressLine.push_back(i);
            }
            serialized["address_line"] = addressLine;
        }

        if (!serializable.getEmail().empty()) {
            json email;
            for (const auto &i : serializable.getEmail()) {
                email.push_back(annotatedStringToJson(i));
            }
            serialized["email"] = email;
        }
        if (!serializable.getPhone().empty()) {
            json phone;
            for (const auto &i : serializable.getPhone()) {
                phone.push_back(annotatedStringToJson(i));
            }
            serialized["phone"] = phone;
        }

        return serialized;
    }

    static unique_ptr<ContactInformation> contactInformationFromJson(const json &j)
    {
        vector<string> addressLine;
        if (j.contains("address_line") && !j["address_line"].is_null()) {
            for (const auto &i : j["address_line"]) {
                addressLine.push_back(i.get<string>());
            }
        }

        vector<unique_ptr<AnnotatedString>> email;
        if (j.contains("email") && !j["email"].is_null()) {
            for (const auto &i : j["email"]) {
                email.push_back(annotatedStringFromJson(i));
            }
        }

        vector<unique_ptr<AnnotatedString>> phone;
        if (j.contains("phone") && !j["phone"].is_null()) {
            for (const auto &i : j["phone"]) {
                phone.push_back(annotatedStringFromJson(i));
            }
        }

        if (j.contains("name") && !j["name"].is_null()) {
            return make_unique<ContactInformation>(move(addressLine), move(email), move(phone),
                                                   j["name"].get<string>());
        }
        return make_unique<ContactInformation>(move(addressLine), move(email), move(phone));
    }

    static json geopoliticalUnitToJson(const GeopoliticalUnit &serializable)
    {
        json serialized = {};
        serialized["object_id"] = serializable.getObjectId();
        serialized["name"] = serializable.getName();
        serialized["type"] = getReportingUnitTypeString(serializable.getType());

        if (serializable.getContactInformation() != nullptr) {
            serialized["contact_information"] =
              contactInformationToJson(*serializable.getContactInformation());
        }

        return serialized;
    }

    static unique_ptr<GeopoliticalUnit> geopoliticalUnitFromJson(const json &j)
    {
        if (j.contains("contact_information") && !j["contact_information"].is_null()) {
            return make_unique<GeopoliticalUnit>(
              j["object_id"].get<string>(), j["name"].get<string>(),
              getReportingUnitType(j["type"].get<string>()),
              contactInformationFromJson(j["contact_information"]));
        }
        return make_unique<GeopoliticalUnit>(j["object_id"].get<string>(), j["name"].get<string>(),
                                             getReportingUnitType(j["type"].get<string>()));
    }

    static json
    geopoliticalUnitsToJson(const vector<reference_wrapper<GeopoliticalUnit>> &serializable)
    {
        json serialized;
        for (const auto &element : serializable) {
            serialized.push_back(geopoliticalUnitToJson(element));
        }
        return serialized;
    }

    static vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnitsFromJson(const json &j)
    {
        vector<unique_ptr<GeopoliticalUnit>> elements;
        for (const auto &i : j) {
            elements.push_back(geopoliticalUnitFromJson(i));
        }
        return elements;
    }

    static json partyToJson(const Party &serializable)
    {
        json serialized = {};
        serialized["object_id"] = serializable.getObjectId();

        if (!serializable.getAbbreviation().empty()) {
            serialized["abbreviation"] = serializable.getAbbreviation();
        }

        if (!serializable.getColor().empty()) {
            serialized["color"] = serializable.getColor();
        }

        if (!serializable.getLogoUri().empty()) {
            serialized["logo_uri"] = serializable.getLogoUri();
        }

        if (serializable.getName() != nullptr) {
            json names;
            for (const auto &element : serializable.getName()->getText()) {
                names.push_back(languageToJson(element.get()));
            }
            serialized["name"] = names;
        }

        return serialized;
    }

    static unique_ptr<Party> partyFromJson(const json &j)
    {
        // TODO: other cases
        if (j.contains("name") && j["name"].is_null()) {
            return make_unique<Party>(j["object_id"].get<string>(),
                                      internationalizedTextFromJson(j["name"]),
                                      j["abbreviation"].get<string>(), j["color"].get<string>(),
                                      j["logo_uri"].get<string>());
        }
        return make_unique<Party>(j["object_id"].get<string>());
    }

    static json partiesToJson(const vector<reference_wrapper<Party>> &serializable)
    {
        json serialized;
        for (const auto &element : serializable) {

            serialized.push_back(partyToJson(element));
        }
        return serialized;
    }

    static vector<unique_ptr<Party>> partiesFromJson(json j)
    {
        vector<unique_ptr<Party>> elements;
        for (const auto &i : j) {
            elements.push_back(partyFromJson(i));
        }
        return elements;
    }

    static json candidateToJson(const Candidate &serializable)
    {
        json serialized = {};
        serialized["object_id"] = serializable.getObjectId();

        if (serializable.isWriteIn()) {
            serialized["is_write_in"] = serializable.isWriteIn();
        }

        if (serializable.getName() != nullptr) {
            serialized["name"] = internationalizedTextToJson(*serializable.getName());
        }

        if (!serializable.getPartyId().empty()) {
            serialized["party_id"] = serializable.getPartyId();
        }

        if (!serializable.getImageUri().empty()) {
            serialized["image_uri"] = serializable.getImageUri();
        }

        return serialized;
    }

    static unique_ptr<Candidate> candidateFromJson(const json &j)
    {
        auto objectId = j["object_id"].get<string>();
        auto name = j.contains("name") && !j["name"].is_null()
                      ? internationalizedTextFromJson(j["name"])
                      : nullptr;
        bool isWriteIn = j.contains("is_write_in") && !j["is_write_in"].is_null()
                           ? j["is_write_in"].get<bool>()
                           : false;
        auto partyId =
          j.contains("party_id") && !j["party_id"].is_null() ? j["party_id"].get<string>() : "";
        auto imageUri =
          j.contains("image_uri") && !j["image_uri"].is_null() ? j["image_uri"].get<string>() : "";

        return make_unique<Candidate>(objectId, name != nullptr ? move(name) : nullptr, partyId,
                                      imageUri, isWriteIn);
    }

    static json candidatesToJson(const vector<reference_wrapper<Candidate>> &serializable)
    {
        json serialized;
        for (const auto &element : serializable) {

            serialized.push_back(candidateToJson(element));
        }
        return serialized;
    }

    static vector<unique_ptr<Candidate>> candidatesFromJson(const json &j)
    {
        vector<unique_ptr<Candidate>> elements;
        for (const auto &i : j) {
            elements.push_back(candidateFromJson(i));
        }
        return elements;
    }

    static json ballotStyleToJson(const BallotStyle &serializable)
    {
        json serialized = {};
        serialized["object_id"] = serializable.getObjectId();

        json gpUnitIds;
        for (const auto &element : serializable.getGeopoliticalUnitIds()) {
            gpUnitIds.push_back(element);
        }

        serialized["geopolitical_unit_ids"] = gpUnitIds;

        if (!serializable.getPartyIds().empty()) {
            json partyIds;
            for (const auto &element : serializable.getPartyIds()) {
                partyIds.push_back(element);
            }
            serialized["party_ids"] = partyIds;
        }

        if (!serializable.getImageUri().empty()) {
            serialized["image_uri"] = serializable.getImageUri();
        }

        return serialized;
    }

    static unique_ptr<BallotStyle> ballotStyleFromJson(const json &j)
    {
        vector<string> gpUnitIds;
        for (const auto &element : j["geopolitical_unit_ids"]) {
            gpUnitIds.push_back(element);
        }

        vector<string> partyIds;
        if (j.contains("party_ids") && !j["party_ids"].is_null()) {
            for (const auto &element : j["party_ids"]) {
                partyIds.push_back(element);
            }
        }

        auto imageUri =
          j.contains("image_uri") && !j["image_uri"].is_null() ? j["image_uri"].get<string>() : "";
        return make_unique<BallotStyle>(j["object_id"].get<string>(), gpUnitIds, partyIds,
                                        imageUri);
    }

    static json ballotStylesToJson(const vector<reference_wrapper<BallotStyle>> &serializable)
    {
        json serialized;
        for (const auto &element : serializable) {
            serialized.push_back(ballotStyleToJson(element));
        }
        return serialized;
    }

    static vector<unique_ptr<BallotStyle>> ballotStylesFromJson(const json &j)
    {
        vector<unique_ptr<BallotStyle>> elements;
        for (const auto &i : j) {
            elements.push_back(ballotStyleFromJson(i));
        }
        return elements;
    }

    static json selectionDescriptionToJson(const SelectionDescription &serializable)
    {
        return {
          {"object_id", serializable.getObjectId()},
          {"candidate_id", serializable.getCandidateId()},
          {"sequence_order", serializable.getSequenceOrder()},
        };
    }

    static unique_ptr<SelectionDescription> selectionDescriptionFromJson(const json &j)
    {
        return make_unique<SelectionDescription>(j["object_id"].get<string>(),
                                                 j["candidate_id"].get<string>(),
                                                 j["sequence_order"].get<uint64_t>());
    }

    static json
    selectionDescriptionsToJson(const vector<reference_wrapper<SelectionDescription>> &serializable)
    {
        json serialized;
        for (const auto &element : serializable) {
            serialized.push_back(selectionDescriptionToJson(element));
        }
        return serialized;
    }

    static vector<unique_ptr<SelectionDescription>> selectionDescriptionsFromJson(const json &j)
    {
        vector<unique_ptr<SelectionDescription>> elements;
        for (const auto &i : j) {
            elements.push_back(selectionDescriptionFromJson(i));
        }
        return elements;
    }

    static json contestDescriptionToJson(const ContestDescription &serializable)
    {
        json serialized;
        serialized["object_id"] = serializable.getObjectId();
        serialized["electoral_district_id"] = serializable.getElectoralDistrictId();
        serialized["sequence_order"] = serializable.getSequenceOrder();
        serialized["vote_variation"] = getVoteVariationTypeString(serializable.getVoteVariation());
        serialized["number_elected"] = serializable.getNumberElected();

        if (serializable.getVotesAllowed() > 0) {
            serialized["votes_allowed"] = serializable.getVotesAllowed();
        }

        serialized["name"] = serializable.getName();

        if (serializable.getBallotTitle() != nullptr) {
            serialized["ballot_title"] = {
              {"text", internationalizedTextToJson(*serializable.getBallotTitle())}};
        }

        if (serializable.getBallotSubtitle() != nullptr) {
            serialized["ballot_subtitle"] = {
              {"text", internationalizedTextToJson(*serializable.getBallotSubtitle())}};
        }

        serialized["ballot_selections"] = selectionDescriptionsToJson(serializable.getSelections());

        return serialized;
    }

    static unique_ptr<ContestDescription> contestDescriptionFromJson(const json &j)
    {
        auto objectId = j["object_id"].get<string>();
        auto electoralDistrictId = j["electoral_district_id"].get<string>();
        auto sequenceOrder = j["sequence_order"].get<uint64_t>();
        auto variation = getVoteVariationType(j["vote_variation"].get<string>());
        auto elected = j["number_elected"].get<uint64_t>();
        auto allowed = j.contains("votes_allowed") && !j["votes_allowed"].is_null()
                         ? j["votes_allowed"].get<uint64_t>()
                         : 0;
        auto name = j["name"].get<string>();
        auto title = j.contains("ballot_title") && !j["ballot_title"].is_null()
                       ? internationalizedTextFromJson(j["ballot_title"])
                       : nullptr;
        auto subtitle = j.contains("ballot_subtitle") && !j["ballot_subtitle"].is_null()
                          ? internationalizedTextFromJson(j["ballot_subtitle"])
                          : nullptr;

        auto selections = selectionDescriptionsFromJson(j["ballot_selections"]);

        return make_unique<ContestDescription>(
          objectId, electoralDistrictId, sequenceOrder, variation, elected, allowed, name,
          title != nullptr ? move(title) : nullptr, subtitle != nullptr ? move(subtitle) : nullptr,
          move(selections));
    }

    static json
    contestDescriptionsToJson(const vector<reference_wrapper<ContestDescription>> &serializable)
    {
        json serialized;
        for (const auto &element : serializable) {
            serialized.push_back(contestDescriptionToJson(element));
        }
        return serialized;
    }

    static vector<unique_ptr<ContestDescription>> contestDescriptionsFromJson(const json &j)
    {
        vector<unique_ptr<ContestDescription>> elements;
        for (const auto &i : j) {
            elements.push_back(contestDescriptionFromJson(i));
        }
        return elements;
    }

    class Serialize
    {
      public:
        class ElectionDescription
        {
          private:
            static json fromObject(const electionguard::ElectionDescription &serializable)
            {
                json serialized;
                serialized["election_scope_id"] = serializable.getElectionScopeId();
                serialized["type"] = getElectionTypeString(serializable.getElectionType());
                serialized["start_date"] =
                  timePointToIsoString(serializable.getStartDate(), "%FT%TZ");
                serialized["end_date"] = timePointToIsoString(serializable.getEndDate(), "%FT%TZ");

                json geopoliticalUnits =
                  geopoliticalUnitsToJson(serializable.getGeopoliticalUnits());
                serialized["geopolitical_units"] = geopoliticalUnits;

                json parties = partiesToJson(serializable.getParties());
                serialized.push_back({"parties", parties});

                json candidates = candidatesToJson(serializable.getCandidates());
                serialized.push_back({"candidates", candidates});

                json contests = contestDescriptionsToJson(serializable.getContests());
                serialized.push_back({"contests", contests});

                json ballotStyles = ballotStylesToJson(serializable.getBallotStyles());
                serialized.push_back({"ballot_styles", ballotStyles});

                if (serializable.getName() != nullptr) {
                    json name = internationalizedTextToJson(*serializable.getName());
                    serialized.push_back({"name", name});
                }

                if (serializable.getContactInformation() != nullptr) {
                    json contactInformation =
                      contactInformationToJson(*serializable.getContactInformation());
                    serialized.push_back({"contact_information", contactInformation});
                }

                return serialized;
            }

            static unique_ptr<electionguard::ElectionDescription> toObject(json j)
            {
                auto electionScopeId = j["election_scope_id"].get<string>();
                auto type = j["type"].get<string>();
                auto startDate = j["start_date"].get<string>();
                auto endDate = j["end_date"].get<string>();

                auto geopoliticalUnits = geopoliticalUnitsFromJson(j["geopolitical_units"]);
                auto parties = partiesFromJson(j["parties"]);
                auto candidates = candidatesFromJson(j["candidates"]);
                auto contests = contestDescriptionsFromJson(j["contests"]);
                auto ballotStyles = ballotStylesFromJson(j["ballot_styles"]);

                // TODO: just a stub for now.
                // we need to handle all of the otpional serilization cases.
                if (j.contains("name") && !j["name"].is_null()) {
                    auto name = internationalizedTextFromJson(j["name"]);
                    auto contactInformation = contactInformationFromJson(j["contact_information"]);

                    return make_unique<electionguard::ElectionDescription>(
                      electionScopeId, getElectionType(type),
                      timePointFromIsoString(startDate, "%FT%TZ"),
                      timePointFromIsoString(endDate, "%FT%TZ"), move(geopoliticalUnits),
                      move(parties), move(candidates), move(contests), move(ballotStyles),
                      move(name), move(contactInformation));
                }
                return make_unique<electionguard::ElectionDescription>(
                  electionScopeId, getElectionType(type),
                  timePointFromIsoString(startDate, "%FT%TZ"),
                  timePointFromIsoString(endDate, "%FT%TZ"), move(geopoliticalUnits), move(parties),
                  move(candidates), move(contests), move(ballotStyles));
            }

          public:
            static vector<uint8_t> toBson(const electionguard::ElectionDescription &serializable)
            {
                return json::to_bson(fromObject(serializable));
            }
            static string toJson(const electionguard::ElectionDescription &serializable)
            {
                return fromObject(serializable).dump();
            }

            static unique_ptr<electionguard::ElectionDescription> fromBson(vector<uint8_t> data)
            {
                return toObject(json::from_bson(data));
            }

            static unique_ptr<electionguard::ElectionDescription> fromJson(string data)
            {
                return toObject(json::parse(data));
            }
        };
        class InternalElectionDescription
        {
          private:
            static json fromObject(const electionguard::InternalElectionDescription &serializable)
            {
                auto geopoliticalUnits =
                  geopoliticalUnitsToJson(serializable.getGeopoliticalUnits());

                // Contests
                json contests;
                for (const auto &contest : serializable.getContests()) {
                    auto selections = selectionDescriptionsToJson(contest.get().getSelections());
                    auto placeholders =
                      selectionDescriptionsToJson(contest.get().getPlaceholders());

                    json contest_json = {
                      {"object_id", contest.get().getObjectId()},
                      {"electoral_district_id", contest.get().getElectoralDistrictId()},
                      {"sequence_order", contest.get().getSequenceOrder()},
                      {"vote_variation",
                       getVoteVariationTypeString(contest.get().getVoteVariation())},
                      {"number_elected", contest.get().getNumberElected()},
                      {"votes_allowed", contest.get().getVotesAllowed()},
                      {"name", contest.get().getName()},
                      {"ballot_selections", selections},
                      {"ballot_placeholders", placeholders}};

                    if (contest.get().getBallotTitle() != nullptr) {
                        contest_json["ballot_title"] = {
                          {"text", internationalizedTextToJson(*contest.get().getBallotTitle())}};
                    }

                    if (contest.get().getBallotSubtitle() != nullptr) {
                        contest_json["ballot_subtitle"] = {
                          {"text",
                           internationalizedTextToJson(*contest.get().getBallotSubtitle())}};
                    }

                    contests.push_back(contest_json);
                }

                auto ballotStyles = ballotStylesToJson(serializable.getBallotStyles());

                Log::debug(" serialized Ballot Styles");

                json result = {{"description_hash", serializable.getDescriptionHash()->toHex()},
                               {"geopolitical_units", geopoliticalUnits},
                               {"contests", contests},
                               {"ballot_styles", ballotStyles}};

                return result;
            }
            static unique_ptr<electionguard::InternalElectionDescription> toObject(json j)
            {
                auto geopoliticalUnits = geopoliticalUnitsFromJson(j["geopolitical_units"]);

                auto contests = j["contests"];

                vector<unique_ptr<ContestDescriptionWithPlaceholders>> contestDescriptions;
                contestDescriptions.reserve(contests.size());
                for (auto &contest : contests) {
                    auto contest_object_id = contest["object_id"].get<string>();
                    auto electoral_district_id = contest["electoral_district_id"].get<string>();
                    auto contest_sequence_order = contest["sequence_order"].get<uint64_t>();
                    auto vote_variation = contest["vote_variation"].get<string>();
                    auto number_elected = contest["number_elected"].get<uint64_t>();
                    auto name = contest["name"].get<string>();

                    auto selections = selectionDescriptionsFromJson(contest["ballot_selections"]);
                    auto placeholders =
                      selectionDescriptionsFromJson(contest["ballot_placeholders"]);

                    // TODO: ust a stub for now.
                    // we need to handle all of the otpional serilization cases.
                    if (contest.contains("ballot_title") && !contest["ballot_title"].is_null()) {
                        auto votes_allowed = contest["votes_allowed"].get<uint64_t>();
                        auto ballotTitle = internationalizedTextFromJson(contest["ballot_title"]);
                        auto ballotSubtitle =
                          internationalizedTextFromJson(contest["ballot_subtitle"]);

                        contestDescriptions.push_back(
                          make_unique<ContestDescriptionWithPlaceholders>(
                            contest_object_id, electoral_district_id, contest_sequence_order,
                            getVoteVariationType(vote_variation), number_elected, votes_allowed,
                            name, move(ballotTitle), move(ballotSubtitle), move(selections),
                            move(placeholders)));

                    } else {
                        contestDescriptions.push_back(
                          make_unique<ContestDescriptionWithPlaceholders>(
                            contest_object_id, electoral_district_id, contest_sequence_order,
                            getVoteVariationType(vote_variation), number_elected, name,
                            move(selections), move(placeholders)));
                    }
                }

                auto ballotStyles = ballotStylesFromJson(j["ballot_styles"]);

                auto description_hash = j["description_hash"].get<string>();
                auto descriptionHash = ElementModQ::fromHex(description_hash);

                return make_unique<electionguard::InternalElectionDescription>(
                  move(geopoliticalUnits), move(contestDescriptions), move(ballotStyles),
                  *descriptionHash);
            }

          public:
            static vector<uint8_t>
            toBson(const electionguard::InternalElectionDescription &serializable)
            {
                return json::to_bson(fromObject(serializable));
            }
            static string toJson(const electionguard::InternalElectionDescription &serializable)
            {
                return fromObject(serializable).dump();
            }

            static unique_ptr<electionguard::InternalElectionDescription>
            fromBson(vector<uint8_t> data)
            {
                return toObject(json::from_bson(data));
            }

            static unique_ptr<electionguard::InternalElectionDescription> fromJson(string data)
            {
                return toObject(json::parse(data));
            }
        };

        class CiphertextelectionContext
        {
          private:
            static json fromObject(const electionguard::CiphertextElectionContext &serializable)
            {
                json j = {
                  {"crypto_base_hash", serializable.getCryptoBaseHash()->toHex()},
                  {"crypto_extended_base_hash", serializable.getCryptoExtendedBaseHash()->toHex()},
                  {"commitment_hash", serializable.getCommitmentHash()->toHex()},
                  {"description_hash", serializable.getDescriptionHash()->toHex()},
                  {"elgamal_public_key", serializable.getElGamalPublicKey()->toHex()},
                  {"number_of_guardians", serializable.getNumberOfGuardians()},
                  {"quorum", serializable.getQuorum()},
                };
                return j;
            }
            static unique_ptr<electionguard::CiphertextElectionContext> toObject(json j)
            {
                auto crypto_base_hash = j["crypto_base_hash"].get<string>();
                auto crypto_extended_base_hash = j["crypto_extended_base_hash"].get<string>();
                auto commitment_hash = j["commitment_hash"].get<string>();
                auto description_hash = j["description_hash"].get<string>();
                auto elgamal_public_key = j["elgamal_public_key"].get<string>();
                auto number_of_guardians = j["number_of_guardians"].get<uint64_t>();
                auto quorum = j["quorum"].get<uint64_t>();

                auto commitmentHash = ElementModQ::fromHex(commitment_hash);
                auto descriptionHash = ElementModQ::fromHex(description_hash);
                auto cryptoBaseHash = ElementModQ::fromHex(crypto_base_hash);
                auto cryptoExtendedBaseHash = ElementModQ::fromHex(crypto_extended_base_hash);
                auto elGamalPublicKey = ElementModP::fromHex(elgamal_public_key);

                return make_unique<CiphertextElectionContext>(
                  number_of_guardians, quorum, move(elGamalPublicKey), move(commitmentHash),
                  move(descriptionHash), move(cryptoBaseHash), move(cryptoExtendedBaseHash));
            }

          public:
            static vector<uint8_t>
            toBson(const electionguard::CiphertextElectionContext &serializable)
            {
                return json::to_bson(fromObject(serializable));
            }
            static string toJson(const electionguard::CiphertextElectionContext &serializable)
            {
                return fromObject(serializable).dump();
            }
            static unique_ptr<electionguard::CiphertextElectionContext>
            fromBson(vector<uint8_t> data)
            {
                return toObject(json::from_bson(data));
            }
            static unique_ptr<electionguard::CiphertextElectionContext> fromJson(string data)
            {
                return toObject(json::parse(data));
            }
        };

        class PlaintextBallot
        {
          private:
            static json fromObject(const electionguard::PlaintextBallot &serializable)
            {
                json contests;
                for (auto contest : serializable.getContests()) {
                    json selections;

                    // TODO: #134 extended data

                    for (auto selection : contest.get().getSelections()) {
                        json s = {{"object_id", selection.get().getObjectId()},
                                  {"vote", selection.get().getVote()},
                                  {"is_placeholder_selection", selection.get().getIsPlaceholder()}};
                        selections.push_back(s);
                    }
                    contests.push_back({
                      {"object_id", contest.get().getObjectId()},
                      {"ballot_selections", selections},
                    });
                }

                json result = {{"object_id", serializable.getObjectId()},
                               {"ballot_style", serializable.getBallotStyle()},
                               {"contests", contests}};
                return result;
            }
            static unique_ptr<electionguard::PlaintextBallot> toObject(json j)
            {

                auto object_id = j["object_id"].get<string>();
                auto ballot_style = j["ballot_style"].get<string>();

                auto contests = j["contests"];

                vector<unique_ptr<electionguard::PlaintextBallotContest>> plaintextContests;
                plaintextContests.reserve(contests.size());
                for (auto &contest : contests) {
                    auto contest_object_id = contest["object_id"].get<string>();

                    auto selections = contest["ballot_selections"];
                    vector<unique_ptr<electionguard::PlaintextBallotSelection>> plaintextSelections;
                    plaintextSelections.reserve(selections.size());
                    for (auto &selection : selections) {
                        auto selection_object_id = selection["object_id"].get<string>();
                        auto vote = selection["vote"].get<uint64_t>();

                        bool isPlaceholder = false;
                        if (selection.contains("is_placeholder_selection") &&
                            !selection["is_placeholder_selection"].is_null()) {
                            isPlaceholder = selection["is_placeholder_selection"].get<bool>();
                        }

                        // TODO: #134 extended data

                        plaintextSelections.push_back(
                          make_unique<electionguard::PlaintextBallotSelection>(
                            selection_object_id, vote, isPlaceholder));
                    }

                    plaintextContests.push_back(make_unique<electionguard::PlaintextBallotContest>(
                      contest_object_id, move(plaintextSelections)));
                }

                return make_unique<electionguard::PlaintextBallot>(object_id, ballot_style,
                                                                   move(plaintextContests));
            }

          public:
            static vector<uint8_t> toBson(const electionguard::PlaintextBallot &serializable)
            {
                return json::to_bson(fromObject(serializable));
            }
            static string toJson(const electionguard::PlaintextBallot &serializable)
            {
                return fromObject(serializable).dump();
            }
            static vector<uint8_t> toMsgPack(const electionguard::PlaintextBallot &serializable)
            {
                return json::to_msgpack(fromObject(serializable));
            }
            static unique_ptr<electionguard::PlaintextBallot> fromBson(vector<uint8_t> data)
            {
                return toObject(json::from_bson(data));
            }
            static unique_ptr<electionguard::PlaintextBallot> fromJson(string data)
            {
                return toObject(json::parse(data));
            }
            static unique_ptr<electionguard::PlaintextBallot> fromMsgPack(vector<uint8_t> data)
            {
                return toObject(json::from_msgpack(data));
            }
        };

        class CompactPlaintextBallot
        {
          protected:
            static json fromObject(const electionguard::CompactPlaintextBallot &serializable)
            {
                json selections;
                for (auto selection : serializable.getSelections()) {
                    selections.push_back(selection);
                }

                json extendedData;
                for (auto &[key, value] : serializable.getExtendedData()) {
                    json data = {{"value", value.get().value}, {"length", value.get().length}};
                    extendedData.emplace(to_string(key), data);
                }

                json result = {{"object_id", serializable.getObjectId()},
                               {"ballot_style", serializable.getBallotStyle()},
                               {"selections", selections},
                               {"extended_data", extendedData}};
                return result;
            }
            static unique_ptr<electionguard::CompactPlaintextBallot> toObject(json j)
            {
                auto object_id = j["object_id"].get<string>();
                auto ballot_style = j["ballot_style"].get<string>();

                auto selections = j["selections"];

                vector<uint64_t> votes;
                votes.reserve(selections.size());
                for (auto &selection : selections) {
                    auto vote = selection.get<uint64_t>();
                    votes.push_back(vote);
                }

                map<uint64_t, unique_ptr<electionguard::ExtendedData>> extendedDataMap;
                if (j.contains("extended_data") && !j["extended_data"].is_null()) {
                    for (auto &[key, body] : j["extended_data"].items()) {
                        auto value = body["value"].get<string>();
                        auto length = body["length"].get<uint64_t>();
                        extendedDataMap.emplace(stoul(key),
                                                make_unique<ExtendedData>(value, length));
                    }
                }

                return make_unique<electionguard::CompactPlaintextBallot>(
                  object_id, ballot_style, move(selections), move(extendedDataMap));
            }

          public:
            static vector<uint8_t> toBson(const electionguard::CompactPlaintextBallot &serializable)
            {
                return json::to_bson(fromObject(serializable));
            }
            static string toJson(const electionguard::CompactPlaintextBallot &serializable)
            {
                return fromObject(serializable).dump();
            }
            static vector<uint8_t>
            toMsgPack(const electionguard::CompactPlaintextBallot &serializable)
            {
                return json::to_msgpack(fromObject(serializable));
            }
            static unique_ptr<electionguard::CompactPlaintextBallot> fromBson(vector<uint8_t> data)
            {
                return toObject(json::from_bson(data));
            }
            static unique_ptr<electionguard::CompactPlaintextBallot> fromJson(string data)
            {
                return toObject(json::parse(data));
            }
            static unique_ptr<electionguard::CompactPlaintextBallot>
            fromMsgPack(vector<uint8_t> data)
            {
                return toObject(json::from_msgpack(data));
            }
        };

        class CiphertextBallot
        {
          private:
            static json fromObject(const electionguard::CiphertextBallot &serializable,
                                   bool withNonces)
            {
                json contests;
                for (const auto &contest : serializable.getContests()) {
                    json selections;
                    for (const auto &selection : contest.get().getSelections()) {
                        json ciphertext = {
                          {"pad", selection.get().getCiphertext()->getPad()->toHex()},
                          {"data", selection.get().getCiphertext()->getData()->toHex()}};
                        auto *p = selection.get().getProof();
                        json selection_proof = {
                          {"proof_zero_pad", p->getProofZeroPad()->toHex()},
                          {"proof_zero_data", p->getProofZeroData()->toHex()},
                          {"proof_one_pad", p->getProofOnePad()->toHex()},
                          {"proof_one_data", p->getProofOneData()->toHex()},
                          {"proof_zero_challenge", p->getProofZeroChallenge()->toHex()},
                          {"proof_one_challenge", p->getProofOneChallenge()->toHex()},
                          {"challenge", p->getChallenge()->toHex()},
                          {"proof_zero_response", p->getProofZeroResponse()->toHex()},
                          {"proof_one_response", p->getProofOneResponse()->toHex()},
                        };

                        json selection_props = {
                          {"object_id", selection.get().getObjectId()},
                          {"description_hash", selection.get().getDescriptionHash()->toHex()},
                          {"ciphertext", ciphertext},
                          {"is_placeholder_selection", selection.get().getIsPlaceholder()},
                          {"crypto_hash", selection.get().getCryptoHash()->toHex()},
                          {"proof", selection_proof}};

                        if (withNonces) {
                            selection_props["nonce"] = selection.get().getNonce()->toHex();
                        }
                        selections.push_back(selection_props);
                    }

                    json ciphertext = {
                      {"pad", contest.get().getCiphertextAccumulation()->getPad()->toHex()},
                      {"data", contest.get().getCiphertextAccumulation()->getData()->toHex()}};

                    auto *p = contest.get().getProof();
                    json contest_proof = {
                      {"pad", p->getPad()->toHex()},
                      {"data", p->getData()->toHex()},
                      {"challenge", p->getChallenge()->toHex()},
                      {"response", p->getResponse()->toHex()},
                      {"constant", p->getConstant()},
                    };
                    json contest_props = {
                      {"object_id", contest.get().getObjectId()},
                      {"description_hash", contest.get().getDescriptionHash()->toHex()},
                      {"ballot_selections", selections},
                      {"ciphertext_accumulation", ciphertext},
                      {"crypto_hash", contest.get().getCryptoHash()->toHex()},
                      {"proof", contest_proof},
                    };
                    if (withNonces) {
                        contest_props["nonce"] = contest.get().getNonce()->toHex();
                    }

                    contests.push_back(contest_props);
                }
                json result = {
                  {"object_id", serializable.getObjectId()},
                  {"ballot_style", serializable.getBallotStyle()},
                  {"description_hash", serializable.getDescriptionHash()->toHex()},
                  {"previous_tracking_hash", serializable.getPreviousTrackingHash()->toHex()},
                  {"contests", contests},
                  {"tracking_hash", serializable.getTrackingHash()->toHex()},
                  {"tracking_code", serializable.getTrackingCode()},
                  {"timestamp", serializable.getTimestamp()},
                  {"crypto_hash", serializable.getCryptoHash()->toHex()},
                };
                if (withNonces) {
                    result["nonce"] = serializable.getNonce()->toHex();
                }
                return result;
            }
            static unique_ptr<electionguard::CiphertextBallot> toObject(json j)
            {
                auto object_id = j["object_id"].get<string>();
                auto ballot_style = j["ballot_style"].get<string>();
                auto description_hash = j["description_hash"].get<string>();
                auto previous_tracking_hash = j["previous_tracking_hash"].get<string>();
                auto tracking_hash = j["tracking_hash"].get<string>();
                auto timestamp = j["timestamp"].get<uint64_t>();
                auto ballot_nonce = j["nonce"].is_null() ? "" : j["nonce"].get<string>();
                auto crypto_hash = j["crypto_hash"].get<string>();

                auto contests = j["contests"];

                vector<unique_ptr<CiphertextBallotContest>> ciphertextContests;
                ciphertextContests.reserve(contests.size());
                for (auto &contest : contests) {
                    auto contest_object_id = contest["object_id"].get<string>();
                    auto contest_description_hash = contest["description_hash"].get<string>();
                    auto contest_nonce =
                      contest["nonce"].is_null() ? "" : contest["nonce"].get<string>();
                    auto contest_ciphertext = contest["ciphertext_accumulation"];
                    auto contest_ciphertext_pad = contest_ciphertext["pad"].get<string>();
                    auto contest_ciphertext_data = contest_ciphertext["data"].get<string>();

                    auto deserialized_contest_ciphertext =
                      make_unique<electionguard::ElGamalCiphertext>(
                        ElementModP::fromHex(contest_ciphertext_pad),
                        ElementModP::fromHex(contest_ciphertext_data));
                    auto contest_crypto_hash = contest["crypto_hash"].get<string>();

                    auto proof = contest["proof"];
                    auto contest_proof_pad = proof["pad"].get<string>();
                    auto contest_proof_data = proof["data"].get<string>();
                    auto contest_proof_challenge = proof["challenge"].get<string>();
                    auto contest_proof_response = proof["response"].get<string>();
                    auto contest_proof_constant = proof["constant"].get<uint64_t>();

                    auto deserializedProof = make_unique<electionguard::ConstantChaumPedersenProof>(
                      ElementModP::fromHex(contest_proof_pad),
                      ElementModP::fromHex(contest_proof_data),
                      ElementModQ::fromHex(contest_proof_challenge),
                      ElementModQ::fromHex(contest_proof_response), contest_proof_constant);

                    auto selections = contest["ballot_selections"];
                    vector<unique_ptr<electionguard::CiphertextBallotSelection>>
                      ciphertextSelections;
                    ciphertextSelections.reserve(selections.size());
                    for (auto &selection : selections) {
                        auto selection_object_id = selection["object_id"].get<string>();
                        auto selection_description_hash =
                          selection["description_hash"].get<string>();
                        auto selection_is_placeholder_selection =
                          selection["is_placeholder_selection"].get<bool>();
                        auto selection_nonce =
                          selection["nonce"].is_null() ? "" : selection["nonce"].get<string>();
                        auto selection_crypto_hash = selection["crypto_hash"].get<string>();

                        auto ciphertext = selection["ciphertext"];
                        auto ciphertext_pad = ciphertext["pad"].get<string>();
                        auto ciphertext_data = ciphertext["data"].get<string>();

                        auto deserializedCiphertext = make_unique<electionguard::ElGamalCiphertext>(
                          ElementModP::fromHex(ciphertext_pad),
                          ElementModP::fromHex(ciphertext_data));

                        auto selection_proof = selection["proof"];
                        auto selection_proof_zero_pad =
                          selection_proof["proof_zero_pad"].get<string>();
                        auto selection_proof_zero_data =
                          selection_proof["proof_zero_data"].get<string>();
                        auto selection_proof_one_pad =
                          selection_proof["proof_one_pad"].get<string>();
                        auto selection_proof_one_data =
                          selection_proof["proof_one_data"].get<string>();
                        auto selection_proof_zero_challenge =
                          selection_proof["proof_zero_challenge"].get<string>();
                        auto selection_proof_one_challenge =
                          selection_proof["proof_one_challenge"].get<string>();
                        auto selection_proof_challenge = selection_proof["challenge"].get<string>();
                        auto selection_proof_zero_response =
                          selection_proof["proof_zero_response"].get<string>();
                        auto selection_proof_one_response =
                          selection_proof["proof_one_response"].get<string>();

                        auto deserializedDisjunctive =
                          make_unique<electionguard::DisjunctiveChaumPedersenProof>(
                            ElementModP::fromHex(selection_proof_zero_pad),
                            ElementModP::fromHex(selection_proof_zero_data),
                            ElementModP::fromHex(selection_proof_one_pad),
                            ElementModP::fromHex(selection_proof_one_data),
                            ElementModQ::fromHex(selection_proof_zero_challenge),
                            ElementModQ::fromHex(selection_proof_one_challenge),
                            ElementModQ::fromHex(selection_proof_challenge),
                            ElementModQ::fromHex(selection_proof_zero_response),
                            ElementModQ::fromHex(selection_proof_one_response));

                        auto nonce = selection_nonce.empty()
                                       ? make_unique<ElementModQ>(ZERO_MOD_Q())
                                       : ElementModQ::fromHex(selection_nonce);

                        ciphertextSelections.push_back(
                          make_unique<electionguard::CiphertextBallotSelection>(
                            selection_object_id, *ElementModQ::fromHex(selection_description_hash),
                            move(deserializedCiphertext), selection_is_placeholder_selection,
                            move(nonce), ElementModQ::fromHex(selection_crypto_hash),
                            move(deserializedDisjunctive)));
                    }

                    auto nonce = contest_nonce.empty() ? make_unique<ElementModQ>(ZERO_MOD_Q())
                                                       : ElementModQ::fromHex(contest_nonce);

                    ciphertextContests.push_back(
                      make_unique<electionguard::CiphertextBallotContest>(
                        contest_object_id, *ElementModQ::fromHex(contest_description_hash),
                        move(ciphertextSelections), move(nonce),
                        move(deserialized_contest_ciphertext),
                        ElementModQ::fromHex(contest_crypto_hash), move(deserializedProof)));
                }

                auto nonce = ballot_nonce.empty() ? make_unique<ElementModQ>(ZERO_MOD_Q())
                                                  : ElementModQ::fromHex(ballot_nonce);

                return make_unique<electionguard::CiphertextBallot>(
                  object_id, ballot_style, *ElementModQ::fromHex(description_hash),
                  ElementModQ::fromHex(previous_tracking_hash), move(ciphertextContests),
                  ElementModQ::fromHex(tracking_hash), timestamp, move(nonce),
                  ElementModQ::fromHex(crypto_hash));
            }

          public:
            static vector<uint8_t> toBson(const electionguard::CiphertextBallot &serializable,
                                          bool withNonces)
            {
                return json::to_bson(fromObject(serializable, withNonces));
            }
            static string toJson(const electionguard::CiphertextBallot &serializable,
                                 bool withNonces)
            {
                return fromObject(serializable, withNonces).dump();
            }
            static vector<uint8_t> toMsgPack(const electionguard::CiphertextBallot &serializable,
                                             bool withNonce)
            {
                return json::to_msgpack(fromObject(serializable, withNonce));
            }
            static unique_ptr<electionguard::CiphertextBallot> fromBson(vector<uint8_t> data)
            {
                return toObject(json::from_bson(data));
            }
            static unique_ptr<electionguard::CiphertextBallot> fromJson(string data)
            {
                return toObject(json::parse(data));
            }
            static unique_ptr<electionguard::CiphertextBallot> fromMsgPack(vector<uint8_t> data)
            {
                return toObject(json::from_msgpack(data));
            }
        };

        class CompactCiphertextBallot
        {
            class CompactPlaintextBallotWrapper : CompactPlaintextBallot
            {
              public:
                static json
                fromObjectWrapper(const electionguard::CompactPlaintextBallot &serializable)
                {
                    return CompactPlaintextBallot::fromObject(serializable);
                }
                static unique_ptr<electionguard::CompactPlaintextBallot> toObjectWrapper(json j)
                {
                    return CompactPlaintextBallot::toObject(j);
                }
            };

          private:
            static json fromObject(const electionguard::CompactCiphertextBallot &serializable)
            {
                json plaintext =
                  CompactPlaintextBallotWrapper::fromObjectWrapper(*serializable.getPlaintext());

                json result = {
                  {"plaintext", plaintext},
                  {"previous_tracking_hash", serializable.getPreviousTrackingHash()->toHex()},
                  {"tracking_hash", serializable.getTrackingHash()->toHex()},
                  {"nonce", serializable.getNonce()->toHex()},
                  {"timestamp", serializable.getTimestamp()},
                  {"ballot_box_state", getBallotBoxStateString(serializable.getBallotBoxState())}};
                return result;
            }
            static unique_ptr<electionguard::CompactCiphertextBallot> toObject(json j)
            {

                auto previousTrackingHash = j["previous_tracking_hash"].get<string>();
                auto trackingHash = j["tracking_hash"].get<string>();
                auto nonce = j["nonce"].get<string>();
                auto timestamp = j["timestamp"].get<uint64_t>();
                auto ballotBoxState = j["ballot_box_state"].get<string>();

                auto plaintext = CompactPlaintextBallotWrapper::toObjectWrapper(j["plaintext"]);

                return make_unique<electionguard::CompactCiphertextBallot>(
                  move(plaintext), getBallotBoxState(ballotBoxState),
                  ElementModQ::fromHex(previousTrackingHash), ElementModQ::fromHex(trackingHash),
                  timestamp, ElementModQ::fromHex(nonce));
            }

          public:
            static vector<uint8_t>
            toBson(const electionguard::CompactCiphertextBallot &serializable)
            {
                return json::to_bson(fromObject(serializable));
            }
            static string toJson(const electionguard::CompactCiphertextBallot &serializable)
            {
                return fromObject(serializable).dump();
            }
            static vector<uint8_t>
            toMsgPack(const electionguard::CompactCiphertextBallot &serializable)
            {
                return json::to_msgpack(fromObject(serializable));
            }
            static unique_ptr<electionguard::CompactCiphertextBallot> fromBson(vector<uint8_t> data)
            {
                return toObject(json::from_bson(data));
            }
            static unique_ptr<electionguard::CompactCiphertextBallot> fromJson(string data)
            {
                return toObject(json::parse(data));
            }
            static unique_ptr<electionguard::CompactCiphertextBallot>
            fromMsgPack(vector<uint8_t> data)
            {
                return toObject(json::from_msgpack(data));
            }
        };
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_SERIALIZE_HPP_INCLUDED__ */
