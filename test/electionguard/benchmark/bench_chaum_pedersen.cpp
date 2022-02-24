#include <benchmark/benchmark.h>
#include <electionguard/chaum_pedersen.hpp>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>

using namespace electionguard;
using namespace std;

class DisjunctiveChaumPedersenProofHarness : DisjunctiveChaumPedersenProof
{
  public:
    static unique_ptr<DisjunctiveChaumPedersenProof>
    make_zero(const ElGamalCiphertext &message, const ElementModQ &r, const ElementModP &k,
              const ElementModQ &q, const ElementModQ &seed)
    {
        return DisjunctiveChaumPedersenProof::make_zero(message, r, k, q, seed);
    }
    static unique_ptr<DisjunctiveChaumPedersenProof>
    make_one(const ElGamalCiphertext &message, const ElementModQ &r, const ElementModP &k,
             const ElementModQ &q, const ElementModQ &seed)
    {
        return DisjunctiveChaumPedersenProof::make_one(message, r, k, q, seed);
    }
};

class ChaumPedersenFixture : public benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &state)
    {
        keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);

        nonce = ONE_MOD_Q().clone();
        seed = TWO_MOD_Q().clone();

        message = elgamalEncrypt(1UL, *nonce, *keypair->getPublicKey());
        disjunctive = DisjunctiveChaumPedersenProof::make(
          *message, *nonce, *keypair->getPublicKey(), ONE_MOD_Q(), *seed, 1);
        constant = ConstantChaumPedersenProof::make(*message, *nonce, *keypair->getPublicKey(),
                                                    *seed, ONE_MOD_Q(), 1UL);
    }

    void TearDown(const ::benchmark::State &state) {}
    unique_ptr<ElementModQ> nonce;
    unique_ptr<ElementModQ> seed;
    unique_ptr<ElementModQ> hashContext;
    unique_ptr<ElGamalKeyPair> keypair;
    unique_ptr<ElGamalCiphertext> message;
    unique_ptr<DisjunctiveChaumPedersenProof> disjunctive;
    unique_ptr<ConstantChaumPedersenProof> constant;
};

BENCHMARK_DEFINE_F(ChaumPedersenFixture, MakeDisjunctiveChaumPedersen)(benchmark::State &state)
{
    while (state.KeepRunning()) {
        DisjunctiveChaumPedersenProof::make(*message, *nonce, *keypair->getPublicKey(), ONE_MOD_Q(),
                                            *seed, 1);
    }
}

BENCHMARK_REGISTER_F(ChaumPedersenFixture, MakeDisjunctiveChaumPedersen)
  ->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(ChaumPedersenFixture, MakeDisjunctiveZero)(benchmark::State &state)
{
    while (state.KeepRunning()) {
        auto item = DisjunctiveChaumPedersenProofHarness::make_zero(
          *message, *nonce, *keypair->getPublicKey(), ONE_MOD_Q(), *seed);
    }
}

BENCHMARK_REGISTER_F(ChaumPedersenFixture, MakeDisjunctiveZero)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(ChaumPedersenFixture, MakeDisjunctiveOne)(benchmark::State &state)
{
    while (state.KeepRunning()) {
        auto item = DisjunctiveChaumPedersenProofHarness::make_one(
          *message, *nonce, *keypair->getPublicKey(), ONE_MOD_Q(), *seed);
    }
}

BENCHMARK_REGISTER_F(ChaumPedersenFixture, MakeDisjunctiveOne)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(ChaumPedersenFixture, CheckDisjunctiveChaumPedersen)(benchmark::State &state)
{
    while (state.KeepRunning()) {
        disjunctive->isValid(*message, *keypair->getPublicKey(), ONE_MOD_Q());
    }
}

BENCHMARK_REGISTER_F(ChaumPedersenFixture, CheckDisjunctiveChaumPedersen)
  ->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(ChaumPedersenFixture, CloneDisjunctiveChaumPedersen)(benchmark::State &state)
{
    while (state.KeepRunning()) {
        auto result = make_unique<DisjunctiveChaumPedersenProof>(*disjunctive);
    }
}

BENCHMARK_REGISTER_F(ChaumPedersenFixture, CloneDisjunctiveChaumPedersen)
  ->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(ChaumPedersenFixture, MakeConstantChaumPedersen)(benchmark::State &state)
{
    for (auto _ : state) {
        auto proof = ConstantChaumPedersenProof::make(*message, *nonce, *keypair->getPublicKey(),
                                                      *seed, ONE_MOD_Q(), 1UL);
    }
}

BENCHMARK_REGISTER_F(ChaumPedersenFixture, MakeConstantChaumPedersen)
  ->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(ChaumPedersenFixture, CheckConstantChaumPedersen)(benchmark::State &state)
{
    for (auto _ : state) {
        auto proof = constant->isValid(*message, *keypair->getPublicKey(), ONE_MOD_Q());
    }
}

BENCHMARK_REGISTER_F(ChaumPedersenFixture, CheckConstantChaumPedersen)
  ->Unit(benchmark::kMillisecond);
