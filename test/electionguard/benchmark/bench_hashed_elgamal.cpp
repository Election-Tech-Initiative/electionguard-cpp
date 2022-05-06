#include "../utils/constants.hpp"

#include <benchmark/benchmark.h>
#include <electionguard/constants.h>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>
#include <electionguard/precompute_buffers.hpp>

using namespace electionguard;
using namespace std;

class HashedElgamalEncryptFixture : public benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &state)
    {
        
        nonce = rand_q();
        secret = ElementModQ::fromHex(a_fixed_secret);
        keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);
        uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                     0x191a1b1c1d1e1f20};
        descriptionHash = make_unique<ElementModQ>(qwords_to_use);
        uint8_t bytes_to_use[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                    0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                    0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};
        vector<uint8_t> plain(bytes_to_use, bytes_to_use + sizeof(bytes_to_use));
        plaintext = plain;

        std::unique_ptr<HashedElGamalCiphertext> HEGResult = hashedElgamalEncrypt(
          plaintext, *nonce, *keypair->getPublicKey(), *descriptionHash, NO_PADDING);
            
    }

    void TearDown(const ::benchmark::State &state) {}

    unique_ptr<ElementModQ> nonce;
    unique_ptr<ElementModQ> secret;
    unique_ptr<ElGamalKeyPair> keypair;
    unique_ptr<ElementModQ> descriptionHash;
    vector<uint8_t> plaintext;
};

BENCHMARK_DEFINE_F(HashedElgamalEncryptFixture, HashedElGamalEncrypt)(benchmark::State &state)
{
    for (auto _ : state) {
        hashedElgamalEncrypt(plaintext, *nonce, *keypair->getPublicKey(),
            *descriptionHash, NO_PADDING);
    }
}

BENCHMARK_REGISTER_F(HashedElgamalEncryptFixture, HashedElGamalEncrypt)->Unit(benchmark::kMillisecond);

class HashedElgamalEncryptPrecomputeFixture : public benchmark::Fixture
{
  public:
    void SetUp(const ::benchmark::State &state)
    {

        nonce = rand_q();
        secret = ElementModQ::fromHex(a_fixed_secret);
        keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);
        uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                     0x191a1b1c1d1e1f20};
        descriptionHash = make_unique<ElementModQ>(qwords_to_use);
        uint8_t bytes_to_use[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                    0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                    0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};
        vector<uint8_t> plain(bytes_to_use, bytes_to_use + sizeof(bytes_to_use));
        plaintext = plain;

        std::unique_ptr<HashedElGamalCiphertext> HEGResult = hashedElgamalEncrypt(
          plaintext, *nonce, *keypair->getPublicKey(), *descriptionHash, NO_PADDING);

        // cause precomputed entries that will be used by the selection
        // encryptions, that should be more than enough and on teardown
        // the rest will be removed.
        PrecomputeBufferContext::init(300);
        PrecomputeBufferContext::populate(*keypair->getPublicKey());
    }

    void TearDown(const ::benchmark::State &state)
    {
        PrecomputeBufferContext::empty_queues(); 
    }

    unique_ptr<ElementModQ> nonce;
    unique_ptr<ElementModQ> secret;
    unique_ptr<ElGamalKeyPair> keypair;
    unique_ptr<ElementModQ> descriptionHash;
    vector<uint8_t> plaintext;
};

BENCHMARK_DEFINE_F(HashedElgamalEncryptPrecomputeFixture, HashedElGamalEncryptPrecompute)
(benchmark::State &state)
{
    for (auto _ : state) {
        hashedElgamalEncrypt(plaintext, *nonce, *keypair->getPublicKey(), *descriptionHash,
                             NO_PADDING);
    }
}

BENCHMARK_REGISTER_F(HashedElgamalEncryptPrecomputeFixture, HashedElGamalEncryptPrecompute)
  ->Unit(benchmark::kMillisecond);


