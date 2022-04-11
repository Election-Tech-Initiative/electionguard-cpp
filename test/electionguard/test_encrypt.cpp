#include "../../src/electionguard/log.hpp"
#include "generators/ballot.hpp"
#include "generators/election.hpp"
#include "generators/manifest.hpp"
#include "utils/constants.hpp"

#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>
#include <electionguard/encrypt.hpp>
#include <electionguard/manifest.hpp>

using namespace electionguard;
using namespace electionguard::tools::generators;
using namespace std;

TEST_CASE("Encrypt simple selection succeeds")
{
    // Arrange
    const auto *candidateId = "some-candidate-id";
    const auto *selectionId = "some-selection-object-id";
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);
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

TEST_CASE("Encrypt simple selection using precomputed values succeeds")
{
    // Arrange
    const auto *candidateId = "some-candidate-id";
    const auto *selectionId = "some-selection-object-id";
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);
    auto nonce = rand_q();
    auto metadata = make_unique<SelectionDescription>(selectionId, candidateId, 1UL);
    auto hashContext = metadata->crypto_hash();
    auto plaintext = BallotGenerator::selectionFrom(*metadata);

    // cause a two triples and a quad to be populated 
    PrecomputeBufferContext::populate(*keypair->getPublicKey(), 1);

    uint32_t max_precomputed_queue_size = PrecomputeBufferContext::get_max_queue_size();
    uint32_t current_precomputed_queue_size = PrecomputeBufferContext::get_current_queue_size();

    CHECK(1 == max_precomputed_queue_size);
    CHECK(1 == current_precomputed_queue_size);

    // and this ecryptSelection will use the precomputed values
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
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);
    auto nonce = rand_q();
    auto metadata = make_unique<SelectionDescription>(selectionId, candidateId, 1UL);
    auto hashContext = metadata->crypto_hash();
    auto plaintext = BallotGenerator::selectionFrom(*metadata);

    // Act
    auto result = encryptSelection(*plaintext, *metadata, *keypair->getPublicKey(), ONE_MOD_Q(),
                                   *nonce, false, true);

    // tamper with the description_hash
    auto malformedDescriptionHash = make_unique<CiphertextBallotSelection>(
      result->getObjectId(), metadata->getSequenceOrder(), TWO_MOD_Q(),
      result->getCiphertext()->clone(), result->getIsPlaceholder(), result->getNonce()->clone(),
      result->getCryptoHash()->clone(), result->getProof()->clone());

    // remove the proof
    auto missingProof = make_unique<CiphertextBallotSelection>(
      result->getObjectId(), metadata->getSequenceOrder(), *result->getDescriptionHash(),
      result->getCiphertext()->clone(), result->getIsPlaceholder(), result->getNonce()->clone(),
      result->getCryptoHash()->clone(), nullptr);

    // Assert
    CHECK(result->isValidEncryption(*hashContext, *keypair->getPublicKey(), ONE_MOD_Q()) == true);
    CHECK(malformedDescriptionHash->isValidEncryption(*hashContext, *keypair->getPublicKey(),
                                                      ONE_MOD_Q()) == false);
    CHECK(missingProof->isValidEncryption(*hashContext, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          false);
}

TEST_CASE("Encrypt PlaintextBallot with EncryptionMediator against constructed "
          "InternalManifest succeeds")
{
    auto secret = ElementModQ::fromHex(a_fixed_secret);
    auto keypair = ElGamalKeyPair::fromSecret(*secret);
    auto init = pow_mod_p(*keypair->getPublicKey(), *ElementModQ::fromUint64(1));
    auto manifest = ManifestGenerator::getJeffersonCountyManifest_Minimal();
    auto internal = make_unique<InternalManifest>(*manifest);
    auto context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");

    auto mediator = make_unique<EncryptionMediator>(*internal, *context, *device);

    // Act
    auto plaintext = BallotGenerator::getFakeBallot(*internal);
    // Log::debug(plaintext->toJson());
    auto ciphertext = mediator->encrypt(*plaintext);

    // Assert
    CHECK(ciphertext->isValidEncryption(*context->getManifestHash(), *keypair->getPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);
}

TEST_CASE("Encrypt PlaintextBallot undervote succeeds")
{
    auto secret = ElementModQ::fromHex(a_fixed_secret);
    auto keypair = ElGamalKeyPair::fromSecret(*secret);
    auto manifest = ManifestGenerator::getJeffersonCountyManifest_Minimal();
    auto internal = make_unique<InternalManifest>(*manifest);
    auto context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");

    auto mediator = make_unique<EncryptionMediator>(*internal, *context, *device);

    // Act
    auto plaintext = BallotGenerator::getFakeBallot(*internal, 0UL);
    // Log::debug(plaintext->toJson());
    auto ciphertext = mediator->encrypt(*plaintext);

    // Assert
    CHECK(ciphertext->isValidEncryption(*context->getManifestHash(), *keypair->getPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);
}

TEST_CASE("Encrypt PlaintextBallot overvote fails")
{
    // Arrange
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);
    auto manifest = ManifestGenerator::getJeffersonCountyManifest_Minimal();
    auto internal = make_unique<InternalManifest>(*manifest);
    auto context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");

    auto mediator = make_unique<EncryptionMediator>(*internal, *context, *device);

    // Act
    auto plaintext = BallotGenerator::getFakeBallot(*internal, 2UL);
    // Log::debug(plaintext->toJson());

    // Assert
    CHECK_THROWS_AS(mediator->encrypt(*plaintext), std::exception);
}

TEST_CASE("Encrypt simple PlaintextBallot with EncryptionMediator succeeds")
{
    auto secret = ElementModQ::fromHex(a_fixed_secret);
    auto keypair = ElGamalKeyPair::fromSecret(*secret);
    auto manifest = ManifestGenerator::getJeffersonCountyManifest_Minimal();
    auto internal = make_unique<InternalManifest>(*manifest);
    auto context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");

    auto mediator = make_unique<EncryptionMediator>(*internal, *context, *device);

    // Act
    auto plaintext = BallotGenerator::getFakeBallot(*manifest);
    auto ciphertext = mediator->encrypt(*plaintext);

    // Assert
    CHECK(ciphertext->isValidEncryption(*context->getManifestHash(), *keypair->getPublicKey(),
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
    // Arrange
    auto secret = ElementModQ::fromHex(a_fixed_secret);
    auto keypair = ElGamalKeyPair::fromSecret(*secret);
    auto manifest = ManifestGenerator::getJeffersonCountyManifest_Minimal();
    auto internal = make_unique<InternalManifest>(*manifest);
    auto context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");
    auto mediator = make_unique<EncryptionMediator>(*internal, *context, *device);
    auto plaintext = BallotGenerator::getFakeBallot(*internal);

    // Act
    auto compactCiphertext = mediator->compactEncrypt(*plaintext);

    // Assert
    CHECK(compactCiphertext->getObjectId() == plaintext->getObjectId());
}

TEST_CASE("Encrypt simple ballot from file with mediator succeeds")
{
    // Arrange
    auto secret = ElementModQ::fromHex(a_fixed_secret);
    auto keypair = ElGamalKeyPair::fromSecret(*secret);
    auto manifest = ManifestGenerator::getManifestFromFile(TEST_SPEC_VERSION, TEST_USE_SAMPLE);
    auto internal = make_unique<InternalManifest>(*manifest);
    auto context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());

    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");
    auto mediator = make_unique<EncryptionMediator>(*internal, *context, *device);
    auto ballot = BallotGenerator::getFakeBallot(*internal);

    // Act
    auto ciphertext = mediator->encrypt(*ballot);

    // Assert
    CHECK(ciphertext->isValidEncryption(*context->getManifestHash(), *keypair->getPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);
}

TEST_CASE("Encrypt simple ballot from file succeeds")
{
    // Arrange
    auto secret = ElementModQ::fromHex(a_fixed_secret);
    auto keypair = ElGamalKeyPair::fromSecret(*secret);
    auto manifest = ManifestGenerator::getManifestFromFile(TEST_SPEC_VERSION, TEST_USE_SAMPLE);
    auto internal = make_unique<InternalManifest>(*manifest);
    auto context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());

    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");

    auto ballot = BallotGenerator::getFakeBallot(*internal);

    // Act
    auto ciphertext = encryptBallot(*ballot, *internal, *context, *device->getHash(),
                                    make_unique<ElementModQ>(TWO_MOD_Q()));

    //Log::debug(ciphertext->toJson());

    // Assert
    CHECK(ciphertext->isValidEncryption(*context->getManifestHash(), *keypair->getPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);
}

TEST_CASE("Encrypt simple ballot from file submitted is valid")
{
    // Arrange
    auto secret = ElementModQ::fromHex(a_fixed_secret);
    auto keypair = ElGamalKeyPair::fromSecret(*secret);
    auto manifest = ManifestGenerator::getManifestFromFile(TEST_SPEC_VERSION, TEST_USE_SAMPLE);
    auto internal = make_unique<InternalManifest>(*manifest);
    auto context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());
    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");

    auto ballot = BallotGenerator::getFakeBallot(*internal);

    // Act
    auto ciphertext = encryptBallot(*ballot, *internal, *context, *device->getHash());
    auto submitted = SubmittedBallot::from(*ciphertext, BallotBoxState::cast);
    auto serialized = submitted->toJson();

    //Log::debug(serialized);
    auto deserialized = SubmittedBallot::fromJson(serialized);

    // Assert
    // TODO: compare other values
    CHECK(submitted->isValidEncryption(*context->getManifestHash(), *keypair->getPublicKey(),
                                       *context->getCryptoExtendedBaseHash()) == true);
    CHECK(deserialized->isValidEncryption(*context->getManifestHash(), *keypair->getPublicKey(),
                                          *context->getCryptoExtendedBaseHash()) == true);
}

TEST_CASE("Encrypt simple ballot from file succeeds with precomputed values")
{
    // Arrange
    auto secret = ElementModQ::fromHex(a_fixed_secret);
    auto keypair = ElGamalKeyPair::fromSecret(*secret);
    auto manifest = ManifestGenerator::getManifestFromFile(TEST_SPEC_VERSION, TEST_USE_SAMPLE);
    auto internal = make_unique<InternalManifest>(*manifest);
    auto context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());

    auto device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");

    auto ballot = BallotGenerator::getFakeBallot(*internal);

    // cause a two triples and a quad to be populated
    PrecomputeBufferContext::populate(*keypair->getPublicKey(), 50);
    PrecomputeBufferContext::stop_populate();
    uint32_t max_precomputed_queue_size = PrecomputeBufferContext::get_max_queue_size();
    uint32_t current_precomputed_queue_size = PrecomputeBufferContext::get_current_queue_size();

    CHECK(50 == max_precomputed_queue_size);
    CHECK(50 == current_precomputed_queue_size);

    // Act
    auto ciphertext = encryptBallot(*ballot, *internal, *context, *device->getHash(),
                                    make_unique<ElementModQ>(TWO_MOD_Q()));

    //Log::debug(ciphertext->toJson());

    // Assert
    CHECK(ciphertext->isValidEncryption(*context->getManifestHash(), *keypair->getPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);
}
