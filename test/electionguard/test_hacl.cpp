#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/hash.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

extern "C" {
#include "../../src/kremlin/Hacl_Bignum4096.h"
}

TEST_CASE("Test Hacl BigNum")
{
    uint8_t oneByte[8] = {0, 0, 0, 0, 0, 0, 0, 1};
    uint64_t *bnResult = Hacl_Bignum4096_new_bn_from_bytes_be(sizeof(oneByte), oneByte);
    size_t bnLength = sizeof(bnResult) / sizeof(bnResult[0]);
    // electionguard::Log::debug(bnResult, bnLength, " : Test Hacl BigNum : bnResult =");

    uint8_t byteResult[512] = {};
    uint64_t bnResult4096[64] = {};
    memcpy(bnResult4096, bnResult, bnLength);
    Hacl_Bignum4096_bn_to_bytes_be(bnResult4096, byteResult);
    // electionguard::Log::debug(byteResult, 512, " : Test Hacl BigNum : byteResult =");
}

TEST_CASE("TEST MOD")
{
    uint64_t mod[64UL] = {0x06};
    // electionguard::Log::debug(mod, 64, " : TEST MOD : mod :");
    uint64_t a[64UL] = {0x0c};
    // electionguard::Log::debug(a, 64, " : TEST MOD : a :");
    uint64_t one[64UL] = {1UL};

    uint64_t resModQ[64UL] = {};
    Hacl_Bignum4096_mod_exp(mod, a, 64UL, one, resModQ);
    // electionguard::Log::debug(resModQ, 64, " : TEST MOD : resModQ :");
}

TEST_CASE("TEST LESS THAN COMP")
{
    uint64_t a[64UL] = {0xFFFFFFFFFFFFFF43, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                        0xFFFFFFFFFFFFFFFF};
    // electionguard::Log::debug(a, 64, " : TEST LESS THAN COMP : a :");
    uint64_t b[64UL] = {0xFFFFFFFFFFFFFF45, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                        0xFFFFFFFFFFFFFFFF};
    // electionguard::Log::debug(b, 64, " : TEST LESS THAN COMP : b :");

    auto isLessThan = Hacl_Bignum4096_lt(a, b);
    // electionguard::Log::debug(" : TEST LESS THAN COMP : isLessThan : " + to_string(isLessThan));
}