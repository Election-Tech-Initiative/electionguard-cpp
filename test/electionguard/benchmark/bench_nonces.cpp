#include "../utils/constants.hpp"

#include <benchmark/benchmark.h>
#include <electionguard/constants.h>
#include <electionguard/group.hpp>
#include <electionguard/nonces.hpp>

using namespace electionguard;
using namespace std;

class NonceFixture : public benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &state)
    {
        nonces =
          make_unique<Nonces>(*ElementModQ::fromUint64(123456789), "some-nonce-string-header");
    }

    void TearDown(const ::benchmark::State &state) {}

    unique_ptr<Nonces> nonces;
};

BENCHMARK_DEFINE_F(NonceFixture, create)(benchmark::State &state)
{
    for (auto _ : state) {
        auto nonce =
          make_unique<Nonces>(*ElementModQ::fromUint64(123456789), "some-nonce-string-header");
    }
}

BENCHMARK_REGISTER_F(NonceFixture, create)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(NonceFixture, next)(benchmark::State &state)
{
    for (auto _ : state) {
        auto p = nonces->next();
    }
}

BENCHMARK_REGISTER_F(NonceFixture, next)->Unit(benchmark::kMillisecond);
