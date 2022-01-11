#include "../../../src/electionguard/facades/Hacl_Bignum4096.hpp"
#include "../utils/constants.hpp"

#include <benchmark/benchmark.h>
#include <electionguard/constants.h>
#include <electionguard/group.hpp>
#include <electionguard/hash.hpp>

using namespace electionguard;
using namespace hacl;
using namespace std;

class HaclBignum4096Fixture : public benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &state)
    {
        p1 = make_unique<ElementModP>(LARGE_P_ARRAY_1, true);
        p2 = make_unique<ElementModP>(LARGE_P_ARRAY_2, true);
        two = ElementModP::fromUint64(2UL);

        q1 = make_unique<ElementModQ>(LARGE_Q_ARRAY_1, true);
        q2 = make_unique<ElementModQ>(LARGE_Q_ARRAY_2, true);

        auto array_size = sizeof(LARGE_P_ARRAY_1) / sizeof(uint64_t);
        p1_vector.assign(&LARGE_P_ARRAY_1[0], &LARGE_P_ARRAY_1[array_size]);
    }

    void TearDown(const ::benchmark::State &state) {}

    unique_ptr<ElementModP> p1;
    unique_ptr<ElementModP> p2;
    unique_ptr<ElementModP> two;
    unique_ptr<ElementModQ> q1;
    unique_ptr<ElementModQ> q2;
    vector<uint64_t> p1_vector;
};

BENCHMARK_DEFINE_F(HaclBignum4096Fixture, pow_mod_p_var_time)(benchmark::State &state)
{
    auto p = P().get();
    auto g = G().get();
    auto e = rand_q()->toElementModP()->get();
    uint64_t result[MAX_P_LEN] = {};
    for (auto _ : state) {
        auto success =
          Bignum4096::modExp(p, g, MAX_P_SIZE, e, static_cast<uint64_t *>(result), false);
    }
}

BENCHMARK_REGISTER_F(HaclBignum4096Fixture, pow_mod_p_var_time)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(HaclBignum4096Fixture, pow_mod_p_const_time)(benchmark::State &state)
{
    auto p = P().get();
    auto g = G().get();
    auto e = rand_q()->toElementModP()->get();
    uint64_t result[MAX_P_LEN] = {};
    for (auto _ : state) {
        auto success =
          Bignum4096::modExp(p, g, MAX_P_SIZE, e, static_cast<uint64_t *>(result), true);
    }
}

BENCHMARK_REGISTER_F(HaclBignum4096Fixture, pow_mod_p_const_time)->Unit(benchmark::kMillisecond);
