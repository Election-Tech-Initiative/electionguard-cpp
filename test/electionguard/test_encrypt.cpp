#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>
#include <electionguard/encrypt.hpp>

using namespace electionguard;
TEST_CASE("Encrypt Ballot with stateful mediator succeeds")
{
    // insantiate the stateful mediator
    auto *cpp_encrypter = new EncryptionMediator();
    CHECK(cpp_encrypter != nullptr);
    delete cpp_encrypter;
}

TEST_CASE("Encrypt Selection with generic data succeeds")
{
    // Arrange
    const auto *candidateId = "some-candidate-id";
    auto *metadata = new SelectionDescription("some-description-object_id", candidateId, 1UL);
    auto *hashContext = metadata->crypto_hash();

    // instantiate a fake public key, hash, and nonce
    uint64_t pub[64] = {10};
    auto *publicKey = new ElementModP(pub);

    uint64_t hash[4] = {9};
    auto *extendedBaseHash = new ElementModQ(hash);

    uint64_t seed[4] = {4};
    auto *nonceSeed = new ElementModQ(seed);

    auto *plaintext = new PlaintextBallotSelection(candidateId, "1");

    // Act
    auto *result =
      encrypt_selection(plaintext, metadata, publicKey, extendedBaseHash, nonceSeed, false, true);

    // Assert
    CHECK(result != nullptr);
    CHECK(result->getCiphertext() != nullptr);
    CHECK(result->isValidEncryption(hashContext, publicKey, extendedBaseHash) == true);

    delete metadata;
    delete publicKey;
    delete extendedBaseHash;
    delete nonceSeed;
    delete plaintext;
    delete result;
}
