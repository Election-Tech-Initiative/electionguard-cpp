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
    assert(encrypter != nullptr);

    // Test the C++ API
    auto *candidateId = "some-candidate-id";

    // insantiate the stateful mediator
    auto *cpp_encrypter = new EncryptionMediator();
    assert(cpp_encrypter != nullptr);

    // instantiate a selection description
    auto metadata = new SelectionDescription("some-description-object_id", candidateId, 1UL);
    auto *hashContext = metadata->crypto_hash();

    // instantiate a fake public key, hash, and nonce
    uint64_t pub[64] = {10};
    auto *publicKey = new ElementModP(pub);

    uint64_t hash[4] = {9};
    auto *extendedBaseHash = new ElementModQ(hash);

    uint64_t seed[4] = {4};
    auto *nonceSeed = new ElementModQ(seed);

    // instantiate a fake selection on a ballot
    auto *plaintext = new PlaintextBallotSelection(candidateId, "1");

    // execute the encryption
    auto *result =
      encrypt_selection(plaintext, metadata, publicKey, extendedBaseHash, nonceSeed, false, true);

    // check some outputs
    assert(result != nullptr);
    assert(result->getCiphertext() != nullptr);
    assert(result->isValidEncryption(hashContext, publicKey, extendedBaseHash) == true);

    delete cpp_encrypter;
    delete metadata;
    delete publicKey;
    delete extendedBaseHash;
    delete nonceSeed;
    delete plaintext;
    delete result;

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
