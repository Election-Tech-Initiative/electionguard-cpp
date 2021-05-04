#include "../../src/electionguard/log.hpp"
#include "mocks/ballot.hpp"
#include "mocks/election.hpp"
#include "mocks/manifest.hpp"

#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/ballot_compact.hpp>
#include <electionguard/encrypt.hpp>

using namespace electionguard;
using namespace electionguard::test::mocks;
using namespace std;

struct TestEncryptFixture {

    unique_ptr<ElGamalKeyPair> keypair;
    unique_ptr<Manifest> manifest;
    unique_ptr<InternalManifest> internal;
    unique_ptr<CiphertextElectionContext> context;
    unique_ptr<EncryptionDevice> device;
    unique_ptr<EncryptionMediator> mediator;
    unique_ptr<PlaintextBallot> plaintext;
    unique_ptr<CompactCiphertextBallot> compactCiphertext;

    TestEncryptFixture()
        : keypair(ElGamalKeyPair::fromSecret(TWO_MOD_Q())),
          manifest(ManifestGenerator::getJeffersonCountyManifest_Minimal()),
          internal(make_unique<InternalManifest>(*manifest)),
          context(ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey())),
          device(make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location")),
          mediator(make_unique<EncryptionMediator>(*internal, *context, *device)),
          plaintext(BallotGenerator::getFakeBallot(*internal)),
          compactCiphertext(mediator->compactEncrypt(*plaintext))
    {
    }
};

TEST_SUITE("Test Encrypt Compact Ballot")
{
    auto fixture = make_unique<TestEncryptFixture>();

    TEST_CASE("Can encrypt compact ballots")
    {
        // Assert
        CHECK(fixture->compactCiphertext->getObjectId() == fixture->plaintext->getObjectId());
    }

    TEST_CASE("Can Serialize CompactCiphertextBallot")
    {
        auto json = fixture->compactCiphertext->toJson();
        Log::debug(json);
        // Assert
        auto msgpack = fixture->compactCiphertext->toMsgPack();
        auto fromMsgpack = CompactCiphertextBallot::fromMsgPack(msgpack);
        CHECK(fromMsgpack->getNonce()->toHex() == fixture->compactCiphertext->getNonce()->toHex());
    }

    TEST_CASE("Can Expand Plaintext")
    {
        auto expandedPlaintext = expandCompactPlaintextBallot(
          *fixture->compactCiphertext->getPlaintext(), *fixture->manifest);

        // Assert
        CHECK(expandedPlaintext->getObjectId() == fixture->compactCiphertext->getObjectId());

        auto msgpack = expandedPlaintext->toMsgPack();
        auto fromMsgPack = PlaintextBallot::fromMsgPack(msgpack);
        CHECK(fromMsgPack->getObjectId() == expandedPlaintext->getObjectId());
    }

    TEST_CASE("Can Expand Ciphertext")
    {
        auto json = fixture->compactCiphertext->toJson();
        Log::debug(json);

        auto expandedCiphertext = expandCompactCiphertextBallot(
          *fixture->compactCiphertext, *fixture->manifest, *fixture->context);

        // Assert
        CHECK(expandedCiphertext->getNonce()->toHex() ==
              fixture->compactCiphertext->getNonce()->toHex());
        CHECK(expandedCiphertext->isValidEncryption(
                *fixture->context->getManifestHash(), *fixture->keypair->getPublicKey(),
                *fixture->context->getCryptoExtendedBaseHash()) == true);
    }
}
