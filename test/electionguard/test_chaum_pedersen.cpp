#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/chaum_pedersen.hpp>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>

using namespace electionguard;

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

TEST_CASE("Disjunctive CP Proof simple valid inputs generate valid proofs")
{
    const auto &nonce = ONE_MOD_Q();
    const auto &seed = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());

    auto firstMessage = elgamalEncrypt(0UL, nonce, *keypair->getPublicKey());

    auto firstMessageZeroProof = DisjunctiveChaumPedersenProofHarness::make_zero(
      *firstMessage, nonce, *keypair->getPublicKey(), ONE_MOD_Q(), seed);
    auto firstMessageOneProof = DisjunctiveChaumPedersenProofHarness::make_one(
      *firstMessage, nonce, *keypair->getPublicKey(), ONE_MOD_Q(), seed);

    CHECK(firstMessageZeroProof->isValid(*firstMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          true);

    CHECK(firstMessageOneProof->isValid(*firstMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          false);

    auto secondMessage = elgamalEncrypt(1UL, nonce, *keypair->getPublicKey());
    auto secondMessageZeroProof = DisjunctiveChaumPedersenProofHarness::make_zero(
      *secondMessage, nonce, *keypair->getPublicKey(), ONE_MOD_Q(), seed);
    auto secondMessageOneProof = DisjunctiveChaumPedersenProofHarness::make_one(
      *secondMessage, nonce, *keypair->getPublicKey(), ONE_MOD_Q(), seed);

    CHECK(secondMessageZeroProof->isValid(*secondMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          false);
    CHECK(secondMessageOneProof->isValid(*secondMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          true);
}

TEST_CASE("Constant CP Proof encryption of zero")
{
    const auto &nonce = ONE_MOD_Q();
    const auto &seed = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());

    auto message = elgamalEncrypt(0UL, nonce, *keypair->getPublicKey());
    auto proof = ConstantChaumPedersenProof::make(*message, nonce, *keypair->getPublicKey(), seed,
                                                  ONE_MOD_Q(), 0UL);
    auto badProof = ConstantChaumPedersenProof::make(*message, nonce, *keypair->getPublicKey(),
                                                     seed, ONE_MOD_Q(), 1UL);

    CHECK(proof->isValid(*message, *keypair->getPublicKey(), ONE_MOD_Q()) == true);
    CHECK(badProof->isValid(*message, *keypair->getPublicKey(), ONE_MOD_Q()) == false);
}

TEST_CASE("Constant CP Proof encryption of one")
{
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    const auto &nonce = ONE_MOD_Q();
    const auto &seed = TWO_MOD_Q();

    auto message = elgamalEncrypt(1UL, nonce, *keypair->getPublicKey());
    auto proof = ConstantChaumPedersenProof::make(*message, nonce, *keypair->getPublicKey(), seed,
                                                  ONE_MOD_Q(), 1UL);
    auto badProof = ConstantChaumPedersenProof::make(*message, nonce, *keypair->getPublicKey(),
                                                     seed, ONE_MOD_Q(), 0UL);

    CHECK(proof->isValid(*message, *keypair->getPublicKey(), ONE_MOD_Q()) == true);
    CHECK(badProof->isValid(*message, *keypair->getPublicKey(), ONE_MOD_Q()) == false);
}
