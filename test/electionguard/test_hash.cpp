#include <doctest/doctest.h>
#include <electionguard/hash.hpp>

TEST_CASE("Hash")
{
    using namespace electionguard;

    CHECK(hash_elems("hello world") == 5);
}