#include "../../src/electionguard/log.hpp"
#include "generators/ballot.hpp"
#include "generators/election.hpp"
#include "generators/manifest.hpp"
#include "utils/constants.hpp"

#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/ballot_compact.hpp>
#include <electionguard/encrypt.hpp>

using namespace electionguard;
using namespace electionguard::tools::generators;
using namespace std;

struct TestEncryptFixture {

    unique_ptr<ElementModQ> secret;
    unique_ptr<ElGamalKeyPair> keypair;
    unique_ptr<Manifest> manifest;
    unique_ptr<InternalManifest> internal;
    unique_ptr<CiphertextElectionContext> context;
    unique_ptr<EncryptionDevice> device;
    unique_ptr<EncryptionMediator> mediator;
    unique_ptr<PlaintextBallot> plaintext;
    unique_ptr<CompactCiphertextBallot> compactCiphertext;

    TestEncryptFixture()
        : secret(ElementModQ::fromHex(a_fixed_secret)),
          keypair(ElGamalKeyPair::fromSecret(*secret)),
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

TEST_CASE("Can encrypt compact ballots")
{
    auto fixture = make_unique<TestEncryptFixture>();
    // Assert
    CHECK(fixture->compactCiphertext->getObjectId() == fixture->plaintext->getObjectId());
}

TEST_CASE("Can Serialize CompactCiphertextBallot")
{
    auto fixture = make_unique<TestEncryptFixture>();
    auto json = fixture->compactCiphertext->toJson();
    Log::debug(json);
    // Assert
    auto msgpack = fixture->compactCiphertext->toMsgPack();
    auto fromMsgpack = CompactCiphertextBallot::fromMsgPack(msgpack);
    CHECK(fromMsgpack->getNonce()->toHex() == fixture->compactCiphertext->getNonce()->toHex());
}

TEST_CASE("Can Expand Plaintext")
{
    auto fixture = make_unique<TestEncryptFixture>();
    auto expandedPlaintext =
      expandCompactPlaintextBallot(*fixture->compactCiphertext->getPlaintext(), *fixture->manifest);

    // Assert
    CHECK(expandedPlaintext->getObjectId() == fixture->compactCiphertext->getObjectId());

    auto msgpack = expandedPlaintext->toMsgPack();
    auto fromMsgPack = PlaintextBallot::fromMsgPack(msgpack);
    CHECK(fromMsgPack->getObjectId() == expandedPlaintext->getObjectId());
}

TEST_CASE("Can Expand Ciphertext")
{
    auto fixture = make_unique<TestEncryptFixture>();
    auto json = fixture->compactCiphertext->toJson();
    Log::debug(json);

    auto expandedCiphertext = expandCompactCiphertextBallot(*fixture->compactCiphertext,
                                                            *fixture->manifest, *fixture->context);

    // Assert
    CHECK(expandedCiphertext->getNonce()->toHex() ==
          fixture->compactCiphertext->getNonce()->toHex());
    CHECK(expandedCiphertext->isValidEncryption(
            *fixture->context->getManifestHash(), *fixture->keypair->getPublicKey(),
            *fixture->context->getCryptoExtendedBaseHash()) == true);
}
