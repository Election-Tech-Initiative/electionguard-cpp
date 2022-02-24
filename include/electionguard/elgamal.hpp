#ifndef __ELECTIONGUARD__CPP_ELGAMAL_HPP_INCLUDED__
#define __ELECTIONGUARD__CPP_ELGAMAL_HPP_INCLUDED__
#include "crypto_hashable.hpp"
#include "export.h"
#include "group.hpp"

#include <memory>

namespace electionguard
{
    /// <summary>
    /// An exponential ElGamal keypair.
    /// </summary>
    class EG_API ElGamalKeyPair
    {
      public:
        ElGamalKeyPair(const ElGamalKeyPair &other);
        ElGamalKeyPair(const ElGamalKeyPair &&other);
        ElGamalKeyPair(std::unique_ptr<ElementModQ> secretKey,
                       std::unique_ptr<ElementModP> publicKey);
        ~ElGamalKeyPair();

        ElGamalKeyPair &operator=(ElGamalKeyPair rhs);
        ElGamalKeyPair &operator=(ElGamalKeyPair &&rhs);

        /// <Summary>
        /// The ElGamal Secret Key.
        /// </Summary>
        ElementModQ *getSecretKey();

        /// <Summary>
        /// The ElGamal Public Key.
        /// </Summary>
        ElementModP *getPublicKey();

        /// <Summary>
        /// Make an elgamal keypair from a secret.
        /// </Summary>
        static std::unique_ptr<ElGamalKeyPair> fromSecret(const ElementModQ &secretKey, bool isFixedBase = true);

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// An "exponential ElGamal ciphertext" (i.e., with the plaintext in the exponent to allow for
    /// homomorphic addition). Create one with `elgamal_encrypt`. Add them with `elgamal_add`.
    /// Decrypt using one of the supplied instance methods.
    /// </summary>
    class EG_API ElGamalCiphertext : public CryptoHashable
    {
      public:
        ElGamalCiphertext(const ElGamalCiphertext &other);
        ElGamalCiphertext(ElGamalCiphertext &&other);
        ElGamalCiphertext(std::unique_ptr<ElementModP> pad, std::unique_ptr<ElementModP> data);
        ~ElGamalCiphertext();

        ElGamalCiphertext &operator=(ElGamalCiphertext rhs);
        ElGamalCiphertext &operator=(ElGamalCiphertext &&rhs);
        bool operator==(const ElGamalCiphertext &other);
        bool operator!=(const ElGamalCiphertext &other);

        /// <Summary>
        /// The pad value also referred to as A, a, 𝑎, or alpha in the spec.
        /// </Summary>
        ElementModP *getPad();

        /// <Summary>
        /// The pad value also referred to as A, a, 𝑎, or alpha in the spec.
        /// </Summary>
        ElementModP *getPad() const;

        /// <Summary>
        /// The data value also referred to as B, b, 𝛽, or beta in the spec.
        /// </Summary>
        ElementModP *getData();

        /// <Summary>
        /// The data value also referred to as B, b, 𝛽, or beta in the spec.
        /// </Summary>
        ElementModP *getData() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

        /// <Summary>
        /// Make an ElGamal Ciphertext from the given pad and data
        /// </Summary>
        static std::unique_ptr<ElGamalCiphertext> make(const ElementModP &pad,
                                                       const ElementModP &data);

        /// <Summary>
        /// Decrypt the ciphertext directly using the provided secret key.
        ///
        /// This is a convenience accessor useful for some use cases.
        /// This method should not be used by consumers operating in live secret ballot elections.
        /// </Summary>
        uint64_t decrypt(const ElementModQ &secretKey);

        /// <Summary>
        /// Clone the value by making a deep copy.
        /// </Summary>
        std::unique_ptr<ElGamalCiphertext> clone() const;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// Encrypts a message with a given random nonce and an ElGamal public key.
    ///
    /// <param name="m"> Message to elgamal_encrypt; must be an integer in [0,Q). </param>
    /// <param name="nonce"> Randomly chosen nonce in [1,Q). </param>
    /// <param name="publicKey"> ElGamal public key. </param>
    /// <returns>A ciphertext tuple.</returns>
    /// </summary>
    EG_API std::unique_ptr<ElGamalCiphertext>
    elgamalEncrypt(const uint64_t m, const ElementModQ &nonce, const ElementModP &publicKey);

    /// <summary>
    /// Accumulate the ciphertexts by adding them together.
    /// </summary>
    EG_API std::unique_ptr<ElGamalCiphertext>
    elgamalAdd(const std::vector<std::reference_wrapper<ElGamalCiphertext>> &ciphertexts);
} // namespace electionguard

#endif /* __ELECTIONGUARD__CPP_ELGAMAL_HPP_INCLUDED__ */
