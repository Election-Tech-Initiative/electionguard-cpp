#include "../generators/ballot.hpp"
#include "../generators/election.hpp"
#include "../generators/manifest.hpp"
#include "../utils/constants.hpp"

#include <benchmark/benchmark.h>
#include <electionguard/ballot.hpp>
#include <electionguard/election.hpp>
#include <electionguard/encrypt.hpp>
#include <electionguard/manifest.hpp>
#include <electionguard/nonces.hpp>

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
        auto secret = ElementModQ::fromHex(a_fixed_secret);
        keypair = ElGamalKeyPair::fromSecret(*secret);

        metadata = make_unique<SelectionDescription>(selectionId, candidateId, 1UL);
        hashContext = metadata->crypto_hash();
        plaintext = BallotGenerator::selectionFrom(*metadata);

        auto nonce = ElementModQ::fromHex(a_fixed_nonce);
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
        auto secret = ElementModQ::fromHex(a_fixed_secret);
        nonce = ElementModQ::fromHex(a_fixed_nonce);
        nonces = make_unique<Nonces>(*nonce, "encrypt-ballot-benchmark");
        keypair = ElGamalKeyPair::fromSecret(*secret);
        manifest = ManifestGenerator::getManifestFromFile(TEST_SPEC_VERSION, TEST_USE_SAMPLE);
        internal = make_unique<InternalManifest>(*manifest);
        context = ElectionGenerator::getFakeContext(*internal, *keypair->getPublicKey());
        contextJson = ElectionGenerator::getJsonContext();
        device = make_unique<EncryptionDevice>(12345UL, 23456UL, 34567UL, "Location");
        ballot = BallotGenerator::getFakeBallot(*internal);

        ciphertext = encryptBallot(*ballot, *internal, *context, *device->getHash(), nonces->next(),
                                   0ULL, false);
        ciphertext = encryptBallot(*ballot, *internal, *contextJson, *device->getHash(),
                                   nonces->next(), 0ULL, false);
    }

    void TearDown(const ::benchmark::State &state) {}

    unique_ptr<Nonces> nonces;
    unique_ptr<ElGamalKeyPair> keypair;
    unique_ptr<Manifest> manifest;
    unique_ptr<InternalManifest> internal;
    unique_ptr<CiphertextElectionContext> context;
    unique_ptr<CiphertextElectionContext> contextJson;
    unique_ptr<EncryptionDevice> device;
    unique_ptr<ElementModQ> nonce;
    unique_ptr<PlaintextBallot> ballot;
    unique_ptr<CiphertextBallot> ciphertext;
};

BENCHMARK_DEFINE_F(EncryptBallotFixture, encryptBallot_Full_FromJSON)(benchmark::State &state)
{
    // setup to test the encrypting of ballots with a context generated from json
    for (auto _ : state) {
        auto result = encryptBallot(*ballot, *internal, *contextJson, *device->getHash(),
                                    make_unique<ElementModQ>(*nonce), 0ULL, false);
    }
}

BENCHMARK_DEFINE_F(EncryptBallotFixture, encryptBallot_Full_NoProofCheck)(benchmark::State &state)
{
    for (auto _ : state) {
        auto result = encryptBallot(*ballot, *internal, *context, *device->getHash(),
                                    make_unique<ElementModQ>(*nonce), 0ULL, false);
    }
}

BENCHMARK_DEFINE_F(EncryptBallotFixture, encryptBallot_Full_WithProofCheck)(benchmark::State &state)
{
    // also generates the nonce internally
    for (auto _ : state) {
        auto result =
          encryptBallot(*ballot, *internal, *context, *device->getHash(), nullptr, 0ULL, true);
    }
}

BENCHMARK_DEFINE_F(EncryptBallotFixture, encryptBallot_Compact_NoProofCheck)
(benchmark::State &state)
{
    for (auto _ : state) {
        auto result = encryptCompactBallot(*ballot, *internal, *context, *device->getHash(),
                                           make_unique<ElementModQ>(*nonce), 0ULL, false);
    }
}

BENCHMARK_DEFINE_F(EncryptBallotFixture, encryptBallot_Compact_WithProofCheck)
(benchmark::State &state)
{
    // also generates the nonce internally
    for (auto _ : state) {
        auto result = encryptCompactBallot(*ballot, *internal, *context, *device->getHash(),
                                           nullptr, 0ULL, true);
    }
}

BENCHMARK_DEFINE_F(EncryptBallotFixture, encryptContests_Full_FromJSON)(benchmark::State &state)
{
    // setup to test the encrypting of ballots with a context generated from json
    for (auto _ : state) {
        auto result = encryptContests(*ballot, *internal, *contextJson, *nonce, false);
    }
}

BENCHMARK_DEFINE_F(EncryptBallotFixture, encryptContests_Full_NoProofCheck)(benchmark::State &state)
{
    // setup to test the encrypting of ballots with a context generated from json
    for (auto _ : state) {
        auto result = encryptContests(*ballot, *internal, *context, *nonce, false);
    }
}

BENCHMARK_REGISTER_F(EncryptBallotFixture, encryptBallot_Full_FromJSON)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(EncryptBallotFixture, encryptBallot_Full_NoProofCheck)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(EncryptBallotFixture, encryptBallot_Full_WithProofCheck)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(EncryptBallotFixture, encryptBallot_Compact_NoProofCheck)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(EncryptBallotFixture, encryptBallot_Compact_WithProofCheck)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(EncryptBallotFixture, encryptContests_Full_FromJSON)
  ->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(EncryptBallotFixture, encryptContests_Full_NoProofCheck)
  ->Unit(benchmark::kMillisecond);

#pragma endregion
