using System;
using NUnit.Framework;

namespace ElectionGuard.Tests
{
    [TestFixture]
    public class TestEncrypt
    {
        string manifest = "{\"spec_version\":\"" +
   "v0.95\",\"geopolitic" +
   "al_units\":[{\"objec" +
   "t_id\":\"jefferson-c" +
   "ounty\",\"name\":\"J" +
   "efferson County\",\"" +
   "type\":\"county\",\"" +
   "contact_information" +
   "\":{\"address_line\"" +
   ":[\"1234 Samuel Adam" +
   "s Way\",\"Jefferson," +
   " Hamilton 999999\"]," +
   "\"name\":\"Jefferson" +
   " County Clerk\",\"em" +
   "ail\":[{\"annotation" +
   "\":\"inquiries\",\"v" +
   "alue\":\"inquiries@j" +
   "efferson.hamilton.st" +
   "ate.gov\"}],\"phone" +
   "\":[{\"annotation\":" +
   "\"domestic\",\"value" +
   "\":\"123-456-7890\"}" +
   "]}},{\"object_id\":" +
   "\"harrison-township" +
   "\",\"name\":\"Harris" +
   "on Township\",\"type" +
   "\":\"township\",\"co" +
   "ntact_information\":" +
   "{\"address_line\":[" +
   "\"1234 Thorton Drive" +
   "\",\"Harrison, Hamil" +
   "ton 999999\"],\"name" +
   "\":\"Harrison Town H" +
   "all\",\"email\":[{\"" +
   "annotation\":\"inqui" +
   "ries\",\"value\":\"i" +
   "nquiries@harrison.ha" +
   "milton.state.gov\"}]" +
   ",\"phone\":[{\"annot" +
   "ation\":\"domestic\"" +
   ",\"value\":\"123-456" +
   "-7890\"}]}},{\"objec" +
   "t_id\":\"harrison-to" +
   "wnship-precinct-east" +
   "\",\"name\":\"Harris" +
   "on Township Precinct" +
   "\",\"type\":\"townsh" +
   "ip\",\"contact_infor" +
   "mation\":{\"address_" +
   "line\":[\"1234 Thort" +
   "on Drive\",\"Harriso" +
   "n, Hamilton 999999\"" +
   "],\"name\":\"Harriso" +
   "n Town Hall\",\"emai" +
   "l\":[{\"annotation\"" +
   ":\"inquiries\",\"val" +
   "ue\":\"inquiries@har" +
   "rison.hamilton.state" +
   ".gov\"}],\"phone\":[" +
   "{\"annotation\":\"do" +
   "mestic\",\"value\":" +
   "\"123-456-7890\"}]}}" +
   ",{\"object_id\":\"ru" +
   "tledge-elementary\"," +
   "\"name\":\"Rutledge " +
   "Elementary School di" +
   "strict\",\"type\":\"" +
   "school\",\"contact_i" +
   "nformation\":{\"addr" +
   "ess_line\":[\"1234 W" +
   "olcott Parkway\",\"H" +
   "arrison, Hamilton 99" +
   "9999\"],\"name\":\"R" +
   "utledge Elementary S" +
   "chool\",\"email\":[{" +
   "\"annotation\":\"inq" +
   "uiries\",\"value\":" +
   "\"inquiries@harrison" +
   ".hamilton.state.gov" +
   "\"}],\"phone\":[{\"a" +
   "nnotation\":\"domest" +
   "ic\",\"value\":\"123" +
   "-456-7890\"}]}}],\"p" +
   "arties\":[{\"object_" +
   "id\":\"whig\",\"abbr" +
   "eviation\":\"WHI\"," +
   "\"color\":\"AAAAAA\"" +
   ",\"logo_uri\":\"http" +
   "://some/path/to/whig" +
   ".svg\",\"name\":{\"t" +
   "ext\":[{\"value\":\"" +
   "Whig Party\",\"langu" +
   "age\":\"en\"}]}},{\"" +
   "object_id\":\"federa" +
   "list\",\"abbreviatio" +
   "n\":\"FED\",\"color" +
   "\":\"CCCCCC\",\"logo" +
   "_uri\":\"http://some" +
   "/path/to/federalist." +
   "svg\",\"name\":{\"te" +
   "xt\":[{\"value\":\"F" +
   "ederalist Party\",\"" +
   "language\":\"en\"}]}" +
   "},{\"object_id\":\"d" +
   "emocratic-republican" +
   "\",\"abbreviation\":" +
   "\"DEMREP\",\"color\"" +
   ":\"EEEEEE\",\"logo_u" +
   "ri\":\"http://some/p" +
   "ath/to/democratic-re" +
   "pulbican.svg\",\"nam" +
   "e\":{\"text\":[{\"va" +
   "lue\":\"Democratic R" +
   "epublican Party\",\"" +
   "language\":\"en\"}]}" +
   "}],\"candidates\":[{" +
   "\"object_id\":\"benj" +
   "amin-franklin\",\"na" +
   "me\":{\"text\":[{\"v" +
   "alue\":\"Benjamin Fr" +
   "anklin\",\"language" +
   "\":\"en\"}]},\"party" +
   "_id\":\"whig\"},{\"o" +
   "bject_id\":\"john-ad" +
   "ams\",\"name\":{\"te" +
   "xt\":[{\"value\":\"J" +
   "ohn Adams\",\"langua" +
   "ge\":\"en\"}]},\"par" +
   "ty_id\":\"federalist" +
   "\"},{\"object_id\":" +
   "\"john-hancock\",\"n" +
   "ame\":{\"text\":[{\"" +
   "value\":\"John Hanco" +
   "ck\",\"language\":\"" +
   "en\"}]},\"party_id\"" +
   ":\"democratic-republ" +
   "ican\"},{\"object_id" +
   "\":\"write-in\",\"na" +
   "me\":{\"text\":[{\"v" +
   "alue\":\"Write In Ca" +
   "ndidate\",\"language" +
   "\":\"en\"},{\"value" +
   "\":\"Escribir en la " +
   "candidata\",\"langua" +
   "ge\":\"es\"}]},\"is_" +
   "write_in\":true},{\"" +
   "object_id\":\"refere" +
   "ndum-pineapple-affir" +
   "mative\",\"name\":{" +
   "\"text\":[{\"value\"" +
   ":\"Pineapple should " +
   "be banned on pizza\"" +
   ",\"language\":\"en\"" +
   "}]}},{\"object_id\":" +
   "\"referendum-pineapp" +
   "le-negative\",\"name" +
   "\":{\"text\":[{\"val" +
   "ue\":\"Pineapple sho" +
   "uld not be banned on" +
   " pizza\",\"language" +
   "\":\"en\"}]}}],\"con" +
   "tests\":[{\"@type\":" +
   "\"CandidateContest\"" +
   ",\"object_id\":\"jus" +
   "tice-supreme-court\"" +
   ",\"sequence_order\":" +
   "0,\"ballot_selection" +
   "s\":[{\"object_id\":" +
   "\"john-adams-selecti" +
   "on\",\"sequence_orde" +
   "r\":0,\"candidate_id" +
   "\":\"john-adams\"},{" +
   "\"object_id\":\"benj" +
   "amin-franklin-select" +
   "ion\",\"sequence_ord" +
   "er\":1,\"candidate_i" +
   "d\":\"benjamin-frank" +
   "lin\"},{\"object_id" +
   "\":\"john-hancock-se" +
   "lection\",\"sequence" +
   "_order\":2,\"candida" +
   "te_id\":\"john-hanco" +
   "ck\"},{\"object_id\"" +
   ":\"write-in-selectio" +
   "n\",\"sequence_order" +
   "\":3,\"candidate_id" +
   "\":\"write-in\"}],\"" +
   "ballot_title\":{\"te" +
   "xt\":[{\"value\":\"J" +
   "ustice of the Suprem" +
   "e Court\",\"language" +
   "\":\"en\"},{\"value" +
   "\":\"Juez de la cort" +
   "e suprema\",\"langua" +
   "ge\":\"es\"}]},\"bal" +
   "lot_subtitle\":{\"te" +
   "xt\":[{\"value\":\"P" +
   "lease choose up to t" +
   "wo candidates\",\"la" +
   "nguage\":\"en\"},{\"" +
   "value\":\"Uno\",\"la" +
   "nguage\":\"es\"}]}," +
   "\"vote_variation\":" +
   "\"n_of_m\",\"elector" +
   "al_district_id\":\"j" +
   "efferson-county\",\"" +
   "name\":\"Justice of " +
   "the Supreme Court\"," +
   "\"primary_party_ids" +
   "\":[\"whig\",\"feder" +
   "alist\"],\"number_el" +
   "ected\":2,\"votes_al" +
   "lowed\":2},{\"@type" +
   "\":\"ReferendumConte" +
   "st\",\"object_id\":" +
   "\"referendum-pineapp" +
   "le\",\"sequence_orde" +
   "r\":1,\"ballot_selec" +
   "tions\":[{\"object_i" +
   "d\":\"referendum-pin" +
   "eapple-affirmative-s" +
   "election\",\"sequenc" +
   "e_order\":0,\"candid" +
   "ate_id\":\"referendu" +
   "m-pineapple-affirmat" +
   "ive\"},{\"object_id" +
   "\":\"referendum-pine" +
   "apple-negative-selec" +
   "tion\",\"sequence_or" +
   "der\":1,\"candidate_" +
   "id\":\"referendum-pi" +
   "neapple-negative\"}]" +
   ",\"ballot_title\":{" +
   "\"text\":[{\"value\"" +
   ":\"Should pineapple " +
   "be banned on pizza?" +
   "\",\"language\":\"en" +
   "\"},{\"value\":\"¿De" +
   "bería prohibirse la " +
   "piña en la pizza?\"," +
   "\"language\":\"es\"}" +
   "]},\"ballot_subtitle" +
   "\":{\"text\":[{\"val" +
   "ue\":\"The township " +
   "considers this issue" +
   " to be very importan" +
   "t\",\"language\":\"e" +
   "n\"},{\"value\":\"El" +
   " municipio considera" +
   " que esta cuestión e" +
   "s muy importante\"," +
   "\"language\":\"es\"}" +
   "]},\"vote_variation" +
   "\":\"one_of_m\",\"el" +
   "ectoral_district_id" +
   "\":\"harrison-townsh" +
   "ip\",\"name\":\"The " +
   "Pineapple Question\"" +
   ",\"number_elected\":" +
   "1,\"votes_allowed\":" +
   "1}],\"ballot_styles" +
   "\":[{\"object_id\":" +
   "\"jefferson-county-b" +
   "allot-style\",\"geop" +
   "olitical_unit_ids\":" +
   "[\"jefferson-county" +
   "\"]},{\"object_id\":" +
   "\"harrison-township-" +
   "ballot-style\",\"geo" +
   "political_unit_ids\"" +
   ":[\"jefferson-county" +
   "\",\"harrison-townsh" +
   "ip\"]},{\"object_id" +
   "\":\"harrison-townsh" +
   "ip-precinct-east-bal" +
   "lot-style\",\"geopol" +
   "itical_unit_ids\":[" +
   "\"jefferson-county\"" +
   ",\"harrison-township" +
   "\",\"harrison-townsh" +
   "ip-precinct-east\"," +
   "\"rutledge-elementar" +
   "y\"]},{\"object_id\"" +
   ":\"rutledge-elementa" +
   "ry-ballot-style\",\"" +
   "geopolitical_unit_id" +
   "s\":[\"jefferson-cou" +
   "nty\",\"harrison-tow" +
   "nship\",\"rutledge-e" +
   "lementary\"]}],\"nam" +
   "e\":{\"text\":[{\"va" +
   "lue\":\"Jefferson Co" +
   "unty Spring Primary" +
   "\",\"language\":\"en" +
   "\"},{\"value\":\"Pri" +
   "maria de primavera d" +
   "el condado de Jeffer" +
   "son\",\"language\":" +
   "\"es\"}]},\"contact_" +
   "information\":{\"add" +
   "ress_line\":[\"1234 " +
   "Paul Revere Run\",\"" +
   "Jefferson, Hamilton " +
   "999999\"],\"name\":" +
   "\"Hamilton State Ele" +
   "ction Commission\"," +
   "\"email\":[{\"annota" +
   "tion\":\"press\",\"v" +
   "alue\":\"inquiries@h" +
   "amilton.state.gov\"}" +
   ",{\"annotation\":\"f" +
   "ederal\",\"value\":" +
   "\"commissioner@hamil" +
   "ton.state.gov\"}],\"" +
   "phone\":[{\"annotati" +
   "on\":\"domestic\",\"" +
   "value\":\"123-456-78" +
   "90\"},{\"annotation" +
   "\":\"international\"" +
   ",\"value\":\"+1-123-" +
   "456-7890\"}]},\"star" +
   "t_date\":\"2020-03-0" +
   "1T08:00:00-05:00\"," +
   "\"end_date\":\"2020-" +
   "03-01T20:00:00-05:00" +
   "\",\"election_scope_" +
   "id\":\"jefferson-cou" +
   "nty-primary\",\"type" +
   "\":\"primary\"}";
        readonly string plaintext = "{\"object_id\":\"some-external-id-string-123\",\"ballot_style\":\"jefferson-county-ballot-style\",\"contests\":[{\"object_id\":\"justice-supreme-court\",\"ballot_selections\":[{\"object_id\":\"john-adams-selection\",\"vote\":1},{\"object_id\":\"write-in-selection\",\"vote\":1,\"extra_data\":\"some writein value\"}]}]}";
        [Test]
        public void test_Compact_Encrypt_Ballot_Simple_Succeeds()
        {
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            var description = new ElectionDescription(manifest);
            var metadata = new InternalElectionDescription(description);
            var context = new CiphertextElectionContext(1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, metadata.DescriptionHash);
            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            var mediator = new EncryptionMediator(metadata, context, device);

            var ballot = new PlaintextBallot(plaintext);

            // Act
            var compact = mediator.CompactEncrypt(ballot);

            // Assert
            Assert.That(compact.ObjectId == ballot.ObjectId);

            var msgpack = compact.ToMsgPack();
            var fromMsgpack = new CompactCiphertextBallot(msgpack);

            Assert.That(compact.ObjectId == fromMsgpack.ObjectId);

        }
    }
}
