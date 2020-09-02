#include "../../src/electionguard/log.hpp"
#include "../../src/kremlin/Hacl_Bignum256.h"
#include "../../src/kremlin/Hacl_Bignum4096.h"

#include <doctest/doctest.h>
#include <electionguard/hash.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

TEST_CASE("Test BigNum 4096 from and to bytes")
{
    uint8_t oneByte[1] = {1};
    uint64_t *bnResult = Hacl_Bignum4096_new_bn_from_bytes_be(sizeof(oneByte), oneByte);
    size_t bnLength = sizeof(bnResult) / sizeof(bnResult[0]);
    electionguard::Log::debug(bnResult, bnLength,
                              " : Test BigNum 4096 from and to bytes : bnResult =");

    uint8_t byteResult[512] = {};
    uint64_t bnResult4096[64] = {};
    memcpy(bnResult4096, bnResult, bnLength);
    Hacl_Bignum4096_bn_to_bytes_be(bnResult4096, byteResult);
    electionguard::Log::debug(byteResult, 512,
                              " : Test BigNum 4096 from and to bytes : byteResult =");
}

TEST_CASE("Test BigNum 256 from and to bytes")
{
    uint8_t oneByte[1] = {1};
    uint64_t *bnResult = Hacl_Bignum256_new_bn_from_bytes_be(sizeof(oneByte), oneByte);
    size_t bnLength = sizeof(bnResult) / sizeof(bnResult[0]);
    electionguard::Log::debug(bnResult, bnLength,
                              " : Test BigNum 256 from and to bytes : bnResult =");

    uint8_t byteResult[32] = {};
    uint64_t bnResult256[4] = {};
    memcpy(bnResult256, bnResult, bnLength);
    Hacl_Bignum256_bn_to_bytes_be(bnResult256, byteResult);
    electionguard::Log::debug(byteResult, 32,
                              " : Test BigNum 256 from and to bytes : byteResult =");
}

TEST_CASE("Test mod for BigNum 4096")
{
    uint64_t mod[64] = {0x06};
    electionguard::Log::debug(mod, 64, " : Test mod for BigNum 4096 : mod :");
    uint64_t a[64] = {0x0c};
    electionguard::Log::debug(a, 64, " : Test mod for BigNum 4096 : a :");
    uint64_t one[64] = {1UL};

    uint64_t resModQ[64] = {};
    Hacl_Bignum4096_mod_exp(mod, a, 64, one, resModQ);
    electionguard::Log::debug(resModQ, 64, " : Test mod for BigNum 4096 : resModQ :");
}

TEST_CASE("Test mod for BigNum 256")
{
    uint64_t mod[4] = {0x06};
    electionguard::Log::debug(mod, 4, " : Test mod for BigNum 256 : mod :");
    uint64_t a[4] = {0x0c};
    electionguard::Log::debug(a, 4, " : Test mod for BigNum 256 : a :");
    uint64_t one[4] = {1UL};

    uint64_t resModQ[4] = {};
    Hacl_Bignum256_mod_exp(mod, a, 4, one, resModQ);
    electionguard::Log::debug(resModQ, 4, " : Test mod for BigNum 256 : resModQ :");
}

TEST_CASE("Test less than BigNum 4096")
{
    uint64_t a[64] = {
      0xFFFFFFFFFFFFFF43, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    electionguard::Log::debug(a, 64, " : Test less than BigNum 4096 : a :");
    uint64_t b[64] = {
      0xFFFFFFFFFFFFFF45, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    electionguard::Log::debug(b, 64, " : Test less than BigNum 4096 : b :");

    auto isLessThan = Hacl_Bignum4096_lt(a, b);
    electionguard::Log::debug(" : Test less than BigNum 4096 : isLessThan : " +
                              to_string(isLessThan));
}

TEST_CASE("Test less than BigNum 256")
{
    uint64_t a[4] = {0xFFFFFFFFFFFFFF43, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                     0xFFFFFFFFFFFFFFFF};
    electionguard::Log::debug(a, 4, " : TEST LESS THAN COMP : a :");
    uint64_t b[4] = {0xFFFFFFFFFFFFFF45, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                     0xFFFFFFFFFFFFFFFF};
    electionguard::Log::debug(b, 4, " : TEST LESS THAN COMP : b :");

    auto isLessThan = Hacl_Bignum256_lt(a, b);
    electionguard::Log::debug(" : TEST LESS THAN COMP : isLessThan : " + to_string(isLessThan));
}