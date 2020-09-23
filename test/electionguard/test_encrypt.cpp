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
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto nonce = rand_q();
    auto *metadata = new SelectionDescription("some-selection-object_id", candidateId, 1UL);
    auto hashContext = metadata->crypto_hash();
    auto *plaintext = new PlaintextBallotSelection(candidateId, "1");

    // Act
    auto result = encryptSelection(*plaintext, *metadata, *keypair->getPublicKey(), ONE_MOD_Q(),
                                   *nonce, false, true);

    // Assert
    CHECK(result != nullptr);
    CHECK(result->getCiphertext() != nullptr);
    CHECK(result->isValidEncryption(*hashContext, *keypair->getPublicKey(), ONE_MOD_Q()) == true);
    CHECK(result->getProof()->isValid(*result->getCiphertext(), *keypair->getPublicKey(),
                                      ONE_MOD_Q()) == true);

    delete metadata;
    delete plaintext;
}
