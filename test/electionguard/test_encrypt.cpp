#include "../../src/electionguard/log.hpp"
#include "generators/ballot.hpp"
#include "generators/election.hpp"

#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>
#include <electionguard/encrypt.hpp>

using namespace electionguard;
using namespace electionguard::test::generators;
using namespace std;

TEST_CASE("Encrypt simple selection succeeds")
{
    // Arrange
    const auto *candidateId = "some-candidate-id";
    const auto *selectionId = "some-selection-object-id";
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto nonce = rand_q();
    auto metadata = make_unique<SelectionDescription>(selectionId, candidateId, 1UL);
    auto hashContext = metadata->crypto_hash();
    auto plaintext = BallotGenerator::selectionFrom(*metadata);

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

TEST_CASE("Encrypt simple selection malformed data fails")
{
    // Arrange
    const auto *candidateId = "some-candidate-id";
    const auto *selectionId = "some-selection-object-id";
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto nonce = rand_q();
    auto metadata = make_unique<SelectionDescription>(selectionId, candidateId, 1UL);
    auto hashContext = metadata->crypto_hash();
    auto plaintext = BallotGenerator::selectionFrom(*metadata);

    // Act
    auto result = encryptSelection(*plaintext, *metadata, *keypair->getPublicKey(), ONE_MOD_Q(),
                                   *nonce, false, true);

    // tamper with the description_hash
    auto malformedDescriptionHash = make_unique<CiphertextBallotSelection>(
      result->getObjectId(), TWO_MOD_Q(), result->getCiphertext()->clone(),
      result->getIsPlaceholder(), result->getNonce()->clone(), result->getCryptoHash()->clone(),
      result->getProof()->clone());

    // remove the proof
    auto missingProof = make_unique<CiphertextBallotSelection>(
      result->getObjectId(), *result->getDescriptionHash(), result->getCiphertext()->clone(),
      result->getIsPlaceholder(), result->getNonce()->clone(), result->getCryptoHash()->clone(),
      nullptr);

    // Assert
    CHECK(result->isValidEncryption(*hashContext, *keypair->getPublicKey(), ONE_MOD_Q()) == true);
    CHECK(malformedDescriptionHash->isValidEncryption(*hashContext, *keypair->getPublicKey(),
                                                      ONE_MOD_Q()) == false);
    CHECK(missingProof->isValidEncryption(*hashContext, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          false);
}

TEST_CASE("Encrypt PlaintextBallot with EncryptionMediator against constructed "
          "InternalElectionDescription succeeds")
{
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto election = ElectionGenerator::getFakeElection();
    auto metadata = make_unique<InternalElectionDescription>(*election);
    auto context = ElectionGenerator::getFakeContext(*metadata, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");

    auto mediator = make_unique<EncryptionMediator>(*metadata, *context, *device);

    // Act
    auto plaintext = BallotGenerator::getFakeBallot(*metadata);
    Log::debug(plaintext->toJson());
    auto ciphertext = mediator->encrypt(*plaintext);

    // Assert
    // TODO: validate against metadata->getDescriptionHash()
    CHECK(ciphertext->isValidEncryption(*context->getDescriptionHash(), *keypair->getPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);
}

TEST_CASE("Encrypt PlaintextBallot undervote succeeds")
{
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto election = ElectionGenerator::getFakeElection();
    auto metadata = make_unique<InternalElectionDescription>(*election);
    auto context = ElectionGenerator::getFakeContext(*metadata, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");

    auto mediator = make_unique<EncryptionMediator>(*metadata, *context, *device);

    // Act
    auto plaintext = BallotGenerator::getFakeBallot(*metadata, 0UL);
    Log::debug(plaintext->toJson());
    auto ciphertext = mediator->encrypt(*plaintext);

    // Assert
    // TODO: validate against metadata->getDescriptionHash()
    CHECK(ciphertext->isValidEncryption(*context->getDescriptionHash(), *keypair->getPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);
}

TEST_CASE("Encrypt simple PlaintextBallot with EncryptionMediator succeeds")
{
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto metadata = ElectionGenerator::getFakeMetadata(TWO_MOD_Q());
    auto context = ElectionGenerator::getFakeContext(*metadata, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");

    auto mediator = make_unique<EncryptionMediator>(*metadata, *context, *device);

    // Act
    auto plaintext = BallotGenerator::getFakeBallot(*metadata);
    auto ciphertext = mediator->encrypt(*plaintext);

    // Assert
    // TODO: validate against metadata->getDescriptionHash()
    CHECK(ciphertext->isValidEncryption(*context->getDescriptionHash(), *keypair->getPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);

    // Can Serialize CiphertextBallot
    auto json = ciphertext->toJson(); // as default values
    auto fromJson = CiphertextBallot::fromJson(json);
    CHECK(fromJson->getNonce()->toHex() == ZERO_MOD_Q().toHex());

    // serialize with nonce values
    auto jsonWithNonces = ciphertext->toJson(true);
    auto fromJsonWithNonces = CiphertextBallot::fromJson(jsonWithNonces);
    CHECK(fromJsonWithNonces->getNonce()->toHex() == ciphertext->getNonce()->toHex());

    CHECK(plaintext->getObjectId() == ciphertext->getObjectId());

    auto bson = ciphertext->toBson();
    auto fromBson = CiphertextBallot::fromBson(bson);
    CHECK(fromBson->getNonce()->toHex() == ZERO_MOD_Q().toHex());
}

TEST_CASE("Encrypt simple CompactPlaintextBallot with EncryptionMediator succeeds")
{
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto metadata = ElectionGenerator::getFakeMetadata(TWO_MOD_Q());
    auto context = ElectionGenerator::getFakeContext(*metadata, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");
    auto mediator = make_unique<EncryptionMediator>(*metadata, *context, *device);
    auto plaintext = BallotGenerator::getFakeBallot(*metadata);

    // Act
    auto compactCiphertext = mediator->compactEncrypt(*plaintext);

    // Assert
    CHECK(compactCiphertext->getObjectId() == plaintext->getObjectId());

    SUBCASE("Can Serialize CompactCiphertextBallot")
    {
        auto json = compactCiphertext->toJson();
        Log::debug(json);
        // Assert
        auto msgpack = compactCiphertext->toMsgPack();
        auto fromMsgpack = CompactCiphertextBallot::fromMsgPack(msgpack);
        CHECK(fromMsgpack->getNonce()->toHex() == compactCiphertext->getNonce()->toHex());
    }

    SUBCASE("Can Expand Plaintext")
    {
        auto expandedPlaintext =
          expandCompactPlaintextBallot(*compactCiphertext->getPlaintext(), *metadata);

        // Assert
        CHECK(expandedPlaintext->getObjectId() == compactCiphertext->getObjectId());

        auto msgpack = expandedPlaintext->toMsgPack();
        auto fromMsgPack = PlaintextBallot::fromMsgPack(msgpack);
        CHECK(fromMsgPack->getObjectId() == expandedPlaintext->getObjectId());
    }

    SUBCASE("Can Expand Ciphertext")
    {
        auto json = compactCiphertext->toJson();
        Log::debug(json);

        auto expandedCiphertext =
          expandCompactCiphertextBallot(*compactCiphertext, *metadata, *context);

        // Assert
        CHECK(expandedCiphertext->getNonce()->toHex() == compactCiphertext->getNonce()->toHex());
        CHECK(expandedCiphertext->isValidEncryption(*context->getDescriptionHash(),
                                                    *keypair->getPublicKey(),
                                                    *context->getCryptoExtendedBaseHash()) == true);
    }
}

TEST_CASE("Encrypt simple ballot from file succeeds")
{
    // Arrange
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto description = ElectionGenerator::getSimpleElectionFromFile();
    auto [metadata, context] =
      ElectionGenerator::getFakeCiphertextElection(*description, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");

    auto ballot = BallotGenerator::getSimpleBallotFromFile();

    // Act
    auto ciphertext = encryptBallot(*ballot, *metadata, *context, *device->getHash());

    // Assert
    CHECK(ciphertext->isValidEncryption(*context->getDescriptionHash(), *keypair->getPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);
}
