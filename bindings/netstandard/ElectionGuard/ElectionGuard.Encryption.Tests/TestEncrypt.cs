using System;
using ElectionGuard.Encryption.Tests.Utils;
using NUnit.Framework;

namespace ElectionGuard.Encrypt.Tests
{
    [TestFixture]
    public class TestEncrypt
    {
        readonly string manifest_json = "{\"ballot_styles\":[{\"geopolitical_unit_ids\":[\"some-geopoltical-unit-id\"],\"object_id\":\"some-ballot-style-id\"}],\"candidates\":[{\"object_id\":\"some-candidate-id-1\"},{\"object_id\":\"some-candidate-id-2\"},{\"object_id\":\"some-candidate-id-3\"}],\"contests\":[{\"ballot_selections\":[{\"candidate_id\":\"some-candidate-id-1\",\"object_id\":\"some-object-id-affirmative\",\"sequence_order\":0},{\"candidate_id\":\"some-candidate-id-2\",\"object_id\":\"some-object-id-negative\",\"sequence_order\":1}],\"electoral_district_id\":\"some-geopoltical-unit-id\",\"name\":\"some-referendum-contest-name\",\"number_elected\":1,\"object_id\":\"some-referendum-contest-object-id\",\"sequence_order\":0,\"vote_variation\":\"one_of_m\"},{\"ballot_selections\":[{\"candidate_id\":\"some-candidate-id-1\",\"object_id\":\"some-object-id-candidate-1\",\"sequence_order\":0},{\"candidate_id\":\"some-candidate-id-2\",\"object_id\":\"some-object-id-candidate-2\",\"sequence_order\":1},{\"candidate_id\":\"some-candidate-id-3\",\"object_id\":\"some-object-id-candidate-3\",\"sequence_order\":2}],\"electoral_district_id\":\"some-geopoltical-unit-id\",\"name\":\"some-candidate-contest-name\",\"number_elected\":2,\"object_id\":\"some-candidate-contest-object-id\",\"sequence_order\":1,\"vote_variation\":\"one_of_m\"}],\"election_scope_id\":\"some-scope-id\",\"end_date\":\"2021-02-04T13:30:10Z\",\"geopolitical_units\":[{\"name\":\"some-gp-unit-name\",\"object_id\":\"some-geopoltical-unit-id\",\"type\":\"unknown\"}],\"parties\":[{\"object_id\":\"some-party-id-1\"},{\"object_id\":\"some-party-id-2\"}],\"start_date\":\"2021-02-04T13:30:10Z\",\"type\":\"unknown\"}";
        readonly string plaintext_json = "{\"object_id\":\"some-external-id-string-123\",\"style_id\":\"some-ballot-style-id\",\"contests\":[{\"object_id\":\"some-referendum-contest-object-id\",\"ballot_selections\":[{\"object_id\":\"some-candidate-id-1\",\"vote\":1}]}]}";

        [Test]
        public void Test_Encrypt_Ballot_Simple_Succeeds()
        {
            // Configure the election context
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            var manifest = new Manifest(manifest_json);
            var internalManifest = new InternalManifest(manifest);
            var context = new CiphertextElectionContext(
                1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, internalManifest.ManifestHash);
            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            var mediator = new EncryptionMediator(internalManifest, context, device);

            var ballot = new PlaintextBallot(plaintext_json);

            // Act
            var ciphertext = mediator.Encrypt(ballot);

            // Assert

            // a property
            Assert.That(ciphertext.ObjectId == ballot.ObjectId);

            // json serialization
            var json = ciphertext.ToJson();
            var fromJson = new CiphertextBallot(json);
            Assert.That(ciphertext.ObjectId == fromJson.ObjectId);

            // TODO:
            // binary serialization
            // var bson = ciphertext.Bson;
            // var fromBson = CiphertextBallot.FromBson(bson);
            // Assert.That(ciphertext.ObjectId == fromBson.ObjectId);

            // TODO: submitted ballot
            // var submitted = SubmittedBallot.From(ciphertext, BallotBoxState.cast);
            // Assert.That(ciphertext.ObjectId == submitted.ObjectId);
        }

        [Test]
        public void Test_Compact_Encrypt_Ballot_Simple_Succeeds()
        {
            // Arrange
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            var manifest = new Manifest(manifest_json);

            // TODO: load from file
            //var manifest = ManifestGenerator.GetJeffersonCountyManifest();
            var internalManifest = new InternalManifest(manifest);
            var context = new CiphertextElectionContext(
                1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, internalManifest.ManifestHash);
            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            var mediator = new EncryptionMediator(internalManifest, context, device);

            var ballot = new PlaintextBallot(plaintext_json);

            // Act
            var compact = mediator.CompactEncrypt(ballot);

            // Assert
            Assert.That(compact.ObjectId == ballot.ObjectId);

            // Act
            var msgpack = compact.ToMsgPack();
            var fromMsgpack = new CompactCiphertextBallot(msgpack);

            // Assert
            Assert.That(compact.ObjectId == fromMsgpack.ObjectId);

        }
    }
}
