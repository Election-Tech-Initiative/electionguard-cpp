#include "../../src/electionguard/log.hpp"
#include "generators/ballot.hpp"
#include "generators/manifest.hpp"

#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>

using namespace electionguard;
using namespace electionguard::tools::generators;
using namespace std;

TEST_CASE("Plaintext Simple Ballot Is Valid")
{
    auto subject = BallotGenerator::getSimpleBallotFromFile();

    CHECK(subject->getObjectId() == "some-external-id-string-123");
}

TEST_CASE("Plaintext Ballot Selection Is Valid")
{
    // Arrange
    const auto *objectId = "some-object-id";

    // Act
    auto subject = make_unique<PlaintextBallotSelection>(objectId, 1UL);

    // Assert
    CHECK(subject->isValid(objectId) == true);
}

TEST_CASE("Plaintext Ballot Selection Is InValid")
{
    SUBCASE("Different object id's fail validity check")
    {
        // Arrange
        const auto *objectId = "some-object-id";

        // Act
        auto subject = make_unique<PlaintextBallotSelection>(objectId, 1UL);

        // Assert
        CHECK(subject->isValid("some-other-object-id") == false);
    }

    SUBCASE("An out of range selection value fails validity check")
    {
        // Arrange
        const auto *objectId = "some-object-id";

        // Act
        auto subject = make_unique<PlaintextBallotSelection>(objectId, 2UL);

        // Assert
        CHECK(subject->isValid(objectId) == false);
    }
}

TEST_CASE("Can serialize PlaintextBallot")
{
    // Arrange
    auto manifest = ManifestGenerator::getJeffersonCountyManifest_Minimal();
    auto internal = make_unique<InternalManifest>(*manifest);
    auto plaintext = BallotGenerator::getFakeBallot(*internal);
    auto json = plaintext->toJson();
    auto bson = plaintext->toBson();
    auto msgPack = plaintext->toMsgPack();

    // Act
    auto fromJson = PlaintextBallot::fromJson(json);
    auto fromBson = PlaintextBallot::fromBson(bson);
    auto fromMsgPack = PlaintextBallot::fromMsgPack(msgPack);

    // Assert
    CHECK(plaintext->getObjectId() == fromJson->getObjectId());
    CHECK(plaintext->getObjectId() == fromBson->getObjectId());
    CHECK(plaintext->getObjectId() == fromMsgPack->getObjectId());
}
