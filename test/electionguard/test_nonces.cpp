#include <doctest/doctest.h>
#include <electionguard/nonces.hpp>

using namespace electionguard;

// TODO: add more tests once `toBigIntString` is implemented in `groups.cpp`

TEST_CASE("Nonces with same seed generated same output")
{
    uint64_t q[1] = {0};
    auto elemQ = new ElementModQ(q);
    auto n1 = new Nonces(elemQ);
    auto n2 = new Nonces(elemQ);
    CHECK(n1->get(0)->toBigIntString() == n2->get(0)->toBigIntString());
}

TEST_CASE("Nonce seeded with header is different from Nonce seeded without header")
{
    uint64_t q[1] = {0};
    auto elemQ = new ElementModQ(q);
    auto n1 = new Nonces(elemQ);
    auto n2 = new Nonces(elemQ, "0");
    CHECK(n1->get(0)->toBigIntString() != n2->get(0)->toBigIntString());
}
