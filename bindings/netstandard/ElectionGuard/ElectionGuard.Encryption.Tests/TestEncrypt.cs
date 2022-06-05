using System;
using ElectionGuard.Encryption.Utils;
using NUnit.Framework;

namespace ElectionGuard.Encrypt.Tests
{
    [TestFixture]
    public class TestEncrypt
    {
        [Test]
        public void Test_Device_Serialization_Succeeds()
        {

            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Brazil");

            var json = device.ToJson();

            var deviceCreatedFromJson = new EncryptionDevice(deviceToJson);

            Assert.True(deviceToJson.Contains("Brazil"));
            Assert.True(deviceCreatedFromJson.ToJson().Contains("Brazil"));

        }

        [Test]
        public void Test_Encrypt_Ballot_Simple_Succeeds()
        {
            // Configure the election context
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            var manifest = ManifestGenerator.GetManifestFromFile();
            var internalManifest = new InternalManifest(manifest);
            var context = new CiphertextElectionContext(
                1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, internalManifest.ManifestHash);
            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            var mediator = new EncryptionMediator(internalManifest, context, device);

            var ballot = BallotGenerator.GetFakeBallot(internalManifest);

            // Act
            var ciphertext = mediator.Encrypt(ballot);

            // Assert

            // a property
            Assert.That(ciphertext.ObjectId == ballot.ObjectId);

            Assert.That(ciphertext.BallotCode.ToHex() != null);

            // json serialization
            var json = ciphertext.ToJson();
            var fromJson = new CiphertextBallot(json);
            Assert.That(ciphertext.ObjectId == fromJson.ObjectId);

            // binary serialization
            var bson = ciphertext.ToBson();
            var fromBson = new CiphertextBallot(bson, BinarySerializationEncoding.BSON);
            Assert.That(ciphertext.ObjectId == fromBson.ObjectId);

            var msgPack = ciphertext.ToMsgPack();
            var fromMsgPack = new CiphertextBallot(msgPack, BinarySerializationEncoding.MsgPack);
            Assert.That(ciphertext.ObjectId == fromMsgPack.ObjectId);

            // submitted ballot
            var submitted = new SubmittedBallot(ciphertext, BallotBoxState.Cast);
            Assert.That(ciphertext.ObjectId == submitted.ObjectId);
        }

        [Test]
        public void Test_Encrypt_Ballot_Undervote_Succeeds()
        {
            // Configure the election context
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            var manifest = ManifestGenerator.GetManifestFromFile();
            var internalManifest = new InternalManifest(manifest);
            var context = new CiphertextElectionContext(
                1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, internalManifest.ManifestHash);
            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            var mediator = new EncryptionMediator(internalManifest, context, device);

            // Act
            var ballot = BallotGenerator.GetFakeBallot(internalManifest, 1);
            var ciphertext = mediator.Encrypt(ballot);

            // Assert

            // a property
            Assert.That(ciphertext.IsValidEncryption(context.ManifestHash, keypair.PublicKey, context.CryptoExtendedBaseHash));
        }

        [Test]
        public void Test_Encrypt_Ballot_Overvote_Succeeds()
        {
            // Configure the election context
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            var manifest = ManifestGenerator.GetManifestFromFile();
            var internalManifest = new InternalManifest(manifest);
            var context = new CiphertextElectionContext(
                1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, internalManifest.ManifestHash);
            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            var mediator = new EncryptionMediator(internalManifest, context, device);

            // Act
            var ballot = BallotGenerator.GetFakeBallot(internalManifest, 2);
            var ciphertext = mediator.Encrypt(ballot);

            Assert.That(ciphertext.IsValidEncryption(context.ManifestHash, keypair.PublicKey, context.CryptoExtendedBaseHash));
        }

        [Test]
        public void Test_Compact_Encrypt_Ballot_Simple_Succeeds()
        {
            // Arrange
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            var manifest = ManifestGenerator.GetManifestFromFile();
            var internalManifest = new InternalManifest(manifest);
            var context = new CiphertextElectionContext(
                1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, internalManifest.ManifestHash);
            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            var mediator = new EncryptionMediator(internalManifest, context, device);

            var ballot = BallotGenerator.GetFakeBallot(internalManifest);

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

        [Test]
        public void Test_Constant_Serialization_Succeeds()
        {
            var constants = Constants.ToJson();
            Assert.That(constants.Contains(Constants.P.ToHex()));
            Assert.That(constants.Contains(Constants.Q.ToHex()));
            Assert.That(constants.Contains(Constants.R.ToHex()));
            Assert.That(constants.Contains(Constants.G.ToHex()));
        }

    }
}
