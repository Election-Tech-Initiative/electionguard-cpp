#include <doctest/doctest.h>
#include <electionguard/hash.hpp>

int untested_function() { return 123; }

TEST_CASE("Hash")
{
    using namespace electionguard;

    CHECK(electionguard::hash_elems("hello world") == 5);
}