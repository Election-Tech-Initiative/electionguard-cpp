#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/chaum_pedersen.hpp>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>
#include <iostream>
#include <string>

using namespace electionguard;
using namespace std;

class DisjunctiveChaumPedersenProofHarness : DisjunctiveChaumPedersenProof
{
  public:
    static unique_ptr<DisjunctiveChaumPedersenProof> make_zero(const ElGamalCiphertext &message,
                                                               const ElementModQ &r,
                                                               const ElementModP &k,
                                                               const ElementModQ &q)
    {
        return DisjunctiveChaumPedersenProof::make_zero(message, r, k, q);
    }
    static unique_ptr<DisjunctiveChaumPedersenProof>
    make_zero(const ElGamalCiphertext &message, const ElementModQ &r, const ElementModP &k,
              const ElementModQ &q, const ElementModQ &seed)
    {
        return DisjunctiveChaumPedersenProof::make_zero(message, r, k, q, seed);
    }
    static unique_ptr<DisjunctiveChaumPedersenProof> make_one(const ElGamalCiphertext &message,
                                                              const ElementModQ &r,
                                                              const ElementModP &k,
                                                              const ElementModQ &q)
    {
        return DisjunctiveChaumPedersenProof::make_one(message, r, k, q);
    }
    static unique_ptr<DisjunctiveChaumPedersenProof>
    make_one(const ElGamalCiphertext &message, const ElementModQ &r, const ElementModP &k,
             const ElementModQ &q, const ElementModQ &seed)
    {
        return DisjunctiveChaumPedersenProof::make_one(message, r, k, q, seed);
    }

    static unique_ptr<DisjunctiveChaumPedersenProof> make_zero_with_precomputed(
                               const ElGamalCiphertext &message,
                               unique_ptr<TwoTriplesAndAQuadruple> precomputedTwoTriplesAndAQuad1,
                               const ElementModQ &q)
    {
        return DisjunctiveChaumPedersenProof::make_zero_with_precomputed(
          message, move(precomputedTwoTriplesAndAQuad1), q);
    }

    static unique_ptr<DisjunctiveChaumPedersenProof> make_one_with_precomputed(
                              const ElGamalCiphertext &message,
                              unique_ptr<TwoTriplesAndAQuadruple> precomputedTwoTriplesAndAQuad1,
                              const ElementModQ &q)
    {
        return DisjunctiveChaumPedersenProof::make_one_with_precomputed(
          message, move(precomputedTwoTriplesAndAQuad1), q);
    }
};

TEST_CASE("Disjunctive CP Proof simple valid inputs generate valid proofs")
{
    // Arrange
    const auto &nonce = ONE_MOD_Q();
    const auto &seed = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);

    auto firstMessage = elgamalEncrypt(0UL, nonce, *keypair->getPublicKey());
    auto secondMessage = elgamalEncrypt(1UL, nonce, *keypair->getPublicKey());

    // Act
    auto firstMessageZeroProof = DisjunctiveChaumPedersenProofHarness::make_zero(
      *firstMessage, nonce, *keypair->getPublicKey(), ONE_MOD_Q());
    auto firstMessageOneProof = DisjunctiveChaumPedersenProofHarness::make_one(
      *firstMessage, nonce, *keypair->getPublicKey(), ONE_MOD_Q());

    auto secondMessageZeroProof = DisjunctiveChaumPedersenProofHarness::make_zero(
      *secondMessage, nonce, *keypair->getPublicKey(), ONE_MOD_Q());

    auto secondMessageOneProof = DisjunctiveChaumPedersenProofHarness::make_one(
      *secondMessage, nonce, *keypair->getPublicKey(), ONE_MOD_Q());

    // Assert
    CHECK(firstMessageZeroProof->isValid(*firstMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          true);

    CHECK(firstMessageOneProof->isValid(*firstMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          false);

    CHECK(secondMessageZeroProof->isValid(*secondMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          false);
    CHECK(secondMessageOneProof->isValid(*secondMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          true);
}

TEST_CASE("Constant CP Proof encryption of zero")
{
    const auto &nonce = ONE_MOD_Q();
    const auto &seed = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);

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
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);
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

TEST_CASE("Disjunctive CP Proof simple valid inputs generate valid proofs")
{
    // Arrange
    const auto &nonce = ONE_MOD_Q();
    const auto &seed = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);

    // cause a two triples and a quad to be populated
    PrecomputeBufferContext::init(4);
    PrecomputeBufferContext::populate(*keypair->getPublicKey());
    PrecomputeBufferContext::stop_populate();

    // this function runs off to look in the precomputed values buffer and if
    // it finds what it needs the the returned class will contain those values
    auto precomputedTwoTriplesAndAQuad1 = PrecomputeBufferContext::getTwoTriplesAndAQuadruple();
    auto precomputedTwoTriplesAndAQuad2 = PrecomputeBufferContext::getTwoTriplesAndAQuadruple();
    auto precomputedTwoTriplesAndAQuad3 = PrecomputeBufferContext::getTwoTriplesAndAQuadruple();
    auto precomputedTwoTriplesAndAQuad4 = PrecomputeBufferContext::getTwoTriplesAndAQuadruple();

    CHECK(precomputedTwoTriplesAndAQuad1 != nullptr);
    CHECK(precomputedTwoTriplesAndAQuad1->isPopulated() == true);
    CHECK(precomputedTwoTriplesAndAQuad2 != nullptr);
    CHECK(precomputedTwoTriplesAndAQuad2->isPopulated() == true);
    CHECK(precomputedTwoTriplesAndAQuad3 != nullptr);
    CHECK(precomputedTwoTriplesAndAQuad3->isPopulated() == true);
    CHECK(precomputedTwoTriplesAndAQuad4 != nullptr);
    CHECK(precomputedTwoTriplesAndAQuad4->isPopulated() == true);

    auto triple1_1 = precomputedTwoTriplesAndAQuad1->get_triple1();
    auto triple1_2 = precomputedTwoTriplesAndAQuad2->get_triple1();
    auto triple1_3 = precomputedTwoTriplesAndAQuad3->get_triple1();
    auto triple1_4 = precomputedTwoTriplesAndAQuad4->get_triple1();

    auto firstMessage = elgamalEncrypt_with_precomputed(0UL, *triple1_1->get_g_to_exp(),
                                                    *triple1_1->get_pubkey_to_exp());

    auto secondMessage = elgamalEncrypt_with_precomputed(0UL, *triple1_2->get_g_to_exp(),
                                                    *triple1_2->get_pubkey_to_exp());
                                                    
    auto thirdMessage = elgamalEncrypt_with_precomputed(1UL, *triple1_3->get_g_to_exp(),
                                                    *triple1_3->get_pubkey_to_exp());

    auto fourthMessage = elgamalEncrypt_with_precomputed(1UL, *triple1_4->get_g_to_exp(),
                                                    *triple1_4->get_pubkey_to_exp());

    // Act
    auto firstMessageZeroProof = DisjunctiveChaumPedersenProofHarness::make_zero_with_precomputed(
      *firstMessage, move(precomputedTwoTriplesAndAQuad1), ONE_MOD_Q());
    auto secondMessageOneProof = DisjunctiveChaumPedersenProofHarness::make_one_with_precomputed(
      *secondMessage, move(precomputedTwoTriplesAndAQuad2), ONE_MOD_Q());

    auto thirdMessageZeroProof = DisjunctiveChaumPedersenProofHarness::make_zero_with_precomputed(
      *thirdMessage, move(precomputedTwoTriplesAndAQuad3), ONE_MOD_Q());

    auto fourthMessageOneProof = DisjunctiveChaumPedersenProofHarness::make_one_with_precomputed(
      *fourthMessage, move(precomputedTwoTriplesAndAQuad4), ONE_MOD_Q());

    // Assert
    CHECK(firstMessageZeroProof->isValid(*firstMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          true);

    CHECK(secondMessageOneProof->isValid(*secondMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          false);

    CHECK(thirdMessageZeroProof->isValid(*thirdMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          false);
    CHECK(fourthMessageOneProof->isValid(*fourthMessage, *keypair->getPublicKey(), ONE_MOD_Q()) ==
          true);
    PrecomputeBufferContext::empty_queues();
}

TEST_CASE("Disjunctive CP Proof encryption of zero with precomputed values")
{
    const auto &nonce = ONE_MOD_Q();
    const auto &seed = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);

    // cause a two triples and a quad to be populated
    PrecomputeBufferContext::init(2);
    PrecomputeBufferContext::populate(*keypair->getPublicKey());
    PrecomputeBufferContext::stop_populate();

    // this function runs off to look in the precomputed values buffer and if
    // it finds what it needs the the returned class will contain those values
    auto precomputedTwoTriplesAndAQuad1 = PrecomputeBufferContext::getTwoTriplesAndAQuadruple();
    auto precomputedTwoTriplesAndAQuad2 = PrecomputeBufferContext::getTwoTriplesAndAQuadruple();

    CHECK(precomputedTwoTriplesAndAQuad1 != nullptr);
    CHECK(precomputedTwoTriplesAndAQuad1->isPopulated() == true);
    CHECK(precomputedTwoTriplesAndAQuad2 != nullptr);
    CHECK(precomputedTwoTriplesAndAQuad2->isPopulated() == true);

    auto triple1_1 = precomputedTwoTriplesAndAQuad1->get_triple1();
    auto triple1_2 = precomputedTwoTriplesAndAQuad2->get_triple1();

    auto message1 = elgamalEncrypt_with_precomputed(0UL, *triple1_1->get_g_to_exp(),
                                                    *triple1_1->get_pubkey_to_exp());

    auto message2 = elgamalEncrypt_with_precomputed(0UL, *triple1_2->get_g_to_exp(),
                                                    *triple1_2->get_pubkey_to_exp());

    auto proof = DisjunctiveChaumPedersenProof::make_with_precomputed(
      *message1, move(precomputedTwoTriplesAndAQuad1), ONE_MOD_Q(), 0UL);
    auto badProof = DisjunctiveChaumPedersenProof::make_with_precomputed(
      *message2, move(precomputedTwoTriplesAndAQuad2), ONE_MOD_Q(), 1UL);

    CHECK(proof->isValid(*message1, *keypair->getPublicKey(), ONE_MOD_Q()) == true);
    CHECK(badProof->isValid(*message2, *keypair->getPublicKey(), ONE_MOD_Q()) == false);
    PrecomputeBufferContext::empty_queues();
}

TEST_CASE("Disjunctive CP Proof encryption of one with precomputed values")
{
    auto keypair = ElGamalKeyPair::fromSecret(TWO_MOD_Q(), false);
    const auto &nonce = ONE_MOD_Q();
    const auto &seed = TWO_MOD_Q();

    // cause a two triples and a quad to be populated
    PrecomputeBufferContext::init(2);
    PrecomputeBufferContext::populate(*keypair->getPublicKey());
    PrecomputeBufferContext::stop_populate();

    // this function runs off to look in the precomputed values buffer and if
    // it finds what it needs the the returned class will contain those values
    auto precomputedTwoTriplesAndAQuad1 = PrecomputeBufferContext::getTwoTriplesAndAQuadruple();
    auto precomputedTwoTriplesAndAQuad2 = PrecomputeBufferContext::getTwoTriplesAndAQuadruple();

    CHECK(precomputedTwoTriplesAndAQuad1 != nullptr);
    CHECK(precomputedTwoTriplesAndAQuad1->isPopulated() == true);
    CHECK(precomputedTwoTriplesAndAQuad2 != nullptr);
    CHECK(precomputedTwoTriplesAndAQuad2->isPopulated() == true);

    auto triple1_1 = precomputedTwoTriplesAndAQuad1->get_triple1();
    auto triple1_2 = precomputedTwoTriplesAndAQuad2->get_triple1();

    auto message1 = elgamalEncrypt_with_precomputed(1UL, *triple1_1->get_g_to_exp(),
                                                    *triple1_1->get_pubkey_to_exp());

    auto message2 = elgamalEncrypt_with_precomputed(1UL, *triple1_2->get_g_to_exp(),
                                                    *triple1_2->get_pubkey_to_exp());

    auto proof = DisjunctiveChaumPedersenProof::make_with_precomputed(
      *message1, move(precomputedTwoTriplesAndAQuad1), ONE_MOD_Q(), 1UL);
    auto badProof = DisjunctiveChaumPedersenProof::make_with_precomputed(
      *message2, move(precomputedTwoTriplesAndAQuad2), ONE_MOD_Q(), 0UL);

    CHECK(proof->isValid(*message1, *keypair->getPublicKey(), ONE_MOD_Q()) == true);
    CHECK(badProof->isValid(*message2, *keypair->getPublicKey(), ONE_MOD_Q()) == false);
    PrecomputeBufferContext::empty_queues();
}
