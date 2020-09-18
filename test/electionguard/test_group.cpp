#include "../../src/electionguard/log.hpp"
#include "utils/constants.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/group.hpp>
#include <exception>
#include <string>

using namespace electionguard;

#pragma region add_mod_q

TEST_CASE("add_mod_q for ints 1 and 1 should return q of 2")
{
    // Arrange
    auto *one1 = ElementModQ::fromUint64(1UL);
    auto *one2 = ElementModQ::fromHex("01");
    auto *two = ElementModQ::fromUint64(2);

    // Act
    auto *result = add_mod_q(one1, one2);
    //Log::debug(": result->toHex() = " + result->toHex() + " and expectedHex = " + two->toHex());

    // Assert
    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_q (Q + 0) % Q = 0")
{
    // Arrange
    auto *q = Q();
    auto *zero = ElementModQ::fromUint64(0UL);
    auto *residue = ElementModQ::fromUint64(189);

    // Act
    auto *result = add_mod_q(q, zero);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    // Assert
    CHECK((*result == *zero));
    CHECK(result->toHex() == zero->toHex());
}

TEST_CASE("add_mod_q (Q + 1) % Q = 1")
{
    auto *q = Q();
    auto *one = ElementModQ::fromUint64(1UL);

    auto *result = add_mod_q(q, one);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *one));
    CHECK(result->toHex() == one->toHex());
}

TEST_CASE("add_mod_q (Q + 2) % Q = 2")
{
    auto *q = Q();
    auto *two = ElementModQ::fromUint64(2UL);

    auto *result = add_mod_q(q, two);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *two));
    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_q (Q + 188) % Q = 188")
{
    auto *q = Q();
    auto *residue = ElementModQ::fromUint64(188UL);

    auto *result = add_mod_q(q, residue);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q (Q + 189) % Q = 189")
{
    auto *q = Q();
    auto *residue = ElementModQ::fromUint64(189UL);

    auto *result = add_mod_q(q, residue);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q (MAX_256 + 0) % Q = 189")
{
    auto *max = new ElementModQ(MAX_256, true);
    auto *zero = ElementModQ::fromUint64(0UL);
    auto *residue = ElementModQ::fromUint64(189UL);

    auto *result = add_mod_q(max, zero);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q (MAX_256 + 1) % Q = 190")
{
    auto *max = new ElementModQ(MAX_256, true);
    auto *one = ElementModQ::fromUint64(1UL);
    auto *residue = ElementModQ::fromUint64(190UL);

    auto *result = add_mod_q(max, one);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q (MAX_256 + MAX_256) % Q = 378")
{
    auto *max = new ElementModQ(MAX_256, true);
    auto *residue = ElementModQ::fromUint64(378UL);

    auto *result = add_mod_q(max, max);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q ((MAX_256 - 1) + MAX_256) % Q = 377")
{
    auto *max = new ElementModQ(MAX_256, true);
    auto *maxMinusOne = new ElementModQ(MAX_256_MINUS_ONE, true);
    auto *residue = ElementModQ::fromUint64(377UL);

    auto *result = add_mod_q(maxMinusOne, max);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("add_mod_q ((MAX_256 - 1) + (MAX_256 - 1)) % Q = 376")
{
    auto *maxMinusOne = new ElementModQ(MAX_256_MINUS_ONE, true);
    auto *residue = ElementModQ::fromUint64(376UL);

    auto *result = add_mod_q(maxMinusOne, maxMinusOne);
    //Log::debug(result->get(), MAX_Q_LEN, ": result->get() = ");

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

#pragma endregion

#pragma region a_minus_b_mod_q

TEST_CASE("a_minus_b_mod_q (2 - 1) % Q == 1 and (1 - 1) & Q == 0")
{
    auto *one = ElementModQ::fromUint64(1);
    auto *two = ElementModQ::fromUint64(2);
    auto *zero = ElementModQ::fromUint64(0);

    auto *result1 = sub_mod_q(two, one);
    // Log::debug(" : result1->toHex() = " + result1->toHex());

    CHECK((*result1 == *one));
    CHECK(result1->toHex() == one->toHex());

    auto *result0 = sub_mod_q(one, one);
    // Log::debug(" : result0->toHex() = " + result0->toHex());

    CHECK((*result0 == *zero));
    CHECK(result0->toHex() == zero->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (Q - Q) % Q == 0")
{
    auto *q = Q();
    auto *residue = ElementModQ::fromUint64(0UL);

    auto *result = sub_mod_q(q, q);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (0 - Q) % Q == 0")
{
    string expectedHex("00");
    auto *zero = ElementModQ::fromUint64(0UL);
    auto *q = Q();

    auto *result = sub_mod_q(zero, q);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *zero));
    CHECK(result->toHex() == zero->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (Q - MAX_256) % Q == (Q - 189)")
{
    string expectedHex("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe86");

    auto *q = Q();
    auto *max = new ElementModQ(MAX_256, true);
    auto *offset = ElementModQ::fromUint64(189UL);
    auto *residue = sub_mod_q(Q(), offset);
    CHECK(residue->toHex() == expectedHex);

    auto *result = sub_mod_q(q, max);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  ((Q + 1) - MAX_256)) % Q == (Q - 188)")
{
    string expectedHex("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe87");

    auto *qplusone = new ElementModQ(Q_PLUS_ONE, true);
    auto *max = new ElementModQ(MAX_256, true);
    auto *offset = ElementModQ::fromUint64(188UL);
    auto *residue = sub_mod_q(Q(), offset);
    CHECK(residue->toHex() == expectedHex);

    auto *result = sub_mod_q(qplusone, max);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (Q - (MAX_256 - 1)) % Q == (Q - 188)")
{
    string expectedHex("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe87");

    auto *q = Q();
    auto *max = new ElementModQ(MAX_256_MINUS_ONE, true);
    auto *offset = ElementModQ::fromUint64(188UL);
    auto *residue = sub_mod_q(Q(), offset);
    CHECK(residue->toHex() == expectedHex);

    auto *result = sub_mod_q(q, max);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  ((Q + 1) - (MAX_256 - 1)) % Q == (Q - 187)")
{
    string expectedHex("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe88");

    auto *qplusone = new ElementModQ(Q_PLUS_ONE, true);
    auto *max = new ElementModQ(MAX_256_MINUS_ONE, true);
    auto *offset = ElementModQ::fromUint64(187UL);
    auto *residue = sub_mod_q(Q(), offset);
    CHECK(residue->toHex() == expectedHex);

    auto *result = sub_mod_q(qplusone, max);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (1 - Q) % Q == 1")
{
    string expectedHex("01");
    auto *one = ElementModQ::fromUint64(1UL);
    auto *q = Q();

    auto *result = sub_mod_q(one, q);
    //Log::debug(" : result->toHex() = " + result->toHex());
    CHECK((*result == *one));
    CHECK(result->toHex() == one->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (1 - 2) % Q == (Q - 1)")
{
    string expectedHex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff42");
    auto *one = ElementModQ::fromUint64(1UL);
    auto *two = ElementModQ::fromUint64(2UL);
    auto *zero = ElementModQ::fromUint64(0UL);
    auto *residue = new ElementModQ(const_cast<uint64_t *>(Q_MINUS_ONE));
    CHECK(residue->toHex() == expectedHex);

    auto *result = sub_mod_q(one, two);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q:  (10 - 15) % Q == (Q - 5)")
{
    string expectedHex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff3e");
    auto *ten = ElementModQ::fromUint64(10UL);
    auto *fifteen = ElementModQ::fromUint64(15UL);
    auto *five = ElementModQ::fromUint64(5UL);
    auto *residue = sub_mod_q(Q(), five);
    CHECK(residue->toHex() == expectedHex);

    auto *result = sub_mod_q(ten, fifteen);
    //Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(result->toHex() == residue->toHex());
}

TEST_CASE("a_minus_b_mod_q (Q - 1) % Q has hex value ending in 42")
{
    string expectedHex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff42");
    auto *q = Q();
    auto *one = ElementModQ::fromUint64(1);
    auto *residue = new ElementModQ(const_cast<uint64_t *>(Q_MINUS_ONE));

    auto *result = sub_mod_q(q, one);
    // Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *residue));
    CHECK(residue->toHex() == expectedHex);
    CHECK(result->toHex() == expectedHex);
}

#pragma endregion

#pragma region a_plus_bc_mod_q

TEST_CASE("a_plus_bc_mod_q for 1 + 2 x 3 is 7")
{
    auto *one = ElementModQ::fromUint64(1);
    auto *two = ElementModQ::fromUint64(2);
    auto *three = ElementModQ::fromUint64(3);
    auto *seven = ElementModQ::fromUint64(7);

    auto *result = a_plus_bc_mod_q(one, two, three);
    // Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *seven));
}

TEST_CASE("negate_mod_q for MAX Q is 0")
{
    auto *maxQ = new ElementModQ(const_cast<uint64_t *>(Q_ARRAY_REVERSE));
    auto *zero = ElementModQ::fromUint64(0);

    auto *result = sub_from_q(maxQ);
    // Log::debug(" : result->toHex() = " + result->toHex());

    CHECK((*result == *zero));
}

#pragma endregion

#pragma region add_mod_p

TEST_CASE("add_mod_p for ints 1 and 1 should return q of 2")
{
    auto *one1 = ElementModP::fromUint64(1UL);
    auto *one2 = ElementModP::fromHex("01");
    auto *two = ElementModP::fromUint64(2);

    auto *result = add_mod_p(one1, one2);

    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_p for two huge P arrays")
{
    auto *p1 = new ElementModP(LARGE_P_ARRAY_1);
    auto *p2 = new ElementModP(LARGE_P_ARRAY_2);

    auto *result = add_mod_p(p1, p2);

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
    auto *p = mul_mod_p(ElementModP::fromUint64(3), ElementModP::fromUint64(3));
    // Log::debug(": p->toHex() = " + p->toHex());

    CHECK(p->toHex() == "09");
}

TEST_CASE("mul_mod_p for max uint64 * max uint64 should equal hex value "
          "fffffffffffffffe0000000000000001)")
{
    auto uint64Max = numeric_limits<uint64_t>::max();
    auto *p = mul_mod_p(ElementModP::fromUint64(uint64Max), ElementModP::fromUint64(uint64Max));
    // Log::debug(": p->toHex() = " + p->toHex());

    CHECK(p->toHex() == "fffffffffffffffe0000000000000001");
}

#pragma endregion

#pragma region pow_mod_p

TEST_CASE("pow_mod_p 2 ^ 3 = 8 and 3 ^ 2 = 9")
{
    auto *two = ElementModP::fromUint64(2);
    auto *three = ElementModP::fromUint64(3);
    auto *eight = ElementModP::fromUint64(8);
    auto *nine = ElementModP::fromUint64(9);

    auto *result8 = pow_mod_p(two, three);
    // Log::debug(" : result8 = " + result8->toHex());

    CHECK((*result8 == *eight));

    auto *result9 = pow_mod_p(three, two);
    // Log::debug(" : result9 = " + result9->toHex());

    CHECK((*result9 == *nine));
}

#pragma endregion

#pragma region g_pow_p

TEST_CASE("Test g_pow_p with 0, 1, and 2")
{
    auto *zero = ElementModP::fromUint64(0);
    auto *one = ElementModP::fromUint64(1);
    auto *two = ElementModP::fromUint64(2);
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
    auto *gPowPFromHex = ElementModP::fromHex(expectedGPow2Hex);

    auto *result0 = g_pow_p(zero);
    auto *result1 = g_pow_p(one);
    auto *result2 = g_pow_p(two);

    // Log::debug(" : result0->toHex() = " + result0->toHex());
    CHECK((*result0 == *one));

    // Log::debug(" : result1->toHex() = " + result1->toHex());
    CHECK((*result1 == *G()));

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

    CHECK_THROWS_WITH(new ElementModQ(const_cast<uint64_t *>(maxArrayQPlus1)),
                      "Value for ElementModQ is greater than allowed");
}

TEST_CASE("Hex string converted to Q matches original hex when converted back toHex")
{
    string expectedHex("f0f0f0f0f0f0f0f0");
    auto *q = ElementModQ::fromHex(expectedHex);
    // Log::debug(": q->toHex() = " + q->toHex() + " and expectedHex = " + expectedHex);

    CHECK(q->toHex() == expectedHex);
}

TEST_CASE("Test Q is converted correctly")
{
    string expectedQHex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff43");

    auto *qFromHex = ElementModQ::fromHex(expectedQHex);
    auto *qFromRawArray = new ElementModQ(const_cast<uint64_t *>(Q_ARRAY_REVERSE), true);
    auto *qFromConstExpr = Q();

    CHECK(qFromHex->toHex() == expectedQHex);
    CHECK(qFromRawArray->toHex() == expectedQHex);
    CHECK(qFromConstExpr->toHex() == expectedQHex);

    CHECK((*qFromHex == *qFromRawArray));
}

#pragma endregion

#pragma region P Misc

TEST_CASE("Test P is converted correctly")
{
    string expectedPHex(
      "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff93c467e37db0c7a4d1be3f810152"
      "cb56a1cecc3af65cc0190c03df34709affbd8e4b59fa03a9f0eed0649ccb621057d11056ae9132135a08e43b4673"
      "d74bafea58deb878cc86d733dbe7bf38154b36cf8a96d1567899aaae0c09d4c8b6b7b86fd2a1ea1de62ff8643ec7"
      "c271827977225e6ac2f0bd61c746961542a3ce3bea5db54fe70e63e6d09f8fc28658e80567a47cfde60ee741e5d8"
      "5a7bd46931ced8220365594964b839896fcaabccc9b31959c083f22ad3ee591c32fab2c7448f2a057db2db49ee52"
      "e0182741e53865f004cc8e704b7c5c40bf304c4d8c4f13edf6047c555302d2238d8ce11df2424f1b66c2c5d238d0"
      "744db679af2890487031f9c0aea1c4bb6fe9554ee528fdf1b05e5b256223b2f09215f3719f9c7ccc69ddf172d0d6"
      "234217fcc0037f18b93ef5389130b7a661e5c26e54214068bbcafea32a67818bd3075ad1f5c7e9cc3d1737fb2817"
      "1baf84dbb6612b7881c1a48e439cd03a92bf52225a2b38e6542e9f722bce15a381b5753ea842763381ccae83512b"
      "30511b32e5e8d80362149ad030aaba5f3a5798bb22aa7ec1b6d0f17903f4e234ea6034aa85973f79a93ffb82a75c"
      "47c03d43d2f9ca02d03199baceddd45334dbc6b5ffffffffffffffffffffffffffffffffffffffffffffffffffff"
      "ffffffffffff");

    auto *pFromHex = ElementModP::fromHex(expectedPHex);
    auto *p = P();

    CHECK(p->toHex() == expectedPHex);
}

TEST_CASE("Test G is converted correctly")
{
    string expectedGHex(
      "037de384f98f6e038d2a3141825b33d5d45ec4cc64cfd15e750d6798f5196cf2a142cdf33f6ef853840ec7d4ec80"
      "4794cfb0cfb65363b2566387b98ee0e3def1b706fa55d5038ffb4a62dcbb93b1ddd8d3b308da86d1c3a525ef356f"
      "e5bb59314e65633480b396e1dd4b795f78de07d86b0e2a05be6af78fd7f736fcba6c032e26e050af50a03c65fa7b"
      "6c87f4554cb57f3dabcbad8eb9d8fdebeef58570669acc3eda17dbfc47b8b3c39aa08b829b28872e62b5d1b13a98"
      "f09d40ac20c2ab74a6750e7c8750b5141e221c41f55bba31d8e41422b64d2cba7aaa0e9fd8785702f6932825bf45"
      "de8386d24900742062c1322b37c50af182158090c35da9355e6cf7f72da39a2284fdfb1918b2a2a30e69501fa234"
      "2b728263df23f1db8355bde1eb276fb3685f371672ceb313fdab069cc9b11ab6c59bce62baad96aac96b0dbe0c7e"
      "71fcb22552545a5d1cedeee01e4bc0cdbdb76b6ad45f09af5e71114a005f93ad97b8fe09274e76c94b2008926b38"
      "caec94c95e96d628f6bc80662ba06207801328b2c6a60526bf7cd02d9661385ac3b1cbdb50f759d0e9f61c11a07b"
      "f4218f299bcb2900520076ebd2d95a3dee96d4809ef34abeb83fdba8a12c5ca82757288a89c931cf564f00e8a317"
      "ae1e1d828e61369ba0ddbadb10c136f8691101ad82dc54775ab8353840d9992197d80a6e94b38ac417cddf40b0c7"
      "3abf03e8e0aa");

    auto *gFromHex = ElementModP::fromHex(expectedGHex);

    CHECK(G()->toHex() == expectedGHex);
}

#pragma endregion
