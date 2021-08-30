#include "../../src/electionguard/convert.hpp"
#include "../../src/electionguard/facades/Hacl_Bignum256.hpp"
#include "../../src/electionguard/log.hpp"
#include "utils/byte_logger.hpp"
#include "utils/constants.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/group.hpp>
#include <exception>
#include <iostream>
#include <string>

using namespace electionguard;
using namespace std;
using namespace hacl;

#pragma region add_mod_q

TEST_CASE("add_mod_q for ints 1 and 1 should return q of 2")
{
    // Arrange
    auto one1 = ElementModQ::fromUint64(1UL);
    auto one2 = ElementModQ::fromHex("01");
    auto two = ElementModQ::fromUint64(2);

    // Act
    auto result = add_mod_q(*one1, *one2);

    // Assert
    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_q (Q + 0) % Q = 0")
{
    // Arrange
    auto zero = ElementModQ::fromUint64(0UL);

    // Act
    auto result = add_mod_q(Q(), *zero);

    // Assert
    CHECK((*result == *zero));
    CHECK(result->toHex() == zero->toHex());
}

TEST_CASE("add_mod_q (Q + 1) % Q = 1")
{
    // Arange
    auto one = ElementModQ::fromUint64(1UL);

    // Act
    auto result = add_mod_q(Q(), *one);

    // Assert
    CHECK((*result == *one));
    CHECK(result->toHex() == one->toHex());
}

TEST_CASE("add_mod_q (Q + 2) % Q = 2")
{
    // Arrange
    auto two = ElementModQ::fromUint64(2UL);

    // Act
    auto result = add_mod_q(Q(), *two);

    // Assert
    CHECK((*result == *two));
    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_q (Q + 188) % Q = 188")
{
    // Arrange
    const auto &q = Q();
    auto residue = ElementModQ::fromUint64(188UL);

    // Act
    auto result = add_mod_q(q, *residue);

    // Assert
    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q (Q + 189) % Q = 189")
{
    // Arrange
    const auto &q = Q();
    auto residue = ElementModQ::fromUint64(189UL);

    // Act
    auto result = add_mod_q(q, *residue);

    // Assert
    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

#ifdef USE_STANDARD_PRIMES

// these tests are only valid when testing the standard primes

TEST_CASE("add_mod_q (MAX_256 + 0) % Q = offset")
{
    // Arrange
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto zero = ElementModQ::fromUint64(0UL);
    auto offset = make_unique<ElementModQ>(Q_ARRAY_INVERSE_OFFSET, true);

    // Act
    auto result = add_mod_q(*max, *zero);

    // Assert
    CHECK((*result == *offset));
    CHECK(result->toHex() == offset->toHex());
}

TEST_CASE("add_mod_q (MAX_256 + 1) % Q = offset + 1")
{
    // Arrange
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto one = ElementModQ::fromUint64(1UL);
    auto offset = make_unique<ElementModQ>(Q_ARRAY_INVERSE_OFFSET, true);
    auto _ = Bignum256::add(offset->get(), ONE_MOD_Q().get(), offset->get());

    // Act
    auto result = add_mod_q(*max, *one);

    // Assert
    CHECK((*result == *offset));
    CHECK(result->toHex() == offset->toHex());
}

TEST_CASE("add_mod_q (MAX_256 + MAX_256) % Q = 378")
{
    // Arrange
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto residue = ElementModQ::fromUint64(378UL);

    // Act
    auto result = add_mod_q(*max, *max);

    // Assert
    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q ((MAX_256 - 1) + MAX_256) % Q = 377")
{
    // Arrange
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto maxMinusOne = make_unique<ElementModQ>(MAX_256_MINUS_ONE, true);
    auto residue = ElementModQ::fromUint64(377UL);

    // Act
    auto result = add_mod_q(*maxMinusOne, *max);

    // Assert
    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q ((MAX_256 - 1) + (MAX_256 - 1)) % Q = 376")
{
    // Arrange
    auto maxMinusOne = make_unique<ElementModQ>(MAX_256_MINUS_ONE, true);
    auto residue = ElementModQ::fromUint64(376UL);

    // Act
    auto result = add_mod_q(*maxMinusOne, *maxMinusOne);

    // Assert
    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

#endif

#pragma endregion

#pragma region a_minus_b_mod_q

TEST_CASE("a_minus_b_mod_q (2 - 1) % Q == 1 and (1 - 1) & Q == 0")
{
    // Arrange
    auto one = ElementModQ::fromUint64(1);
    auto two = ElementModQ::fromUint64(2);
    auto zero = ElementModQ::fromUint64(0);

    // Act
    auto result1 = sub_mod_q(*two, *one);
    auto result0 = sub_mod_q(*one, *one);

    // Assert
    CHECK((*result1 == *one));
    CHECK(result1->toHex() == one->toHex());
    CHECK((*result0 == *zero));
    CHECK(result0->toHex() == zero->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (Q - Q) % Q == 0")
{
    // Arrange
    const auto &q = Q();
    auto residue = ElementModQ::fromUint64(0UL);

    // Act
    auto result = sub_mod_q(q, q);

    // Assert
    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (0 - Q) % Q == 0")
{
    // Arrange
    string expectedHex("00");
    auto zero = ElementModQ::fromUint64(0UL);
    const auto &q = Q();

    // Act
    auto result = sub_mod_q(*zero, q);

    // Assert
    CHECK((*result == *zero));
    CHECK(result->toHex() == zero->toHex());
}

#ifdef USE_STANDARD_PRIMES

TEST_CASE("a_minus_b_mod_q:  (Q - MAX_256) % Q == (Q - 189)")
{
    // Arrange
    string expectedHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE86");

    const auto &q = Q();
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto offset = ElementModQ::fromUint64(189UL);
    auto residue = sub_mod_q(q, *offset);
    CHECK(residue->toHex() == expectedHex);

    // Act
    auto result = sub_mod_q(q, *max);

    // Assert
    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  ((Q + 1) - MAX_256)) % Q == (Q - 188)")
{
    // Arrange
    string expectedHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE87");

    auto qplusone = make_unique<ElementModQ>(Q_PLUS_ONE, true);
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto offset = ElementModQ::fromUint64(188UL);
    auto residue = sub_mod_q(Q(), *offset);
    CHECK(residue->toHex() == expectedHex);

    // Act
    auto result = sub_mod_q(*qplusone, *max);

    // Assert
    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (Q - (MAX_256 - 1)) % Q == (Q - 188)")
{
    // Arrange
    string expectedHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE87");

    const auto &q = Q();
    auto max = make_unique<ElementModQ>(MAX_256_MINUS_ONE, true);
    auto offset = ElementModQ::fromUint64(188UL);
    auto residue = sub_mod_q(q, *offset);
    CHECK(residue->toHex() == expectedHex);

    // Act
    auto result = sub_mod_q(q, *max);

    // Assert
    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  ((Q + 1) - (MAX_256 - 1)) % Q == (Q - 187)")
{
    // Arrange
    string expectedHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE88");

    auto qplusone = make_unique<ElementModQ>(Q_PLUS_ONE, true);
    auto max = make_unique<ElementModQ>(MAX_256_MINUS_ONE, true);
    auto offset = ElementModQ::fromUint64(187UL);
    auto residue = sub_mod_q(Q(), *offset);
    CHECK(residue->toHex() == expectedHex);

    // Act
    auto result = sub_mod_q(*qplusone, *max);

    // Assert
    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

#endif

TEST_CASE("a_minus_b_mod_q:  (1 - Q) % Q == 1")
{
    // Arrange
    string expectedHex("01");
    auto one = ElementModQ::fromUint64(1UL);
    const auto &q = Q();

    // Act
    auto result = sub_mod_q(*one, q);

    // Assert
    CHECK((*result == *one));
    CHECK(result->toHex() == one->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (1 - 2) % Q == (Q - 1)")
{
    // Arange
    auto one = ElementModQ::fromUint64(1UL);
    auto two = ElementModQ::fromUint64(2UL);
    auto zero = ElementModQ::fromUint64(0UL);

    // Act
    auto expected = sub_mod_q(Q(), *one);
    auto result = sub_mod_q(*one, *two);

    // Assert
    CHECK((*result == *expected));
    CHECK(result->toHex() == expected->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (10 - 15) % Q == (Q - 5)")
{
    //Arrange
    auto ten = ElementModQ::fromUint64(10UL);
    auto fifteen = ElementModQ::fromUint64(15UL);
    auto five = ElementModQ::fromUint64(5UL);

    // Act
    auto expected = sub_mod_q(Q(), *five);
    auto result = sub_mod_q(*ten, *fifteen);

    // Assert
    CHECK((*result == *expected));
    CHECK(result->toHex() == expected->toHex());
}

#pragma endregion

#pragma region a_plus_bc_mod_q

TEST_CASE("a_plus_bc_mod_q for 1 + (2 x 3) % Q = 7")
{
    // Arrange
    auto one = ElementModQ::fromUint64(1);
    auto two = ElementModQ::fromUint64(2);
    auto three = ElementModQ::fromUint64(3);
    auto seven = ElementModQ::fromUint64(7);

    // Act
    auto result = a_plus_bc_mod_q(*one, *two, *three);

    // Assert
    CHECK((*result == *seven));
    CHECK(result->toHex() == seven->toHex());
}

TEST_CASE("negate_mod_q for MAX Q is 0")
{
    // Arrange
    auto maxQ = make_unique<ElementModQ>(Q_ARRAY_REVERSE);
    auto zero = ElementModQ::fromUint64(0);

    // Act
    auto result = sub_from_q(*maxQ);

    // Assert
    CHECK((*result == *zero));
}

#pragma endregion

#pragma region add_mod_p

TEST_CASE("add_mod_p for ints 1 and 1 should return q of 2")
{
    // Arrange
    auto one1 = ElementModP::fromUint64(1UL);
    auto one2 = ElementModP::fromHex("01");
    auto two = ElementModP::fromUint64(2);

    // Act
    auto result = add_mod_p(*one1, *one2);

    // Assert
    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_p for two huge P arrays")
{
    // Arrange
    auto p1 = make_unique<ElementModP>(LARGE_P_ARRAY_1, true);
    auto p2 = make_unique<ElementModP>(LARGE_P_ARRAY_2, true);

    // Act
    auto result = add_mod_p(*p1, *p2);

    // Assert
    CHECK(p1->toHex() != "00");
    CHECK(p2->toHex() != "00");
    CHECK(result->toHex() != "00");
}

TEST_CASE("add_mod_p (P + 0) % P = 0")
{
    // Arrange
    auto zero = ElementModP::fromUint64(0UL);

    // Act
    auto result = add_mod_p(P(), *zero);

    // Assert
    CHECK((*result == *zero));
    CHECK(result->toHex() == zero->toHex());
}

TEST_CASE("add_mod_p (P + 1) % P = 1")
{
    // Arrange
    auto one = ElementModP::fromUint64(1UL);

    // Act
    auto result = add_mod_p(P(), *one);

    // Assert
    CHECK((*result == *one));
    CHECK(result->toHex() == one->toHex());
}

TEST_CASE("add_mod_p (P + 2) % P = 2")
{
    // Arrange
    auto two = ElementModP::fromUint64(2UL);

    // Act
    auto result = add_mod_p(P(), *two);

    // Assert
    CHECK((*result == *two));
    CHECK(result->toHex() == two->toHex());
}

#ifdef USE_STANDARD_PRIMES

TEST_CASE("add_mod_p (MAX_4096 + 0) % P = offset")
{
    // Arrange
    auto max = make_unique<ElementModP>(MAX_4096, true);
    auto zero = ElementModP::fromUint64(0UL);
    auto offset = make_unique<ElementModP>(P_ARRAY_INVERSE_OFFSET, true);

    // Act
    auto result = add_mod_p(*max, *zero);

    // Assert
    CHECK((*result == *offset));
    CHECK(result->toHex() == offset->toHex());
}

TEST_CASE("add_mod_p (MAX_4096 + 1) % P = offset + 1")
{
    // Arrange
    auto max = make_unique<ElementModP>(MAX_4096, true);
    auto one = ElementModP::fromUint64(1UL);
    auto offset = make_unique<ElementModP>(P_ARRAY_INVERSE_OFFSET, true);
    auto _ = Bignum4096::add(offset->get(), ONE_MOD_P().get(), offset->get());

    // Act
    auto result = add_mod_p(*max, *one);

    // Assert
    CHECK((*result == *offset));
    CHECK(result->toHex() == offset->toHex());
}

#endif

#pragma endregion

#pragma region mul_mod_p

TEST_CASE("mul_mod_p 3 * 3 should equal 9")
{
    // Arrange
    auto three = ElementModP::fromUint64(3UL);

    // Act
    auto p = mul_mod_p(*three, *three);

    // Assert
    CHECK(p->toHex() == "09");
}

#ifdef USE_STANDARD_PRIMES

TEST_CASE("mul_mod_p for max uint64 * max uint64 should equal hex value "
          "FFFFFFFFFFFFFFFE0000000000000001)")
{
    // Arrange
    uint64_t uint64Max = 0xffffffffffffffff;
    auto max = ElementModP::fromUint64(uint64Max, true);

    // Act
    auto p = mul_mod_p(*max, *max);

    // Assert
    CHECK(p->toHex() == "FFFFFFFFFFFFFFFE0000000000000001");
}

#endif

#pragma endregion

#pragma region pow_mod_p

TEST_CASE("pow_mod_p 2 ^ 3 = 8 and 3 ^ 2 = 9")
{
    // Arrange
    auto two = ElementModP::fromUint64(2);
    auto three = ElementModP::fromUint64(3);
    auto eight = ElementModP::fromUint64(8);
    auto nine = ElementModP::fromUint64(9);

    // Act
    auto result8 = pow_mod_p(*two, *three);
    auto result9 = pow_mod_p(*three, *two);

    // Assert
    CHECK((*result8 == *eight));
    CHECK((*result9 == *nine));
}

#pragma endregion

#pragma region g_pow_p

TEST_CASE("Test g_pow_p with 0, 1, and 2")
{
    // Arrange
    auto zero = ElementModP::fromUint64(0);
    auto one = ElementModP::fromUint64(1);
    auto two = ElementModP::fromUint64(2);

    // Act
    auto result0 = g_pow_p(*zero);
    auto result1 = g_pow_p(*one);
    auto result2 = g_pow_p(*two);

    // Assert
    CHECK((*result0 == *one));
    CHECK((*result1 == G()));

    CHECK((*result2 != *one));
    CHECK((*result2 != G()));
}

#pragma endregion

#pragma region Q Misc

TEST_CASE("Max of Q + 1 throws")
{
    // Arrange
    uint64_t q_plus_one[MAX_Q_LEN];
    std::copy(Q_ARRAY_REVERSE, Q_ARRAY_REVERSE + MAX_Q_LEN, q_plus_one);

    // Act
    auto _ = Bignum256::add(q_plus_one, ONE_MOD_Q().get(), q_plus_one);

    // Assert
    CHECK_THROWS_WITH(new ElementModQ(q_plus_one, false),
                      "Value for ElementModQ is greater than allowed");
}

TEST_CASE("Hex string converted to Q matches original hex when converted back toHex")
{
    // Arrange
    string expectedHex("F0F0F0F0F0F0F0F0");

    // Act
    auto q = ElementModQ::fromHex(expectedHex, true);

    // Assert
    CHECK(q->toHex() == expectedHex);
}

TEST_CASE("Test Q is converted correctly")
{
    // Arrange
    auto converted_bytes = bytes_to_hex(Q().toBytes());
    auto expected = bignum_to_hex_string(const_cast<uint64_t *>(Q_ARRAY), MAX_Q_LEN);

    // Act
    auto converted_hex = ElementModQ::fromHex(expected, true);
    auto converted_array = make_unique<ElementModQ>(Q_ARRAY_REVERSE, true);

    // Assert
    CHECK(Q().toHex() == expected);
    CHECK(converted_bytes == expected);
    CHECK(converted_hex->toHex() == expected);
    CHECK(converted_array->toHex() == expected);

    CHECK((*converted_hex == *converted_array));
}

#pragma endregion

#pragma region P Misc

TEST_CASE("Test P is converted correctly")
{
    // Arrange
    auto convert_bytes = bytes_to_hex(P().toBytes());
    auto expected = bignum_to_hex_string(const_cast<uint64_t *>(P_ARRAY), MAX_P_LEN);

    // Act
    auto result = ElementModP::fromHex(expected, true);

    // Assert
    CHECK((P().toHex() == expected));
    CHECK(convert_bytes == expected);
    CHECK(result->toHex() == expected);
}

TEST_CASE("Test R is converted correctly")
{
    // Arrange
    auto convert_bytes = bytes_to_hex(R().toBytes());
    auto expected = bignum_to_hex_string(const_cast<uint64_t *>(R_ARRAY), MAX_P_LEN);

    // Act
    auto result = ElementModP::fromHex(expected);

    // Assert
    CHECK(R().toHex() == result->toHex());
}

TEST_CASE("Test G is converted correctly")
{
    // Arrange
    auto expected = bignum_to_hex_string(const_cast<uint64_t *>(G_ARRAY), MAX_P_LEN);

    // Act
    auto result = ElementModP::fromHex(expected);

    // Assert
    CHECK(G().toHex() == result->toHex());
}

#pragma endregion
