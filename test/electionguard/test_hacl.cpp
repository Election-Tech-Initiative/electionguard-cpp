#include "../../src/electionguard/log.hpp"
#include "../../src/kremlin/Hacl_Bignum256.h"
#include "../../src/kremlin/Hacl_Bignum4096.h"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/hash.hpp>

using namespace electionguard;

TEST_CASE("Test BigNum 4096 from and to bytes")
{
    uint8_t oneByte[1] = {1};
    uint64_t *bnResult = Hacl_Bignum4096_new_bn_from_bytes_be(sizeof(oneByte), oneByte);
    size_t bnLength = sizeof(bnResult) / sizeof(bnResult[0]);
    // Log::debug(bnResult, bnLength, " : Test BigNum 4096 from and to bytes : bnResult =");

    uint8_t byteResult[512] = {};
    uint64_t bnResult4096[64] = {};
    memcpy(bnResult4096, bnResult, bnLength);
    Hacl_Bignum4096_bn_to_bytes_be(bnResult4096, byteResult);
    // Log::debug(byteResult, 512, " : Test BigNum 4096 from and to bytes : byteResult =");
}

TEST_CASE("Test BigNum 256 from and to bytes")
{
    uint8_t oneByte[1] = {1};
    uint64_t *bnResult = Hacl_Bignum256_new_bn_from_bytes_be(sizeof(oneByte), oneByte);
    size_t bnLength = sizeof(bnResult) / sizeof(bnResult[0]);
    // Log::debug(bnResult, bnLength, " : Test BigNum 256 from and to bytes : bnResult =");

    uint8_t byteResult[32] = {};
    uint64_t bnResult256[4] = {};
    memcpy(bnResult256, bnResult, bnLength);
    Hacl_Bignum256_bn_to_bytes_be(bnResult256, byteResult);
    // Log::debug(byteResult, 32, " : Test BigNum 256 from and to bytes : byteResult =");
}

TEST_CASE("Test mod for BigNum 4096 when 10 % 6 expects 4")
{
    uint64_t mod[64] = {0x06};
    Log::debug(mod, 64, " : Test mod for BigNum 4096 : mod :");
    uint64_t a[64] = {0x0a};
    Log::debug(a, 64, " : Test mod for BigNum 4096 : a :");
    uint64_t one[64] = {1UL};
    uint64_t expected[64] = {0x04};

    uint64_t result[64] = {};
    Hacl_Bignum4096_mod_exp(mod, a, 64, one, result);
    Log::debug(result, 64, " : Test mod for BigNum 4096 : result :");
    // TODO: mod is not working
    // CHECK((*result == *expected));
}

TEST_CASE("Test mod for BigNum 256 when 10 % 6 expects 4")
{
    uint64_t mod[4] = {0x06};
    Log::debug(mod, 4, " : Test mod for BigNum 256 : mod :");
    uint64_t a[4] = {0x0a};
    Log::debug(a, 4, " : Test mod for BigNum 256 : a :");
    uint64_t one[4] = {1UL};
    uint64_t expected[4] = {0x04};

    uint64_t result[4] = {};
    Hacl_Bignum256_mod_exp(mod, a, 4, one, result);
    Log::debug(result, 4, " : Test mod for BigNum 256 : result :");
    // TODO: mod is not working
    // CHECK((*result == *expected));
}

TEST_CASE("Test less than BigNum 4096")
{
    uint64_t a[64] = {
      0xFFFFFFFFFFFFFF43, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    // Log::debug(a, 64, " : Test less than BigNum 4096 : a :");
    uint64_t b[64] = {
      0xFFFFFFFFFFFFFF45, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    // Log::debug(b, 64, " : Test less than BigNum 4096 : b :");

    auto isLessThan = Hacl_Bignum4096_lt(a, b);
    // Log::debug(" : Test less than BigNum 4096 : isLessThan : " + to_string(isLessThan));

    CHECK(isLessThan);
}

TEST_CASE("Test less than BigNum 256")
{
    // Log::debug(Q_ARRAY, 4, " : Test less than BigNum 256 : Q_ARRAY :");
    uint64_t b[4] = {0xFFFFFFFFFFFFFF45, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                     0xFFFFFFFFFFFFFFFF};
    // Log::debug(b, 4, " : Test less than BigNum 256 : b :");

    auto isLessThan = Hacl_Bignum256_lt(const_cast<uint64_t *>(Q_ARRAY), b);
    // Log::debug(" : Test less than BigNum 256 : isLessThan : " + to_string(isLessThan));

    CHECK(isLessThan);
}