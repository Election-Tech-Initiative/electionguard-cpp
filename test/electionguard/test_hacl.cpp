#include "../../src/electionguard/facades/Hacl_Bignum256.hpp"
#include "../../src/electionguard/facades/Hacl_Bignum4096.hpp"
#include "../../src/electionguard/log.hpp"
#include "utils/constants.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/hash.hpp>

using namespace electionguard;
using namespace hacl;

namespace electionguard
{
    template <typename T> T *decay(T &element) { return static_cast<T *>(element); }
    template <typename T> T *cast(const T &element) { return const_cast<T *>(element); }
} // namespace electionguard

#pragma region add

TEST_CASE("Hacl_Bignum256_add MAX_256 + 1 overflows")
{
    uint64_t factor[MAX_Q_LEN] = {0x01};

    uint64_t result[MAX_Q_LEN_DOUBLE] = {};
    uint64_t carry = Bignum256::add(const_cast<uint64_t *>(MAX_256), factor, result);
    CHECK(carry == 0x01);

    CHECK(result[0] == 0x00);
    CHECK(result[1] == 0x00);
    CHECK(result[2] == 0x00);
    CHECK(result[3] == 0x00);
}

TEST_CASE("Hacl_Bignum256_add MAX_256 + MAX_64 overflows")
{
    uint64_t factor[MAX_Q_LEN] = {0xffffffffffffffff};

    uint64_t result[MAX_Q_LEN_DOUBLE] = {};
    uint64_t carry = Bignum256::add(const_cast<uint64_t *>(MAX_256), factor, result);
    CHECK(carry == 0x01);

    CHECK(result[0] == 0xfffffffffffffffe);
    CHECK(result[1] == 0x00);
    CHECK(result[2] == 0x00);
    CHECK(result[3] == 0x00);
}

TEST_CASE("Hacl_Bignum256_add MAX_256 + MAX_256 overflows")
{
    uint64_t result[MAX_Q_LEN_DOUBLE] = {};
    uint64_t carry =
      Bignum256::add(const_cast<uint64_t *>(MAX_256), const_cast<uint64_t *>(MAX_256), result);
    CHECK(carry == 0x01);

    CHECK(result[0] == 0xfffffffffffffffe);
    CHECK(result[1] == 0xffffffffffffffff);
    CHECK(result[2] == 0xffffffffffffffff);
    CHECK(result[3] == 0xffffffffffffffff);
}

#pragma endregion

#pragma region subtract

TEST_CASE("Hacl_Bignum256_sub 0 - MAX_256 underflows")
{
    // Arrange
    uint64_t factor[MAX_Q_LEN] = {0x00};

    uint64_t result[MAX_Q_LEN_DOUBLE] = {};
    uint64_t carry = Bignum256::sub(factor, const_cast<uint64_t *>(MAX_256), result);
    CHECK(carry == 0x01);

    CHECK(result[0] == 0x01);
    CHECK(result[1] == 0x00);
    CHECK(result[2] == 0x00);
    CHECK(result[3] == 0x00);
}

TEST_CASE("Hacl_Bignum256_sub 1 - MAX_256 underflows")
{
    // Arrange
    uint64_t factor[MAX_Q_LEN] = {0x01};

    uint64_t result[MAX_Q_LEN_DOUBLE] = {};
    uint64_t carry = Bignum256::sub(factor, const_cast<uint64_t *>(MAX_256), result);
    CHECK(carry == 0x01);

    CHECK(result[0] == 0x02);
    CHECK(result[1] == 0x00);
    CHECK(result[2] == 0x00);
    CHECK(result[3] == 0x00);
}

TEST_CASE("Hacl_Bignum256_sub MAX_64 - MAX_256 underflows")
{
    // Arrange
    uint64_t factor[MAX_Q_LEN] = {0xffffffffffffffff};

    uint64_t result[MAX_Q_LEN_DOUBLE] = {};
    uint64_t carry = Bignum256::sub(factor, const_cast<uint64_t *>(MAX_256), result);
    CHECK(carry == 0x01);

    CHECK(result[0] == 0x00);
    CHECK(result[1] == 0x01);
    CHECK(result[2] == 0x00);
    CHECK(result[3] == 0x00);
}

TEST_CASE("Hacl_Bignum256_sub MAX_256 - MAX_256 == 0")
{
    // Arrange
    uint64_t factor[MAX_Q_LEN] = {0x00};

    uint64_t result[MAX_Q_LEN_DOUBLE] = {};
    uint64_t carry =
      Bignum256::sub(const_cast<uint64_t *>(MAX_256), const_cast<uint64_t *>(MAX_256), result);
    CHECK(carry == 0x00);

    CHECK(result[0] == 0x00);
    CHECK(result[1] == 0x00);
    CHECK(result[2] == 0x00);
    CHECK(result[3] == 0x00);
}

#pragma endregion

#pragma region multiply

#pragma endregion

#pragma region modulus

TEST_CASE("Hacl_Bignum256_mod Test mod for BigNum 256 invalid preconditions fails")
{
    uint64_t mod_valid[MAX_Q_LEN] = {0x05};
    uint64_t mod_invalid[MAX_Q_LEN] = {0x06};
    uint64_t a_valid[MAX_Q_LEN_DOUBLE] = {0x04};
    uint64_t a_invalid[MAX_Q_LEN_DOUBLE] = {0x1a};

    uint64_t result[MAX_Q_LEN] = {};

    // verify that n % 2 = 1
    bool n_is_odd = Bignum256::mod(mod_invalid, a_valid, result);
    CHECK(n_is_odd == false);

    // verify that 1 < n
    uint64_t mod_zero[MAX_Q_LEN] = {0x00};
    bool n_is_in_range = Bignum256::mod(mod_zero, a_valid, result);
    CHECK(n_is_in_range == false);

    // verify a < (n * n)
    bool a_is_in_range = Bignum256::mod(mod_valid, a_invalid, result);
    CHECK(a_is_in_range == false);
}

TEST_CASE("Hacl_Bignum256_mod Test mod for BigNum 256 valid preconditions succeeds")
{
    uint64_t mod[MAX_Q_LEN] = {0x09};
    uint64_t a[MAX_Q_LEN_DOUBLE] = {0xc};
    uint64_t expected[MAX_Q_LEN] = {0x03};

    // 12 % 9 = 3
    uint64_t result[MAX_Q_LEN] = {};
    bool success = Bignum256::mod(mod, a, result);
    CHECK(success == true);
    CHECK((*result == *expected));
}

#pragma endregion

#pragma region modular exponentiation

TEST_CASE("Hacl_Bignum4096_mod_exp Test mod exp for BigNum 4096 invalid preconditions fails")
{
    uint64_t mod_valid[MAX_P_LEN] = {0x05};
    uint64_t mod_invalid[MAX_P_LEN] = {0x06};
    uint64_t a_valid[MAX_P_LEN] = {0x04};
    uint64_t a_invalid[MAX_P_LEN] = {0x0a};
    uint64_t b_valid[MAX_P_LEN] = {0x01};
    uint64_t b_invalid[MAX_P_LEN] = {0x00};

    uint64_t result[MAX_P_LEN] = {};

    // verify that n % 2 = 1
    bool n_is_odd = Bignum4096::modExp(mod_invalid, a_valid, MAX_P_SIZE, b_valid, result);
    CHECK(n_is_odd == false);

    // verify that 1 < n
    uint64_t mod_zero[MAX_P_LEN] = {0x00};
    bool n_is_in_range = Bignum4096::modExp(mod_zero, a_valid, MAX_P_SIZE, b_valid, result);
    CHECK(n_is_in_range == false);

    // verify that 0 < b
    bool b_is_in_range = Bignum4096::modExp(mod_valid, a_valid, MAX_P_SIZE, b_invalid, result);
    CHECK(b_is_in_range == false);

    // TODO: verify b < pow2 bBits

    // verify a < n
    bool a_is_in_range = Bignum4096::modExp(mod_valid, a_invalid, MAX_P_SIZE, b_valid, result);
    CHECK(a_is_in_range == false);
}

TEST_CASE("Hacl_Bignum4096_mod_exp Test mod exp for BigNum 4096 valid preconditions succeeds")
{
    uint64_t mod[MAX_P_LEN] = {0x09};
    uint64_t a[MAX_P_LEN] = {0x04};
    uint64_t b[MAX_P_LEN] = {0x02};
    uint64_t expected[MAX_P_LEN] = {0x07};

    // 4 ^ 2 % 9 = 7
    uint64_t result[MAX_P_LEN] = {};
    bool success = Bignum4096::modExp(mod, a, MAX_Q_SIZE, b, result);
    CHECK(success == true);
    CHECK((*result == *expected));
}

TEST_CASE("Hacl_Bignum256_mod_exp Test mod exp for BigNum 256 invalid preconditions fails")
{
    uint64_t mod_valid[MAX_Q_LEN] = {0x05};
    uint64_t mod_invalid[MAX_Q_LEN] = {0x06};
    uint64_t a_valid[MAX_Q_LEN] = {0x04};
    uint64_t a_invalid[MAX_Q_LEN] = {0x0a};
    uint64_t b_valid[MAX_Q_LEN] = {0x01};
    uint64_t b_invalid[MAX_Q_LEN] = {0x00};

    uint64_t result[MAX_Q_LEN] = {};

    // verify that n % 2 = 1
    bool n_is_odd = Bignum256::modExp(mod_invalid, a_valid, MAX_Q_SIZE, b_valid, result);
    CHECK(n_is_odd == false);

    // verify that 1 < n
    uint64_t mod_zero[MAX_Q_LEN] = {0x00};
    bool n_is_in_range = Bignum256::modExp(mod_zero, a_valid, MAX_Q_SIZE, b_valid, result);
    CHECK(n_is_in_range == false);

    // verify that 0 < b
    bool b_is_in_range = Bignum256::modExp(mod_valid, a_valid, MAX_Q_SIZE, b_invalid, result);
    CHECK(b_is_in_range == false);

    // TODO: verify b < pow2 bBits

    // verify a < n
    bool a_is_in_range = Bignum256::modExp(mod_valid, a_invalid, MAX_Q_SIZE, b_valid, result);
    CHECK(a_is_in_range == false);
}

TEST_CASE("Hacl_Bignum256_mod_exp Test mod exp for BigNum 256 valid preconditions succeeds")
{
    uint64_t mod[MAX_Q_LEN] = {0x09};
    uint64_t a[MAX_Q_LEN] = {0x04};
    uint64_t b[MAX_Q_LEN] = {0x02};
    uint64_t expected[MAX_Q_LEN] = {0x07};

    // 4 ^ 2 % 9 = 7
    uint64_t result[MAX_Q_LEN] = {};
    bool success = Bignum256::modExp(mod, a, MAX_Q_SIZE, b, result);
    CHECK(success == true);
    CHECK((*result == *expected));
}

#pragma endregion

#pragma region Loads and Stores

TEST_CASE("Hacl_Bignum4096_new_bn_from_bytes_be Test BigNum 4096 from and to bytes")
{
    uint8_t oneByte[MAX_P_SIZE] = {1};
    uint64_t *bnResult = Bignum4096::fromBytes(sizeof(oneByte), oneByte);

    uint8_t byteResult[MAX_P_SIZE] = {};
    Bignum4096::toBytes(bnResult, byteResult);

    CHECK(byteResult[0] == oneByte[0]);

    free(bnResult);
}

TEST_CASE("Hacl_Bignum256_new_bn_from_bytes_be Test BigNum 256 from and to bytes")
{
    uint8_t oneByte[MAX_Q_SIZE] = {1};
    uint64_t *bnResult = Bignum256::fromBytes(sizeof(oneByte), oneByte);

    uint8_t byteResult[MAX_Q_SIZE] = {};
    Bignum256::toBytes(bnResult, byteResult);

    CHECK(byteResult[0] == oneByte[0]);

    free(bnResult);
}

#pragma endregion

#pragma region Comparisons

TEST_CASE("Hacl_Bignum4096_lt_mask Test less than BigNum 4096")
{
    const uint64_t a[MAX_P_LEN] = {
      0xFFFFFFFFFFFFFF43, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};
    const uint64_t b[MAX_P_LEN] = {
      0xFFFFFFFFFFFFFF45, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};

    auto isLessThan =
      Bignum4096::lessThan(const_cast<uint64_t *>(a), const_cast<uint64_t *>(b)) > 0;
    CHECK(isLessThan);
}

TEST_CASE("Test less than BigNum 256")
{

    const uint64_t larger_than_q[MAX_Q_LEN] = {0xFFFFFFFFFFFFFF45, 0xFFFFFFFFFFFFFFFF,
                                               0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};

    auto isLessThan = Bignum256::lessThan(Q().get(), const_cast<uint64_t *>(larger_than_q)) > 0;
    CHECK(isLessThan);
}

#pragma endregion