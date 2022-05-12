#ifndef __ELECTIONGUARD__CPP_ELGAMAL_HPP_INCLUDED__
#define __ELECTIONGUARD__CPP_ELGAMAL_HPP_INCLUDED__
#include "crypto_hashable.hpp"
#include "export.h"
#include "group.hpp"

#include <memory>
#include <vector>

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
        static std::unique_ptr<ElGamalKeyPair> fromSecret(const ElementModQ &secretKey,
                                                          bool isFixedBase = true);

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
    /// Encrypts a message with given precomputed values (two triples and a quadruple).
    /// However, only the first triple is used in this function.
    ///
    /// <param name="m"> Message to elgamal_encrypt; must be an integer in [0,Q). </param>
    /// <param name="precomputedTwoTriplesAndAQuad"> Precomputed two triples and a quad. </param>
    /// <returns>A ciphertext tuple.</returns>
    /// </summary>
    EG_API std::unique_ptr<ElGamalCiphertext>
    elgamalEncrypt_with_precomputed(uint64_t m, ElementModP &gToRho, ElementModP &pubkeyToRho);
    /// <summary>
    /// Accumulate the ciphertexts by adding them together.
    /// </summary>
    EG_API std::unique_ptr<ElGamalCiphertext>
    elgamalAdd(const std::vector<std::reference_wrapper<ElGamalCiphertext>> &ciphertexts);

#define HASHED_CIPHERTEXT_BLOCK_LENGTH 32U
#define HASHED_BLOCK_LENGTH_IN_BITS 256U
#define _PAD_INDICATOR_SIZE sizeof(uint16_t)

    typedef enum padded_data_size_e {
        NO_PADDING = 0,
        BYTES_512 = 512 - _PAD_INDICATOR_SIZE
    } padded_data_size_t;

    /// <summary>
    /// A "Hashed ElGamal Ciphertext" as specified as the Auxiliary Encryption in
    /// the ElectionGuard specification. The tuple g ^ r mod p concatenated with
    /// K ^ r mod p are used to feed into a hash function to generate a main key
    /// from which other keys derive to perform XOR encryption and to MAC the
    /// result. Create one with `hashedElgamalEncrypt`. Decrypt using one the
    /// 'decrypt' method.
    /// </summary>
    class EG_API HashedElGamalCiphertext : public CryptoHashable
    {
      public:
        HashedElGamalCiphertext(const HashedElGamalCiphertext &other);
        HashedElGamalCiphertext(HashedElGamalCiphertext &&other);
        HashedElGamalCiphertext(std::unique_ptr<ElementModP> pad, std::vector<uint8_t> data,
                                std::vector<uint8_t> mac);
        ~HashedElGamalCiphertext();

        HashedElGamalCiphertext &operator=(HashedElGamalCiphertext rhs);
        HashedElGamalCiphertext &operator=(HashedElGamalCiphertext &&rhs);
        bool operator==(const HashedElGamalCiphertext &other);
        bool operator!=(const HashedElGamalCiphertext &other);

        /// <Summary>
        /// The g ^r mod p value also referred to as pad in the code and
        /// c0 in the spec.
        /// </Summary>
        ElementModP *getPad();

        /// <Summary>
        /// The g ^r mod p value also referred to as pad in the code and
        /// c0 in the spec.
        /// </Summary>
        ElementModP *getPad() const;

        /// <Summary>
        /// The vector of encrypted ciphertext bytes. Referred to as c1
        /// in the spec.
        /// </Summary>
        std::vector<uint8_t> getData();

        /// <Summary>
        /// The vector of encrypted ciphertext bytes. Referred to as c1
        /// in the spec.
        /// </Summary>
        std::vector<uint8_t> getData() const;

        /// <Summary>
        /// The vector of MAC bytes. Referred to as c2 in the spec.
        /// </Summary>
        std::vector<uint8_t> getMac();

        /// <Summary>
        /// The vector of MAC bytes. Referred to as c2 in the spec.
        /// </Summary>
        std::vector<uint8_t> getMac() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

        /// <summary>
        /// Decrypts ciphertext with the Auxiliary Encryption method (as specified in the
        /// ElectionGuard specification) given a random nonce, an ElGamal public key,
        /// and a description hash. The encrypt may be called to look for padding to
        /// verify and remove, in this case the plaintext will be smaller than
        /// the ciphertext, or not to look for padding in which case the
        /// plaintext will be the same size as the ciphertext.
        ///
        /// <param name="nonce"> Randomly chosen nonce in [1,Q). </param>
        /// <param name="publicKey"> ElGamal public key. </param>
        /// <param name="descriptionHash"> Hash of the ballot description. </param>
        /// <param name="look_for_padding"> Indicates if padding removed. </param>
        /// <returns>A plaintext vector.</returns>
        /// </summary>
        std::vector<uint8_t> decrypt(const ElementModQ &secret_key,
                                     const ElementModQ &descriptionHash, bool look_for_padding);

        /// <Summary>
        /// Clone the value by making a deep copy.
        /// </Summary>
        std::unique_ptr<HashedElGamalCiphertext> clone() const;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// Encrypts a message with the Auxiliary Encryption method (as specified in the
    /// ElectionGuard specification) given a random nonce, an ElGamal public key,
    /// and a description hash. The encrypt may be called to apply padding. If
    /// padding is to be applied then the max_len parameter may be used with
    /// any of the padded_data_size_t enumeration values that is not NO_PADDING.
    /// This value indicates the maximum length of the plaintext that may be
    /// encrypted. The padding scheme applies two bytes for length of padding
    /// plus padding bytes. If padding is not to be applied then the
    /// max_len parameter must be NO_PADDING. and the plaintext must
    /// be a multiple of the block length (32) and the ciphertext will be
    /// the same size. If the max_len is set to  something other than
    /// NO_PADDING and the allow_truncation parameter is set to
    /// true then if the message parameter data is longer than
    /// max_len then it will be truncated to max_len. If the max_len is set to
    /// something other than NO_PADDING and the allow_truncation parameter
    /// is set to false then if the message parameter data is longer than
    /// max_len then an exception will be thrown.
    ///
    /// <param name="message"> Message to hashed elgamal encrypt. </param>
    /// <param name="nonce"> Randomly chosen nonce in [1,Q). </param>
    /// <param name="publicKey"> ElGamal public key. </param>
    /// <param name="descriptionHash"> Hash of the ballot description. </param>
    /// <param name="max_len"> If padding is to be applied then this indicates the
    ///  maximum length of plaintext, must be one padded_data_size_t enumeration
    ///  values. If padding is not to be applied then this parameter must use
    ///  the NO_PADDING padded_data_size_t enumeration value.</param>
    /// <param name="allow_truncation"> Truncates data to the max_len if set to
    /// true. If max_len is set to NO_PADDING then this parameter is ignored. </param>
    /// <returns>A ciphertext triple.</returns>
    /// </summary>
    EG_API std::unique_ptr<HashedElGamalCiphertext>
    hashedElgamalEncrypt(std::vector<uint8_t> plaintext, const ElementModQ &nonce,
                         const ElementModP &publicKey, const ElementModQ &descriptionHash,
                         padded_data_size_t max_len, bool allow_truncation);

} // namespace electionguard

#endif /* __ELECTIONGUARD__CPP_ELGAMAL_HPP_INCLUDED__ */
