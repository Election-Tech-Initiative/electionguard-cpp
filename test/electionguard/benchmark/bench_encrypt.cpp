#include "../generators/ballot.hpp"
#include "../generators/election.hpp"
#include "../generators/manifest.hpp"

#include <benchmark/benchmark.h>
#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>
#include <electionguard/encrypt.hpp>
#include <electionguard/manifest.hpp>

using namespace electionguard;
using namespace electionguard::tools::generators;
using namespace std;

#pragma region encryptSelection

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

BENCHMARK_DEFINE_F(EncryptSelectionFixture, encryptSelection)(benchmark::State &state)
{
    for (auto _ : state) {
        auto nonce = rand_q();
        encryptSelection(*plaintext, *metadata, *keypair->getPublicKey(), ONE_MOD_Q(), *nonce,
                         false, false);
    }
}

BENCHMARK_REGISTER_F(EncryptSelectionFixture, encryptSelection)->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(EncryptSelectionFixture, encryptSelection_isValid)(benchmark::State &state)
{
    while (state.KeepRunning()) {
        ciphertext->isValidEncryption(*hashContext, *keypair->getPublicKey(), ONE_MOD_Q());
    }
}

BENCHMARK_REGISTER_F(EncryptSelectionFixture, encryptSelection_isValid)
  ->Unit(benchmark::kMillisecond);

BENCHMARK_DEFINE_F(EncryptSelectionFixture, encryptSelection_getProof_isValid)
(benchmark::State &state)
{
    while (state.KeepRunning()) {
        ciphertext->getProof()->isValid(*ciphertext->getCiphertext(), *keypair->getPublicKey(),
                                        ONE_MOD_Q());
    }
}

BENCHMARK_REGISTER_F(EncryptSelectionFixture, encryptSelection_getProof_isValid)
  ->Unit(benchmark::kMillisecond);

#pragma endregion

#pragma region encryptBallot

class EncryptBallotFixture : public benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &state)
    {
        keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
        manifest = ManifestGenerator::getJeffersonCountryManifest_multipleBallotStyle_fromFile();
        internal = make_unique<InternalManifest>(*manifest);
        context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());
        device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");
        ballot = BallotGenerator::getSimpleBallotFromFile();

        nonce = rand_q();
        ciphertext = encryptBallot(*ballot, *internal, *context, *device->getHash(),
                                   make_unique<ElementModQ>(*nonce), 0ULL, false);
    }

    void TearDown(const ::benchmark::State &state) {}

    unique_ptr<ElGamalKeyPair> keypair;
    unique_ptr<Manifest> manifest;
    unique_ptr<InternalManifest> internal;
    unique_ptr<CiphertextElectionContext> context;
    unique_ptr<EncryptionDevice> device;
    unique_ptr<ElementModQ> nonce;
    unique_ptr<PlaintextBallot> ballot;
    unique_ptr<CiphertextBallot> ciphertext;
};

BENCHMARK_DEFINE_F(EncryptBallotFixture, encryptBallot)(benchmark::State &state)
{
    for (auto _ : state) {
        auto result = encryptBallot(*ballot, *internal, *context, *device->getHash(),
                                    make_unique<ElementModQ>(*nonce), 0ULL, false);
    }
}

BENCHMARK_REGISTER_F(EncryptBallotFixture, encryptBallot)->Unit(benchmark::kMillisecond);

#pragma endregion
