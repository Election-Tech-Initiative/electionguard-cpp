#include "../../src/electionguard/log.hpp"
#include "mocks/election.hpp"
#include "mocks/manifest.hpp"

#include <doctest/doctest.h>
#include <electionguard/election.hpp>
#include <electionguard/elgamal.hpp>
#include <electionguard/manifest.hpp>

using namespace electionguard;
using namespace electionguard::test::mocks;
using namespace std;

TEST_CASE("Simple Election Is Valid")
{
    auto subject = ManifestGenerator::getJeffersonCountryManifest_multipleBallotStyle_fromFile();

    CHECK(subject->getElectionScopeId() == "jefferson-county-primary");
    CHECK(subject->isValid() == true);
}

TEST_CASE("Can serialize Jefferson County Minimal Manifest")
{
    // Arrange
    auto subject = ManifestGenerator::getJeffersonCountyManifest_Minimal();
    auto json = subject->toJson();

    Log::debug(json);

    // Act
    auto result = Manifest::fromJson(json);

    // Assert
    CHECK(subject->getElectionScopeId().compare(result->getElectionScopeId()) == 0);
    CHECK(subject->crypto_hash()->toHex() == result->crypto_hash()->toHex());
    CHECK(result->isValid() == true);
}

TEST_CASE("Can deserialize Jefferson County Manifest from file")
{
    // Arrange
    auto subject = ManifestGenerator::getJeffersonCountryManifest_multipleBallotStyle_fromFile();
    auto json = subject->toJson();

    Log::debug(json);

    // Act
    auto result = Manifest::fromJson(json);

    // Assert
    CHECK(subject->getElectionScopeId().compare(result->getElectionScopeId()) == 0);
    CHECK(subject->crypto_hash()->toHex() == result->crypto_hash()->toHex());
    CHECK(result->isValid() == true);
}

TEST_CASE("Can construct InternalManifest from Manifest")
{
    // Arrange
    auto data = ManifestGenerator::getJeffersonCountyManifest_Minimal();

    // Act
    auto result = make_unique<InternalManifest>(*data);

    // Assert
    CHECK(data->crypto_hash()->toHex() == result->getManifestHash()->toHex());
    CHECK(data->getContests().size() == result->getContests().size());
}

TEST_CASE("Can serialize InternalManifest")
{
    // Arrange
    auto manifest = ManifestGenerator::getJeffersonCountyManifest_Minimal();
    auto internal = make_unique<InternalManifest>(*manifest);
    auto json = internal->toJson();
    auto bson = internal->toBson();

    Log::debug(json);

    // Act
    auto fromJson = InternalManifest::fromJson(json);
    //auto fromBson = InternalManifest::fromBson(bson);

    // Assert
    CHECK(internal->getManifestHash()->toHex() == fromJson->getManifestHash()->toHex());
}
