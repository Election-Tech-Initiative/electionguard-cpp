#include <doctest/doctest.h>
#include <electionguard/chaum_pedersen.hpp>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>

using namespace electionguard;

class DisjunctiveChaumPedersenProofHarness : DisjunctiveChaumPedersenProof
{
  public:
    static DisjunctiveChaumPedersenProof *make_zero(ElGamalCiphertext *message, ElementModQ *r,
                                                    ElementModP *k, ElementModQ *q,
                                                    ElementModQ *seed)
    {
        return DisjunctiveChaumPedersenProof::make_zero(message, r, k, q, seed);
    }
    static DisjunctiveChaumPedersenProof *make_one(ElGamalCiphertext *message, ElementModQ *r,
                                                   ElementModP *k, ElementModQ *q,
                                                   ElementModQ *seed)
    {
        return DisjunctiveChaumPedersenProof::make_one(message, r, k, q, seed);
    }
};

TEST_CASE("Disjunctive CP Proof simple valid inputs generate valid proofs")
{
    auto *keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q());
    auto *nonce = ONE_MOD_Q();
    auto *seed = TWO_MOD_Q();

    auto *firstMessage = elgamalEncrypt(0, nonce, keypair->getPublicKey());
    auto *firstMessageZeroProof = DisjunctiveChaumPedersenProofHarness::make_zero(
      firstMessage, nonce, keypair->getPublicKey(), ONE_MOD_Q(), seed);
    auto *firstMessageOneProof = DisjunctiveChaumPedersenProofHarness::make_one(
      firstMessage, nonce, keypair->getPublicKey(), ONE_MOD_Q(), seed);

    CHECK(firstMessageZeroProof->isValid(firstMessage, keypair->getPublicKey(), ONE_MOD_Q()) ==
          true);
    // TODO: execute with new math
    // CHECK(firstMessageOneProof->isValid(firstMessage, keypair->getPublicKey(), ONE_MOD_Q()) ==
    //       false);

    auto *secondMessage = elgamalEncrypt(1, nonce, keypair->getPublicKey());
    auto *secondMessageZeroProof = DisjunctiveChaumPedersenProofHarness::make_zero(
      secondMessage, nonce, keypair->getPublicKey(), ONE_MOD_Q(), seed);
    auto *secondMessageOneProof = DisjunctiveChaumPedersenProofHarness::make_one(
      secondMessage, nonce, keypair->getPublicKey(), ONE_MOD_Q(), seed);

    // TODO: execute with new math
    // CHECK(secondMessageZeroProof->isValid(secondMessage, keypair->getPublicKey(), ONE_MOD_Q()) ==
    //       false);
    CHECK(secondMessageOneProof->isValid(firstMessage, keypair->getPublicKey(), ONE_MOD_Q()) ==
          true);
}

TEST_CASE("Disjunctive CP Proof simple invalid inputs generate invalid proofs") {}