#include <assert.h>
#include <electionguard/ballot.h>
#include <electionguard/ballot.hpp>
#include <electionguard/encrypt.h>
#include <electionguard/encrypt.hpp>
#include <electionguard/tracker.hpp>

using namespace electionguard;

bool test_encrypt_selection();
bool test_generate_tracking_code();

int main()
{
    assert(test_encrypt_selection() == true);
    assert(test_generate_tracking_code() == true);
}

bool test_encrypt_selection()
{
    // Test the C API

    // insantiate the stateful mediator
    eg_encryption_mediator_t *encrypter = eg_encryption_mediator_new();
    int instance_encrypt = eg_encryption_mediator_encrypt(encrypter);
    assert(instance_encrypt == 9);

    // Test the C++ API
    auto *candidate_id = "some-candidate-id";

    // insantiate the stateful mediator
    auto *cpp_encrypter = new EncryptionMediator();
    auto cpp_instance_encrypt = cpp_encrypter->encrypt();
    assert(cpp_instance_encrypt == 9);

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
    assert(ciphertext != nullptr);

    // check the object id by accessing the property getter
    auto plaintext_object_id = plaintext->getObjectId();
    auto ciphertext_object_id = ciphertext->getObjectId();
    assert(string(plaintext_object_id) == string(ciphertext_object_id));

    // get out one of the ElementModQ hashes
    auto *description_hash = ciphertext->getDescriptionHash()->get();

    // the current test just arbitrarily assigns the vote to the hash
    assert(description_hash[0] == 1);

    delete cpp_encrypter;
    delete description;
    delete public_key;
    delete extended_base_hash;
    delete nonce_seed;
    delete plaintext;
    delete ciphertext;

    return true;
}

bool test_generate_tracking_code()
{
    // generate a device hash
    auto *deviceHash = Tracker::getHashForDevice(1234, "some-location-string");
    uint64_t firstHash[4] = {1, 2, 3, 4};
    auto firstBallotHash = new ElementModQ(firstHash);
    uint64_t secondHash[4] = {2, 3, 4, 5};
    auto secondBallotHash = new ElementModQ(secondHash);

    // demonstrate using the rotating tracker hash api
    auto *rotatingHash1 = Tracker::getRotatingTrackerHash(deviceHash, 1000, firstBallotHash);
    auto *rotatingHash2 = Tracker::getRotatingTrackerHash(rotatingHash1, 1001, secondBallotHash);

    //Assert
    assert((*rotatingHash1 != *deviceHash));
    assert(&rotatingHash1 != &deviceHash);

    delete deviceHash;
    delete firstBallotHash;
    delete secondBallotHash;
    delete rotatingHash1;
    delete rotatingHash2;

    return true;
}
