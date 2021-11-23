#include "../../src/electionguard/log.hpp"
#include "generators/ballot.hpp"
#include "generators/manifest.hpp"

#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/ballot_compact.hpp>

using namespace electionguard;
using namespace electionguard::tools::generators;
using namespace std;

TEST_CASE("Can serialize CompactPlaintextBallot")
{
    // Arrange
    auto manifest = ManifestGenerator::getJeffersonCountyManifest_Minimal();
    auto internal = make_unique<InternalManifest>(*manifest);
    auto plaintext = BallotGenerator::getFakeBallot(*internal);
    auto compact = CompactPlaintextBallot::make(*plaintext);
    auto msgpack = compact->toMsgPack();
    auto json = compact->toJson();
    Log::debug(json);

    // Act
    auto fromMsgPack = CompactPlaintextBallot::fromMsgPack(msgpack);

    // Assert
    CHECK(plaintext->getObjectId() == fromMsgPack->getObjectId());
}
