#include "electionguard/elgamal.hpp"

#include "electionguard/hash.hpp"
#include "log.hpp"

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
            Log::debug(" ElGamal secret key needs to be in [2,Q).");
            return nullptr;
        }
        auto privateKey = make_unique<ElementModQ>(secretKey);
        auto publicKey = g_pow_p(secretKey);
        return make_unique<ElGamalKeyPair>(move(privateKey), move(publicKey));
    }

#pragma endregion

#pragma region ElGamalCiphertext

    struct ElGamalCiphertext::Impl {
        Impl(unique_ptr<ElementModP> pad, unique_ptr<ElementModP> data)
            : pad(move(pad)), data(move(data))
        {
        }

        unique_ptr<ElementModP> pad;
        unique_ptr<ElementModP> data;
    };

    ElGamalCiphertext::ElGamalCiphertext(unique_ptr<ElementModP> pad, unique_ptr<ElementModP> data)
        : pimpl(new Impl(move(pad), move(data)))
    {
    }

    ElGamalCiphertext::~ElGamalCiphertext() = default;

    ElGamalCiphertext &ElGamalCiphertext::operator=(ElGamalCiphertext rhs)
    {
        swap(pimpl, rhs.pimpl);
        return *this;
    }

    ElementModP *ElGamalCiphertext::getPad() { return pimpl->pad.get(); }
    ElementModP *ElGamalCiphertext::getData() { return pimpl->data.get(); }

    unique_ptr<ElementModQ> ElGamalCiphertext::crypto_hash() const
    {
        return hash_elems({pimpl->pad.get(), pimpl->data.get()});
    }

    unique_ptr<ElGamalCiphertext> elgamalEncrypt(uint64_t m, const ElementModQ &nonce,
                                                 const ElementModP &publicKey)
    {
        if ((const_cast<ElementModQ &>(nonce) == ZERO_MOD_Q())) {
            Log::debug("ElGamal encryption requires a non-zero nonce");
            return nullptr;
        }

        const auto nonce4096 = nonce.toElementModP();
        auto pad = g_pow_p(nonce);
        auto e = ElementModP::fromUint64(m);
        auto gpowp_m = g_pow_p(*e);
        auto pubkey_pow_n = pow_mod_p(publicKey, *nonce4096);
        auto data = mul_mod_p(*gpowp_m, *pubkey_pow_n);

        return make_unique<ElGamalCiphertext>(move(pad), move(data));
    }

#pragma endregion

} // namespace electionguard
