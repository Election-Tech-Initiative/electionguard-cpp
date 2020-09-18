#include "electionguard/chaum_pedersen.hpp"

#include "log.hpp"
#include "nonces.hpp"

#include <cstdlib>
#include <cstring>
#include <electionguard/hash.hpp>
#include <map>

using std::map;
using std::string;
namespace electionguard
{
#pragma region DisjunctiveChaumPedersenProof

    // Lifecycle Methods

    DisjunctiveChaumPedersenProof::DisjunctiveChaumPedersenProof(
      ElementModP *proof_zero_pad, ElementModP *proof_zero_data, ElementModP *proof_one_pad,
      ElementModP *proof_one_data, ElementModQ *proof_zero_challenge,
      ElementModQ *proof_one_challenge, ElementModQ *challenge, ElementModQ *proof_zero_response,
      ElementModQ *proof_one_response)
    {
        data.proof_zero_pad = proof_zero_pad;
        data.proof_zero_data = proof_zero_data;
        data.proof_one_pad = proof_one_pad;
        data.proof_one_data = proof_one_data;
        data.proof_zero_challenge = proof_zero_challenge;
        data.proof_one_challenge = proof_one_challenge;
        data.challenge = challenge;
        data.proof_zero_response = proof_zero_response;
        data.proof_one_response = proof_one_response;
    }

    DisjunctiveChaumPedersenProof::~DisjunctiveChaumPedersenProof() {}

    // Property Getters

    // Public Members

    bool DisjunctiveChaumPedersenProof::isValid(ElGamalCiphertext *message, ElementModP *k,
                                                ElementModQ *q)
    {

        auto *alpha = message->getPad();
        auto *beta = message->getData();

        auto *a0 = data.proof_zero_pad;
        auto *b0 = data.proof_zero_data;
        auto *a1 = data.proof_one_pad;
        auto *b1 = data.proof_one_data;
        auto *c0 = data.proof_zero_challenge;
        auto *c1 = data.proof_one_challenge;
        auto *c = data.challenge;
        auto *v0 = data.proof_zero_response;
        auto *v1 = data.proof_one_response;

        auto inBounds_alpha = alpha->isValidResidue();
        auto inBounds_beta = beta->isValidResidue();
        auto inBounds_a0 = a0->isValidResidue();
        auto inBounds_b0 = b0->isValidResidue();
        auto inBounds_a1 = a1->isValidResidue();
        auto inBounds_b1 = b1->isValidResidue();
        auto inBounds_c0 = c0->isInBounds();
        auto inBounds_c1 = c1->isInBounds();
        auto inBounds_v0 = v0->isInBounds();
        auto inBounds_v1 = v1->isInBounds();

        auto consistent_c =
          (*add_mod_q(c0, c1) == *c) && (*c == *hash_elems({q, alpha, beta, a0, b0, a1, b1}));

        // 𝑔^𝑣 mod 𝑝 = 𝑎 ⋅ 𝛼^𝑐 mod 𝑝
        auto consistent_gv0 = (*g_pow_p(v0) == *mul_mod_p(a0, pow_mod_p(alpha, c0)));

        auto consistent_gv1 = (*g_pow_p(v1) == *mul_mod_p(a1, pow_mod_p(alpha, c1)));

        // 𝐾^𝑣 mod 𝑝 = 𝑏 ⋅ 𝛽^𝑐 mod 𝑝
        auto consistent_kv0 = (*pow_mod_p(k, v0) == *mul_mod_p(b0, pow_mod_p(beta, c0)));

        // 𝑔^𝑐 ⋅ 𝐾^𝑣 mod 𝑝 = 𝑏 ⋅ 𝛽^𝑐 mod 𝑝
        auto consistent_gc1kv1 =
          (*mul_mod_p(g_pow_p(c1), pow_mod_p(k, v1)) == *mul_mod_p(b1, pow_mod_p(beta, c1)));

        auto success = inBounds_alpha && inBounds_beta && inBounds_a0 && inBounds_b0 &&
                       inBounds_a1 && inBounds_b1 && inBounds_c0 && inBounds_c1 && inBounds_v0 &&
                       inBounds_v1 && consistent_c && consistent_gv0 && consistent_gv1 &&
                       consistent_kv0 && consistent_gc1kv1;

        if (!success) {

            map<string, bool> printMap{
              {"inBounds_alpha", inBounds_alpha},
              {"inBounds_beta", inBounds_beta},
              {"inBounds_a0", inBounds_a0},
              {"inBounds_b0", inBounds_b0},
              {"inBounds_a1", inBounds_a1},
              {"inBounds_b1", inBounds_b1},
              {"inBounds_c0", inBounds_c0},
              {"inBounds_c1", inBounds_c1},
              {"inBounds_v0", inBounds_v0},
              {"inBounds_v1", inBounds_v1},
              {"consistent_c", consistent_c},
              {"consistent_gv0", consistent_gv0},
              {"consistent_gv1", consistent_gv1},
              {"consistent_kv0", consistent_kv0},
              {"consistent_gc1kv1", consistent_gc1kv1},
            };

            Log::debug(printMap, ": found an invalid Disjunctive Chaum-Pedersen proof: ");

            Log::debugHex(" q->get: ", q->toHex());
            Log::debugHex(" alpha->get: ", alpha->toHex());
            Log::debugHex(" beta->get: ", beta->toHex());
            Log::debugHex(" a0->get: ", a0->toHex());
            Log::debugHex(" b0->get: ", b0->toHex());
            Log::debugHex(" a1->get: ", a1->toHex());
            Log::debugHex(" b1->get: ", b1->toHex());
            Log::debugHex(" c0->get: ", c0->toHex());
            Log::debugHex(" c1->get: ", c1->toHex());
            Log::debugHex(" c->get: ", c->toHex());
            Log::debugHex(" v0->get: ", v0->toHex());
            Log::debugHex(" v1->get: ", v1->toHex());

            return false;
        }

        return success;
    }

    DisjunctiveChaumPedersenProof *
    DisjunctiveChaumPedersenProof::make(ElGamalCiphertext *message, ElementModQ *r, ElementModP *k,
                                        ElementModQ *q, ElementModQ *seed, uint64_t plaintext)
    {
        if (plaintext < 0 || plaintext > 1) {
            throw invalid_argument(
              "DisjunctiveChaumPedersenProof::make:: only supports plaintexts of 0 or 1");
        }
        if (plaintext == 1) {
            return make_one(message, r, k, q, seed);
        }
        return make_zero(message, r, k, q, seed);
    }

    // Private Members
    DisjunctiveChaumPedersenProof *
    DisjunctiveChaumPedersenProof::make_zero(ElGamalCiphertext *message, ElementModQ *r,
                                             ElementModP *k, ElementModQ *q, ElementModQ *seed)
    {
        auto *alpha = message->getPad();
        auto *beta = message->getData();

        // Pick three random numbers in Q.
        auto *nonces = new Nonces(seed, "disjoint-chaum-pedersen-proof");
        auto *c1 = nonces->get(0);
        auto *v1 = nonces->get(1);
        auto *u0 = nonces->get(2);

        // Compute the NIZKP
        auto *a0 = g_pow_p(u0);      //𝑔^𝑢 mod 𝑝
        auto *b0 = pow_mod_p(k, u0); // 𝐾^𝑢 mod 𝑝
        auto *q_min_c1 = sub_from_q(c1);
        auto *a1 = mul_mod_p(g_pow_p(v1), pow_mod_p(alpha, q_min_c1));
        auto *b1 = mul_mod_p({pow_mod_p(k, v1), g_pow_p(c1), pow_mod_p(beta, q_min_c1)});
        auto *c = hash_elems({q, alpha, beta, a0, b0, a1, b1});
        auto *c0 = sub_mod_q(c, c1);
        auto *v0 = a_plus_bc_mod_q(u0, c0, r);

        return new DisjunctiveChaumPedersenProof(a0, b0, a1, b1, c0, c1, c, v0, v1);
    }

    DisjunctiveChaumPedersenProof *
    DisjunctiveChaumPedersenProof::make_one(ElGamalCiphertext *message, ElementModQ *r,
                                            ElementModP *k, ElementModQ *q, ElementModQ *seed)
    {
        auto *alpha = message->getPad();
        auto *beta = message->getData();

        // Pick three random numbers in Q.
        auto *nonces = new Nonces(seed, "disjoint-chaum-pedersen-proof");
        auto *c0 = nonces->get(0);
        auto *v0 = nonces->get(1);
        auto *u1 = nonces->get(2);

        auto *q_min_c0 = sub_from_q(c0);
        auto *a0 = mul_mod_p(g_pow_p(v0), pow_mod_p(alpha, q_min_c0));
        auto *b0 = mul_mod_p(pow_mod_p(k, v0), pow_mod_p(beta, q_min_c0));
        auto *a1 = g_pow_p(u1);
        auto *b1 = pow_mod_p(k, u1);
        auto *c = hash_elems({q, alpha, beta, a0, b0, a1, b1});
        auto *c1 = sub_mod_q(c, c0);
        auto *v1 = a_plus_bc_mod_q(u1, c1, r);

        return new DisjunctiveChaumPedersenProof(a0, b0, a1, b1, c0, c1, c, v0, v1);
    }

#pragma endregion

#pragma region ConstantChaumPedersenProof

    // Lifecycle Methods

    ConstantChaumPedersenProof::ConstantChaumPedersenProof(ElementModP *pad, ElementModP *data,
                                                           ElementModQ *challenge,
                                                           ElementModQ *response, uint64_t constant)
    {
        this->data.pad = pad;
        this->data.data = data;
        this->data.challenge = challenge;
        this->data.response = response;
        this->data.constant = constant;
    }

    ConstantChaumPedersenProof::~ConstantChaumPedersenProof() {}

    // Property Getters

    // Public Members

    bool ConstantChaumPedersenProof::isValid(ElGamalCiphertext *message, ElementModP *k,
                                             ElementModQ *q)
    {
        // TODO: implement
        return true;
    }

    ConstantChaumPedersenProof *
    ConstantChaumPedersenProof::make(ElGamalCiphertext *message, ElementModQ *r, ElementModP *k,
                                     ElementModQ *seed, ElementModQ *hash_header, uint64_t constant)
    {
        // TODO: implement
        return new ConstantChaumPedersenProof(nullptr, nullptr, nullptr, nullptr, 0);
    }

#pragma endregion
} // namespace electionguard