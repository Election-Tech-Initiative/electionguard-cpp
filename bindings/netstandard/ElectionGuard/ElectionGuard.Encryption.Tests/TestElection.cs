using System;
using System.Diagnostics;
using NUnit.Framework;

namespace ElectionGuard.Encrypt.Tests
{
    [TestFixture]
    public class TestElection
    {

        [Test]
        public void Test_Can_Create_Context_Config()
        {
            // Act
            var result = new ContextConfiguration(true, 350);

            // Assert
            Assert.AreEqual(350, result.MaxBallots);
            Assert.IsTrue(result.AllowedOverVotes);
        }

        [Test]
        public void Test_Can_Create_Party()
        {
            var candidateName = new Language("father time", "en");
            var candidate = new Candidate(
                "2022-holidays",
                new InternationalizedText(new[] { candidateName }),
                "new-years-id",
                null,
                false);


            var partyName = new Language("new years", "en");
            var party = new ElectionGuard.Party(
                "new-years-id",
                new InternationalizedText(new[] { partyName }),
                "ny",
                null,
                null);

            // Assert
            Assert.IsNotNull(candidate);
            Assert.IsNotNull(party);
        }






        [Test]
        public void Test_Can_Deserialize_Ciphertext_Election_Context()
        {
            // Arrange
            const string data = "{\"crypto_base_hash\":\"B8CF\",\"crypto_extended_base_hash\":\"7310\",\"commitment_hash\":\"02\",\"manifest_hash\":\"02\",\"elgamal_public_key\":\"F258\",\"number_of_guardians\":1,\"quorum\":1}";

            // Act
            var result = new CiphertextElectionContext(data);

            // Assert
            Assert.AreEqual("02", result.CommitmentHash.ToHex());
            Assert.AreEqual("02", result.ManifestHash.ToHex());
            Assert.AreEqual(1000000, result.Configuration.MaxBallots);
            Assert.IsTrue(result.Configuration.AllowedOverVotes);
        }

        [Test]
        public void Test_Can_Deserialize_Ciphertext_Election_Context_and_Config()
        {
            // Arrange
            const string data = "{\"configuration\":{ \"allow_overvotes\": false, \"max_votes\": 50000 }, \"crypto_base_hash\":\"B8CF\",\"crypto_extended_base_hash\":\"7310\",\"commitment_hash\":\"02\",\"manifest_hash\":\"02\",\"elgamal_public_key\":\"F258\",\"number_of_guardians\":1,\"quorum\":1}";

            // Act
            var result = new CiphertextElectionContext(data);

            // Assert
            Assert.AreEqual(50000, result.Configuration.MaxBallots);
            Assert.IsFalse(result.Configuration.AllowedOverVotes);
        }

        [Test]
        public void Test_Can_Create_Ciphertext_Election_Context()
        {
            var config = new ContextConfiguration(false, 4200);

            // Act
            var result = new CiphertextElectionContext(1, 1,
                Constants.ONE_MOD_P,
                new ElementModQ("02"), new ElementModQ("02"), config);

            // Assert
            Assert.AreEqual("02", result.CommitmentHash.ToHex());
            Assert.AreEqual("02", result.ManifestHash.ToHex());
            Assert.IsFalse(result.Configuration.AllowedOverVotes);
            Assert.AreEqual(4200, result.Configuration.MaxBallots);
        }


        [Test]
        public void Test_Can_Assign_Extended_Data_To_Ciphertext_Election_Context()
        {
            // Arrange
            var extendedData = new LinkedList();
            extendedData.Append("ballot_base_uri", "http://something.vote/");

            // Act
            var context = new CiphertextElectionContext(
                3, 2,
                Constants.ONE_MOD_P, Constants.ONE_MOD_Q, Constants.ONE_MOD_Q,
                extendedData);

            var cachedValue = context.ExtendedData.GetValueAt(0);

            // Assert
            Assert.AreEqual("http://something.vote/", cachedValue);
        }
    }
}
