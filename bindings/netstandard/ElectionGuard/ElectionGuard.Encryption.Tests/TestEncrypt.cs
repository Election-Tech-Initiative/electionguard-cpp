using System;
using ElectionGuard.Encryption.Utils;
using NUnit.Framework;

namespace ElectionGuard.Encryption.Tests
{
    [TestFixture]
    public class TestEncrypt
    {
        [Test]
        public void Test_Device_Serialization_Succeeds()
        {

            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Brazil");

            var json = device.ToJson();

            var deviceCreatedFromJson = new EncryptionDevice(json);

            Assert.True(json.Contains("Brazil"));
            Assert.True(deviceCreatedFromJson.ToJson().Contains("Brazil"));

        }
        [Test]
        public void Test_Device_Serialization_From_JSON_Succeeds()
        {
            string deviceJson = "{ \"device_id\": 91755434160, \"session_id\": 12345, \"launch_code\": 45678, \"location\": \"polling-place\" }";
            var device = new EncryptionDevice(deviceJson);

            var json = device.ToJson();

            Assert.True(json.Contains("polling-place"));
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
            var ballot = BallotGenerator.GetFakeBallot(internalManifest);
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
        public void Test_EncryptAndDecryptOfBallot_WithMultipleVotesAllowed()
        {
            // Arrange
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            var manifest = ManifestGenerator.GetManifestFromFile();
            var internalManifest = new InternalManifest(manifest);
            var context = new CiphertextElectionContext(
                1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, internalManifest.ManifestHash);
            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            var mediator = new EncryptionMediator(internalManifest, context, device);
            const string styleId = "congress-district-7-arlington-pismo-beach";
            var ballot = BallotGenerator.GetFakeBallotWithContest(internalManifest, "pismo-beach-school-board-contest", 3, styleId);

            // Act
            var ciphertext = mediator.Encrypt(ballot);

            // Assert
            Assert.AreEqual(5, ciphertext.ContestsSize);
            var pismoBeach = FindContestOrDefault(ciphertext, i => i.ObjectId == "pismo-beach-school-board-contest");
            Assert.IsNotNull(pismoBeach);
            Assert.AreEqual(1L, DecryptSelection(pismoBeach, keypair, 0));
            Assert.AreEqual(1L, DecryptSelection(pismoBeach, keypair, 1));
            Assert.AreEqual(1L, DecryptSelection(pismoBeach, keypair, 2));
            Assert.AreEqual(0L, DecryptSelection(pismoBeach, keypair, 3));
            Assert.AreEqual(0L, DecryptSelection(pismoBeach, keypair, 4));
        }

        private static ulong? DecryptSelection(CiphertextBallotContest pismoBeach, ElGamalKeyPair keypair, int index)
        {
            var selection = pismoBeach.GetSelectionAt((ulong)index);
            return selection.Ciphertext.Decrypt(keypair.SecretKey);
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

        [Test]
        public void Test_EncryptMediator_Hashes_Match()
        {
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            var manifest = ManifestGenerator.GetManifestFromFile();
            var internalManifest = new InternalManifest(manifest);
            var context = new CiphertextElectionContext(
                1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, internalManifest.ManifestHash);   // make a context with the correct manifesthash
            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            try
            {
                var mediator = new EncryptionMediator(internalManifest, context, device);
                Assert.IsNotNull(mediator);     // should not be null if it gets created
            }
            catch (Exception)
            {
                // if there is an exception then the manifest hash would not be equal
                Assert.AreNotEqual(context.ManifestHash.ToHex(), internalManifest.ManifestHash.ToHex());
            }
        }

        [Test]
        public void Test_EncryptMediator_Hashes_Dont_Match()
        {
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            var manifest = ManifestGenerator.GetManifestFromFile();
            var internalManifest = new InternalManifest(manifest);
            var context = new CiphertextElectionContext(
                1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, Constants.ONE_MOD_Q);     // make a context with a different manifesthash
            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            try
            {
                var mediator = new EncryptionMediator(internalManifest, context, device);
                Assert.IsNull(mediator);    // should not be created, so null at best
            }
            catch (Exception)
            {
                // if there is an exception then the manifest hash would not be equal
                Assert.AreNotEqual(context.ManifestHash.ToHex(), internalManifest.ManifestHash.ToHex());
            }
        }


        private static CiphertextBallotContest FindContestOrDefault(CiphertextBallot ballot, Func<CiphertextBallotContest, bool> func)
        {
            var contestSize = (int)ballot.ContestsSize;
            for (int i = 0; i < contestSize; i++)
            {
                var contest = ballot.GetContestAt((ulong)i);
                if (func(contest))
                {
                    return contest;
                }
            }

            return null;
        }
    }
}
