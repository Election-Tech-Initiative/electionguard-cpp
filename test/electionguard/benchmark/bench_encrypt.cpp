#include "../mocks/ballot.hpp"
#include "../mocks/election.hpp"
#include "../mocks/manifest.hpp"

#include <benchmark/benchmark.h>
#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>
#include <electionguard/encrypt.hpp>
#include <electionguard/manifest.hpp>

using namespace electionguard;
using namespace electionguard::test::mocks;
using namespace std;

class EncryptSelectionFixture : public benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &state)
    {
        const auto *candidateId = "some-candidate-id";
        const auto *selectionId = "some-selection-object-id";
        keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());

        metadata = make_unique<SelectionDescription>(selectionId, candidateId, 1UL);
        hashContext = metadata->crypto_hash();
        plaintext = BallotGenerator::selectionFrom(*metadata);

        auto nonce = rand_q();
        ciphertext = encryptSelection(*plaintext, *metadata, *keypair->getPublicKey(), ONE_MOD_Q(),
                                      *nonce, false, false);
    }

    void TearDown(const ::benchmark::State &state) {}

    unique_ptr<ElementModQ> hashContext;
    unique_ptr<ElGamalKeyPair> keypair;
    unique_ptr<SelectionDescription> metadata;
    unique_ptr<PlaintextBallotSelection> plaintext;
    unique_ptr<CiphertextBallotSelection> ciphertext;
};

BENCHMARK_DEFINE_F(EncryptSelectionFixture, Encrypt)(benchmark::State &state)
{
    for (auto _ : state) {
        auto nonce = rand_q();
        encryptSelection(*plaintext, *metadata, *keypair->getPublicKey(), ONE_MOD_Q(), *nonce,
                         false, false);
    }
}

BENCHMARK_REGISTER_F(EncryptSelectionFixture, Encrypt)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(EncryptSelectionFixture, CheckEncryption)(benchmark::State &state)
{
    while (state.KeepRunning()) {
        ciphertext->isValidEncryption(*hashContext, *keypair->getPublicKey(), ONE_MOD_Q());
    }
}

BENCHMARK_REGISTER_F(EncryptSelectionFixture, CheckEncryption)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(EncryptSelectionFixture, CheckProof)(benchmark::State &state)
{
    while (state.KeepRunning()) {
        ciphertext->getProof()->isValid(*ciphertext->getCiphertext(), *keypair->getPublicKey(),
                                        ONE_MOD_Q());
    }
}

BENCHMARK_REGISTER_F(EncryptSelectionFixture, CheckProof)->Unit(benchmark::kMillisecond);
