#include "../../src/electionguard/log.hpp"
#include "mocks/ballot.hpp"
#include "mocks/election.hpp"

#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/ballot_compact.hpp>
#include <electionguard/encrypt.hpp>

using namespace electionguard;
using namespace electionguard::test::mocks;
using namespace std;

TEST_CASE("Can encrypt compact ballots")
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