#include <doctest/doctest.h>
#include <electionguard/hash.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace electionguard;

TEST_CASE("ElementModP and ElementModQ with same Zero data")
{
    uint64_t p[1] = {0};
    uint64_t q[1] = {0};
    auto elemP = new ElementModP(p);
    auto elemQ = new ElementModQ(q);
    auto zero_hash_p = hash_elems(elemP);
    auto zero_hash_q = hash_elems(elemQ);

    CHECK((*zero_hash_p == *zero_hash_q));
    // but different addresses
    CHECK(&zero_hash_p != &zero_hash_q);
}

TEST_CASE("Same Zero Value Hash with vectors")
{
    vector<uint64_t> z1 = {0};
    vector<uint64_t> z2 = {0};
    auto zero_hash1 = hash_elems(z1);
    auto zero_hash2 = hash_elems(z2);

    CHECK((*zero_hash1 == *zero_hash2));
    // but different addresses
    CHECK(&zero_hash1 != &zero_hash2);
}

TEST_CASE("Same Zero Value Hash with explicit number")
{
    uint64_t z1[1] = {0};
    uint64_t z2[1] = {0};
    auto zero_hash1 = hash_elems(z1[0]);
    auto zero_hash2 = hash_elems(z2[0]);

    CHECK((*zero_hash1 == *zero_hash2));
    // but different addresses
    CHECK(&zero_hash1 != &zero_hash2);
}

TEST_CASE("Same Zero Value Hash for string and number")
{
    auto zero_hash1 = hash_elems((uint64_t)0);
    auto zero_hash2 = hash_elems("0");

    CHECK((*zero_hash1 == *zero_hash2));
    // but different addresses
    CHECK(&zero_hash1 != &zero_hash2);
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

TEST_CASE("Hash for nullptr same as null string")
{
    CHECK((*hash_elems("null") == *hash_elems(nullptr)));
}

TEST_CASE("Hash of multiple zeros is different has than hash for single zero")
{
    CHECK((*hash_elems("0") != *hash_elems({"0", "0"})));
}

TEST_CASE("Hash of same amount of multiple zeros are the same hash")
{
    CHECK((*hash_elems({0UL, 0UL}) == *hash_elems({"0", "0"})));
}

// TODO: equivalent of hashing a Sequence in Python?
// Need `ElementModQ.toBigIntString` implementation to validate crunching the recursive Q output works