#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>
#include <electionguard/encrypt.hpp>

using namespace electionguard;
TEST_CASE("Encrypt Ballot with stateful mediator succeeds")
{
    // insantiate the stateful mediator
    auto *cpp_encrypter = new EncryptionMediator();
    CHECK(cpp_encrypter->encrypt() == 9);

    delete cpp_encrypter;
}

TEST_CASE("Encrypt Selection with generic data succeeds")
{
    // Arrange
    const auto *candidate_id = "some-candidate-id";
    auto *metadata = new SelectionDescription("some-description-object_id", candidate_id, 1UL);
    auto *hashContext = metadata->crypto_hash();

    // instantiate a fake public key, hash, and nonce
    uint64_t pub[64] = {10};
    auto *publicKey = new ElementModP(pub);

    uint64_t hash[4] = {9};
    auto *extendedBaseHash = new ElementModQ(hash);

    uint64_t seed[4] = {4};
    auto *nonce_seed = new ElementModQ(seed);

    auto *plaintext = new PlaintextBallotSelection(candidate_id, "1");

    // Act
    auto *result =
      encrypt_selection(plaintext, metadata, publicKey, extendedBaseHash, nonce_seed, false, true);

    // Assert
    CHECK(result != nullptr);
    CHECK(result->getCiphertext() != nullptr);
    CHECK(result->isValidEncryption(hashContext, publicKey, extendedBaseHash) == true);

    delete metadata;
    delete publicKey;
    delete extendedBaseHash;
    delete nonce_seed;
    delete plaintext;
    delete result;
}