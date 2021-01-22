#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/nonces.hpp>

using namespace electionguard;

TEST_CASE("Nonces with same seed generated same output")
{
    uint64_t a[4] = {};
    auto aModQ = make_unique<ElementModQ>(a);
    auto n1 = make_unique<Nonces>(*aModQ);
    auto n2 = make_unique<Nonces>(*aModQ);

    CHECK(n1->get(0)->toHex() == n2->get(0)->toHex());
}

TEST_CASE("Nonce seeded with header is different from Nonce seeded without header")
{
    uint64_t a[4] = {};
    auto aModQ = make_unique<ElementModQ>(a);
    auto n1 = make_unique<Nonces>(*aModQ);
    auto n2 = make_unique<Nonces>(*aModQ, "0");

    CHECK(n1->get(0)->toHex() != n2->get(0)->toHex());
}

TEST_CASE("Nonces slices produce same output as iterating for each index")
{
    uint64_t a[4] = {};
    auto aModQ = make_unique<ElementModQ>(a);
    auto n = make_unique<Nonces>(*aModQ);
    vector<unique_ptr<ElementModQ>> l1;
    l1.reserve(10);
    for (uint64_t i(0); i < 10; ++i) {
        l1.push_back(n->get(i));
    }
    auto n2 = make_unique<Nonces>(*aModQ);
    auto l2 = n2->get(0UL, 10UL);

    CHECK(l1.size() == l2.size());

    for (size_t i(0); i < l1.size(); ++i) {
        CHECK(l1[i]->toHex() == l2[i]->toHex());
    }
}