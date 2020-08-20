#include <doctest/doctest.h>
#include <electionguard/hash.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

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
    auto zero_hash1 = hash_elems(0);
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