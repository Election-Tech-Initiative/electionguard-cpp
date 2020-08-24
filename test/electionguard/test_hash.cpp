#include <doctest/doctest.h>
#include <electionguard/hash.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace electionguard;

bool hashes_are_eq(ElementModQ *a, ElementModQ *b)
{
    bool done = false;
    bool are_equal = true;
    int max_len = 64;
    int i = 0;
    while (!done) {
        auto l = a->get()[i];
        auto r = b->get()[i];

        are_equal = l == r;

        // done when we've reached the max_len OR bytes are no longer equal
        // OR we've reach null-terminating bytes for both
        done = (i == max_len) || !are_equal || ((l == (uint64_t)0U) && (r == (uint64_t)0U));

        // cout << "[" << i << "]" << l << " = " << r << endl;
        i++;
    }
    return are_equal;
}

TEST_CASE("ElementModP and ElementModQ with same Zero data")
{
    uint64_t p[1] = {0};
    uint64_t q[1] = {0};
    auto elemP = new ElementModP(p);
    auto elemQ = new ElementModQ(q);
    auto zero_hash_p = hash_elems(elemP);
    auto zero_hash_q = hash_elems(elemQ);

    CHECK(hashes_are_eq(zero_hash_p, zero_hash_q));
    // but different addresses
    CHECK(&zero_hash_p != &zero_hash_q);
}

TEST_CASE("Same Zero Value Hash with signed numbers")
{
    int z1 = 0;
    long z2 = 0;
    auto zero_hash1 = hash_elems(z1);
    auto zero_hash2 = hash_elems(z2);

    CHECK(hashes_are_eq(zero_hash1, zero_hash2));
    // but different addresses
    CHECK(&zero_hash1 != &zero_hash2);
}

TEST_CASE("Same Zero Value Hash with unsigned numbers")
{
    unsigned short z1 = 0;
    unsigned long z2 = 0;
    auto zero_hash1 = hash_elems(z1);
    auto zero_hash2 = hash_elems(z2);

    CHECK(hashes_are_eq(zero_hash1, zero_hash2));
    // but different addresses
    CHECK(&zero_hash1 != &zero_hash2);
}

TEST_CASE("Same Zero Value Hash for string and number")
{
    auto zero_hash1 = hash_elems((uint64_t)0);
    auto zero_hash2 = hash_elems("0");

    CHECK(hashes_are_eq(zero_hash1, zero_hash2));
    // but different addresses
    CHECK(&zero_hash1 != &zero_hash2);
}

TEST_CASE("Different strings not the same Hash")
{
    CHECK(false ==
          hashes_are_eq(
            hash_elems("0"),
            hash_elems(
              "51550449938001064785844756727912747714949358666715026308259290402648561267962")));
}

TEST_CASE("Different strings casing not the same Hash")
{
    CHECK(false == hashes_are_eq(hash_elems("Pam Was Here"), hash_elems("pam was here")));
}

TEST_CASE("Hash for empty string same as null string")
{
    CHECK(hashes_are_eq(hash_elems("null"), hash_elems("")));
}

TEST_CASE("Hash for nullptr same as null string")
{
    CHECK(hashes_are_eq(hash_elems("null"), hash_elems(nullptr)));
}

TEST_CASE("Hash of multiple zeros is different has than hash for single zero")
{
    CHECK(false == hashes_are_eq(hash_elems("0"), hash_elems({"0", "0"})));
}

TEST_CASE("Hash of same amount of multiple zeros are the same hash")
{
    CHECK(hashes_are_eq(hash_elems({0, 0}), hash_elems({"0", "0"})));
}

// TODO: equivalent of hashing a Sequence in Python?
// Need `ElementModQ.toBigIntString` implementation to validate crunching the recursive Q output works