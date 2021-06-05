#include <benchmark/benchmark.h>
#include <electionguard/constants.h>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>

using namespace electionguard;
using namespace std;

class ElgamalEncryptFixture : public benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &state)
    {
        nonce = ONE_MOD_Q().clone();
        secret = TWO_MOD_Q().clone();
        keypair = ElGamalKeyPair::fromSecret(*secret);

        ciphertext = elgamalEncrypt(0UL, *nonce, *keypair->getPublicKey());
    }

    void TearDown(const ::benchmark::State &state) {}

    unique_ptr<ElementModQ> nonce;
    unique_ptr<ElementModQ> secret;
    unique_ptr<ElGamalKeyPair> keypair;
    unique_ptr<ElGamalCiphertext> ciphertext;
};

BENCHMARK_DEFINE_F(ElgamalEncryptFixture, ElGamalEncrypt)(benchmark::State &state)
{
    for (auto _ : state) {
        elgamalEncrypt(0UL, *nonce, *keypair->getPublicKey());
    }
}

BENCHMARK_REGISTER_F(ElgamalEncryptFixture, ElGamalEncrypt)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(ElgamalEncryptFixture, ElGamalDecrypt)(benchmark::State &state)
{
    for (auto _ : state) {
        ciphertext->decrypt(*secret);
    }
}

BENCHMARK_REGISTER_F(ElgamalEncryptFixture, ElGamalDecrypt)->Unit(benchmark::kMillisecond);
