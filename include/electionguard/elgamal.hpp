#ifndef __ELECTIONGUARD__CORE_ELGAMAL_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_ELGAMAL_HPP_INCLUDED__
#include "crypto_hashable.hpp"
#include "export.h"
#include "group.hpp"

#include <memory>

namespace electionguard
{
    /// <summary>
    /// An exponential ElGamal keypair
    /// </summary>
    class EG_API ElGamalKeyPair
    {
      public:
        ElGamalKeyPair(const ElGamalKeyPair &other);
        ElGamalKeyPair(const ElGamalKeyPair &&other);
        ElGamalKeyPair(unique_ptr<ElementModQ> secretKey, unique_ptr<ElementModP> publicKey);
        ~ElGamalKeyPair();

        ElGamalKeyPair &operator=(ElGamalKeyPair rhs);
        ElGamalKeyPair &operator=(ElGamalKeyPair &&rhs);

        ElementModQ *getSecretKey();
        ElementModP *getPublicKey();

        static unique_ptr<ElGamalKeyPair> fromSecret(const ElementModQ &secretKey);

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
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
        ElGamalCiphertext(unique_ptr<ElementModP> pad, unique_ptr<ElementModP> data);
        ~ElGamalCiphertext();

        ElGamalCiphertext &operator=(ElGamalCiphertext rhs);
        ElGamalCiphertext &operator=(ElGamalCiphertext &&rhs);

        ElementModP *getPad();
        ElementModP *getPad() const;
        ElementModP *getData();
        ElementModP *getData() const;

        virtual unique_ptr<ElementModQ> crypto_hash() override;
        virtual unique_ptr<ElementModQ> crypto_hash() const override;

        static unique_ptr<ElGamalCiphertext> make(const ElementModP &pad, const ElementModP &data);

        uint64_t decrypt(const ElementModQ &secretKey);

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// Encrypts a message with a given random nonce and an ElGamal public key.
    ///
    /// <param name="m"> Message to elgamal_encrypt; must be an integer in [0,Q). </param>
    /// <param name="nonce"> Randomly chosen nonce in [1,Q). </param>
    /// <param name="publicKey"> ElGamal public key. </param>
    /// <returns>A ciphertext tuple.</returns>
    /// </summary>
    EG_API unique_ptr<ElGamalCiphertext> elgamalEncrypt(const uint64_t m, const ElementModQ &nonce,
                                                        const ElementModP &publicKey);

    EG_API unique_ptr<ElGamalCiphertext>
    elgamalAdd(const vector<reference_wrapper<ElGamalCiphertext>> &ciphertexts);
} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_ELGAMAL_HPP_INCLUDED__ */
