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
    // Log::debug(" : \n\t p1 = " + p1->toHex() + "\n\t p2 = " + p2->toHex() +
    //            "\n\t result = " + result->toHex());

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
    // Log::debug(": max q as hex = " + maxQ->toHex());

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

    // G in Hacl_Bignum4096 format
    uint64_t g_array[64UL] = {
      0xb0c73abf03e8e0aa, 0x94b38ac417cddf40, 0x40d9992197d80a6e, 0x82dc54775ab83538,
      0x10c136f8691101ad, 0x8e61369ba0ddbadb, 0xe8a317ae1e1d82,   0x288a89c931cf564f,
      0xdba8a12c5ca82757, 0xd4809ef34abeb83f, 0x76ebd2d95a3dee96, 0x8f299bcb29005200,
      0xe9f61c11a07bf421, 0xc3b1cbdb50f759d0, 0xbf7cd02d9661385a, 0x801328b2c6a60526,
      0xf6bc80662ba06207, 0xcaec94c95e96d628, 0x76c94b2008926b38, 0x93ad97b8fe09274e,
      0x9af5e71114a005f,  0xc0cdbdb76b6ad45f, 0x5a5d1cedeee01e4b, 0xc7e71fcb2255254,
      0xbaad96aac96b0dbe, 0xc9b11ab6c59bce62, 0x72ceb313fdab069c, 0xeb276fb3685f3716,
      0xdf23f1db8355bde1, 0x501fa2342b728263, 0xfb1918b2a2a30e69, 0xf7f72da39a2284fd,
      0x8090c35da9355e6c, 0x322b37c50af18215, 0x86d24900742062c1, 0xf6932825bf45de83,
      0x7aaa0e9fd8785702, 0xd8e41422b64d2cba, 0x1e221c41f55bba31, 0xa6750e7c8750b514,
      0xf09d40ac20c2ab74, 0x872e62b5d1b13a98, 0xb3c39aa08b829b28, 0xcc3eda17dbfc47b8,
      0xfdebeef58570669a, 0x7f3dabcbad8eb9d8, 0xfa7b6c87f4554cb5, 0x26e050af50a03c65,
      0xd7f736fcba6c032e, 0x6b0e2a05be6af78f, 0xdd4b795f78de07d8, 0x4e65633480b396e1,
      0x25ef356fe5bb5931, 0xd3b308da86d1c3a5, 0x4a62dcbb93b1ddd8, 0xb706fa55d5038ffb,
      0x6387b98ee0e3def1, 0xcfb0cfb65363b256, 0x840ec7d4ec804794, 0xa142cdf33f6ef853,
      0x750d6798f5196cf2, 0xd45ec4cc64cfd15e, 0x8d2a3141825b33d5, 0x037de384f98f6e03,
    };

    auto g = new ElementModP(g_array, true);
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