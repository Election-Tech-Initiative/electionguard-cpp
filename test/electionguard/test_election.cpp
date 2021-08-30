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
