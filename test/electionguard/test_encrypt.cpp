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

TEST_CASE("Encrypt PlaintextBallot with EncryptionMediator against constructed "
          "InternalElectionDescription succeeds")
{
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto election = ElectionGenerator::getFakeElection();
    auto metadata = make_unique<InternalElectionDescription>(*election);
    auto context = ElectionGenerator::getFakeContext(*metadata, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, "Location");

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
    auto device = make_unique<EncryptionDevice>(12345UL, "Location");

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
    auto device = make_unique<EncryptionDevice>(12345UL, "Location");

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

    CHECK(plaintext->getObjectId() == plaintext->getObjectId());

    auto bson = ciphertext->toBson();
    auto fromBson = CiphertextBallot::fromBson(bson);
}

TEST_CASE("Can serialize PlaintextBallot")
{
    // Arrange
    auto metadata = ElectionGenerator::getFakeMetadata(TWO_MOD_Q());
    auto plaintext = BallotGenerator::getFakeBallot(*metadata);
    auto json = plaintext->toJson();
    auto bson = plaintext->toBson();

    // Act
    auto fromJson = PlaintextBallot::fromJson(json);
    auto fromBson = PlaintextBallot::fromBson(bson);

    // Assert
    CHECK(plaintext->getObjectId() == plaintext->getObjectId());
}
