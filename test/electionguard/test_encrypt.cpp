#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>
#include <electionguard/encrypt.hpp>

TEST_CASE("Encrypt")
{
    using namespace electionguard;

    // insantiate the stateful mediator
    auto *cpp_encrypter = new EncryptionMediator();
    CHECK(cpp_encrypter->encrypt() == 9);

    const auto *candidate_id = "some-candidate-id";

    // instantiate a selection description
    auto description = new SelectionDescription("some-description-object_id", candidate_id, 1UL);

    // instantiate a fake public key, hash, and nonce
    uint64_t pub[64] = {10};
    auto *public_key = new ElementModP(pub);

    uint64_t hash[4] = {9};
    auto *extended_base_hash = new ElementModQ(hash);

    uint64_t seed[4] = {4};
    auto *nonce_seed = new ElementModQ(seed);

    // instantiate a fake selection on a ballot
    auto *plaintext = new PlaintextBallotSelection(candidate_id, "1");

    // execute the encryption
    auto *ciphertext = encrypt_selection(plaintext, description, public_key, extended_base_hash,
                                         nonce_seed, false, true);
    CHECK(ciphertext != nullptr);

    // check the object id by accessing the property getter
    auto plaintext_object_id = plaintext->getObjectId();
    auto ciphertext_object_id = ciphertext->getObjectId();
    CHECK(string(plaintext_object_id) == string(ciphertext_object_id));

    // get out one of the ElementModQ hashes
    auto *description_hash = ciphertext->getDescriptionHash()->get();

    // the current test just arbitrarily assigns the vote to the hash
    CHECK(description_hash[0] == 1);

    delete cpp_encrypter;
    delete description;
    delete public_key;
    delete extended_base_hash;
    delete nonce_seed;
    delete plaintext;
    delete ciphertext;
}