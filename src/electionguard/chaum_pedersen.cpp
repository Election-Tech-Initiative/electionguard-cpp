#include "electionguard/chaum_pedersen.hpp"

#include "log.hpp"
#include "nonces.hpp"

#include <cstdlib>
#include <cstring>
#include <electionguard/hash.hpp>
#include <map>
#include <stdexcept>

using std::invalid_argument;
using std::make_unique;
using std::map;
using std::string;
using std::unique_ptr;

namespace electionguard
{
#pragma region DisjunctiveChaumPedersenProof

    struct DisjunctiveChaumPedersenProof::Impl {

        unique_ptr<ElementModP> proof_zero_pad;
        unique_ptr<ElementModP> proof_zero_data;
        unique_ptr<ElementModP> proof_one_pad;
        unique_ptr<ElementModP> proof_one_data;
        unique_ptr<ElementModQ> proof_zero_challenge;
        unique_ptr<ElementModQ> proof_one_challenge;
        unique_ptr<ElementModQ> challenge;
        unique_ptr<ElementModQ> proof_zero_response;
        unique_ptr<ElementModQ> proof_one_response;

        Impl(unique_ptr<ElementModP> proof_zero_pad, unique_ptr<ElementModP> proof_zero_data,
             unique_ptr<ElementModP> proof_one_pad, unique_ptr<ElementModP> proof_one_data,
             unique_ptr<ElementModQ> proof_zero_challenge,
             unique_ptr<ElementModQ> proof_one_challenge, unique_ptr<ElementModQ> challenge,
             unique_ptr<ElementModQ> proof_zero_response,
             unique_ptr<ElementModQ> proof_one_response)
            : proof_zero_pad(move(proof_zero_pad)), proof_zero_data(move(proof_zero_data)),
              proof_one_pad(move(proof_one_pad)), proof_one_data(move(proof_one_data)),
              proof_zero_challenge(move(proof_zero_challenge)),
              proof_one_challenge(move(proof_one_challenge)), challenge(move(challenge)),
              proof_zero_response(move(proof_zero_response)),
              proof_one_response(move(proof_one_response))
        {
        }

        [[nodiscard]] unique_ptr<DisjunctiveChaumPedersenProof::Impl> clone() const
        {
            auto _proof_zero_pad = make_unique<ElementModP>(*proof_zero_pad);
            auto _proof_zero_data = make_unique<ElementModP>(*proof_zero_data);
            auto _proof_one_pad = make_unique<ElementModP>(*proof_one_pad);
            auto _proof_one_data = make_unique<ElementModP>(*proof_one_data);
            auto _proof_zero_challenge = make_unique<ElementModQ>(*proof_zero_challenge);
            auto _proof_one_challenge = make_unique<ElementModQ>(*proof_one_challenge);
            auto _challenge = make_unique<ElementModQ>(*challenge);
            auto _proof_zero_response = make_unique<ElementModQ>(*proof_zero_response);
            auto _proof_one_response = make_unique<ElementModQ>(*proof_one_response);

            return make_unique<DisjunctiveChaumPedersenProof::Impl>(
              move(_proof_zero_pad), move(_proof_zero_data), move(_proof_one_pad),
              move(_proof_one_data), move(_proof_zero_challenge), move(_proof_one_challenge),
              move(_challenge), move(_proof_zero_response), move(_proof_one_response));
        }
    };

    // Lifecycle Methods

    DisjunctiveChaumPedersenProof::DisjunctiveChaumPedersenProof(
      const DisjunctiveChaumPedersenProof &other)
        : pimpl(other.pimpl->clone())
    {
    }

    DisjunctiveChaumPedersenProof::DisjunctiveChaumPedersenProof(
      unique_ptr<ElementModP> proof_zero_pad, unique_ptr<ElementModP> proof_zero_data,
      unique_ptr<ElementModP> proof_one_pad, unique_ptr<ElementModP> proof_one_data,
      unique_ptr<ElementModQ> proof_zero_challenge, unique_ptr<ElementModQ> proof_one_challenge,
      unique_ptr<ElementModQ> challenge, unique_ptr<ElementModQ> proof_zero_response,
      unique_ptr<ElementModQ> proof_one_response)
        : pimpl(new Impl(move(proof_zero_pad), move(proof_zero_data), move(proof_one_pad),
                         move(proof_one_data), move(proof_zero_challenge),
                         move(proof_one_challenge), move(challenge), move(proof_zero_response),
                         move(proof_one_response)))
    {
    }

    DisjunctiveChaumPedersenProof::~DisjunctiveChaumPedersenProof() = default;

    // Operator Overloads

    DisjunctiveChaumPedersenProof &
    DisjunctiveChaumPedersenProof::operator=(DisjunctiveChaumPedersenProof other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    ElementModP *DisjunctiveChaumPedersenProof::getProofZeroPad() const
    {
        return pimpl->proof_zero_pad.get();
    }
    ElementModP *DisjunctiveChaumPedersenProof::getProofZeroData() const
    {
        return pimpl->proof_zero_data.get();
    }
    ElementModP *DisjunctiveChaumPedersenProof::getProofOnePad() const
    {
        return pimpl->proof_one_pad.get();
    }
    ElementModP *DisjunctiveChaumPedersenProof::getProofOneData() const
    {
        return pimpl->proof_one_data.get();
    }
    ElementModQ *DisjunctiveChaumPedersenProof::getProofZeroChallenge() const
    {
        return pimpl->proof_zero_challenge.get();
    }
    ElementModQ *DisjunctiveChaumPedersenProof::getProofOneChallenge() const
    {
        return pimpl->proof_one_challenge.get();
    }
    ElementModQ *DisjunctiveChaumPedersenProof::getChallenge() const
    {
        return pimpl->challenge.get();
    }
    ElementModQ *DisjunctiveChaumPedersenProof::getProofZeroResponse() const
    {
        return pimpl->proof_zero_response.get();
    }
    ElementModQ *DisjunctiveChaumPedersenProof::getProofOneResponse() const
    {
        return pimpl->proof_one_response.get();
    }

    // Public Static Methods

    unique_ptr<DisjunctiveChaumPedersenProof>
    DisjunctiveChaumPedersenProof::make(const ElGamalCiphertext &message, const ElementModQ &r,
                                        const ElementModP &k, const ElementModQ &q,
                                        const ElementModQ &seed, uint64_t plaintext)
    {
        if (plaintext > 1) {
            throw invalid_argument(
              "DisjunctiveChaumPedersenProof::make:: only supports plaintexts of 0 or 1");
        }
        Log::trace("DisjunctiveChaumPedersenProof: making proof.");
        if (plaintext == 1) {
            return make_one(message, r, k, q, seed);
        }
        return make_zero(message, r, k, q, seed);
    }

    // Public Methods

    bool DisjunctiveChaumPedersenProof::isValid(const ElGamalCiphertext &message,
                                                const ElementModP &k, const ElementModQ &q)
    {
        Log::trace("DisjunctiveChaumPedersenProof::isValid: ");
        auto *alpha = message.getPad();
        auto *beta = message.getData();

        auto *a0p = pimpl->proof_zero_pad.get();
        auto *b0p = pimpl->proof_zero_data.get();
        auto *a1p = pimpl->proof_one_pad.get();
        auto *b1p = pimpl->proof_one_data.get();

        auto a0 = *pimpl->proof_zero_pad;
        auto b0 = *pimpl->proof_zero_data;
        auto a1 = *pimpl->proof_one_pad;
        auto b1 = *pimpl->proof_one_data;
        auto c0 = *pimpl->proof_zero_challenge;
        auto c1 = *pimpl->proof_one_challenge;
        auto c = *pimpl->challenge;
        auto v0 = *pimpl->proof_zero_response;
        auto v1 = *pimpl->proof_one_response;

        auto inBounds_alpha = alpha->isValidResidue();
        auto inBounds_beta = beta->isValidResidue();
        auto inBounds_a0 = a0.isValidResidue();
        auto inBounds_b0 = b0.isValidResidue();
        auto inBounds_a1 = a1.isValidResidue();
        auto inBounds_b1 = b1.isValidResidue();
        auto inBounds_c0 = c0.isInBounds();
        auto inBounds_c1 = c1.isInBounds();
        auto inBounds_v0 = v0.isInBounds();
        auto inBounds_v1 = v1.isInBounds();

        auto consistent_c =
          (*add_mod_q(c0, c1) == c) &&
          (c == *hash_elems({&const_cast<ElementModQ &>(q), alpha, beta, a0p, b0p, a1p, b1p}));

        // 𝑔^𝑣 mod 𝑝 = 𝑎 ⋅ 𝛼^𝑐 mod 𝑝
        auto consistent_gv0 = (*g_pow_p(v0) == *mul_mod_p(a0, *pow_mod_p(*alpha, c0)));

        auto consistent_gv1 = (*g_pow_p(v1) == *mul_mod_p(a1, *pow_mod_p(*alpha, c1)));

        // 𝐾^𝑣 mod 𝑝 = 𝑏 ⋅ 𝛽^𝑐 mod 𝑝
        auto consistent_kv0 = (*pow_mod_p(k, v0) == *mul_mod_p(b0, *pow_mod_p(*beta, c0)));

        // 𝑔^𝑐 ⋅ 𝐾^𝑣 mod 𝑝 = 𝑏 ⋅ 𝛽^𝑐 mod 𝑝
        auto consistent_gc1kv1 =
          (*mul_mod_p(*g_pow_p(c1), *pow_mod_p(k, v1)) == *mul_mod_p(b1, *pow_mod_p(*beta, c1)));

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

            Log::info("found an invalid Disjunctive Chaum-Pedersen proof", printMap);

            Log::debug("k->get", k.toHex());
            Log::debug("q->get", q.toHex());
            Log::debug("alpha->get", alpha->toHex());
            Log::debug("beta->get", beta->toHex());
            Log::debug("a0->get", a0.toHex());
            Log::debug("b0->get", b0.toHex());
            Log::debug("a1->get", a1.toHex());
            Log::debug("b1->get", b1.toHex());
            Log::debug("c0->get", c0.toHex());
            Log::debug("c1->get", c1.toHex());
            Log::debug("c->get", c.toHex());
            Log::debug("v0->get", v0.toHex());
            Log::debug("v1->get", v1.toHex());

            return false;
        }
        Log::trace("DisjunctiveChaumPedersenProof::isValid: TRUE!");
        return success;
    }

    std::unique_ptr<DisjunctiveChaumPedersenProof> DisjunctiveChaumPedersenProof::clone() const
    {
        return make_unique<DisjunctiveChaumPedersenProof>(
          pimpl->proof_zero_pad->clone(), pimpl->proof_zero_data->clone(),
          pimpl->proof_one_pad->clone(), pimpl->proof_one_pad->clone(),
          pimpl->proof_zero_challenge->clone(), pimpl->proof_one_challenge->clone(),
          pimpl->challenge->clone(), pimpl->proof_zero_response->clone(),
          pimpl->proof_one_response->clone());
    }

    // Protected Methods

    unique_ptr<DisjunctiveChaumPedersenProof>
    DisjunctiveChaumPedersenProof::make_zero(const ElGamalCiphertext &message, const ElementModQ &r,
                                             const ElementModP &k, const ElementModQ &q,
                                             const ElementModQ &seed)
    {
        auto *alpha = message.getPad();
        auto *beta = message.getData();

        // Pick three random numbers in Q.
        auto nonces = make_unique<Nonces>(seed, "disjoint-chaum-pedersen-proof");
        auto c1 = nonces->get(0);
        auto v1 = nonces->get(1);
        auto u0 = nonces->get(2);

        // Compute the NIZKP
        auto a0 = g_pow_p(*u0);      //𝑔^𝑢 mod 𝑝
        auto b0 = pow_mod_p(k, *u0); // 𝐾^𝑢 mod 𝑝
        auto q_min_c1 = sub_from_q(*c1);
        auto a1 = mul_mod_p(*g_pow_p(*v1), *pow_mod_p(*alpha, *q_min_c1));
        auto b1 = mul_mod_p(
          {pow_mod_p(k, *v1).get(), g_pow_p(*c1).get(), pow_mod_p(*beta, *q_min_c1).get()});
        auto c = hash_elems(
          {&const_cast<ElementModQ &>(q), alpha, beta, a0.get(), b0.get(), a1.get(), b1.get()});
        auto c0 = sub_mod_q(*c, *c1);
        auto v0 = a_plus_bc_mod_q(*u0, *c0, r);

        return make_unique<DisjunctiveChaumPedersenProof>(
          move(a0), move(b0), move(a1), move(b1), move(c0), move(c1), move(c), move(v0), move(v1));
    }

    unique_ptr<DisjunctiveChaumPedersenProof>
    DisjunctiveChaumPedersenProof::make_one(const ElGamalCiphertext &message, const ElementModQ &r,
                                            const ElementModP &k, const ElementModQ &q,
                                            const ElementModQ &seed)
    {
        auto *alpha = message.getPad();
        auto *beta = message.getData();

        // Pick three random numbers in Q.
        auto nonces = make_unique<Nonces>(seed, "disjoint-chaum-pedersen-proof");
        auto c0 = nonces->get(0);
        auto v0 = nonces->get(1);
        auto u1 = nonces->get(2);

        auto q_min_c0 = sub_from_q(*c0);
        auto a0 = mul_mod_p(*g_pow_p(*v0), *pow_mod_p(*alpha, *q_min_c0));
        auto b0 = mul_mod_p(*pow_mod_p(k, *v0), *pow_mod_p(*beta, *q_min_c0));
        auto a1 = g_pow_p(*u1);
        auto b1 = pow_mod_p(k, *u1);
        auto c = hash_elems(
          {&const_cast<ElementModQ &>(q), alpha, beta, a0.get(), b0.get(), a1.get(), b1.get()});
        auto c1 = sub_mod_q(*c, *c0);
        auto v1 = a_plus_bc_mod_q(*u1, *c1, r);

        return make_unique<DisjunctiveChaumPedersenProof>(
          move(a0), move(b0), move(a1), move(b1), move(c0), move(c1), move(c), move(v0), move(v1));
    }

#pragma endregion

#pragma region ConstantChaumPedersenProof

    struct ConstantChaumPedersenProof::Impl {
        unique_ptr<ElementModP> pad;
        unique_ptr<ElementModP> data;
        unique_ptr<ElementModQ> challenge;
        unique_ptr<ElementModQ> response;
        uint64_t constant;

        Impl(unique_ptr<ElementModP> pad, unique_ptr<ElementModP> data,
             unique_ptr<ElementModQ> challenge, unique_ptr<ElementModQ> response, uint64_t constant)
            : pad(move(pad)), data(move(data)), challenge(move(challenge)), response(move(response))
        {
            this->constant = constant;
        }

        [[nodiscard]] unique_ptr<ConstantChaumPedersenProof::Impl> clone() const
        {
            auto _pad = make_unique<ElementModP>(*pad);
            auto _data = make_unique<ElementModP>(*data);
            auto _challenge = make_unique<ElementModQ>(*challenge);
            auto _response = make_unique<ElementModQ>(*response);

            return make_unique<ConstantChaumPedersenProof::Impl>(
              move(_pad), move(_data), move(_challenge), move(_response), constant);
        }
    };

    // Lifecycle Methods

    ConstantChaumPedersenProof::ConstantChaumPedersenProof(const ConstantChaumPedersenProof &other)
        : pimpl(other.pimpl->clone())
    {
    }

    ConstantChaumPedersenProof::ConstantChaumPedersenProof(unique_ptr<ElementModP> pad,
                                                           unique_ptr<ElementModP> data,
                                                           unique_ptr<ElementModQ> challenge,
                                                           unique_ptr<ElementModQ> response,
                                                           uint64_t constant)
        : pimpl(new Impl(move(pad), move(data), move(challenge), move(response), constant))
    {
    }

    ConstantChaumPedersenProof::~ConstantChaumPedersenProof() = default;

    // Operator Overloads

    ConstantChaumPedersenProof &
    ConstantChaumPedersenProof::operator=(ConstantChaumPedersenProof other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    ElementModP *ConstantChaumPedersenProof::getPad() const { return pimpl->pad.get(); }
    ElementModP *ConstantChaumPedersenProof::getData() const { return pimpl->data.get(); }
    ElementModQ *ConstantChaumPedersenProof::getChallenge() const { return pimpl->challenge.get(); }
    ElementModQ *ConstantChaumPedersenProof::getResponse() const { return pimpl->response.get(); }
    uint64_t ConstantChaumPedersenProof::getConstant() const { return pimpl->constant; }

    // Public Static Methods

    unique_ptr<ConstantChaumPedersenProof>
    ConstantChaumPedersenProof::make(const ElGamalCiphertext &message, const ElementModQ &r,
                                     const ElementModP &k, const ElementModQ &seed,
                                     const ElementModQ &hash_header, uint64_t constant)
    {
        Log::trace("ConstantChaumPedersenProof:: making proof");
        auto *alpha = message.getPad();
        auto *beta = message.getData();

        // Pick a random number in Q.
        auto nonces = make_unique<Nonces>(seed, "constant-chaum-pedersen-proof");
        auto u = nonces->get(0);

        // Compute the NIZKP
        auto a = g_pow_p(*u);      //𝑔^𝑢 mod 𝑝
        auto b = pow_mod_p(k, *u); // 𝐾^𝑢 mod 𝑝

        // sha256(𝑄', A, B, a, b)
        auto c =
          hash_elems({&const_cast<ElementModQ &>(hash_header), alpha, beta, a.get(), b.get()});
        auto v = a_plus_bc_mod_q(*u, *c, r);

        return make_unique<ConstantChaumPedersenProof>(move(a), move(b), move(c), move(v),
                                                       constant);
    }

    // Public Methods

    bool ConstantChaumPedersenProof::isValid(const ElGamalCiphertext &message, const ElementModP &k,
                                             const ElementModQ &q)
    {
        Log::trace("ConstantChaumPedersenProof::isValid: checking validity");
        auto *alpha = message.getPad();
        auto *beta = message.getData();

        auto *a_ptr = pimpl->pad.get();
        auto *b_ptr = pimpl->data.get();
        auto *c_ptr = pimpl->challenge.get();

        auto a = *pimpl->pad;
        auto b = *pimpl->data;
        auto c = *pimpl->challenge;
        auto v = *pimpl->response;
        auto constant = pimpl->constant;

        auto inBounds_alpha = alpha->isValidResidue();
        auto inBounds_beta = beta->isValidResidue();
        auto inBounds_a = a.isValidResidue();
        auto inBounds_b = b.isValidResidue();
        auto inBounds_c = c.isInBounds();
        auto inBounds_v = v.isInBounds();

        auto constant_q = ElementModQ::fromUint64(constant);

        auto consistent_c =
          (c == *hash_elems({&const_cast<ElementModQ &>(q), alpha, beta, a_ptr, b_ptr}));

        // 𝑔^𝑉 = 𝑎 ⋅ 𝐴^𝐶 mod 𝑝
        auto consistent_gv = (*g_pow_p(v) == *mul_mod_p(a, *pow_mod_p(*alpha, c)));

        // 𝑔^𝐿 ⋅ 𝐾^𝑣 = 𝑏 ⋅ 𝐵^𝐶 mod 𝑝
        auto consistent_kv = (*mul_mod_p(*g_pow_p(*mul_mod_p({c_ptr, constant_q.get()})),
                                         *pow_mod_p(k, v)) == *mul_mod_p(b, *pow_mod_p(*beta, c)));

        auto success = inBounds_alpha && inBounds_beta && inBounds_a && inBounds_b && inBounds_c &&
                       inBounds_v && consistent_c && consistent_gv && consistent_kv;

        if (!success) {

            map<string, bool> printMap{
              {"inBounds_alpha", inBounds_alpha}, {"inBounds_beta", inBounds_beta},
              {"inBounds_a", inBounds_a},         {"inBounds_b", inBounds_b},
              {"inBounds_c", inBounds_c},         {"inBounds_v", inBounds_v},
              {"consistent_c", consistent_c},     {"consistent_gv", consistent_gv},
              {"consistent_kv", consistent_kv},
            };

            Log::info("found an invalid Constant Chaum-Pedersen proof", printMap);

            Log::debug("k->get", k.toHex());
            Log::debug("q->get", q.toHex());
            Log::debug("alpha->get", alpha->toHex());
            Log::debug("beta->get", beta->toHex());
            Log::debug("a->get", a.toHex());
            Log::debug("b->get", b.toHex());
            Log::debug("c->get", c.toHex());
            Log::debug("v->get", v.toHex());

            return false;
        }
        Log::trace("ConstantChaumPedersenProof::isValid: TRUE!");
        return success;
    }
#pragma endregion
} // namespace electionguard
