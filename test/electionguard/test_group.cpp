#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
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
    Log::debug(" : \n\t p1 = " + p1->toHex() + "\n\t p2 = " + p2->toHex() +
               "\n\t result = " + result->toHex());

    CHECK(p1->toHex() != "00");
    CHECK(p2->toHex() != "00");
    CHECK(result->toHex() != "00");
}

TEST_CASE("Max of Q")
{
    // Max Q value in Hacl_Bignum4096 format
    uint64_t maxArrayQ[4U] = {0xFFFFFFFFFFFFFF43, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                              0xFFFFFFFFFFFFFFFF};
    auto maxQ = new ElementModQ(maxArrayQ);
    Log::debug(": max q as hex = " + maxQ->toHex());

    // Add 1 to the max of Q to catch exception from going over the max
    auto oneQ = uint64_to_q(1UL);
    Log::debug(": one q as hex = " + oneQ->toHex());
    try {
        auto sumQ = add_mod_q(oneQ, maxQ);
        Log::debug(": sumQ as hex= " + sumQ->toHex());
    } catch (exception &e) {
        Log::debug(": exception = " + (string)e.what());
    }
}

TEST_CASE("Hex string converted to Q matches original hex when converted back toHex")
{
    string expectedHex("f0f0f0f0f0f0f0f0");
    auto q = hex_to_q(expectedHex);
    CHECK(q->toHex() == expectedHex);
    Log::debug(": q->toHex() = " + q->toHex() + " and expectedHex = " + expectedHex);
}

TEST_CASE("mul_mod_p 3 * 3 should equal 9")
{
    auto p = mul_mod_p(uint64_to_p(3), uint64_to_p(3));
    CHECK(p->toHex() == "09");
    Log::debug(": p->toHex() = " + p->toHex());
}

TEST_CASE(
  "mul_mod_p for max uint64 * max uint64 should equal hex value fffffffffffffffe0000000000000001)")
{
    auto uint64Max = numeric_limits<uint64_t>::max();
    auto p = mul_mod_p(uint64_to_p(uint64Max), uint64_to_p(uint64Max));
    CHECK(p->toHex() == "fffffffffffffffe0000000000000001");
    Log::debug(": p->toHex() = " + p->toHex());
}