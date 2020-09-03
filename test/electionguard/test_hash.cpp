#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/hash.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace electionguard;

TEST_CASE("Same ElementModQs with same Zero data produce same Hash")
{
    uint64_t z1[4] = {};
    uint64_t z2[4] = {};

    auto zero_hash_q1 = hash_elems(new ElementModQ(z1));
    auto zero_hash_q2 = hash_elems(new ElementModQ(z2));

    // Log::debug(" : zero_hash_q1 = " + zero_hash_q1->toHex());
    // Log::debug(" : zero_hash_q2 = " + zero_hash_q2->toHex());
    CHECK((*zero_hash_q1 == *zero_hash_q2));
    // but different addresses
    CHECK(&zero_hash_q1 != &zero_hash_q2);
}

TEST_CASE("Hash Value for Zero string different from Zero number")
{
    auto zero_hash1 = hash_elems(0UL);
    auto zero_hash2 = hash_elems("0");

    // Log::debug(" : zero_hash1 = " + zero_hash1->toHex());
    // Log::debug(" : zero_hash2 = " + zero_hash2->toHex());
    CHECK((*zero_hash1 != *zero_hash2));
    // but different addresses
    CHECK(&zero_hash1 != &zero_hash2);
}

TEST_CASE("Hash Value for non-zero number string same as explicit number")
{
    auto one_hash1 = hash_elems(1UL);
    auto one_hash2 = hash_elems("1");

    // Log::debug(" : one_hash1 = " + one_hash1->toHex());
    // Log::debug(" : one_hash2 = " + one_hash2->toHex());
    CHECK((*one_hash1 == *one_hash2));
    // but different addresses
    CHECK(&one_hash1 != &one_hash2);
}

TEST_CASE("Same Number Value Hash with explicit number")
{
    auto one_hash1 = hash_elems(1UL);
    auto one_hash2 = hash_elems(1UL);

    // Log::debug(" : one_hash1 = " + one_hash1->toHex());
    // Log::debug(" : one_hash2 = " + one_hash2->toHex());
    CHECK((*one_hash1 == *one_hash2));
    // but different addresses
    CHECK(&one_hash1 != &one_hash2);
}

TEST_CASE("Same strings are the same Hash") { CHECK((*hash_elems("0") == *hash_elems("0"))); }

TEST_CASE("Different strings not the same Hash")
{
    CHECK((*hash_elems("0") !=
           *hash_elems(
             "51550449938001064785844756727912747714949358666715026308259290402648561267962")));
}

TEST_CASE("Different strings casing not the same Hash")
{
    CHECK((*hash_elems("Welcome To ElectionGuard") != *hash_elems("welcome to electionguard")));
}

TEST_CASE("Hash for empty string same as null string")
{
    CHECK((*hash_elems("null") == *hash_elems("")));
}

TEST_CASE("Hash for nullptr same as explicit zero number")
{
    CHECK((*hash_elems(0UL) == *hash_elems(nullptr)));
}

TEST_CASE("Hash for nullptr same as null string")
{
    CHECK((*hash_elems("null") == *hash_elems(nullptr)));
}

TEST_CASE("Hash of multiple zeros in list is different has than hash for single zero")
{
    CHECK((*hash_elems("0") != *hash_elems({"0", "0"})));
}

TEST_CASE("Hash of same values in list are the same hash")
{
    CHECK((*hash_elems({"0", "0"}) == *hash_elems({"0", "0"})));
}

TEST_CASE("Same Hash Value from nested-list and taking result of hashed list and hashing it's hex")
{
    auto nestedHash = hash_elems({vector<string>{"0", "1"}, "3"});
    auto nonNestedHash1 = hash_elems({"0", "1"});
    auto nonNestedHash2 = hash_elems({nonNestedHash1->toHex(), "3"});

    // Log::debug(" : nestedHash = " + nestedHash->toHex());
    // Log::debug(" : nonNestedHash2 = " + nonNestedHash2->toHex());
    CHECK((*nestedHash != *nonNestedHash1));
    CHECK((*nestedHash == *nonNestedHash2));
    // but different addresses
    CHECK(&nestedHash != &nonNestedHash2);
}
