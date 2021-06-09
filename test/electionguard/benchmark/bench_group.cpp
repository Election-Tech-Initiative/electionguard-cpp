#include "../utils/constants.hpp"

#include <benchmark/benchmark.h>
#include <electionguard/constants.h>
#include <electionguard/group.hpp>

using namespace electionguard;
using namespace std;

class GroupElementFixture : public benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &state)
    {
        p1 = make_unique<ElementModP>(LARGE_P_ARRAY_1);
        p2 = make_unique<ElementModP>(LARGE_P_ARRAY_2);
        two = ElementModP::fromUint64(2);

        q1 = make_unique<ElementModQ>(LARGE_Q_ARRAY_1);
        q2 = make_unique<ElementModQ>(LARGE_Q_ARRAY_2);

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

BENCHMARK_DEFINE_F(GroupElementFixture, ElementModP_fromArray)(benchmark::State &state)
{
    for (auto _ : state) {
        auto p = make_unique<ElementModP>(LARGE_P_ARRAY_1);
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, ElementModP_fromArray)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(GroupElementFixture, ElementModP_fromVector)(benchmark::State &state)
{
    for (auto _ : state) {
        auto p = make_unique<ElementModP>(p1_vector);
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, ElementModP_fromVector)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(GroupElementFixture, ElementModP_fromUint)(benchmark::State &state)
{
    for (auto _ : state) {
        auto one1 = ElementModP::fromUint64(1234567UL);
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, ElementModP_fromUint)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(GroupElementFixture, ElementModP_fromHex)(benchmark::State &state)
{
    for (auto _ : state) {
        auto one1 = ElementModP::fromHex(
          "F7D495A4FFBA3EEFB59DF18C34BE0675911D745E2D5AC500CDD2C670BE0A6B995B7B3E147FF066FA63DEF83D"
          "9C58096BAEDEAAB01F5BABE64D66EF6349B4900106CE80027B974601FE97E86C0F055D7EC563B80147A26820"
          "0C06CD702677430FDCC33BC7B5517C82999324271A60212B25502FF01832DCF0F0686B6D06BF0FE02B9EE729"
          "2EF4E70C650B374C959E506204E691E9180DB431251B681BEC38BF950550110A596712B374AB557AF8DCE2BA"
          "2820076D82A44ED0554E9443D1E61624CB8F0C3A8C4F9439225255B3570FBCBD3A0988B4FBF539426E5B10B9"
          "A97E0931275C8D65166220C9E85B9EBC3F73FAEF7AF21DA8E2AC0CFDB91CB23026C007F133C3DE22540EEC62"
          "51C066ABB8A400FB3693111022B9537632CF0C1DF8C3BA1767774657AB2595DBC6B16CA56B4C92D1620839F5"
          "320F2C854692CFB34A3D526E17358CFA2676E386D955212AA88ABC8381C680335C4BC116DE9169B074BE75A0"
          "007792660B42C6CE9DCE1C6286609F422F2CAF3CA84AB7EBEE67A408E8930A0EB10D1D0A81ABA2E12C4DE802"
          "188DB57652A5C8CD4AD08DCDB375BBD5212E08F8A88F23EEF964DDBB975001F1DF6648F21DF5CB96BA3FA7CC"
          "B887F3626B539FE518DE6B8C5931C1695C738B25F444A1040FD2484D62CB55C0078477CCB648198FFB490128"
          "6D7EFC54B53381285FF6449656A455C15D5B9848A232729EACCD9752");
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, ElementModP_fromHex)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(GroupElementFixture, add_mod_p)(benchmark::State &state)
{
    for (auto _ : state) {
        auto sum1 = add_mod_p(ONE_MOD_P(), TWO_MOD_P());
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, add_mod_p)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(GroupElementFixture, add_mod_p_large)(benchmark::State &state)
{
    for (auto _ : state) {
        auto sum1 = add_mod_p(*p1, *p2);
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, add_mod_p_large)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(GroupElementFixture, mul_mod_p)(benchmark::State &state)
{
    for (auto _ : state) {
        auto prod = mul_mod_p(*p1, *p2);
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, mul_mod_p)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(GroupElementFixture, pow_mod_p)(benchmark::State &state)
{
    for (auto _ : state) {
        auto exp = pow_mod_p(*p1, *p2);
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, pow_mod_p)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(GroupElementFixture, g_pow_p)(benchmark::State &state)
{
    for (auto _ : state) {
        auto exp = g_pow_p(*p2);
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, g_pow_p)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(GroupElementFixture, add_mod_q)(benchmark::State &state)
{
    for (auto _ : state) {
        auto sum1 = add_mod_q(ONE_MOD_Q(), TWO_MOD_Q());
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, add_mod_q)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(GroupElementFixture, add_mod_q_large)(benchmark::State &state)
{
    for (auto _ : state) {
        auto sum1 = add_mod_q(*q1, *q2);
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, add_mod_q_large)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(GroupElementFixture, add_mod_q_overflow)(benchmark::State &state)
{
    auto overflow = 188UL;
    for (auto _ : state) {
        overflow += 1;
        auto residue = ElementModQ::fromUint64(overflow);
        auto sum1 = add_mod_q(Q(), *residue);
    }
}

BENCHMARK_REGISTER_F(GroupElementFixture, add_mod_q_overflow)->Unit(benchmark::kMillisecond);
