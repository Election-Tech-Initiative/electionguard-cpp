#include "../../src/electionguard/log.hpp"
#include "utils/constants.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/group.hpp>
#include <exception>
#include <string>

using namespace electionguard;
using namespace std;

#pragma region add_mod_q

TEST_CASE("add_mod_q for ints 1 and 1 should return q of 2")
{
    // Arrange
    auto one1 = ElementModQ::fromUint64(1UL);
    auto one2 = ElementModQ::fromHex("01");
    auto two = ElementModQ::fromUint64(2);

    // Act
    auto result = add_mod_q(*one1, *one2);
    //Log::debug(": result->toHex() = " + result->toHex() + " and expectedHex = " + two->toHex());

    // Assert
    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_q (Q + 0) % Q = 0")
{
    // Arrange
    const auto &q = Q();
    auto zero = ElementModQ::fromUint64(0UL);
    auto residue = ElementModQ::fromUint64(189);

    // Act
    auto result = add_mod_q(q, *zero);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    // Assert
    CHECK((*result == *zero));
    CHECK(result->toHex() == zero->toHex());
}

TEST_CASE("add_mod_q (Q + 1) % Q = 1")
{
    const auto &q = Q();
    auto one = ElementModQ::fromUint64(1UL);

    auto result = add_mod_q(q, *one);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *one));
    CHECK(result->toHex() == one->toHex());
}

TEST_CASE("add_mod_q (Q + 2) % Q = 2")
{
    const auto &q = Q();
    auto two = ElementModQ::fromUint64(2UL);

    auto result = add_mod_q(q, *two);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *two));
    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_q (Q + 188) % Q = 188")
{
    const auto &q = Q();
    auto residue = ElementModQ::fromUint64(188UL);

    auto result = add_mod_q(q, *residue);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q (Q + 189) % Q = 189")
{
    const auto &q = Q();
    auto residue = ElementModQ::fromUint64(189UL);

    auto result = add_mod_q(q, *residue);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q (MAX_256 + 0) % Q = 189")
{
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto zero = ElementModQ::fromUint64(0UL);
    auto residue = ElementModQ::fromUint64(189UL);

    auto result = add_mod_q(*max, *zero);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q (MAX_256 + 1) % Q = 190")
{
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto one = ElementModQ::fromUint64(1UL);
    auto residue = ElementModQ::fromUint64(190UL);

    auto result = add_mod_q(*max, *one);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q (MAX_256 + MAX_256) % Q = 378")
{
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto residue = ElementModQ::fromUint64(378UL);

    auto result = add_mod_q(*max, *max);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q ((MAX_256 - 1) + MAX_256) % Q = 377")
{
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto maxMinusOne = make_unique<ElementModQ>(MAX_256_MINUS_ONE, true);
    auto residue = ElementModQ::fromUint64(377UL);

    auto result = add_mod_q(*maxMinusOne, *max);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q ((MAX_256 - 1) + (MAX_256 - 1)) % Q = 376")
{
    auto maxMinusOne = make_unique<ElementModQ>(MAX_256_MINUS_ONE, true);
    auto residue = ElementModQ::fromUint64(376UL);

    auto result = add_mod_q(*maxMinusOne, *maxMinusOne);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

#pragma endregion

#pragma region a_minus_b_mod_q

TEST_CASE("a_minus_b_mod_q (2 - 1) % Q == 1 and (1 - 1) & Q == 0")
{
    auto one = ElementModQ::fromUint64(1);
    auto two = ElementModQ::fromUint64(2);
    auto zero = ElementModQ::fromUint64(0);

    auto result1 = sub_mod_q(*two, *one);
    // Log::debug(" : result1->toHex() = " + result1->toHex());

    CHECK((*result1 == *one));
    CHECK(result1->toHex() == one->toHex());

    auto result0 = sub_mod_q(*one, *one);
    // Log::debug(" : result0->toHex() = " + result0->toHex());

    CHECK((*result0 == *zero));
    CHECK(result0->toHex() == zero->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (Q - Q) % Q == 0")
{
    const auto &q = Q();
    auto residue = ElementModQ::fromUint64(0UL);

    auto result = sub_mod_q(q, q);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (0 - Q) % Q == 0")
{
    string expectedHex("00");
    auto zero = ElementModQ::fromUint64(0UL);
    const auto &q = Q();

    auto result = sub_mod_q(*zero, q);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *zero));
    CHECK(result->toHex() == zero->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (Q - MAX_256) % Q == (Q - 189)")
{
    string expectedHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE86");

    const auto &q = Q();
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto offset = ElementModQ::fromUint64(189UL);
    auto residue = sub_mod_q(q, *offset);
    CHECK(residue->toHex() == expectedHex);

    auto result = sub_mod_q(q, *max);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  ((Q + 1) - MAX_256)) % Q == (Q - 188)")
{
    string expectedHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE87");

    auto qplusone = make_unique<ElementModQ>(Q_PLUS_ONE, true);
    auto max = make_unique<ElementModQ>(MAX_256, true);
    auto offset = ElementModQ::fromUint64(188UL);
    auto residue = sub_mod_q(Q(), *offset);
    CHECK(residue->toHex() == expectedHex);

    auto result = sub_mod_q(*qplusone, *max);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (Q - (MAX_256 - 1)) % Q == (Q - 188)")
{
    string expectedHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE87");

    const auto &q = Q();
    auto max = make_unique<ElementModQ>(MAX_256_MINUS_ONE, true);
    auto offset = ElementModQ::fromUint64(188UL);
    auto residue = sub_mod_q(q, *offset);
    CHECK(residue->toHex() == expectedHex);

    auto result = sub_mod_q(q, *max);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  ((Q + 1) - (MAX_256 - 1)) % Q == (Q - 187)")
{
    string expectedHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE88");

    auto qplusone = make_unique<ElementModQ>(Q_PLUS_ONE, true);
    auto max = make_unique<ElementModQ>(MAX_256_MINUS_ONE, true);
    auto offset = ElementModQ::fromUint64(187UL);
    auto residue = sub_mod_q(Q(), *offset);
    CHECK(residue->toHex() == expectedHex);

    auto result = sub_mod_q(*qplusone, *max);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (1 - Q) % Q == 1")
{
    string expectedHex("01");
    auto one = ElementModQ::fromUint64(1UL);
    const auto &q = Q();

    auto result = sub_mod_q(*one, q);
    //Log::debug(" : result->toHex() = " + result->toHex());
    CHECK((*result == *one));
    CHECK(result->toHex() == one->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (1 - 2) % Q == (Q - 1)")
{
    string expectedHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF42");
    auto one = ElementModQ::fromUint64(1UL);
    auto two = ElementModQ::fromUint64(2UL);
    auto zero = ElementModQ::fromUint64(0UL);
    auto residue = make_unique<ElementModQ>(Q_MINUS_ONE);
    CHECK(residue->toHex() == expectedHex);

    auto result = sub_mod_q(*one, *two);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (10 - 15) % Q == (Q - 5)")
{
    string expectedHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3E");
    auto ten = ElementModQ::fromUint64(10UL);
    auto fifteen = ElementModQ::fromUint64(15UL);
    auto five = ElementModQ::fromUint64(5UL);
    auto residue = sub_mod_q(Q(), *five);
    CHECK(residue->toHex() == expectedHex);

    auto result = sub_mod_q(*ten, *fifteen);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q (Q - 1) % Q has hex value ending in 42")
{
    string expectedHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF42");
    const auto &q = Q();
    auto one = ElementModQ::fromUint64(1);
    auto residue = make_unique<ElementModQ>(Q_MINUS_ONE);

    auto result = sub_mod_q(q, *one);
    // Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(residue->toHex() == expectedHex);
    CHECK(result->toHex() == expectedHex);
}

#pragma endregion

#pragma region a_plus_bc_mod_q

TEST_CASE("a_plus_bc_mod_q for 1 + (2 x 3) % Q = 7")
{
    auto one = ElementModQ::fromUint64(1);
    auto two = ElementModQ::fromUint64(2);
    auto three = ElementModQ::fromUint64(3);
    auto seven = ElementModQ::fromUint64(7);

    auto result = a_plus_bc_mod_q(*one, *two, *three);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *seven));
    CHECK(result->toHex() == seven->toHex());
}

TEST_CASE("negate_mod_q for MAX Q is 0")
{
    auto maxQ = make_unique<ElementModQ>(Q_ARRAY_REVERSE);
    auto zero = ElementModQ::fromUint64(0);

    auto result = sub_from_q(*maxQ);
    // Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *zero));
}

#pragma endregion

#pragma region add_mod_p

TEST_CASE("add_mod_p for ints 1 and 1 should return q of 2")
{
    auto one1 = ElementModP::fromUint64(1UL);
    auto one2 = ElementModP::fromHex("01");
    auto two = ElementModP::fromUint64(2);

    auto result = add_mod_p(*one1, *one2);

    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_p for two huge P arrays")
{
    auto p1 = make_unique<ElementModP>(LARGE_P_ARRAY_1);
    auto p2 = make_unique<ElementModP>(LARGE_P_ARRAY_2);

    auto result = add_mod_p(*p1, *p2);

    CHECK(p1->toHex() != "00");
    CHECK(p2->toHex() != "00");
    CHECK(result->toHex() != "00");
}

// TODO: handle carry like in ElementModQ

// TEST_CASE("add_mod_p (P + 0) % P = 0")
// {
//     auto *p = P();
//     auto *zero = ElementModP::fromUint64(0UL);
//     auto *residue = ElementModP::fromUint64(189);

//     auto *result = add_mod_p(p, zero);
//     //Log::debug(result->get(), MAX_P_LEN, ": result->get() = ");

//     CHECK((*result == *zero));
//     CHECK(result->toHex() == zero->toHex());
// }

// TEST_CASE("add_mod_p (P + 1) % P = 1")
// {
//     auto *p = P();
//     auto *one = ElementModP::fromUint64(1UL);

//     auto *result = add_mod_p(p, one);
//     //Log::debug(result->get(), MAX_P_LEN, ": result->get() = ");

//     CHECK((*result == *one));
//     CHECK(result->toHex() == one->toHex());
// }

// TEST_CASE("add_mod_p (P + 2) % P = 2")
// {
//     auto *p = P();
//     auto *two = ElementModP::fromUint64(2UL);

//     auto *result = add_mod_p(p, two);
//     //Log::debug(result->get(), MAX_P_LEN, ": result->get() = ");

//     CHECK((*result == *two));
//     CHECK(result->toHex() == two->toHex());
// }

// TEST_CASE("add_mod_p (Q + 188) % Q = 188")
// {
//     auto *p = P();
//     auto *residue = ElementModP::fromUint64(188UL);

//     auto *result = add_mod_p(q, residue);
//     //Log::debug(result->get(), MAX_P_LEN, ": result->get() = ");
//     CHECK((*result == *residue));
//     CHECK(result->toHex() == residue->toHex());
// }

// TEST_CASE("add_mod_p (Q + 189) % Q = 189")
// {
//     auto *p = P();
//     auto *residue = ElementModP::fromUint64(189UL);

//     auto *result = add_mod_p(q, residue);
//     //Log::debug(result->get(), MAX_P_LEN, ": result->get() = ");

//     CHECK((*result == *residue));
//     CHECK(result->toHex() == residue->toHex());
// }

// TEST_CASE("add_mod_p (MAX_4096 + 0) % Q = ???")
// {
//     auto *max = new ElementModP(MAX_4096, true);
//     auto *zero = ElementModP::fromUint64(0UL);
//     auto *residue = ElementModP::fromUint64(189UL);

//     auto *result = add_mod_p(max, zero);
//     //Log::debug(result->get(), MAX_P_LEN, ": result->get() = ");

//     CHECK((*result == *residue));
//     CHECK(result->toHex() == residue->toHex());
// }

// TEST_CASE("add_mod_p (MAX_4096 + 1) % Q = 190")
// {
//     auto *max = new ElementModP(MAX_4096, true);
//     auto *one = ElementModP::fromUint64(1UL);
//     auto *residue = ElementModP::fromUint64(190UL);

//     auto *result = add_mod_p(max, one);
//     //Log::debug(result->get(), MAX_P_LEN, ": result->get() = ");

//     CHECK((*result == *residue));
//     CHECK(result->toHex() == residue->toHex());
// }

// TEST_CASE("add_mod_p (MAX_4096 + MAX_4096) % Q = 378")
// {
//     auto *max = new ElementModP(MAX_4096, true);
//     auto *residue = ElementModP::fromUint64(378UL);

//     auto *result = add_mod_p(max, max);
//     //Log::debug(result->get(), MAX_P_LEN, ": result->get() = ");

//     CHECK((*result == *residue));
//     CHECK(result->toHex() == residue->toHex());
// }

// TEST_CASE("add_mod_p ((MAX_4096 - 1) + MAX_4096) % Q = 377")
// {
//     auto *max = new ElementModP(MAX_4096, true);
//     auto *maxMinusOne = new ElementModP(MAX_4096_MINUS_ONE, true);
//     auto *residue = ElementModP::fromUint64(377UL);

//     auto *result = add_mod_p(maxMinusOne, max);
//     //Log::debug(result->get(), MAX_P_LEN, ": result->get() = ");

//     CHECK((*result == *residue));
//     CHECK(result->toHex() == residue->toHex());
// }

// TEST_CASE("add_mod_p ((MAX_256 - 1) + (MAX_256 - 1)) % Q = 376")
// {
//     auto *maxMinusOne = new ElementModP(MAX_4096_MINUS_ONE, true);
//     auto *residue = ElementModP::fromUint64(376UL);

//     auto *result = add_mod_p(maxMinusOne, maxMinusOne);
//     //Log::debug(result->get(), MAX_P_LEN, ": result->get() = ");

//     CHECK((*result == *residue));
//     CHECK(result->toHex() == residue->toHex());
// }

#pragma endregion

#pragma region mul_mod_p

TEST_CASE("mul_mod_p 3 * 3 should equal 9")
{
    auto p = mul_mod_p(*ElementModP::fromUint64(3), *ElementModP::fromUint64(3));
    // Log::debug(": p->toHex() = " + p->toHex());

    CHECK(p->toHex() == "09");
}

TEST_CASE("mul_mod_p for max uint64 * max uint64 should equal hex value "
          "FFFFFFFFFFFFFFFE0000000000000001)")
{
    uint64_t uint64Max = 0xffffffffffffffff;
    auto p = mul_mod_p(*ElementModP::fromUint64(uint64Max), *ElementModP::fromUint64(uint64Max));
    // Log::debug(": p->toHex() = " + p->toHex());

    CHECK(p->toHex() == "FFFFFFFFFFFFFFFE0000000000000001");
}

#pragma endregion

#pragma region pow_mod_p

TEST_CASE("pow_mod_p 2 ^ 3 = 8 and 3 ^ 2 = 9")
{
    auto two = ElementModP::fromUint64(2);
    auto three = ElementModP::fromUint64(3);
    auto eight = ElementModP::fromUint64(8);
    auto nine = ElementModP::fromUint64(9);

    auto result8 = pow_mod_p(*two, *three);
    // Log::debug(" : result8 = " + result8->toHex());

    CHECK((*result8 == *eight));

    auto result9 = pow_mod_p(*three, *two);
    // Log::debug(" : result9 = " + result9->toHex());

    CHECK((*result9 == *nine));
}

#pragma endregion

#pragma region g_pow_p

TEST_CASE("Test g_pow_p with 0, 1, and 2")
{
    auto zero = ElementModP::fromUint64(0);
    auto one = ElementModP::fromUint64(1);
    auto two = ElementModP::fromUint64(2);
    string expectedGPow2Hex(
      "f258e409b1a130e00a3793555e0eab2f560aa12cc01a3cb6b357035c6e734256b4d67877c018cb57af150ddbbd0a"
      "c22b9d74c0b15c1ac80953086fddfaab7fc503022b61be8c6e4fecd02136f4afc68b51390d0e7e90661763455b8b"
      "1d79ddcba82c7ffba5bc7c28a323b2624fe7a72e693e45b0b88acf1ebc4f2743ac0fbb9d3414070a37a7cd52b1c1"
      "443b69982b86ddcfea3137c383a4c10705b7b2a244c93cd0179bd1203e5dce19a078452c0525aa577d6214d1a849"
      "af5bce02581f381a872436497da0245f10245604069562f98be4f424227331252d955e8e77b42b20d3ab85530ac8"
      "24fa4d5a8edc4a6c7e310019bed8b8062b9a95b63cc126efcc74d7512095183e2a2a99386b40210a9b1e9b2433f1"
      "c00d04dccda4cae17e27456c6bd7d55c45c5f98514400eda58d93a82e0d456c209087ed41106190f969b1da4841f"
      "8ea26907ff4d29caf05a4c5159aed129f980a06351b18cc88fb0954e958984070c0c8999d043bc68e1bfb8151f06"
      "1103c9d89d9fdf709f747f816275463568fa6e18baad0c21a6c204c3795383f91eeece312a4fcfd71b541718ff21"
      "f7d408d3a2572c889a7d25957bd7d206041452b60f513b24604cd8336c351d6b7a70bd39bedbad0b910d329230f4"
      "a3228df1398cbe7ee7eef49cb22c94be32ed6c3f51b4f2c43a40bc5c217106cd7a0550ba12bb3d84643f90976ecf"
      "ef79614f2c46");
    auto gPowPFromHex = ElementModP::fromHex(expectedGPow2Hex);

    auto result0 = g_pow_p(*zero);
    auto result1 = g_pow_p(*one);
    auto result2 = g_pow_p(*two);

    // Log::debug(" : result0->toHex() = " + result0->toHex());
    CHECK((*result0 == *one));

    // Log::debug(" : result1->toHex() = " + result1->toHex());
    CHECK((*result1 == G()));

    // Log::debug(" : result2->toHex() = " + result2->toHex());
    // Log::debug(" : gPowPFromHex->toHex() = " + gPowPFromHex->toHex());
    CHECK((*result2 == *gPowPFromHex));
}

#pragma endregion

#pragma region Q Misc

TEST_CASE("Max of Q")
{
    // when more than max of Q is passed in, ElementModQ should throw an exception
    const uint64_t maxArrayQPlus1[4U] = {0xFFFFFFFFFFFFFF44, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                                         0xFFFFFFFFFFFFFFFF};

    CHECK_THROWS_WITH(new ElementModQ(maxArrayQPlus1),
                      "Value for ElementModQ is greater than allowed");
}

TEST_CASE("Hex string converted to Q matches original hex when converted back toHex")
{
    string expectedHex("F0F0F0F0F0F0F0F0");
    auto q = ElementModQ::fromHex(expectedHex);
    // Log::debug(": q->toHex() = " + q->toHex() + " and expectedHex = " + expectedHex);

    CHECK(q->toHex() == expectedHex);
}

TEST_CASE("Test Q is converted correctly")
{
    string expectedQHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF43");

    auto qFromHex = ElementModQ::fromHex(expectedQHex);
    auto qFromRawArray = make_unique<ElementModQ>(Q_ARRAY_REVERSE, true);
    const auto &q = Q();

    CHECK(qFromHex->toHex() == expectedQHex);
    CHECK(qFromRawArray->toHex() == expectedQHex);
    CHECK(q.toHex() == expectedQHex);

    CHECK((*qFromHex == *qFromRawArray));
}

#pragma endregion

#pragma region P Misc

TEST_CASE("Test P is converted correctly")
{
    string expectedPHex(
      "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF93C467E37DB0C7A4D1BE3F810152"
      "CB56A1CECC3AF65CC0190C03DF34709AFFBD8E4B59FA03A9F0EED0649CCB621057D11056AE9132135A08E43B4673"
      "D74BAFEA58DEB878CC86D733DBE7BF38154B36CF8A96D1567899AAAE0C09D4C8B6B7B86FD2A1EA1DE62FF8643EC7"
      "C271827977225E6AC2F0BD61C746961542A3CE3BEA5DB54FE70E63E6D09F8FC28658E80567A47CFDE60EE741E5D8"
      "5A7BD46931CED8220365594964B839896FCAABCCC9B31959C083F22AD3EE591C32FAB2C7448F2A057DB2DB49EE52"
      "E0182741E53865F004CC8E704B7C5C40BF304C4D8C4F13EDF6047C555302D2238D8CE11DF2424F1B66C2C5D238D0"
      "744DB679AF2890487031F9C0AEA1C4BB6FE9554EE528FDF1B05E5B256223B2F09215F3719F9C7CCC69DDF172D0D6"
      "234217FCC0037F18B93EF5389130B7A661E5C26E54214068BBCAFEA32A67818BD3075AD1F5C7E9CC3D1737FB2817"
      "1BAF84DBB6612B7881C1A48E439CD03A92BF52225A2B38E6542E9F722BCE15A381B5753EA842763381CCAE83512B"
      "30511B32E5E8D80362149AD030AABA5F3A5798BB22AA7EC1B6D0F17903F4E234EA6034AA85973F79A93FFB82A75C"
      "47C03D43D2F9CA02D03199BACEDDD45334DBC6B5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
      "FFFFFFFFFFFF");

    auto pFromHex = ElementModP::fromHex(expectedPHex);
    const auto &p = P();

    CHECK(p.toHex() == expectedPHex);
}

TEST_CASE("Test G is converted correctly")
{
    string expectedGHex(
      "037DE384F98F6E038D2A3141825B33D5D45EC4CC64CFD15E750D6798F5196CF2A142CDF33F6EF853840EC7D4EC80"
      "4794CFB0CFB65363B2566387B98EE0E3DEF1B706FA55D5038FFB4A62DCBB93B1DDD8D3B308DA86D1C3A525EF356F"
      "E5BB59314E65633480B396E1DD4B795F78DE07D86B0E2A05BE6AF78FD7F736FCBA6C032E26E050AF50A03C65FA7B"
      "6C87F4554CB57F3DABCBAD8EB9D8FDEBEEF58570669ACC3EDA17DBFC47B8B3C39AA08B829B28872E62B5D1B13A98"
      "F09D40AC20C2AB74A6750E7C8750B5141E221C41F55BBA31D8E41422B64D2CBA7AAA0E9FD8785702F6932825BF45"
      "DE8386D24900742062C1322B37C50AF182158090C35DA9355E6CF7F72DA39A2284FDFB1918B2A2A30E69501FA234"
      "2B728263DF23F1DB8355BDE1EB276FB3685F371672CEB313FDAB069CC9B11AB6C59BCE62BAAD96AAC96B0DBE0C7E"
      "71FCB22552545A5D1CEDEEE01E4BC0CDBDB76B6AD45F09AF5E71114A005F93AD97B8FE09274E76C94B2008926B38"
      "CAEC94C95E96D628F6BC80662BA06207801328B2C6A60526BF7CD02D9661385AC3B1CBDB50F759D0E9F61C11A07B"
      "F4218F299BCB2900520076EBD2D95A3DEE96D4809EF34ABEB83FDBA8A12C5CA82757288A89C931CF564F00E8A317"
      "AE1E1D828E61369BA0DDBADB10C136F8691101AD82DC54775AB8353840D9992197D80A6E94B38AC417CDDF40B0C7"
      "3ABF03E8E0AA");

    auto gFromHex = ElementModP::fromHex(expectedGHex);

    CHECK(const_cast<ElementModP &>(G()).toHex() == expectedGHex);
}

#pragma endregion
