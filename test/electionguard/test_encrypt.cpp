#include <doctest/doctest.h>
#include <electionguard/encrypt.hpp>

TEST_CASE("Encrypt")
{
    using namespace electionguard;

    auto cpp_encrypter = new EncryptionCompositor();

    CHECK(cpp_encrypter->encrypt() == 9);
    CHECK(encrypt_ballot() == 8);
}