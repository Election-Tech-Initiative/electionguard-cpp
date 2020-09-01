#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/encrypt.hpp>

// TEST_CASE("Encrypt")
// {
//     using namespace electionguard;

//     auto *cpp_encrypter = new EncryptionMediator();
//     CHECK(cpp_encrypter->encrypt() == 9);

//     auto *plaintext = new PlaintextBallotSelection("some-unique-id", "1");
//     auto *ciphertext = encrypt_selection(plaintext);

//     uint64_t *result = ciphertext->getDescriptionHash()->get();

//     CHECK(result[0] == 1);

//     delete cpp_encrypter;
//     delete plaintext;
// }