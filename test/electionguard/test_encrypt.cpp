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
      "contest-1-id", "district-1-id", 1UL, "n_of_m", numElected, numElected, "contest-1-name",
      "contest-1-title", "contest-1-sub", move(selections1)));

    const auto numElected2 = 1UL;
    contests.push_back(make_unique<ContestDescription>(
      "contest-2-id", "district-1-id", 1UL, "n_of_m", numElected, numElected, "contest-2-name",
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
    string ballotId = "ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123ballot-"
                      "id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-"
                      "123-ballot-id-123-ballot-id-123-ballot-id-123";

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
    // TODO: validate against metadata->getDescriptionHash()
    CHECK(ciphertext->isValidEncryption(*context->getDescriptionHash(), *keypair->getPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);

    // Can Serialize CiphertextBallot
    auto json = ciphertext->toJson();
    auto fromJson = CiphertextBallot::fromJson(json);

    CHECK(plaintext->getObjectId() == plaintext->getObjectId());

    auto bson = ciphertext->toBson();
    auto fromBson = CiphertextBallot::fromBson(bson);
}

TEST_CASE("Can serialize InternalElectionDescription")
{
    // Arrange
    auto metadata = getFakeMetadata(TWO_MOD_Q());
    auto json = metadata->toJson();
    auto bson = metadata->toBson();

    // Act
    auto fromJson = InternalElectionDescription::fromJson(json);
    auto fromBson = InternalElectionDescription::fromBson(bson);

    // Assert
    CHECK(metadata->getDescriptionHash().toHex() == fromJson->getDescriptionHash().toHex());
}

TEST_CASE("Can serialize CiphertextElectionContext")
{
    // Arrange
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto metadata = getFakeMetadata(TWO_MOD_Q());
    auto context = getFakeContext(*metadata, *keypair->getPublicKey());
    auto json = context->toJson();
    auto bson = context->toBson();

    // Act
    auto fromJson = CiphertextElectionContext::fromJson(json);
    auto fromBson = CiphertextElectionContext::fromBson(bson);

    // Assert
    // TODO: validate against metadata->getDescriptionHash()
    CHECK(fromJson->getDescriptionHash()->toHex() == context->getDescriptionHash()->toHex());
    CHECK(fromBson->getDescriptionHash()->toHex() == context->getDescriptionHash()->toHex());
}

TEST_CASE("Can Deserialize CiphertextElectionContext")
{
    // Arrange
    // Note this string includes a couple of bad hex strings
    // with leading zeroes and are therefore out of range
    auto json = string(
      "{\"number_of_guardians\":5,\"quorum\":3,\"elgamal_public_key\":"
      "\"09F8C96DA142602B8E1F731779AE1ECFD7467F61CBDE20E3A93DA2D8D8DEFCF3B8230404E03887F2E34BAE3CE0"
      "C73DD8A80D18FCB4C10884B5D1368319868D583DEFD16D237755BF16D4D406A2F71BC685EB9E08CD7A8E049F3EC3"
      "86D2E094A9A1E9C2E1494CE3DF488E5FE8995ABBDBEA490C1AFD2EB9E5AAD59812C16CB921D7A7FF663B40AE1C1B"
      "A9D181E4AE5CF518CA5314D7859729D94AC457F01CF40F7DD7D3D22A515C1B5C28A4743CFC072D8222D8DB4862CC"
      "8DBBDAC02357F243AF5F31C3790BD367C04E77105C2ACEE984D69E5D11BE02CB19271D80B1089EE33E977934A3FA"
      "5A33C6D278CEE9607B38C084BE05284BAA344BB1B3D742C0E279D1085AA01F6CAA0D0C807AB85A7AB7EB4F042C60"
      "D884748B494E2B9651543AFD5E619C5C458DB87357A2497A03DEDED1504FF849F9FB6115460D51E757E6FBF1B2E0"
      "DD2D55EBA8C58E4A6D328462DB0FB4C8A0B174933E62FCF38E94CAAFA369A52565B7C8B9AFA627EAE4A928C0B5CA"
      "3EE24EE4AB4C4EC2776961A2C84EC95C174C84CBC6E9F0E7A24887AE859735DE68D2266420188626FD01ABA55DAF"
      "7E57306BABDC3FD7F00DBA16B8CC7893E448CEED67B3FF72B0682753FDF1DF066B798422A07B4B2FE9EDF0AC5D5E"
      "FB29F530152DA4ADA4154218B93E73519B38E0F04D811C4050BBC28A575A4EEFAAF970CAEE3F1EE4575C929E53C6"
      "ABF7D2870F485DA\",\"description_hash\":"
      "\"7749DF5EC06636F64780798E90D7EBD6C6E9872695629E0B68F25E4E11C068F0\",\"crypto_base_hash\":"
      "\"0FA1F1BFE97937F099FD39102D1FB452500F13AC12EFDE4E5FFD0D0D1C3896056\",\"crypto_extended_"
      "base_hash\":\"0E92481188ACDBC604EC7B8563FBF4024063E874797415CC1E72C7BF5F0114BAD\"}");
    auto descriptionHash =
      ElementModQ::fromHex("7749DF5EC06636F64780798E90D7EBD6C6E9872695629E0B68F25E4E11C068F0");

    // Act
    auto fromJson = CiphertextElectionContext::fromJson(json);

    // Assert
    // TODO: validate against metadata->getDescriptionHash()
    CHECK(fromJson->getDescriptionHash()->toHex() == descriptionHash->toHex());
}

TEST_CASE("Can serialize PlaintextBallot")
{
    // Arrange
    auto metadata = getFakeMetadata(TWO_MOD_Q());
    auto plaintext = getFakeBallot(*metadata);
    auto json = plaintext->toJson();
    auto bson = plaintext->toBson();

    // Act
    auto fromJson = PlaintextBallot::fromJson(json);
    auto fromBson = PlaintextBallot::fromBson(bson);

    // Assert
    CHECK(plaintext->getObjectId() == plaintext->getObjectId());
}
