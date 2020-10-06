#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>
#include <electionguard/encrypt.hpp>

using namespace electionguard;

unique_ptr<InternalElectionDescription> getFakeMetadata(const ElementModQ &descriptionHash)
{
    vector<unique_ptr<SelectionDescription>> selections1;
    selections1.push_back(
      make_unique<SelectionDescription>("contest-1-selection-1-id", "candidate-1-id", 1UL));
    selections1.push_back(
      make_unique<SelectionDescription>("contest-1-selection-2-id", "candidate-2-id", 2UL));
    selections1.push_back(
      make_unique<SelectionDescription>("contest-1-selection-3-id", "candidate-3-id", 3UL));

    vector<unique_ptr<SelectionDescription>> selections2;
    selections2.push_back(
      make_unique<SelectionDescription>("contest-2-selection-1-id", "candidate-1-id", 1UL));
    selections2.push_back(
      make_unique<SelectionDescription>("contest-2-selection-2-id", "candidate-2-id", 2UL));

    vector<unique_ptr<Language>> contest1TitleText;
    contest1TitleText.push_back(make_unique<Language>("some-title-string", "en"));
    contest1TitleText.push_back(make_unique<Language>("some-title-string-es", "es"));

    vector<unique_ptr<Language>> contest1SubTitleText;
    contest1SubTitleText.push_back(make_unique<Language>("some-title-string", "en"));
    contest1SubTitleText.push_back(make_unique<Language>("some-title-string-es", "es"));

    auto title1 = make_unique<InternationalizedText>(move(contest1TitleText));
    auto subtitle1 = make_unique<InternationalizedText>(move(contest1SubTitleText));

    vector<unique_ptr<Language>> contest2TitleText;
    contest2TitleText.push_back(make_unique<Language>("some-title-string", "en"));
    contest2TitleText.push_back(make_unique<Language>("some-title-string-es", "es"));
    vector<unique_ptr<Language>> contest2SubTitleText;
    contest2SubTitleText.push_back(make_unique<Language>("some-title-string", "en"));
    contest2SubTitleText.push_back(make_unique<Language>("some-title-string-es", "es"));

    auto title2 = make_unique<InternationalizedText>(move(contest2TitleText));
    auto subtitle2 = make_unique<InternationalizedText>(move(contest2SubTitleText));

    vector<unique_ptr<ContestDescription>> contests;
    const auto numElected = 2UL;
    contests.push_back(make_unique<ContestDescription>(
      "contest-1-id", "district-1-id", 1UL, "n_of_m", numElected, numElected, "contest-1-name",
      move(title1), move(subtitle1), move(selections1)));

    const auto numElected2 = 1UL;
    contests.push_back(make_unique<ContestDescription>(
      "contest-2-id", "district-1-id", 1UL, "n_of_m", numElected, numElected, "contest-2-name",
      move(title2), move(subtitle2), move(selections2)));

    auto metadata = make_unique<InternalElectionDescription>(descriptionHash, move(contests));
    return metadata;
}

unique_ptr<CiphertextElectionContext> getFakeContext(const InternalElectionDescription &metadata,
                                                     const ElementModP &publicKey)
{

    auto context =
      CiphertextElectionContext::make(1UL, 1UL, make_unique<ElementModP>(publicKey),
                                      make_unique<ElementModQ>(metadata.getDescriptionHash()));
    return context;
}

unique_ptr<PlaintextBallotSelection> selectionFrom(const SelectionDescription &description,
                                                   bool choice)
{
    return make_unique<PlaintextBallotSelection>(description.getObjectId(), choice ? "1" : "0");
}

unique_ptr<PlaintextBallotContest> contestFrom(const ContestDescription &contest)
{

    uint64_t choices = 0;
    vector<unique_ptr<PlaintextBallotSelection>> selections;

    // jsut loop through and select the necessary amount of trues
    for (const auto &description : contest.getSelections()) {
        if (choices < contest.getNumberElected()) {
            ++choices;
            selections.push_back(selectionFrom(description.get(), true));
        } else {
            selections.push_back(selectionFrom(description.get(), false));
        }
    }

    return make_unique<PlaintextBallotContest>(contest.getObjectId(), move(selections));
}

unique_ptr<PlaintextBallot> getFakeBallot(const InternalElectionDescription &metadata)
{
    string ballotId = "ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123ballot-"
                      "id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-123-ballot-id-"
                      "123-ballot-id-123-ballot-id-123-ballot-id-123";

    vector<unique_ptr<PlaintextBallotContest>> contests;
    for (const auto &contest : metadata.getContests()) {
        contests.push_back(contestFrom(contest.get()));
    }
    return make_unique<PlaintextBallot>(ballotId, "ballot-style-1", move(contests));
}

// TEST_CASE("Encrypt Selection with generic data succeeds")
// {
//     // Arrange
//     const auto *candidateId = "some-candidate-id";
//     auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
//     auto nonce = rand_q();
//     auto metadata = make_unique<SelectionDescription>("some-selection-object_id", candidateId, 1UL);
//     auto hashContext = metadata->crypto_hash();
//     auto plaintext = make_unique<PlaintextBallotSelection>(candidateId, "1");

//     // Act
//     auto result = encryptSelection(*plaintext, *metadata, *keypair->getPublicKey(), ONE_MOD_Q(),
//                                    *nonce, false, true);

//     // Assert
//     CHECK(result != nullptr);
//     CHECK(result->getCiphertext() != nullptr);
//     CHECK(result->isValidEncryption(*hashContext, *keypair->getPublicKey(), ONE_MOD_Q()) == true);
//     CHECK(result->getProof()->isValid(*result->getCiphertext(), *keypair->getPublicKey(),
//                                       ONE_MOD_Q()) == true);
// }

// TEST_CASE("Encrypt Ballot with mediator succeeds")
// {
//     auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
//     auto metadata = getFakeMetadata(TWO_MOD_Q());
//     auto context = getFakeContext(*metadata, *keypair->getPublicKey());
//     auto device = make_unique<EncryptionDevice>(12345UL, "Location");

//     auto mediator = make_unique<EncryptionMediator>(*metadata, *context, *device);

//     // Act
//     auto plaintext = getFakeBallot(*metadata);
//     auto ciphertext = mediator->encrypt(*plaintext);

//     // Assert
//     // TODO: validate against metadata->getDescriptionHash()
//     CHECK(ciphertext->isValidEncryption(*context->getDescriptionHash(), *keypair->getPublicKey(),
//                                         *context->getCryptoExtendedBaseHash()) == true);

//     // Can Serialize CiphertextBallot
//     auto json = ciphertext->toJson();
//     auto fromJson = CiphertextBallot::fromJson(json);

//     CHECK(plaintext->getObjectId() == plaintext->getObjectId());

//     auto bson = ciphertext->toBson();
//     auto fromBson = CiphertextBallot::fromBson(bson);
// }

TEST_CASE("Encrypt Ballot with strings succeeds")
{
    auto metadata_ = string(
      "{\"ballot_styles\": [{\"geopolitical_unit_ids\": [\"neu-york-county\"], \"object_id\": "
      "\"neu-york-county-ballot\"}], \"candidates\": [{\"ballot_name\": {\"text\": [{\"language\": "
      "\"en\", \"value\": \"Cookies\"}]}, \"object_id\": \"cookies\"}, {\"ballot_name\": "
      "{\"text\": [{\"language\": \"en\", \"value\": \"Variety of Fresh Fruits\"}]}, "
      "\"object_id\": \"fruits\"}, {\"ballot_name\": {\"text\": [{\"language\": \"en\", \"value\": "
      "\"Donuts (and other fried dough desserts like churros, beignets, gulab jamun, funnel cake, "
      "etc.)\"}]}, \"object_id\": \"donuts\"}, {\"ballot_name\": {\"text\": [{\"language\": "
      "\"en\", \"value\": \"Cake (includes layer cakes, cup cakes, cake rolls, tres leches, "
      "tiramisu, etc.)\"}]}, \"object_id\": \"cake\"}, {\"ballot_name\": {\"text\": "
      "[{\"language\": \"en\", \"value\": \"Ice Cream\"}]}, \"object_id\": \"icecream\"}, "
      "{\"ballot_name\": {\"text\": [{\"language\": \"en\", \"value\": \"Yes\"}]}, \"object_id\": "
      "\"pineapple-on-pizza-affirmative\"}, {\"ballot_name\": {\"text\": [{\"language\": \"en\", "
      "\"value\": \"No\"}]}, \"object_id\": \"pineapple-on-pizza-negative\"}, {\"ballot_name\": "
      "{\"text\": [{\"language\": \"en\", \"value\": \"President James Marshall (Harrison "
      "Ford)\"}]}, \"object_id\": \"marshall\", \"party_id\": \"air-force-one\"}, "
      "{\"ballot_name\": {\"text\": [{\"language\": \"en\", \"value\": \"President Josiah "
      "\\\"Jed\\\" Bartlet (Martin Sheen)\"}]}, \"object_id\": \"bartlet\", \"party_id\": "
      "\"west-wing\"}, {\"ballot_name\": {\"text\": [{\"language\": \"en\", \"value\": \"President "
      "Thomas J. Whitmore (Bill Pullman)\"}]}, \"object_id\": \"whitmore\", \"party_id\": "
      "\"independence-day\"}, {\"ballot_name\": {\"text\": [{\"language\": \"en\", \"value\": "
      "\"President James Sawyer (Jamie Foxx)\"}]}, \"object_id\": \"sawyer\", \"party_id\": "
      "\"white-house-down\"}, {\"ballot_name\": {\"text\": [{\"language\": \"en\", \"value\": "
      "\"President Not Sure/Joe Bauers (Luke Wilson)\"}]}, \"object_id\": \"bauers\", "
      "\"party_id\": \"idiocracy\"}, {\"ballot_name\": {\"text\": [{\"language\": \"en\", "
      "\"value\": \"President Beck (Morgan Freeman)\"}]}, \"object_id\": \"beck\", \"party_id\": "
      "\"deep-impact\"}, {\"ballot_name\": {\"text\": [{\"language\": \"en\", \"value\": "
      "\"President Selina Meyer (Julia Louis-Dreyfus)\"}]}, \"object_id\": \"meyer\", "
      "\"party_id\": \"veep\"}], \"contact_information\": {\"address_line\": [\"1234 Neuvote "
      "Run\", \"Neu York, Neu York 99999\"], \"email\": [{\"annotation\": \"press\", \"value\": "
      "\"inquiries@neuvote.com\"}], \"name\": \"Neuvote Election Commission\", \"phone\": "
      "[{\"annotation\": \"domestic\", \"value\": \"123-456-7890\"}, {\"annotation\": "
      "\"international\", \"value\": \"+1-123-456-7890\"}]}, \"contests\": [{\"@type\": "
      "\"CandidateContest\", \"ballot_selections\": [{\"candidate_id\": \"cookies\", "
      "\"object_id\": \"cookies-selection\", \"sequence_order\": 0}, {\"candidate_id\": "
      "\"fruits\", \"object_id\": \"fruits-selection\", \"sequence_order\": 1}, {\"candidate_id\": "
      "\"donuts\", \"object_id\": \"donuts-selection\", \"sequence_order\": 2}, {\"candidate_id\": "
      "\"cake\", \"object_id\": \"cake-selection\", \"sequence_order\": 3}, {\"candidate_id\": "
      "\"icecream\", \"object_id\": \"icecream-selection\", \"sequence_order\": 4}], "
      "\"ballot_subtitle\": {\"text\": [{\"language\": \"en\", \"value\": \"Vote for 3\"}]}, "
      "\"ballot_title\": {\"text\": [{\"language\": \"en\", \"value\": \"Which 3 desserts could "
      "you never give up?\"}]}, \"electoral_district_id\": \"neu-york-county\", \"name\": \"Which "
      "3 desserts could you never give up?\", \"number_elected\": 3, \"object_id\": "
      "\"desserts-contest\", \"sequence_order\": 0, \"vote_variation\": \"n_of_m\", "
      "\"votes_allowed\": 3}, {\"@type\": \"ReferendumContest\", \"ballot_selections\": "
      "[{\"candidate_id\": \"pineapple-on-pizza-affirmative\", \"object_id\": "
      "\"pineapple-on-pizza-affirmative-selection\", \"sequence_order\": 0}, {\"candidate_id\": "
      "\"pineapple-on-pizza-negative\", \"object_id\": \"pineapple-on-pizza-negative-selection\", "
      "\"sequence_order\": 1}], \"ballot_subtitle\": {\"text\": [{\"language\": \"en\", \"value\": "
      "\"Choose \'Yes\' or \'No\'\"}]}, \"ballot_title\": {\"text\": [{\"language\": \"en\", "
      "\"value\": \"Should Pineapple be allowed on Pizza?\"}]}, \"electoral_district_id\": "
      "\"neu-york-county\", \"name\": \"Should Pineapple be allowed on Pizza?\", "
      "\"number_elected\": 1, \"object_id\": \"pineapple-on-pizza-contest\", \"sequence_order\": "
      "1, \"vote_variation\": \"one_of_m\", \"votes_allowed\": 1}, {\"@type\": "
      "\"CandidateContest\", \"ballot_selections\": [{\"candidate_id\": \"marshall\", "
      "\"object_id\": \"marshall-selection\", \"sequence_order\": 0}, {\"candidate_id\": "
      "\"bartlet\", \"object_id\": \"bartlet-selection\", \"sequence_order\": 1}, "
      "{\"candidate_id\": \"whitmore\", \"object_id\": \"whitmore-selection\", \"sequence_order\": "
      "2}, {\"candidate_id\": \"sawyer\", \"object_id\": \"sawyer-selection\", \"sequence_order\": "
      "3}, {\"candidate_id\": \"bauers\", \"object_id\": \"bauers-selection\", \"sequence_order\": "
      "4}, {\"candidate_id\": \"beck\", \"object_id\": \"beck-selection\", \"sequence_order\": 5}, "
      "{\"candidate_id\": \"meyer\", \"object_id\": \"meyer-selection\", \"sequence_order\": 6}], "
      "\"ballot_subtitle\": {\"text\": [{\"language\": \"en\", \"value\": \"Vote for 1\"}]}, "
      "\"ballot_title\": {\"text\": [{\"language\": \"en\", \"value\": \"Favorite Fictional U.S. "
      "President?\"}]}, \"electoral_district_id\": \"neu-york-county\", \"name\": \"Favorite "
      "Fictional U.S. President?\", \"number_elected\": 1, \"object_id\": \"president-contest\", "
      "\"sequence_order\": 2, \"vote_variation\": \"one_of_m\", \"votes_allowed\": 1}], "
      "\"election_scope_id\": \"neu-york-county-general-election\", \"end_date\": "
      "\"2020-10-06T23:00:00+00:00\", \"geopolitical_units\": [{\"contact_information\": "
      "{\"address_line\": [\"1234 Neuvote Way\", \"Neu York, Neu York 99999\"], \"email\": "
      "[{\"annotation\": \"inquiries\", \"value\": \"inquiries@neuvote.com\"}], \"name\": \"Neu "
      "York County Clerk\", \"phone\": [{\"annotation\": \"domestic\", \"value\": "
      "\"123-456-7890\"}]}, \"name\": \"Neu York County\", \"object_id\": \"neu-york-county\", "
      "\"type\": \"county\"}], \"name\": {\"text\": [{\"language\": \"en\", \"value\": \"Neu York "
      "County General Election\"}]}, \"parties\": [{\"abbreviation\": \"AFO\", \"ballot_name\": "
      "{\"text\": []}, \"color\": \"AAAAAA\", \"logo_uri\": \"http://some/path/to/logo.svg\", "
      "\"name\": {\"text\": [{\"value\": \"Air Force One\", \"language\": \"en\"}]}, "
      "\"object_id\": \"air-force-one\"}, {\"abbreviation\": \"WW\", \"ballot_name\": {\"text\": "
      "[]}, \"color\": \"BBBBBB\", \"logo_uri\": \"http://some/path/to/logo.svg\", \"name\": "
      "{\"text\": [{\"value\": \"The West Wing\", \"language\": \"en\"}]}, \"object_id\": "
      "\"west-wing\"}, {\"abbreviation\": \"ID\", \"ballot_name\": {\"text\": []}, \"color\": "
      "\"CCCCCC\", \"logo_uri\": \"http://some/path/to/logo.svg\", \"name\": {\"text\": "
      "[{\"value\": \"Independence Day\", \"language\": \"en\"}]}, \"object_id\": "
      "\"independence-day\"}, {\"abbreviation\": \"WHD\", \"ballot_name\": {\"text\": []}, "
      "\"color\": \"DDDDDD\", \"logo_uri\": \"http://some/path/to/logo.svg\", \"name\": {\"text\": "
      "[{\"value\": \"White House Down\", \"language\": \"en\"}]}, \"object_id\": "
      "\"white-house-down\"}, {\"abbreviation\": \"I\", \"ballot_name\": {\"text\": []}, "
      "\"color\": \"EEEEEE\", \"logo_uri\": \"http://some/path/to/logo.svg\", \"name\": {\"text\": "
      "[{\"value\": \"Idiocracy\", \"language\": \"en\"}]}, \"object_id\": \"idiocracy\"}, "
      "{\"abbreviation\": \"DI\", \"ballot_name\": {\"text\": []}, \"color\": \"FFFFFF\", "
      "\"logo_uri\": \"http://some/path/to/logo.svg\", \"name\": {\"text\": [{\"value\": \"Deep "
      "Impact\", \"language\": \"en\"}]}, \"object_id\": \"deep-impact\"}, {\"abbreviation\": "
      "\"V\", \"ballot_name\": {\"text\": []}, \"color\": \"000000\", \"logo_uri\": "
      "\"http://some/path/to/logo.svg\", \"name\": {\"text\": [{\"value\": \"Veep\", \"language\": "
      "\"en\"}]}, \"object_id\": \"veep\"}], \"start_date\": \"2020-10-06T14:00:00+00:00\", "
      "\"type\": \"general\"}");

    auto context_ = string(
      "{\"number_of_guardians\":0,\"quorum\":0,\"elgamal_public_key\":"
      "\"0DEAD6387A8C57909C711BC7D581BA83893EE7E22FD429BBA79E0402E883FA2B6A2B1E26CFC5A80F2BC5A76E13"
      "949D40CCBBC76AD7E75E10297EE01A092E6AD0406CC78E11CEA2E85B754BAA5BCB2CAAD2E57B885D3874496BE04C"
      "182A4C1F1A16906175B070F95BE040B5FFDE8038F57592689E0189E160A621EC2926CDA25A9948677B774916CCA4"
      "AA9D8B73DB8FB7127EEC096DB521234AA965255C78C7B187F1C70535C7EBA08812BC745986063A269700B9048488"
      "3932D8B565629DFC741760DB9AB55E61860C5EA4E37860F54B7F705DACAB683BEDD14C70AA9C3CD701A1DD3AC7E4"
      "216093428CBB522F5FC1F3B00083EF7BC8ADC4EC40CF5619A63EFF4C472229DFDE26E5C5244B9559BD0861A0F03D"
      "B9DF13D3308ACAB52EC80B1DFA771160E90C559DF10D2B3F86A1A8E9A392C7EC3559ED965BDDE12B5CED9911B613"
      "3DC29EB01C366CF88E34492F666E204E04A5A5B65DB112D106CA518055499A2DB7929C2AB325B5ABF3CC9133336A"
      "2CB61ED979352185D94FE7360B13019A3AC2F47A2912FDA80EB794B853C8CD74FC794C9D0809E5F4643481C3EFAD"
      "038B14FF2B92D4F2645F0811F54756687F9325793A8A20EC0489221A9E74FB84524D3DED052590AFD5FDDF51BAAA"
      "3C1EB1A4B1F16BEE2EA8EBCB8FD4F919C4C27D8B4908EEB8EC5EB98D68EB0F8AC8F72E95D09836A21A50FDA25501"
      "74A052498640EDB\",\"description_hash\":"
      "\"2322C4C3D8AF985B4A5BBD68D008883409390809BAA39918501926667503ABD8\",\"crypto_base_hash\":"
      "\"0847EAD02A9EB101795775807D86DA05F4FDCB8639D34AB290893C0929C32D928\",\"crypto_extended_"
      "base_hash\":\"67CFA2C4F4422483CCEB09B150F53A17A9C235509533A1BF878F6ACD677E44F5\"}");
    auto plaintext_ = string(
      "{\"object_id\":\"80002c15-d292-4820-acd3-12ce33b33d2f\",\"ballot_style\":\"neu-york-county-"
      "ballot\",\"contests\":[{\"object_id\":\"desserts-contest\",\"ballot_selections\":[{\"object_"
      "id\":\"cookies-selection\",\"vote\":\"1\",\"is_placeholder_selection\":false,\"extended_"
      "data\":null},{\"object_id\":\"fruits-selection\",\"vote\":\"1\",\"is_placeholder_"
      "selection\":false,\"extended_data\":null},{\"object_id\":\"donuts-selection\",\"vote\":"
      "\"0\",\"is_placeholder_selection\":false,\"extended_data\":null},{\"object_id\":\"cake-"
      "selection\",\"vote\":\"0\",\"is_placeholder_selection\":false,\"extended_data\":null},{"
      "\"object_id\":\"icecream-selection\",\"vote\":\"1\",\"is_placeholder_selection\":false,"
      "\"extended_data\":null}]},{\"object_id\":\"pineapple-on-pizza-contest\",\"ballot_"
      "selections\":[{\"object_id\":\"pineapple-on-pizza-affirmative-selection\",\"vote\":\"1\","
      "\"is_placeholder_selection\":false,\"extended_data\":null},{\"object_id\":\"pineapple-on-"
      "pizza-negative-selection\",\"vote\":\"0\",\"is_placeholder_selection\":false,\"extended_"
      "data\":null}]},{\"object_id\":\"president-contest\",\"ballot_selections\":[{\"object_id\":"
      "\"marshall-selection\",\"vote\":\"0\",\"is_placeholder_selection\":false,\"extended_data\":"
      "null},{\"object_id\":\"bartlet-selection\",\"vote\":\"0\",\"is_placeholder_selection\":"
      "false,\"extended_data\":null},{\"object_id\":\"whitmore-selection\",\"vote\":\"0\",\"is_"
      "placeholder_selection\":false,\"extended_data\":null},{\"object_id\":\"sawyer-selection\","
      "\"vote\":\"0\",\"is_placeholder_selection\":false,\"extended_data\":null},{\"object_id\":"
      "\"bauers-selection\",\"vote\":\"0\",\"is_placeholder_selection\":false,\"extended_data\":"
      "null},{\"object_id\":\"beck-selection\",\"vote\":\"1\",\"is_placeholder_selection\":false,"
      "\"extended_data\":null},{\"object_id\":\"meyer-selection\",\"vote\":\"0\",\"is_placeholder_"
      "selection\":false,\"extended_data\":null}]}]}");

    auto metadata = InternalElectionDescription::fromJson(metadata_);
    auto context = CiphertextElectionContext::fromJson(context_);
    auto plaintext = PlaintextBallot::fromJson(plaintext_);

    auto device = make_unique<EncryptionDevice>(12345UL, "Location");

    auto mediator = make_unique<EncryptionMediator>(*metadata, *context, *device);

    // Act
    //auto plaintext = getFakeBallot(*metadata);
    auto ciphertext = mediator->encrypt(*plaintext, false);

    // Assert
    // TODO: validate against metadata->getDescriptionHash()
    CHECK(ciphertext->isValidEncryption(*context->getDescriptionHash(),
                                        *context->getElGamalPublicKey(),
                                        *context->getCryptoExtendedBaseHash()) == true);

    Log::debug("metadata:");
    Log::debug(metadata->toJson());

    Log::debug("context:");
    Log::debug(context->toJson());
    Log::debug("ciphertext:");
    Log::debug(ciphertext->toJson());
}

// TEST_CASE("Can serialize InternalElectionDescription")
// {
//     // Arrange
//     auto metadata = getFakeMetadata(TWO_MOD_Q());
//     auto json = metadata->toJson();
//     auto bson = metadata->toBson();

//     // Act
//     auto fromJson = InternalElectionDescription::fromJson(json);
//     auto fromBson = InternalElectionDescription::fromBson(bson);

//     // Assert
//     CHECK(metadata->getDescriptionHash().toHex() == fromJson->getDescriptionHash().toHex());
// }

// TEST_CASE("Can serialize CiphertextElectionContext")
// {
//     // Arrange
//     auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
//     auto metadata = getFakeMetadata(TWO_MOD_Q());
//     auto context = getFakeContext(*metadata, *keypair->getPublicKey());
//     auto json = context->toJson();
//     auto bson = context->toBson();

//     // Act
//     auto fromJson = CiphertextElectionContext::fromJson(json);
//     auto fromBson = CiphertextElectionContext::fromBson(bson);

//     // Assert
//     // TODO: validate against metadata->getDescriptionHash()
//     CHECK(fromJson->getDescriptionHash()->toHex() == context->getDescriptionHash()->toHex());
//     CHECK(fromBson->getDescriptionHash()->toHex() == context->getDescriptionHash()->toHex());
// }

// TEST_CASE("Can Deserialize CiphertextElectionContext")
// {
//     // Arrange
//     // Note this string includes a couple of bad hex strings
//     // with leading zeroes and are therefore out of range
//     auto json = string(
//       "{\"number_of_guardians\":5,\"quorum\":3,\"elgamal_public_key\":"
//       "\"09F8C96DA142602B8E1F731779AE1ECFD7467F61CBDE20E3A93DA2D8D8DEFCF3B8230404E03887F2E34BAE3CE0"
//       "C73DD8A80D18FCB4C10884B5D1368319868D583DEFD16D237755BF16D4D406A2F71BC685EB9E08CD7A8E049F3EC3"
//       "86D2E094A9A1E9C2E1494CE3DF488E5FE8995ABBDBEA490C1AFD2EB9E5AAD59812C16CB921D7A7FF663B40AE1C1B"
//       "A9D181E4AE5CF518CA5314D7859729D94AC457F01CF40F7DD7D3D22A515C1B5C28A4743CFC072D8222D8DB4862CC"
//       "8DBBDAC02357F243AF5F31C3790BD367C04E77105C2ACEE984D69E5D11BE02CB19271D80B1089EE33E977934A3FA"
//       "5A33C6D278CEE9607B38C084BE05284BAA344BB1B3D742C0E279D1085AA01F6CAA0D0C807AB85A7AB7EB4F042C60"
//       "D884748B494E2B9651543AFD5E619C5C458DB87357A2497A03DEDED1504FF849F9FB6115460D51E757E6FBF1B2E0"
//       "DD2D55EBA8C58E4A6D328462DB0FB4C8A0B174933E62FCF38E94CAAFA369A52565B7C8B9AFA627EAE4A928C0B5CA"
//       "3EE24EE4AB4C4EC2776961A2C84EC95C174C84CBC6E9F0E7A24887AE859735DE68D2266420188626FD01ABA55DAF"
//       "7E57306BABDC3FD7F00DBA16B8CC7893E448CEED67B3FF72B0682753FDF1DF066B798422A07B4B2FE9EDF0AC5D5E"
//       "FB29F530152DA4ADA4154218B93E73519B38E0F04D811C4050BBC28A575A4EEFAAF970CAEE3F1EE4575C929E53C6"
//       "ABF7D2870F485DA\",\"description_hash\":"
//       "\"7749DF5EC06636F64780798E90D7EBD6C6E9872695629E0B68F25E4E11C068F0\",\"crypto_base_hash\":"
//       "\"0FA1F1BFE97937F099FD39102D1FB452500F13AC12EFDE4E5FFD0D0D1C3896056\",\"crypto_extended_"
//       "base_hash\":\"0E92481188ACDBC604EC7B8563FBF4024063E874797415CC1E72C7BF5F0114BAD\"}");
//     auto descriptionHash =
//       ElementModQ::fromHex("7749DF5EC06636F64780798E90D7EBD6C6E9872695629E0B68F25E4E11C068F0");

//     // Act
//     auto fromJson = CiphertextElectionContext::fromJson(json);

//     // Assert
//     // TODO: validate against metadata->getDescriptionHash()
//     CHECK(fromJson->getDescriptionHash()->toHex() == descriptionHash->toHex());
// }

// TEST_CASE("Can serialize PlaintextBallot")
// {
//     // Arrange
//     auto metadata = getFakeMetadata(TWO_MOD_Q());
//     auto plaintext = getFakeBallot(*metadata);
//     auto json = plaintext->toJson();
//     auto bson = plaintext->toBson();

//     // Act
//     auto fromJson = PlaintextBallot::fromJson(json);
//     auto fromBson = PlaintextBallot::fromBson(bson);

//     // Assert
//     CHECK(plaintext->getObjectId() == plaintext->getObjectId());
// }
