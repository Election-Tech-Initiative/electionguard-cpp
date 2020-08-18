#include <doctest/doctest.h>
#include <electionguard/ballot.hpp>
#include <electionguard/encrypt.hpp>

TEST_CASE("Encrypt")
{
    using namespace electionguard;

    auto cpp_encrypter = new EncryptionMediator();

    CHECK(cpp_encrypter->encrypt() == 9);

    auto selection = new PlaintextBallotSelection("some-unique-id", "1");
    CHECK(encrypt_selection(selection) == 1);

    delete cpp_encrypter;
    delete selection;
}