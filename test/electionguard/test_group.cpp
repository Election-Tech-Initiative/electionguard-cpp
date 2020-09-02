#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/group.hpp>
#include <exception>
#include <string>

using namespace electionguard;

TEST_CASE("add_mod_q for ints 1 and 1 should return q of 2")
{
    auto one1 = uint64_to_q(1UL);
    auto one2 = hex_to_q("01");
    auto two = uint64_to_q(2);
    auto result = add_mod_q(one1, one2);
    CHECK(result->toHex() == two->toHex());
}

TEST_CASE("add_mod_p for two huge P arrays")
{
    uint64_t p1Array[64U] = {
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
    uint64_t p2Array[64U] = {
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
    auto p1 = new ElementModP(p1Array);
    auto p2 = new ElementModP(p2Array);
    auto result = add_mod_p(p1, p2);
    // Log::debug(" : \n\t p1 = " + p1->toHex() + "\n\t p2 = " + p2->toHex() +
    //            "\n\t result = " + result->toHex());

    CHECK(p1->toHex() != "00");
    CHECK(p2->toHex() != "00");
    CHECK(result->toHex() != "00");
}

TEST_CASE("Max of Q")
{
    auto maxQ = new ElementModQ(const_cast<uint64_t *>(Q_ARRAY));
    // Log::debug(": ElementModQ initalized with Q_ARRAY succeeds = " + maxQ->toHex());

    // when more than max of Q is passed in, ElementModQ should throw an exception
    uint64_t maxArrayQPlus1[4U] = {0xFFFFFFFFFFFFFF44, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                                   0xFFFFFFFFFFFFFFFF};
    CHECK_THROWS_WITH(new ElementModQ(maxArrayQPlus1),
                      "Value for ElementModQ is greater than allowed");
}

TEST_CASE("Hex string converted to Q matches original hex when converted back toHex")
{
    string expectedHex("f0f0f0f0f0f0f0f0");
    auto q = hex_to_q(expectedHex);
    CHECK(q->toHex() == expectedHex);
    // Log::debug(": q->toHex() = " + q->toHex() + " and expectedHex = " + expectedHex);
}

TEST_CASE("mul_mod_p 3 * 3 should equal 9")
{
    auto p = mul_mod_p(uint64_to_p(3), uint64_to_p(3));
    CHECK(p->toHex() == "09");
    // Log::debug(": p->toHex() = " + p->toHex());
}

TEST_CASE("mul_mod_p for max uint64 * max uint64 should equal hex value "
          "fffffffffffffffe0000000000000001)")
{
    auto uint64Max = numeric_limits<uint64_t>::max();
    auto p = mul_mod_p(uint64_to_p(uint64Max), uint64_to_p(uint64Max));
    CHECK(p->toHex() == "fffffffffffffffe0000000000000001");
    // Log::debug(": p->toHex() = " + p->toHex());
}

TEST_CASE("TEST G")
{
    string expectedGHex("037de384f98f6e038d2a3141825b33d5d45ec4cc64cfd15e750d6798f5196cf2a142cd"
                        "f33f6ef853840ec7d4ec80"
                        "4794cfb0cfb65363b2566387b98ee0e3def1b706fa55d5038ffb4a62dcbb93b1ddd8d3"
                        "b308da86d1c3a525ef356f"
                        "e5bb59314e65633480b396e1dd4b795f78de07d86b0e2a05be6af78fd7f736fcba6c03"
                        "2e26e050af50a03c65fa7b"
                        "6c87f4554cb57f3dabcbad8eb9d8fdebeef58570669acc3eda17dbfc47b8b3c39aa08b"
                        "829b28872e62b5d1b13a98"
                        "f09d40ac20c2ab74a6750e7c8750b5141e221c41f55bba31d8e41422b64d2cba7aaa0e"
                        "9fd8785702f6932825bf45"
                        "de8386d24900742062c1322b37c50af182158090c35da9355e6cf7f72da39a2284fdfb"
                        "1918b2a2a30e69501fa234"
                        "2b728263df23f1db8355bde1eb276fb3685f371672ceb313fdab069cc9b11ab6c59bce"
                        "62baad96aac96b0dbe0c7e"
                        "71fcb22552545a5d1cedeee01e4bc0cdbdb76b6ad45f09af5e71114a005f93ad97b8fe"
                        "09274e76c94b2008926b38"
                        "caec94c95e96d628f6bc80662ba06207801328b2c6a60526bf7cd02d9661385ac3b1cb"
                        "db50f759d0e9f61c11a07b"
                        "f4218f299bcb2900520076ebd2d95a3dee96d4809ef34abeb83fdba8a12c5ca8275728"
                        "8a89c931cf564f00e8a317"
                        "ae1e1d828e61369ba0ddbadb10c136f8691101ad82dc54775ab8353840d9992197d80a"
                        "6e94b38ac417cddf40b0c7"
                        "3abf03e8e0aa");

    auto gFromHex = hex_to_p(expectedGHex);
    // Log::debug(gFromHex->get(), 64UL, " : gFromHex = ");
    auto g = new ElementModP(const_cast<uint64_t *>(G_ARRAY), true);
    // Log::debug(" : g->toHex() = " + g->toHex());
    CHECK(g->toHex() == expectedGHex);
}

TEST_CASE("a_minus_b_mod_q 2 - 1 is 1 and 1 - 1 is 0")
{
    auto one = uint64_to_q(1);
    auto two = uint64_to_q(2);
    auto zero = uint64_to_q(0);

    auto result1 = a_minus_b_mod_q(two, one);
    // Log::debug(" : result1->toHex() = " + result1->toHex());
    CHECK((*result1 == *one));

    auto result0 = a_minus_b_mod_q(one, one);
    // Log::debug(" : result0->toHex() = " + result0->toHex());
    CHECK((*result0 == *zero));
}

TEST_CASE("a_minus_b_mod_q for max of q - 1 has hex value ending in 42")
{
    string expectedHex("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff42");
    // Max Q value in Hacl_Bignum4096 format
    uint64_t maxArrayQ[4U] = {0xFFFFFFFFFFFFFF43, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                              0xFFFFFFFFFFFFFFFF};
    auto maxQ = new ElementModQ(maxArrayQ, true);
    auto one = uint64_to_q(1);

    auto result = a_minus_b_mod_q(maxQ, one);
    // Log::debug(" : result->toHex() = " + result->toHex());
    CHECK(result->toHex() == expectedHex);
}