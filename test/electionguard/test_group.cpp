#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/group.hpp>
#include <exception>
#include <string>

using namespace electionguard;

TEST_CASE("add_mod_q for ints 1 and 1 should return q of 2")
{
    auto *one1 = ElementModQ::fromUint64(1UL);
    auto *one2 = ElementModQ::fromHex("01");
    auto *two = ElementModQ::fromUint64(2);
    //Log::debug(": one1 = " + one1->toHex() + " one2 = " + one2->toHex());
    auto *result = add_mod_q(one1, one2);
    //Log::debug(": result->toHex() = " + result->toHex() + " and expectedHex = " + two->toHex());
    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_p for ints 1 and 1 should return q of 2")
{
    auto *one1 = ElementModP::fromUint64(1UL);
    auto *one2 = ElementModP::fromHex("01");
    auto *two = ElementModP::fromUint64(2);
    //Log::debug(": one1 = " + one1->toHex() + " one2 = " + one2->toHex());
    auto *result = add_mod_p(one1, one2);
    //Log::debug(": result->toHex() = " + result->toHex() + " and expectedHex = " + two->toHex());
    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_p for two huge P arrays")
{
    const uint64_t p1Array[64U] = {
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
      0x0000000000000001, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
    const uint64_t p2Array[64U] = {
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
      0x0000000000000001, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
      0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
    auto *p1 = new ElementModP(p1Array);
    auto *p2 = new ElementModP(p2Array);
    auto *result = add_mod_p(p1, p2);
    // Log::debug(" : \n\t p1 = " + p1->toHex() + "\n\t p2 = " + p2->toHex() +
    //            "\n\t result = " + result->toHex());

    CHECK(p1->toHex() != "00");
    CHECK(p2->toHex() != "00");
    CHECK(result->toHex() != "00");
}

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
    CHECK(q->toHex() == expectedHex);
    // Log::debug(": q->toHex() = " + q->toHex() + " and expectedHex = " + expectedHex);
}

TEST_CASE("mul_mod_p 3 * 3 should equal 9")
{
    auto *p = mul_mod_p(ElementModP::fromUint64(3), ElementModP::fromUint64(3));
    CHECK(p->toHex() == "09");
    // Log::debug(": p->toHex() = " + p->toHex());
}

TEST_CASE("mul_mod_p for max uint64 * max uint64 should equal hex value "
          "fffffffffffffffe0000000000000001)")
{
    auto uint64Max = numeric_limits<uint64_t>::max();
    auto *p = mul_mod_p(ElementModP::fromUint64(uint64Max), ElementModP::fromUint64(uint64Max));
    CHECK(p->toHex() == "fffffffffffffffe0000000000000001");
    // Log::debug(": p->toHex() = " + p->toHex());
}

TEST_CASE("pow_mod_p 2 ^ 3 = 8 and 3 ^ 2 = 9")
{
    auto *two = ElementModP::fromUint64(2);
    auto *three = ElementModP::fromUint64(3);
    auto *eight = ElementModP::fromUint64(8);
    auto *nine = ElementModP::fromUint64(9);

    auto *result8 = pow_mod_p(two, three);
    CHECK((*result8 == *eight));
    // Log::debug(" : result8 = " + result8->toHex());

    auto *result9 = pow_mod_p(three, two);
    CHECK((*result9 == *nine));
    // Log::debug(" : result9 = " + result9->toHex());
}

TEST_CASE("Test Q is converted correctly")
{
    string expectedQHex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff43");

    auto *qFromHex = ElementModQ::fromHex(expectedQHex);
    auto *qFromRawArray = new ElementModQ(const_cast<uint64_t *>(Q_ARRAY), true);
    auto *qFromConstExpr = Q();

    CHECK(qFromHex->toHex() == expectedQHex);
    CHECK(qFromRawArray->toHex() == expectedQHex);
    CHECK(qFromConstExpr->toHex() == expectedQHex);

    CHECK((*qFromHex == *qFromRawArray));
}

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
    //Log::debug(pFromHex->get(), 64UL, " : pFromHex = ");
    auto *p = new ElementModP(const_cast<uint64_t *>(P_ARRAY), true);
    //Log::debug(" : p->toHex() = " + p->toHex());
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
    // Log::debug(gFromHex->get(), 64UL, " : gFromHex = ");
    // Log::debug(" : g->toHex() = " + g->toHex());
    CHECK(G()->toHex() == expectedGHex);
}

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

TEST_CASE("a_minus_b_mod_q 2 - 1 is 1 and 1 - 1 is 0")
{
    auto *one = ElementModQ::fromUint64(1);
    auto *two = ElementModQ::fromUint64(2);
    auto *zero = ElementModQ::fromUint64(0);

    auto *result1 = a_minus_b_mod_q(two, one);
    // Log::debug(" : result1->toHex() = " + result1->toHex());
    CHECK((*result1 == *one));

    auto *result0 = a_minus_b_mod_q(one, one);
    // Log::debug(" : result0->toHex() = " + result0->toHex());
    CHECK((*result0 == *zero));
}

TEST_CASE("a_minus_b_mod_q for max of q - 1 has hex value ending in 42")
{
    string expectedHex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff42");
    // Max Q value in Hacl_Bignum4096 format
    auto *maxQ = new ElementModQ(const_cast<uint64_t *>(Q_ARRAY), true);
    auto *one = ElementModQ::fromUint64(1);

    auto *result = a_minus_b_mod_q(maxQ, one);
    // Log::debug(" : result->toHex() = " + result->toHex());
    CHECK(result->toHex() == expectedHex);
}

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
    auto *maxQ = new ElementModQ(const_cast<uint64_t *>(Q_ARRAY));
    auto *zero = ElementModQ::fromUint64(0);

    auto *result = negate_mod_q(maxQ);
    // Log::debug(" : result->toHex() = " + result->toHex());
    CHECK((*result == *zero));
}