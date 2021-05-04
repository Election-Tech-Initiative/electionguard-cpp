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

TEST_CASE("Can serialize Manifest")
{
    // Arrange
    auto subject = ManifestGenerator::getFakeElection();
    auto json = subject->toJson();

    Log::debug(json);

    // Act
    auto result = Manifest::fromJson(json);

    // Assert
    CHECK(subject->getElectionScopeId().compare(result->getElectionScopeId()) == 0);
}

TEST_CASE("Can deserialize Manifest")
{
    const char *manifest_json =
      "{\"ballot_styles\":[{\"geopolitical_unit_ids\":[\"some-geopoltical-unit-id\"],\"object_id\":"
      "\"some-ballot-style-id\"}],\"candidates\":[{\"object_id\":\"some-candidate-id-1\"},{"
      "\"object_id\":\"some-candidate-id-2\"},{\"object_id\":\"some-candidate-id-3\"}],"
      "\"contests\":[{\"ballot_selections\":[{\"candidate_id\":\"some-candidate-id-1\",\"object_"
      "id\":\"some-object-id-affirmative\",\"sequence_order\":0},{\"candidate_id\":\"some-"
      "candidate-id-2\",\"object_id\":\"some-object-id-negative\",\"sequence_order\":1}],"
      "\"electoral_district_id\":\"some-geopoltical-unit-id\",\"name\":\"some-referendum-contest-"
      "name\",\"number_elected\":1,\"object_id\":\"some-referendum-contest-object-id\",\"sequence_"
      "order\":0,\"vote_variation\":\"one_of_m\"},{\"ballot_selections\":[{\"candidate_id\":\"some-"
      "candidate-id-1\",\"object_id\":\"some-object-id-candidate-1\",\"sequence_order\":0},{"
      "\"candidate_id\":\"some-candidate-id-2\",\"object_id\":\"some-object-id-candidate-2\","
      "\"sequence_order\":1},{\"candidate_id\":\"some-candidate-id-3\",\"object_id\":\"some-object-"
      "id-candidate-3\",\"sequence_order\":2}],\"electoral_district_id\":\"some-geopoltical-unit-"
      "id\",\"name\":\"some-candidate-contest-name\",\"number_elected\":2,\"object_id\":\"some-"
      "candidate-contest-object-id\",\"sequence_order\":1,\"vote_variation\":\"one_of_m\"}],"
      "\"election_scope_id\":\"some-scope-id\",\"end_date\":\"2021-02-04T13:30:10Z\","
      "\"geopolitical_units\":[{\"name\":\"some-gp-unit-name\",\"object_id\":\"some-geopoltical-"
      "unit-id\",\"type\":\"unknown\"}],\"parties\":[{\"object_id\":\"some-party-id-1\"},{\"object_"
      "id\":\"some-party-id-2\"}],\"start_date\":\"2021-02-04T13:30:10Z\",\"type\":\"unknown\"}";

    // Arrange

    // Act
    auto result = Manifest::fromJson(manifest_json);

    // Assert
    CHECK(result->getElectionScopeId().compare("some-scope-id") == 0);
    CHECK(result->isValid() == true);
}

TEST_CASE("Can construct InternalManifest from Manifest")
{
    // Arrange
    auto data = ManifestGenerator::getFakeElection();

    // Act
    auto result = make_unique<InternalManifest>(*data);

    // Assert
    CHECK(data->crypto_hash()->toHex() == result->getManifestHash()->toHex());
    CHECK(data->getContests().size() == result->getContests().size());
}

TEST_CASE("Can serialize InternalManifest")
{
    // Arrange
    auto manifest = ManifestGenerator::getFakeManifest(TWO_MOD_Q());
    auto json = manifest->toJson();
    auto bson = manifest->toBson();

    Log::debug(json);

    // Act
    auto fromJson = InternalManifest::fromJson(json);
    //auto fromBson = InternalManifest::fromBson(bson);

    // Assert
    CHECK(manifest->getManifestHash()->toHex() == fromJson->getManifestHash()->toHex());
}

TEST_CASE("Can serialize CiphertextElectionContext")
{
    // Arrange
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto manifest = ManifestGenerator::getFakeManifest(TWO_MOD_Q());
    auto context = ElectionGenerator::getFakeContext(*manifest, *keypair->getPublicKey());
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

TEST_CASE("Can Deserialize CiphertextElectionContext")
{
    // Arrange
    // Note this string includes a couple of bad hex strings
    // with leading zeroes and are therefore out of range
    auto json = string(
      "{\"number_of_guardians\":5,\"quorum\":3,\"elgamal_public_key\":"
      "\"09F8C96DA142602B8E1F731779AE1ECFD7467F61CBDE20E3A93DA2D8D8DEFCF3B8230404E03887F2E34BAE3CE0"
      "C73DD8A80D18FCB4C10884B5D1368319868D583DEFD16D237755BF16D4D406A2F71BC685EB9E08CD7A8E049F3EC3"
      "86D2E094A9A1E9C2E1494CE3DF488E5FE8995ABBDBEA490C1AFD2EB9E5AAD59812C16CB921D7A7FF663B40AE1C1B"
      "A9D181E4AE5CF518CA5314D7859729D94AC457F01CF40F7DD7D3D22A515C1B5C28A4743CFC072D8222D8DB4862CC"
      "8DBBDAC02357F243AF5F31C3790BD367C04E77105C2ACEE984D69E5D11BE02CB19271D80B1089EE33E977934A3FA"
      "5A33C6D278CEE9607B38C084BE05284BAA344BB1B3D742C0E279D1085AA01F6CAA0D0C807AB85A7AB7EB4F042C60"
      "D884748B494E2B9651543AFD5E619C5C458DB87357A2497A03DEDED1504FF849F9FB6115460D51E757E6FBF1B2E0"
      "DD2D55EBA8C58E4A6D328462DB0FB4C8A0B174933E62FCF38E94CAAFA369A52565B7C8B9AFA627EAE4A928C0B5CA"
      "3EE24EE4AB4C4EC2776961A2C84EC95C174C84CBC6E9F0E7A24887AE859735DE68D2266420188626FD01ABA55DAF"
      "7E57306BABDC3FD7F00DBA16B8CC7893E448CEED67B3FF72B0682753FDF1DF066B798422A07B4B2FE9EDF0AC5D5E"
      "FB29F530152DA4ADA4154218B93E73519B38E0F04D811C4050BBC28A575A4EEFAAF970CAEE3F1EE4575C929E53C6"
      "ABF7D2870F485DA\",\"commitment_hash\":"
      "\"7749DF5EC06636F64780798E90D7EBD6C6E9872695629E0B68F25E4E11C068F0\",\"manifest_hash\":"
      "\"7749DF5EC06636F64780798E90D7EBD6C6E9872695629E0B68F25E4E11C068F0\",\"crypto_base_hash\":"
      "\"0FA1F1BFE97937F099FD39102D1FB452500F13AC12EFDE4E5FFD0D0D1C3896056\",\"crypto_extended_"
      "base_hash\":\"0E92481188ACDBC604EC7B8563FBF4024063E874797415CC1E72C7BF5F0114BAD\"}");

    auto manifestHash =
      ElementModQ::fromHex("7749DF5EC06636F64780798E90D7EBD6C6E9872695629E0B68F25E4E11C068F0");

    // Act
    auto fromJson = CiphertextElectionContext::fromJson(json);

    // Assert
    // TODO: validate against manifest->getManifestHash()
    CHECK(fromJson->getManifestHash()->toHex() == manifestHash->toHex());
}
