#include <doctest/doctest.h>
#include <electionguard/nonces.hpp>

using namespace electionguard;

// // TODO: add more tests once `toHex` is implemented in `groups.cpp`

TEST_CASE("Nonces with same seed generated same output")
{
    uint64_t a1[4] = {};
    uint64_t a2[4] = {};
    auto n1 = new Nonces(new ElementModQ(a1));
    auto n2 = new Nonces(new ElementModQ(a2));
    CHECK(n1->get(0)->toHex() == n2->get(0)->toHex());
}

TEST_CASE("Nonce seeded with header is different from Nonce seeded without header")
{
    uint64_t q[4] = {};
    auto n1 = new Nonces(new ElementModQ(q));
    auto n2 = new Nonces(new ElementModQ(q), "0");
    CHECK(n1->get(0)->toHex() != n2->get(0)->toHex());
}
