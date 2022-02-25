#include "../../src/electionguard/log.hpp"
#include "generators/election.hpp"
#include "generators/manifest.hpp"

#include <doctest/doctest.h>
#include <electionguard/election.hpp>
#include <electionguard/elgamal.hpp>
#include <electionguard/manifest.hpp>

using namespace electionguard;
using namespace electionguard::tools::generators;
using namespace std;

TEST_CASE("Sample Manifest Is Valid")
{
    auto subject = ManifestGenerator::getManifestFromFile(TEST_SPEC_VERSION, TEST_USE_SAMPLE);

    CHECK(subject->isValid() == true);
}

TEST_CASE("Can serialize Sample Manifest")
{
    // Arrange
    auto subject = ManifestGenerator::getManifestFromFile(TEST_SPEC_VERSION, TEST_USE_SAMPLE);
    auto json = subject->toJson();

    // Log::debug(json);

    // Act
    auto result = Manifest::fromJson(json);

    // Assert
    CHECK(subject->getElectionScopeId().compare(result->getElectionScopeId()) == 0);
    CHECK(subject->crypto_hash()->toHex() == result->crypto_hash()->toHex());
    CHECK(result->isValid() == true);
}

TEST_CASE("Can construct InternalManifest from Sample Manifest")
{
    // Arrange
    auto data = ManifestGenerator::getManifestFromFile(TEST_SPEC_VERSION, TEST_USE_SAMPLE);

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

    // Log::debug(json);

    // Act
    auto fromJson = InternalManifest::fromJson(json);
    //auto fromBson = InternalManifest::fromBson(bson);

    // Assert
    CHECK(internal->getManifestHash()->toHex() == fromJson->getManifestHash()->toHex());
}
