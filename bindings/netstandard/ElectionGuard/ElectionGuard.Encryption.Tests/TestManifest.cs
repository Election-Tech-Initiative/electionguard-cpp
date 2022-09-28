using System;
using System.Diagnostics;
using NUnit.Framework;
using System.Collections.Generic;

using ElectionGuard.Encryption.Utils;

namespace ElectionGuard.Encrypt.Tests
{
    [TestFixture]
    public class TestManifest
    {

        [Test]
        public void Test_Votes_Allowed_On_Create_Contest()
        {
            List<SelectionDescription> selections = new List<SelectionDescription>();

            var contestThreeVotes = new ContestDescription("contest-id", "district-id", 1, VoteVariationType.n_of_m, 3,
                                      "test election", selections.ToArray());

            // Assert
            Assert.AreEqual(3, contestThreeVotes.VotesAllowed);
        }


        [Test]
        public void Test_Can_Serialize_Sample_manifest()
        {
            var subject = ManifestGenerator.GetManifestFromFile();
            Assert.That(subject.IsValid);
        }

        [Test]
        public void Test_Can_Construct_Internationalized_Text()
        {
            // Arrange
            var language_1 = new Language("some words", "en");
            var language_2 = new Language("algunas palabras", "es");
            var languages = new[] { language_1, language_2 };

            // Act
            var subject = new InternationalizedText(languages);

            // Assert
            var actual = subject.GetTextAt(0);
            Assert.That(actual.Value == "some words");
        }

        [Test]
        public void Test_Can_Party()
        {
            // Act
            var party = new Party("new party");

            // Assert
            Assert.IsNotNull(party.Name);
        }

        [Test]
        public void Test_Can_Construct_Ballot_style()
        {
            var gpUnitIds = new[] { "gp-unit-1", "gp-unit-2" };

            var subject = new BallotStyle("some-object-id", gpUnitIds);

            var actual = subject.GetGeopoliticalUnitIdAt(0);
            Assert.That(actual == "gp-unit-1");
        }

        [Test]
        public void Test_Can_Construct_InternalManifest_From_Sample_Manifest()
        {
            // Get a simple manifest that shows the bare minimum data required
            var manifest = ManifestGenerator.GetManifestFromFile();
            var internalManifest = new InternalManifest(manifest);

            Assert.That(manifest.CryptoHash().ToHex() == internalManifest.ManifestHash.ToHex());
            Assert.That(manifest.IsValid());
        }

        [Test]
        public void Test_Can_Construct_InternalManifest_From_Manifest_MultipleBallotStyles()
        {
            // Get a slightly more complex manifest that shows including multiple ballot styles
            var manifest = ManifestGenerator.GetJeffersonCountyManifest_MultipleBallotStyles();
            var internalManifest = new InternalManifest(manifest);

            Assert.That(manifest.CryptoHash().ToHex() == internalManifest.ManifestHash.ToHex());
            Assert.That(manifest.IsValid());
        }

        [Test]
        public void Test_Can_Serialize_Manifest_Minimal()
        {
            var manifest = ManifestGenerator.GetJeffersonCountyManifest_Minimal();
            var json = manifest.ToJson();

            var result = new Manifest(json);

            Assert.That(manifest.CryptoHash().ToHex() == result.CryptoHash().ToHex());
            Assert.That(result.IsValid());
        }

        [Test]
        public void Test_Can_Serialize_Manifest_MultipleBallotStyles()
        {
            var manifest = ManifestGenerator.GetJeffersonCountyManifest_MultipleBallotStyles();
            var json = manifest.ToJson();

            var result = new Manifest(json);

            Assert.That(manifest.CryptoHash().ToHex() == result.CryptoHash().ToHex());
            Assert.That(result.IsValid());
        }


        [Test]
        public void Test_Can_Create_Manifest_With_Name()
        {
            var language = new Language(
                string.Format("{0},{1}", "my jurisdiction", "here"), "en");
            List<GeopoliticalUnit> gpUnits = new List<GeopoliticalUnit>();
            List<Party> parties = new List<Party>();
            List<Candidate> candidates = new List<Candidate>();
            List<ContestDescription> contests = new List<ContestDescription>();
            List<BallotStyle> ballotStyles = new List<BallotStyle>();

            gpUnits.Add(new GeopoliticalUnit("mydistrict", "first unit", ReportingUnitType.city));
            parties.Add(new Party("myparty"));
            candidates.Add(new Candidate("mycandidate", false));
            List<SelectionDescription> selections = new List<SelectionDescription>();
            selections.Add(new SelectionDescription("selection1", "mycandidate", 1));
            contests.Add(new ContestDescription("firstcontest", "mydistrict", 1, VoteVariationType.n_of_m, 1, "mrmayor", selections.ToArray()));
            string[] gps = { "mydistrict" };
            ballotStyles.Add(new BallotStyle("style1", gps));

            var result = new Manifest(
                "test-manifest",
                ElectionType.general,
                DateTime.Now.AddDays(1),
                DateTime.Now.AddDays(1).AddDays(1),
                gpUnits.ToArray(),
                parties.ToArray(),
                candidates.ToArray(),
                contests.ToArray(),
                ballotStyles.ToArray(),
                new InternationalizedText(new[] { language }),
                new ContactInformation("na"));

            var json = result.ToJson();

            Assert.IsTrue(result.IsValid());
            Assert.IsFalse(json.Contains("\"name\":{\"text\":null"));   // check to make sure the party name serialized correctly
        }

        [Test]
        public void Test_Unicode_CandidateNames()
        {
            var candidateName = new Language("Raúl", "en");
            var candidate = new Candidate(
                "candidate-1",
                new InternationalizedText(new[] { candidateName }),
                string.Empty,
                string.Empty,
                false);

            List<Candidate> candidates = new List<Candidate>
            {
                candidate
            };

            var result = new Manifest(
                "test-manifest",
                ElectionType.general,
                DateTime.Now,
                DateTime.Now,
                new GeopoliticalUnit[] { },
                new Party[] { },
                candidates.ToArray(),
                new ContestDescription[] { },
                new BallotStyle[] { },
                new InternationalizedText(new Language[] { }),
                new ContactInformation("na"));

            var json = result.ToJson();
            Assert.IsTrue(json.Contains("\"value\":\"Raul\""));
        }
    }
}
