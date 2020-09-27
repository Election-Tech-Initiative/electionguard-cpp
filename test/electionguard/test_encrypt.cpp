#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>
#include <electionguard/encrypt.hpp>

using namespace electionguard;

unique_ptr<InternalElectionDescription> getFakeMetadata(const ElementModQ &descriptionHash)
{
    vector<unique_ptr<SelectionDescription>> selections1;
    selections1.push_back(
      make_unique<SelectionDescription>("contest-1-selection-1-id", "candidate-1-id", 1UL));
    selections1.push_back(
      make_unique<SelectionDescription>("contest-1-selection-2-id", "candidate-2-id", 2UL));
    selections1.push_back(
      make_unique<SelectionDescription>("contest-1-selection-3-id", "candidate-3-id", 3UL));

    vector<unique_ptr<SelectionDescription>> selections2;
    selections2.push_back(
      make_unique<SelectionDescription>("contest-2-selection-1-id", "candidate-1-id", 1UL));
    selections2.push_back(
      make_unique<SelectionDescription>("contest-2-selection-2-id", "candidate-2-id", 2UL));

    vector<unique_ptr<ContestDescription>> contests;
    const auto numElected = 2UL;
    contests.push_back(make_unique<ContestDescription>(
      "contest-1-id", "district-1-id", 1UL, 1UL, numElected, numElected, "contest-1-name",
      "contest-1-title", "contest-1-sub", move(selections1)));

    const auto numElected2 = 1UL;
    contests.push_back(make_unique<ContestDescription>(
      "contest-2-id", "district-1-id", 1UL, 1UL, numElected, numElected, "contest-2-name",
      "contest-2-title", "contest-1-sub", move(selections2)));

    auto metadata = make_unique<InternalElectionDescription>(descriptionHash, move(contests));
    return metadata;
}

unique_ptr<CiphertextElectionContext> getFakeContext(const InternalElectionDescription &metadata,
                                                     const ElementModP &publicKey)
{

    auto context =
      CiphertextElectionContext::make(1UL, 1UL, make_unique<ElementModP>(publicKey),
                                      make_unique<ElementModQ>(metadata.getDescriptionHash()));
    return context;
}

unique_ptr<PlaintextBallotSelection> selectionFrom(const SelectionDescription &description,
                                                   bool choice)
{
    return make_unique<PlaintextBallotSelection>(description.getObjectId(), choice ? "1" : "0");
}

unique_ptr<PlaintextBallotContest> contestFrom(const ContestDescription &contest)
{

    uint64_t choices = 0;
    vector<unique_ptr<PlaintextBallotSelection>> selections;

    // jsut loop through and select the necessary amount of trues
    for (const auto &description : contest.getSelections()) {
        if (choices < contest.getNumberElected()) {
            ++choices;
            selections.push_back(selectionFrom(description.get(), true));
        } else {
            selections.push_back(selectionFrom(description.get(), false));
        }
    }

    return make_unique<PlaintextBallotContest>(contest.getObjectId(), move(selections));
}

unique_ptr<PlaintextBallot> getFakeBallot(const InternalElectionDescription &metadata)
{
    string ballotId = "ballot-id-123";

    vector<unique_ptr<PlaintextBallotContest>> contests;
    for (const auto &contest : metadata.getContests()) {
        contests.push_back(contestFrom(contest.get()));
    }
    return make_unique<PlaintextBallot>(ballotId, "ballot-style-1", move(contests));
}

TEST_CASE("Encrypt Selection with generic data succeeds")
{
    // Arrange
    const auto *candidateId = "some-candidate-id";
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto nonce = rand_q();
    auto metadata = make_unique<SelectionDescription>("some-selection-object_id", candidateId, 1UL);
    auto hashContext = metadata->crypto_hash();
    auto plaintext = make_unique<PlaintextBallotSelection>(candidateId, "1");

    // Act
    auto result = encryptSelection(*plaintext, *metadata, *keypair->getPublicKey(), ONE_MOD_Q(),
                                   *nonce, false, true);

    // Assert
    CHECK(result != nullptr);
    CHECK(result->getCiphertext() != nullptr);
    CHECK(result->isValidEncryption(*hashContext, *keypair->getPublicKey(), ONE_MOD_Q()) == true);
    CHECK(result->getProof()->isValid(*result->getCiphertext(), *keypair->getPublicKey(),
                                      ONE_MOD_Q()) == true);
}

TEST_CASE("Encrypt Ballot with mediator succeeds")
{
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto metadata = getFakeMetadata(TWO_MOD_Q());
    auto context = getFakeContext(*metadata, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, "Location");

    auto mediator = make_unique<EncryptionMediator>(*metadata, *context, *device);

    // Act
    auto plaintext = getFakeBallot(*metadata);
    auto ciphertext = mediator->encrypt(*plaintext);

    // Assert
    CHECK(ciphertext->isValidEncryption(metadata->getDescriptionHash(), *keypair->getPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);
}
