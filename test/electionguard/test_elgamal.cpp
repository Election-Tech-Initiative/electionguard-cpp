#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>

using namespace electionguard;

TEST_CASE("elgamalEncrypt simple with nonce is 1, publickey is g_pow_p(2)")
{
    auto *one = uint64_to_q(1UL);
    auto *two = uint64_to_p(2UL);
    auto *publicKey = g_pow_p(two);
    auto *g = new ElementModP(const_cast<uint64_t *>(G_ARRAY), true);

    auto *cipherText = elgamalEncrypt(0UL, one, publicKey);

    // Log::debug(" : cipherText->getData()->toHex() = " + cipherText->getData()->toHex());
    // Log::debug(" : cipherText->getPad()->toHex() = " + cipherText->getPad()->toHex());
    CHECK((*publicKey == *cipherText->getData()));
    CHECK((*g == *cipherText->getPad()));
}