#include "../../src/electionguard/log.hpp"
#include "generators/election.hpp"
#include "generators/manifest.hpp"

#include <doctest/doctest.h>
#include <electionguard/election.hpp>
#include <electionguard/elgamal.hpp>
#include <electionguard/manifest.hpp>
#include <unordered_map>

using namespace electionguard;
using namespace electionguard::tools::generators;
using namespace std;

TEST_CASE("Can serialize CiphertextElectionContext")
{
    // Arrange
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto manifest = ManifestGenerator::getJeffersonCountyManifest_Minimal();
    auto internal = make_unique<InternalManifest>(*manifest);
    auto context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());
    auto json = context->toJson();
    auto bson = context->toBson();

    Log::debug(json);

    // Act
    auto fromJson = CiphertextElectionContext::fromJson(json);
    auto fromBson = CiphertextElectionContext::fromBson(bson);

    // Assert
    // TODO: validate against manifest->getManifestHash()
    CHECK(fromJson->getManifestHash()->toHex() == context->getManifestHash()->toHex());
    CHECK(fromBson->getManifestHash()->toHex() == context->getManifestHash()->toHex());
}

TEST_CASE("Assign ExtraData to CiphertextElectionContext")
{
    // Arrange
    auto key = "ballot_base_uri";
    auto value = "http://something.vote/";
    unordered_map<string, string> extendedData({{key, value}});

    // Act
    auto context = CiphertextElectionContext::make(
      3UL, 2UL, TWO_MOD_P().clone(), TWO_MOD_Q().clone(), TWO_MOD_Q().clone(), extendedData);

    auto cached = context->getExtendedData();
    auto resolved = cached.find(key);

    // Assert
    if (resolved == cached.end()) {
        FAIL(resolved);
    } else {
        CHECK(resolved->second == value);
    }
}

TEST_CASE("Assign ExtraData to CiphertextElectionContextand Serialize")
{
    // Arrange
    auto key = "ballot_base_uri";
    auto value = "http://something.vote/";
    unordered_map<string, string> extendedData({{key, value}});

    // Act
    auto context = CiphertextElectionContext::make(
      3UL, 2UL, TWO_MOD_P().clone(), TWO_MOD_Q().clone(), TWO_MOD_Q().clone(), extendedData);

    auto json = context->toJson();
    auto bson = context->toBson();

    Log::debug(json);

    // Act
    auto fromJson = CiphertextElectionContext::fromJson(json);
    auto fromBson = CiphertextElectionContext::fromBson(bson);

    // Assert
    // TODO: validate against manifest->getManifestHash()
    CHECK(fromJson->getExtendedData().at("ballot_base_uri") ==
          context->getExtendedData().at("ballot_base_uri"));
    CHECK(fromBson->getExtendedData().at("ballot_base_uri") ==
          context->getExtendedData().at("ballot_base_uri"));
}
