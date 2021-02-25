#include "electionguard/elgamal.hpp"

#include "../kremlin/Hacl_Bignum4096.h"
#include "electionguard/hash.hpp"
#include "log.hpp"

using std::invalid_argument;
using std::make_unique;
using std::move;
using std::reference_wrapper;
using std::unique_ptr;

namespace electionguard
{
#pragma region ElgamalKeyPair

    struct ElGamalKeyPair::Impl {
        Impl(unique_ptr<ElementModQ> secretKey, unique_ptr<ElementModP> publicKey)
            : secretKey(move(secretKey)), publicKey(move(publicKey))
        {
        }

        unique_ptr<ElementModQ> secretKey;
        unique_ptr<ElementModP> publicKey;
    };

    // Lifecycle Methods

    ElGamalKeyPair::ElGamalKeyPair(const ElGamalKeyPair &other)
        : pimpl(new Impl(move(*other.pimpl)))
    {
    }

    ElGamalKeyPair::ElGamalKeyPair(unique_ptr<ElementModQ> secretKey,
                                   unique_ptr<ElementModP> publicKey)
        : pimpl(new Impl(move(secretKey), move(publicKey)))
    {
    }

    ElGamalKeyPair &ElGamalKeyPair::operator=(ElGamalKeyPair rhs)
    {
        swap(pimpl, rhs.pimpl);
        return *this;
    }

    ElGamalKeyPair::~ElGamalKeyPair() = default;

    // Property Getters

    ElementModQ *ElGamalKeyPair::getSecretKey() { return pimpl->secretKey.get(); }

    ElementModP *ElGamalKeyPair::getPublicKey() { return pimpl->publicKey.get(); }

    // Public Members

    unique_ptr<ElGamalKeyPair> ElGamalKeyPair::fromSecret(const ElementModQ &secretKey)
    {
        if (const_cast<ElementModQ &>(secretKey) < TWO_MOD_Q()) {
            throw "ElGamalKeyPair fromSecret secret key needs to be in [2,Q).";
        }
        auto privateKey = make_unique<ElementModQ>(secretKey);
        auto publicKey = g_pow_p(secretKey);
        return make_unique<ElGamalKeyPair>(move(privateKey), move(publicKey));
    }

#pragma endregion

#pragma region ElGamalCiphertext

    struct ElGamalCiphertext::Impl {
        unique_ptr<ElementModP> pad;
        unique_ptr<ElementModP> data;

        Impl(unique_ptr<ElementModP> pad, unique_ptr<ElementModP> data)
            : pad(move(pad)), data(move(data))
        {
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
            return hash_elems({pad.get(), data.get()});
        }

        bool operator==(const Impl &other) { return *pad == *other.pad && *data == *other.data; }
    };

    // Lifecycle Methods

    ElGamalCiphertext::ElGamalCiphertext(unique_ptr<ElementModP> pad, unique_ptr<ElementModP> data)
        : pimpl(new Impl(move(pad), move(data)))
    {
    }

    ElGamalCiphertext::~ElGamalCiphertext() = default;

    // Operator Overloads

    ElGamalCiphertext &ElGamalCiphertext::operator=(ElGamalCiphertext rhs)
    {
        swap(pimpl, rhs.pimpl);
        return *this;
    }

    bool ElGamalCiphertext::operator==(const ElGamalCiphertext &other)
    {
        return *pimpl == *other.pimpl;
    }

    bool ElGamalCiphertext::operator!=(const ElGamalCiphertext &other) { return !(*this == other); }

    // Property Getters

    ElementModP *ElGamalCiphertext::getPad() { return pimpl->pad.get(); }
    ElementModP *ElGamalCiphertext::getPad() const { return pimpl->pad.get(); }
    ElementModP *ElGamalCiphertext::getData() { return pimpl->data.get(); }
    ElementModP *ElGamalCiphertext::getData() const { return pimpl->data.get(); }

    // Interface Overrides

    unique_ptr<ElementModQ> ElGamalCiphertext::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> ElGamalCiphertext::crypto_hash() const { return pimpl->crypto_hash(); }

    unique_ptr<ElGamalCiphertext> ElGamalCiphertext::make(const ElementModP &pad,
                                                          const ElementModP &data)
    {
        return make_unique<ElGamalCiphertext>(make_unique<ElementModP>(pad),
                                              make_unique<ElementModP>(data));
    }

    // Public Methods

    uint64_t ElGamalCiphertext::decrypt(const ElementModQ &secretKey)
    {
        // Note this decryption method is primarily used for testing
        // and is only capable of decrypting boolean results (0/1)
        // it should be extended with a discrete_log search to decrypt
        // values other than 0 or 1

        const auto &p = P();
        auto secret = secretKey.toElementModP();
        uint64_t divisor[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod_exp(p.get(), this->getPad()->get(), MAX_P_SIZE,
                                               secret->get(), static_cast<uint64_t *>(divisor));
        if (!success) {
            Log::debug(": could not calculate mod exp");
            return 0;
        }

        uint64_t inverse[MAX_P_LEN] = {};
        Hacl_Bignum4096_mod_inv_prime(p.get(), static_cast<uint64_t *>(divisor),
                                      static_cast<uint64_t *>(inverse));

        uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
        Hacl_Bignum4096_mul(this->getData()->get(), static_cast<uint64_t *>(inverse),
                            static_cast<uint64_t *>(mulResult));

        uint64_t result[MAX_P_LEN] = {};
        success = Hacl_Bignum4096_mod(p.get(), static_cast<uint64_t *>(mulResult),
                                      static_cast<uint64_t *>(result));
        if (!success) {
            Log::debug(": could not calculate mod");
            return 0;
        }

        // TODO: ISSUE #133: traverse a discrete_log lookup to find the result
        auto result_as_p = make_unique<ElementModP>(result);
        uint64_t retval = 0xffffffffffffffff;
        if (*result_as_p == ONE_MOD_P()) {
            // if it is 1 it is false
            retval = 0;
        } else if (*result_as_p == G()) {
            // if it is g, it is true
            retval = 1;
        }

        // if it is anything else no result found (decrypt failed)
        return retval;
    }

    unique_ptr<ElGamalCiphertext> ElGamalCiphertext::clone() const
    {
        return make_unique<ElGamalCiphertext>(pimpl->pad->clone(), pimpl->data->clone());
    }

#pragma endregion

    unique_ptr<ElGamalCiphertext> elgamalEncrypt(uint64_t m, const ElementModQ &nonce,
                                                 const ElementModP &publicKey)
    {
        if ((const_cast<ElementModQ &>(nonce) == ZERO_MOD_Q())) {
            throw "elgamalEncrypt encryption requires a non-zero nonce";
        }

        const auto nonce4096 = nonce.toElementModP();
        auto pad = g_pow_p(nonce);
        auto e = ElementModP::fromUint64(m);
        auto gpowp_m = g_pow_p(*e);
        auto pubkey_pow_n = pow_mod_p(publicKey, *nonce4096);
        auto data = mul_mod_p(*gpowp_m, *pubkey_pow_n);

        Log::debug(": Generated Encryption");
        Log::debugHex(": publicKey: ", publicKey.toHex());
        Log::debugHex(": pad: ", pad->toHex());
        Log::debugHex(": data: ", data->toHex());

        return make_unique<ElGamalCiphertext>(move(pad), move(data));
    }

    unique_ptr<ElGamalCiphertext>
    elgamalAdd(const vector<reference_wrapper<ElGamalCiphertext>> &ciphertexts)
    {
        if (ciphertexts.empty()) {
            throw invalid_argument("must have one or more ciphertexts");
        }

        auto resultPad = ElementModP::fromUint64(1UL);
        auto resultData = ElementModP::fromUint64(1UL);
        for (auto ciphertext : ciphertexts) {
            auto pad = mul_mod_p(*resultPad, *ciphertext.get().getPad());
            resultPad.swap(pad);
            auto data = mul_mod_p(*resultData, *ciphertext.get().getData());
            resultData.swap(data);
        }
        return make_unique<ElGamalCiphertext>(move(resultPad), move(resultData));
    }

} // namespace electionguard
