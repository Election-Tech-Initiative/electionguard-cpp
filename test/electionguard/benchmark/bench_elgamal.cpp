#include "../utils/constants.hpp"

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
        nonce = rand_q();
        secret = ElementModQ::fromHex(a_fixed_secret);
        keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);
        fixed_base_keypair = ElGamalKeyPair::fromSecret(*secret);

        ciphertext = elgamalEncrypt(0UL, *nonce, *keypair->getPublicKey());
        fixed_base_ciphertext = elgamalEncrypt(0UL, *nonce, *fixed_base_keypair->getPublicKey());
    }

    void TearDown(const ::benchmark::State &state) {}

    unique_ptr<ElementModQ> nonce;
    unique_ptr<ElementModQ> secret;
    unique_ptr<ElGamalKeyPair> keypair;
    unique_ptr<ElGamalKeyPair> fixed_base_keypair;
    unique_ptr<ElGamalCiphertext> ciphertext;
    unique_ptr<ElGamalCiphertext> fixed_base_ciphertext;
};

BENCHMARK_DEFINE_F(ElgamalEncryptFixture, ElGamalEncrypt)(benchmark::State &state)
{
    for (auto _ : state) {
        elgamalEncrypt(0UL, *nonce, *keypair->getPublicKey());
    }
}

BENCHMARK_REGISTER_F(ElgamalEncryptFixture, ElGamalEncrypt)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(ElgamalEncryptFixture, ElGamalEncryptfixed_base)(benchmark::State &state)
{
    for (auto _ : state) {
        elgamalEncrypt(0UL, *nonce, *fixed_base_keypair->getPublicKey());
    }
}

BENCHMARK_REGISTER_F(ElgamalEncryptFixture, ElGamalEncryptfixed_base)
  ->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(ElgamalEncryptFixture, ElGamalDecrypt)(benchmark::State &state)
{
    auto two = TWO_MOD_Q();
    for (auto _ : state) {
        ciphertext->decrypt(two);
    }
}

BENCHMARK_REGISTER_F(ElgamalEncryptFixture, ElGamalDecrypt)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(ElgamalEncryptFixture, ElGamalDecrypt_fixed_base)(benchmark::State &state)
{
    for (auto _ : state) {
        fixed_base_ciphertext->decrypt(*secret);
    }
}

BENCHMARK_REGISTER_F(ElgamalEncryptFixture, ElGamalDecrypt_fixed_base)
  ->Unit(benchmark::kMillisecond);
