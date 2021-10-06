using System;
using System.Diagnostics;
using NUnit.Framework;

namespace ElectionGuard.Encrypt.Tests
{
    [TestFixture]
    public class TestManifest
    {
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
        public void Test_Can_Construct_Ballot_style()
        {
            var gpUnitIds = new[] { "gp-unit-1", "gp-unit-2" };

            var subject = new BallotStyle("some-object-id", gpUnitIds);

            var actual = subject.GetGeopoliticalUnitIdAt(0);
            Assert.That(actual == "gp-unit-1");
        }

        [Test]
        public void Test_Can_Deserialize_Election_Description()
        {
            // Arrange
            const string data = @"{""ballot_styles"":[{""geopolitical_unit_ids"":[""some-geopolitical-unit-id""],""object_id"":""some-ballot-style-id""}],""candidates"":[{""object_id"":""some-candidate-id-1""},{""object_id"":""some-candidate-id-2""},{""object_id"":""some-candidate-id-3""}],""contests"":[{""ballot_selections"":[{""candidate_id"":""some-candidate-id-1"",""object_id"":""some-object-id-affirmative"",""sequence_order"":0},{""candidate_id"":""some-candidate-id-2"",""object_id"":""some-object-id-negative"",""sequence_order"":1}],""electoral_district_id"":""some-geopoltical-unit-id"",""name"":""some-referendum-contest-name"",""number_elected"":1,""object_id"":""some-referendum-contest-object-id"",""sequence_order"":0,""vote_variation"":""one_of_m""},{""ballot_selections"":[{""candidate_id"":""some-candidate-id-1"",""object_id"":""some-object-id-candidate-1"",""sequence_order"":0},{""candidate_id"":""some-candidate-id-2"",""object_id"":""some-object-id-candidate-2"",""sequence_order"":1},{""candidate_id"":""some-candidate-id-3"",""object_id"":""some-object-id-candidate-3"",""sequence_order"":2}],""electoral_district_id"":""some-geopoltical-unit-id"",""name"":""some-candidate-contest-name"",""number_elected"":2,""object_id"":""some-candidate-contest-object-id"",""sequence_order"":1,""vote_variation"":""one_of_m""}],""election_scope_id"":""some-scope-id"",""end_date"":""2021-02-04T17:19:34Z"",""geopolitical_units"":[{""name"":""some-gp-unit-name"",""object_id"":""some-geopoltical-unit-id"",""type"":""unknown""}],""parties"":[{""object_id"":""some-party-id-1""},{""object_id"":""some-party-id-2""}],""start_date"":""2021-02-04T17:19:34Z"",""type"":""unknown""}";

            // Act
            var result = new Manifest(data);

            // Assert
            Assert.That(result.ElectionScopeId == "some-scope-id");


        }

        [Test]
        public void Test_Can_Deserialize_Internal_Election_Description()
        {
            // Arrange
            const string data = "{\"ballot_styles\":[{\"geopolitical_unit_ids\":[\"geopolitical-unit-1\"],\"image_uri\":\"some-uri\",\"object_id\":\"some-ballot-style-id\",\"party_ids\":[\"party-1\"]}],\"contests\":[{\"ballot_placeholders\":[{\"candidate_id\":\"candidate-4-id\",\"object_id\":\"contest-1-placeholder-selection-4-id\",\"sequence_order\":4}],\"ballot_selections\":[{\"candidate_id\":\"candidate-1-id\",\"object_id\":\"contest-1-selection-1-id\",\"sequence_order\":1},{\"candidate_id\":\"candidate-2-id\",\"object_id\":\"contest-1-selection-2-id\",\"sequence_order\":2},{\"candidate_id\":\"candidate-3-id\",\"object_id\":\"contest-1-selection-3-id\",\"sequence_order\":3}],\"ballot_subtitle\":{\"text\":[{\"language\":\"en\",\"value\":\"some-title-string\"},{\"language\":\"es\",\"value\":\"some-title-string-es\"}]},\"ballot_title\":{\"text\":[{\"language\":\"en\",\"value\":\"some-title-string\"},{\"language\":\"es\",\"value\":\"some-title-string-es\"}]},\"electoral_district_id\":\"geopolitical-unit-1\",\"name\":\"contest-1-name\",\"number_elected\":2,\"object_id\":\"contest-1-id\",\"sequence_order\":1,\"vote_variation\":\"n_of_m\",\"votes_allowed\":2},{\"ballot_placeholders\":[{\"candidate_id\":\"candidate-3-id\",\"object_id\":\"contest-2-placeholder-selection-3-id\",\"sequence_order\":3}],\"ballot_selections\":[{\"candidate_id\":\"candidate-1-id\",\"object_id\":\"contest-2-selection-1-id\",\"sequence_order\":1},{\"candidate_id\":\"candidate-2-id\",\"object_id\":\"contest-2-selection-2-id\",\"sequence_order\":2}],\"ballot_subtitle\":{\"text\":[{\"language\":\"en\",\"value\":\"some-title-string\"},{\"language\":\"es\",\"value\":\"some-title-string-es\"}]},\"ballot_title\":{\"text\":[{\"language\":\"en\",\"value\":\"some-title-string\"},{\"language\":\"es\",\"value\":\"some-title-string-es\"}]},\"electoral_district_id\":\"geopolitical-unit-1\",\"name\":\"contest-2-name\",\"number_elected\":2,\"object_id\":\"contest-2-id\",\"sequence_order\":2,\"vote_variation\":\"n_of_m\",\"votes_allowed\":2}],\"manifest_hash\":\"02\",\"geopolitical_units\":[{\"contact_information\":{\"address_line\":[\"some-street\",\"some-city\",\"some-whatever\"],\"name\":\"gp-unit-contact-info\"},\"name\":\"district-1-id\",\"object_id\":\"geopolitical-unit-1\",\"type\":\"city\"}]}";

            // Act
            var result = new InternalManifest(data);

            // Assert
            Assert.That(result.ManifestHash.ToHex() == "02");
        }
    }
}
