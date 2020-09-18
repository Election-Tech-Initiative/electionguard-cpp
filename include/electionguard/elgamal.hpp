#ifndef __ELECTIONGUARD__CORE_ELGAMAL_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_ELGAMAL_HPP_INCLUDED__
#include "crypto_hashable.hpp"
#include "export.h"
#include "group.hpp"

namespace electionguard
{
    struct ElGamalKeyPairData {
        ElementModQ *secretKey;
        ElementModP *publicKey;
    };

    /// <summary>
    /// An exponential ElGamal keypair
    /// </summary>
    class EG_API ElGamalKeyPair
    {
      public:
        ElGamalKeyPair(ElementModQ *secretKey, ElementModP *publicKey);
        ~ElGamalKeyPair();

        ElementModQ *getSecretKey();
        ElementModP *getPublicKey();

        static ElGamalKeyPair *fromSecret(ElementModQ *a);

      private:
        ElGamalKeyPairData data;
    };

    struct ElGamalCiphertextData {
        ElementModP *pad;
        ElementModP *data;
    };

    /// <summary>
    /// An "exponential ElGamal ciphertext" (i.e., with the plaintext in the exponent to allow for
    /// homomorphic addition). Create one with `elgamal_encrypt`. Add them with `elgamal_add`.
    /// Decrypt using one of the supplied instance methods.
    /// </summary>
    class EG_API ElGamalCiphertext : public CryptoHashable
    {
      public:
        ElGamalCiphertext(ElementModP *pad, ElementModP *data);
        ~ElGamalCiphertext();

        ElementModP *getPad();
        ElementModP *getData();

        uint64_t decrypt(ElementModP *product);
        uint64_t decrypt(ElementModQ *secretKey);
        uint64_t decrypt(ElementModP *publicKey, ElementModQ *nonce);

        virtual ElementModQ *crypto_hash();

      private:
        ElGamalCiphertextData data;
    };

    /// <summary>
    /// Encrypts a message with a given random nonce and an ElGamal public key.
    ///
    /// <param name="m"> Message to elgamal_encrypt; must be an integer in [0,Q). </param>
    /// <param name="nonce"> Randomly chosen nonce in [1,Q). </param>
    /// <param name="publicKey"> ElGamal public key. </param>
    /// <returns>A ciphertext tuple.</returns>
    /// </summary>
    EG_API ElGamalCiphertext *elgamalEncrypt(uint64_t m, ElementModQ *nonce,
                                             ElementModP *publicKey);
} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_ELGAMAL_HPP_INCLUDED__ */
