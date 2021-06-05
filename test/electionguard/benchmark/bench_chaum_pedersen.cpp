#include <benchmark/benchmark.h>
#include <electionguard/chaum_pedersen.hpp>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>

using namespace electionguard;
using namespace std;

class ChaumPedersenFixture : public benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &state)
    {
        keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());

        nonce = ONE_MOD_Q().clone();
        seed = TWO_MOD_Q().clone();

        message = elgamalEncrypt(1UL, *nonce, *keypair->getPublicKey());
        disjunctive = DisjunctiveChaumPedersenProof::make(
          *message, *nonce, *keypair->getPublicKey(), ONE_MOD_Q(), *seed, 1);
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

BENCHMARK_DEFINE_F(ChaumPedersenFixture, CheckDisjunctiveChaumPedersen)(benchmark::State &state)
{
    while (state.KeepRunning()) {
        disjunctive->isValid(*message, *keypair->getPublicKey(), ONE_MOD_Q());
    }
}

BENCHMARK_REGISTER_F(ChaumPedersenFixture, CheckDisjunctiveChaumPedersen)
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
