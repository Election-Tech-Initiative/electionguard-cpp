#ifndef __ELECTIONGUARD_CORE_SERIALIZE_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_SERIALIZE_HPP_INCLUDED__

#include "electionguard/ballot.hpp"
#include "electionguard/election.hpp"
#include "electionguard/export.h"
#include "electionguard/group.hpp"
#include "log.hpp"

#include <cstdint>
#include <cstring>
#include <memory>
#include <nlohmann/json.hpp>

using std::string;

using nlohmann::json;

namespace electionguard
{
    class Serialize
    {
      public:
        class InternalElectionDescription
        {
          private:
            static json fromObject(const electionguard::InternalElectionDescription &serializable)
            {
                json contests;
                for (auto contest : serializable.getContests()) {
                    json selections;
                    for (auto selection : contest.get().getSelections()) {
                        json s = {
                          {"object_id", selection.get().getObjectId()},
                          {"candidate_id", selection.get().getCandidateId()},
                          {"sequence_order", selection.get().getSequenceOrder()},
                        };
                        selections.push_back(s);
                    }
                    contests.push_back({
                      {"object_id", contest.get().getObjectId()},
                      {"electoral_district_id", contest.get().getElectoralDistrictId()},
                      {"sequence_order", contest.get().getSequenceOrder()},
                      {"vote_variation", contest.get().getVoteVariation()},
                      {"number_elected", contest.get().getNumberElected()},
                      {"votes_allowed", contest.get().getVotesAllowed()},
                      {"name", contest.get().getName()},
                      {"ballot_title", contest.get().getBallotTitle()},
                      {"ballot_subtitle", contest.get().getBallotSubtitle()},
                      {"ballot_selections", selections},
                    });
                }

                json result = {{"description_hash", serializable.getDescriptionHash().toHex()},
                               {"contests", contests}};

                return result;
            }
            static unique_ptr<electionguard::InternalElectionDescription> toObject(json j)
            {
                auto contests = j["contests"];

                vector<unique_ptr<ContestDescription>> contestDescriptions;
                contestDescriptions.reserve(contests.size());
                for (auto &contest : contests) {
                    auto contest_object_id = contest["object_id"].get<string>();
                    auto electoral_district_id = contest["electoral_district_id"].get<string>();
                    auto contest_sequence_order = contest["sequence_order"].get<uint64_t>();
                    auto vote_variation = contest["vote_variation"].get<string>();
                    auto number_elected = contest["number_elected"].get<uint64_t>();
                    auto votes_allowed = contest["votes_allowed"].get<uint64_t>();
                    auto name = contest["name"].get<string>();
                    auto ballot_title = contest["ballot_title"].is_object()
                                          ? contest["ballot_title"].dump()
                                          : contest["ballot_title"].get<string>();
                    auto ballot_subtitle = contest["ballot_subtitle"].is_object()
                                             ? contest["ballot_subtitle"].dump()
                                             : contest["ballot_subtitle"].get<string>();

                    auto selections = contest["ballot_selections"];
                    vector<unique_ptr<SelectionDescription>> selectionDescriptions;
                    selectionDescriptions.reserve(selections.size());
                    for (auto &selection : selections) {
                        auto selection_object_id = selection["object_id"].get<string>();
                        auto candidate_id = selection["candidate_id"].get<string>();
                        auto selection_sequence_order = selection["sequence_order"].get<uint64_t>();
                        selectionDescriptions.push_back(make_unique<SelectionDescription>(
                          selection_object_id, candidate_id, selection_sequence_order));
                    }

                    contestDescriptions.push_back(make_unique<ContestDescription>(
                      contest_object_id, electoral_district_id, contest_sequence_order,
                      vote_variation, number_elected, votes_allowed, name, ballot_title,
                      ballot_subtitle, move(selectionDescriptions)));
                }

                if (j["description_hash"] != nullptr) {
                    auto description_hash = j["description_hash"].get<string>();
                    auto descriptionHash = ElementModQ::fromHex(description_hash);

                    return make_unique<electionguard::InternalElectionDescription>(
                      *descriptionHash, move(contestDescriptions));
                } else {
                    return make_unique<electionguard::InternalElectionDescription>(
                      ZERO_MOD_Q(), move(contestDescriptions));
                }
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
                auto description_hash = j["description_hash"].get<string>();
                auto elgamal_public_key = j["elgamal_public_key"].get<string>();
                auto number_of_guardians = j["number_of_guardians"].get<uint64_t>();
                auto quorum = j["quorum"].get<uint64_t>();

                auto elGamalPublicKey = ElementModP::fromHex(elgamal_public_key);
                auto descriptionHash = ElementModQ::fromHex(description_hash);
                auto cryptoBaseHash = ElementModQ::fromHex(crypto_base_hash);
                auto cryptoExtendedBaseHash = ElementModQ::fromHex(crypto_extended_base_hash);

                return make_unique<CiphertextElectionContext>(
                  number_of_guardians, quorum, move(elGamalPublicKey), move(descriptionHash),
                  move(cryptoBaseHash), move(cryptoExtendedBaseHash));
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
                    for (auto selection : contest.get().getSelections()) {
                        json s = {{"object_id", selection.get().getObjectId()},
                                  {"vote", to_string(selection.get().toInt())}};
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
                auto balolot_style = j["ballot_style"].get<string>();

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
                        auto vote = selection["vote"].get<string>();
                        plaintextSelections.push_back(
                          make_unique<electionguard::PlaintextBallotSelection>(selection_object_id,
                                                                               vote));
                    }

                    plaintextContests.push_back(make_unique<electionguard::PlaintextBallotContest>(
                      contest_object_id, move(plaintextSelections)));
                }

                return make_unique<electionguard::PlaintextBallot>(object_id, balolot_style,
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
            static unique_ptr<electionguard::PlaintextBallot> fromBson(vector<uint8_t> data)
            {
                return toObject(json::from_bson(data));
            }
            static unique_ptr<electionguard::PlaintextBallot> fromJson(string data)
            {
                return toObject(json::parse(data));
            }
        };

        class CiphertextBallot
        {
          private:
            static json fromObject(const electionguard::CiphertextBallot &serializable)
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
                        selections.push_back(
                          {{"object_id", selection.get().getObjectId()},
                           {"description_hash", selection.get().getDescriptionHash()->toHex()},
                           {"ciphertext", ciphertext},
                           {"is_placeholder_selection", false},
                           {"nonce", selection.get().getNonce()->toHex()},
                           {"crypto_hash", selection.get().getCryptoHash()->toHex()},
                           {"proof", selection_proof}});
                    }
                    auto *p = contest.get().getProof();
                    json contest_proof = {
                      {"pad", p->getPad()->toHex()},
                      {"data", p->getData()->toHex()},
                      {"challenge", p->getChallenge()->toHex()},
                      {"response", p->getResponse()->toHex()},
                      {"constant", p->getConstant()},
                    };
                    contests.push_back({
                      {"object_id", contest.get().getObjectId()},
                      {"description_hash", contest.get().getDescriptionHash()->toHex()},
                      {"selections", selections},
                      {"nonce", contest.get().getNonce()->toHex()},
                      {"crypto_hash", contest.get().getCryptoHash()->toHex()},
                      {"proof", contest_proof},
                    });
                }
                json result = {
                  {"object_id", serializable.getObjectId()},
                  {"balolot_style", serializable.getBallotStyle()},
                  {"description_hash", serializable.getDescriptionHash()->toHex()},
                  {"previous_tracking_hash", serializable.getPreviousTrackingHash()->toHex()},
                  {"contests", contests},
                  {"tracking_hash", serializable.getTrackingHash()->toHex()},
                  {"tracking_code", serializable.getTrackingCode()},
                  {"timestamp", serializable.getTimestamp()},
                  {"nonce", serializable.getNonce()->toHex()},
                  {"crypto_hash", serializable.getCryptoHash()->toHex()},
                };
                return result;
            }
            static unique_ptr<electionguard::CiphertextBallot> toObject(json j)
            {
                auto object_id = j["object_id"].get<string>();
                auto balolot_style = j["balolot_style"].get<string>();
                auto description_hash = j["description_hash"].get<string>();
                auto previous_tracking_hash = j["previous_tracking_hash"].get<string>();
                auto tracking_hash = j["tracking_hash"].get<string>();
                auto timestamp = j["timestamp"].get<uint64_t>();
                auto nonce = j["nonce"].get<string>();
                auto crypto_hash = j["crypto_hash"].get<string>();

                auto contests = j["contests"];

                vector<unique_ptr<CiphertextBallotContest>> plaintextContests;
                plaintextContests.reserve(contests.size());
                for (auto &contest : contests) {
                    auto contest_object_id = contest["object_id"].get<string>();
                    auto contest_description_hash = contest["description_hash"].get<string>();
                    auto contest_nonce = contest["nonce"].get<string>();
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

                    auto selections = contest["selections"];
                    vector<unique_ptr<electionguard::CiphertextBallotSelection>>
                      plaintextSelections;
                    plaintextSelections.reserve(selections.size());
                    for (auto &selection : selections) {
                        auto selection_object_id = selection["object_id"].get<string>();
                        auto selection_description_hash =
                          selection["description_hash"].get<string>();
                        auto selection_is_placeholder_selection =
                          selection["is_placeholder_selection"].get<bool>();
                        auto selection_nonce = selection["nonce"].get<string>();
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

                        plaintextSelections.push_back(
                          make_unique<electionguard::CiphertextBallotSelection>(
                            selection_object_id, *ElementModQ::fromHex(selection_description_hash),
                            move(deserializedCiphertext), selection_is_placeholder_selection,
                            ElementModQ::fromHex(selection_nonce),
                            ElementModQ::fromHex(selection_crypto_hash),
                            move(deserializedDisjunctive)));
                    }

                    plaintextContests.push_back(make_unique<electionguard::CiphertextBallotContest>(
                      contest_object_id, *ElementModQ::fromHex(contest_description_hash),
                      move(plaintextSelections), ElementModQ::fromHex(contest_nonce),
                      ElementModQ::fromHex(contest_crypto_hash), move(deserializedProof)));
                }

                return make_unique<electionguard::CiphertextBallot>(
                  object_id, balolot_style, *ElementModQ::fromHex(description_hash),
                  ElementModQ::fromHex(previous_tracking_hash), move(plaintextContests),
                  ElementModQ::fromHex(tracking_hash), timestamp, ElementModQ::fromHex(nonce),
                  ElementModQ::fromHex(crypto_hash));
            }

          public:
            static vector<uint8_t> toBson(const electionguard::CiphertextBallot &serializable)
            {
                return json::to_bson(fromObject(serializable));
            }
            static string toJson(const electionguard::CiphertextBallot &serializable)
            {
                return fromObject(serializable).dump();
            }
            static unique_ptr<electionguard::CiphertextBallot> fromBson(vector<uint8_t> data)
            {
                return toObject(json::from_bson(data));
            }
            static unique_ptr<electionguard::CiphertextBallot> fromJson(string data)
            {
                return toObject(json::parse(data));
            }
        };
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_SERIALIZE_HPP_INCLUDED__ */