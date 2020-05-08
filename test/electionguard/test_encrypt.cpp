#include <doctest/doctest.h>
#include <electionguard/encrypt.hpp>

TEST_CASE("Encrypt")
{
    using namespace electionguard;

    auto cpp_encrypter = new electionguard::EncryptionCompositor();

    CHECK(cpp_encrypter->encrypt() == 9);
    CHECK(electionguard::encrypt_ballot() == 8);
}