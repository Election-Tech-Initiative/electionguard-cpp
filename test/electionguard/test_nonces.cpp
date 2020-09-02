#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/nonces.hpp>

using namespace electionguard;

TEST_CASE("Nonces with same seed generated same output")
{
    uint64_t a[4] = {};
    auto n1 = new Nonces(new ElementModQ(a));
    auto n2 = new Nonces(new ElementModQ(a));
    // Log::debug(" : n1->get(0)->toHex() = " + n1->get(0)->toHex());
    // Log::debug(" : n2->get(0)->toHex() = " + n2->get(0)->toHex());
    CHECK(n1->get(0)->toHex() == n2->get(0)->toHex());
}

TEST_CASE("Nonce seeded with header is different from Nonce seeded without header")
{
    uint64_t a[4] = {};
    auto n1 = new Nonces(new ElementModQ(a));
    auto n2 = new Nonces(new ElementModQ(a), "0");
    // Log::debug(" : n1->get(0)->toHex() = " + n1->get(0)->toHex());
    // Log::debug(" : n2->get(0)->toHex() = " + n2->get(0)->toHex());
    CHECK(n1->get(0)->toHex() != n2->get(0)->toHex());
}

TEST_CASE("Nonces slices produce same output as iterating for each index")
{
    uint64_t a[4] = {};
    auto n = new Nonces(new ElementModQ(a));
    vector<ElementModQ *> l1;
    for (int i(0); i < 10; ++i) {
        l1.push_back(n->get(i));
    }
    auto l2 = (new Nonces(new ElementModQ(a)))->get(0UL, 10UL);

    CHECK(l1.size() == l2.size());

    for (size_t i(0); i < l1.size(); ++i) {
        // Log::debug(" : l1[" + to_string(i) + "]->toHex() = " + l1[i]->toHex());
        // Log::debug(" : l2[" + to_string(i) + "]->toHex() = " + l2[i]->toHex());
        CHECK(l1[i]->toHex() == l2[i]->toHex());
    }
}