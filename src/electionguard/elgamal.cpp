#include "electionguard/elgamal.hpp"

#include "../kremlin/Hacl_Hash.h"
#include "../kremlin/Hacl_HMAC.h"
#include "../kremlin/Hacl_Bignum4096.h"
#include "electionguard/hash.hpp"
#include "log.hpp"

#include <stdexcept>
#include <array>
#include <memory>

using std::invalid_argument;
using std::make_unique;
using std::move;
using std::reference_wrapper;
using std::unique_ptr;
using std::runtime_error;

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

    unique_ptr<ElGamalKeyPair> ElGamalKeyPair::fromSecret(const ElementModQ &secretKey,
                                                          bool isFixedBase /* = true */)
    {
        if (const_cast<ElementModQ &>(secretKey) < TWO_MOD_Q()) {
            throw invalid_argument("ElGamalKeyPair fromSecret secret key needs to be in [2,Q).");
        }
        auto privateKey = make_unique<ElementModQ>(secretKey);
        auto publicKey = g_pow_p(secretKey);
        publicKey->setIsFixedBase(isFixedBase);
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

        [[nodiscard]] unique_ptr<ElGamalCiphertext::Impl> clone() const
        {
            auto _pad = make_unique<ElementModP>(*pad);
            auto _data = make_unique<ElementModP>(*data);
            return make_unique<ElGamalCiphertext::Impl>(move(_pad), move(_data));
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
            return hash_elems({pad.get(), data.get()});
        }

        bool operator==(const Impl &other) { return *pad == *other.pad && *data == *other.data; }
    };

    // Lifecycle Methods

    ElGamalCiphertext::ElGamalCiphertext(const ElGamalCiphertext &other)
        : pimpl(other.pimpl->clone())
    {
    }

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
        bool success =
          Hacl_Bignum4096_mod_exp_consttime(p.get(), this->getPad()->get(), MAX_P_SIZE,
                                            secret->get(), static_cast<uint64_t *>(divisor));
        if (!success) {
            Log::warn("could not calculate mod exp");
            return 0;
        }

        uint64_t inverse[MAX_P_LEN] = {};
        Hacl_Bignum4096_mod_inv_prime_vartime(p.get(), static_cast<uint64_t *>(divisor),
                                              static_cast<uint64_t *>(inverse));

        uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
        Hacl_Bignum4096_mul(this->getData()->get(), static_cast<uint64_t *>(inverse),
                            static_cast<uint64_t *>(mulResult));

        uint64_t result[MAX_P_LEN] = {};
        success = Hacl_Bignum4096_mod(p.get(), static_cast<uint64_t *>(mulResult),
                                      static_cast<uint64_t *>(result));
        if (!success) {
            Log::warn("could not calculate mod");
            return 0;
        }

        // TODO: ISSUE #133: traverse a discrete_log lookup to find the result
        auto result_as_p = make_unique<ElementModP>(result);
        uint64_t retval = MAX_UINT64;
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
            throw invalid_argument("elgamalEncrypt encryption requires a non-zero nonce");
        }

        auto pad = g_pow_p(nonce);
        auto e = ElementModP::fromUint64(m);
        auto gpowp_m = g_pow_p(*e);
        auto pubkey_pow_n = pow_mod_p(publicKey, nonce);
        auto data = mul_mod_p(*gpowp_m, *pubkey_pow_n);

        Log::trace("Generated Encryption");
        Log::trace("publicKey", publicKey.toHex());
        Log::trace("pad", pad->toHex());
        Log::trace("data", data->toHex());

        return make_unique<ElGamalCiphertext>(move(pad), move(data));
    }

    unique_ptr<ElGamalCiphertext>
    elgamalEncrypt_with_precomputed(uint64_t m, ElementModP &g_to_rho, ElementModP &pubkey_to_rho)
    {
        ElementModP data = pubkey_to_rho;

        if (m == 1) {
            unique_ptr<ElementModP> temp = mul_mod_p(G(), pubkey_to_rho);
            data = *temp;
        }

        Log::trace("Generated Encryption with Precomputed Values");
        Log::trace("pad", g_to_rho.toHex());
        Log::trace("data", data.toHex());

        return make_unique<ElGamalCiphertext>(make_unique<ElementModP>(g_to_rho),
            make_unique<ElementModP>(data));
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


#pragma region HashedElGamalCiphertext
    /*
    struct HashedElGamalCiphertext::Impl {
        unique_ptr<ElementModP> pad;
        vector<uint8_t> ciphertext;
        unique_ptr<ElementModQ> mac;


        Impl(unique_ptr<ElementModP> pad, vector<uint8_t> ciphertext)
            : pad(move(pad)), ciphertext(move(ciphertext))
        {
        }

        [[nodiscard]] unique_ptr<HashedElGamalCiphertext::Impl> clone() const
        {
            auto _pad = make_unique<ElementModP>(*pad);
//            auto _data = make_unique<ElementModP>(*data);
//            return make_unique<HashedElGamalCiphertext::Impl>(move(_pad), move(_data));
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
//            return hash_elems({pad.get(), ciphertext.get()});
        }

        bool operator==(const Impl &other) { return *pad == *other.pad; } // &&*data == *other.data;
        }
    };
    */
    // Lifecycle Methods

    HashedElGamalCiphertext::HashedElGamalCiphertext(const HashedElGamalCiphertext &other){}
    //        : pimpl(other.pimpl->clone())
    //{
    //}

    HashedElGamalCiphertext::HashedElGamalCiphertext(const ElementModP &pad,
                                                     std::vector<uint8_t> ciphertext,
                                                     std::vector<uint8_t> mac)
    {
        this->pad = make_unique<ElementModP>(pad);
        this->ciphertext = ciphertext;
        this->mac = mac;
    }

    HashedElGamalCiphertext::~HashedElGamalCiphertext(){};

    // Operator Overloads

    HashedElGamalCiphertext &HashedElGamalCiphertext::operator=(HashedElGamalCiphertext rhs)
    {
//        swap(pimpl, rhs.pimpl);
        return *this;
    }

    bool HashedElGamalCiphertext::operator==(const HashedElGamalCiphertext &other)
    {
//        return *pimpl == *other.pimpl;
        return true;
    }

    bool HashedElGamalCiphertext::operator!=(const HashedElGamalCiphertext &other)
    {
        return !(*this == other);
    }

    // Property Getters

//    ElementModP *HashedElGamalCiphertext::getPad() { return pimpl->pad.get(); }
//    ElementModP *HashedElGamalCiphertext::getPad() const { return pimpl->pad.get(); }
//    ElementModP *HashedElGamalCiphertext::getData() { return pimpl->data.get(); }
//    ElementModP *HashedElGamalCiphertext::getData() const { return pimpl->data.get(); }


    // Interface Overrides

//    unique_ptr<ElementModQ> HashedElGamalCiphertext::crypto_hash() { return pimpl->crypto_hash(); }
//    unique_ptr<ElementModQ> HashedElGamalCiphertext::crypto_hash() const
//    {
//        return pimpl->crypto_hash();
//    }

//    unique_ptr<HashedElGamalCiphertext> HashedElGamalCiphertext::make(const ElementModP &pad,
//                                                                        std::vector<uint8_t> ciphertext,
//                                                                        const ElementModP &mac)
//    {
//        return make_unique<HashedElGamalCiphertext>(make_unique<ElementModP>(pad),
//                                                    make_unique<ElementModP>(data));
//    }

    // Public Methods

    unique_ptr<HashedElGamalCiphertext>
    HashedElGamalCiphertext::encrypt(const ElementModQ &secretKey,
                                     const ElementModP &publicKey,
                                     const ElementModQ &descriptionHash,
                                     vector<uint8_t> plaintext,
                                     bool apply_padding)
    {
        uint8_t key_bytes[HASHED_CIPHERTEXT_BLOCK_LENGTH];
        uint8_t temp_xor_key_bytes[HASHED_CIPHERTEXT_BLOCK_LENGTH];
        vector<uint8_t> ciphertext;
        vector<uint8_t> plaintext_on_boundary;

        // padding scheme is to just add 0x00 octets to the end of the plaintext
        // out to a HASHED_CIPHERTEXT_BLOCK_LENGTH boundary
        if (apply_padding) {
            uint32_t original_plaintext_len = plaintext.size();
            uint32_t original_plaintext_len_be = htobe32(original_plaintext_len);
            uint32_t pad_len = HASHED_CIPHERTEXT_BLOCK_LENGTH -
                               ((original_plaintext_len + sizeof(uint32_t)) % HASHED_CIPHERTEXT_BLOCK_LENGTH);

            // note the mod guarantees that pad_len is less than HASHED_CIPHERTEXT_BLOCK_LENGTH
            // we dont pad 0x00s if the length field plus plaintext length falls on a block length boundary
            if (pad_len < HASHED_CIPHERTEXT_BLOCK_LENGTH) {
                uint8_t padding[HASHED_CIPHERTEXT_BLOCK_LENGTH];
                memset(padding, 0x00, sizeof(padding));

                // insert length in big endian form
                plaintext_on_boundary.insert(plaintext_on_boundary.end(), (uint8_t *)&original_plaintext_len_be,
                  (uint8_t *)&original_plaintext_len_be + sizeof(original_plaintext_len_be));

                // insert plaintest
                plaintext_on_boundary.insert(plaintext_on_boundary.end(), plaintext.begin(),
                                             plaintext.end());
                // insert padding
                plaintext_on_boundary.insert(plaintext_on_boundary.end(), padding,
                                             padding + pad_len);
            }
        } else {
            if (0 != (plaintext.size() % HASHED_CIPHERTEXT_BLOCK_LENGTH)) {
                throw invalid_argument(
                    "HashedElGamalCiphertext::encrypt the apply_padding was false "
                    "but the plaintext is not a multiple of the block length 32");
            }
            plaintext_on_boundary.insert(plaintext_on_boundary.end(), plaintext.begin(),
                                         plaintext.end());
        }

        uint32_t plaintext_len = plaintext_on_boundary.size();
        uint32_t num_xor_keys = plaintext_len / HASHED_CIPHERTEXT_BLOCK_LENGTH;

        auto g_to_r = g_pow_p(secretKey);

        auto publicKey_to_r = pow_mod_p(publicKey, secretKey);
        vector<uint8_t> publicKey_to_r_bytes = publicKey_to_r->toBytes();

        // now we need to hash the concatenation of g_to_r with publicKey_to_r
        // in order to get the base key
        std::vector<uint8_t> a_b(g_to_r->toBytes());
        a_b.insert(a_b.end(), publicKey_to_r_bytes.begin(), publicKey_to_r_bytes.end());

        Hacl_Hash_SHA2_hash_256(&a_b.front(), a_b.size(), key_bytes);

        // reverse the length because we hash it in big endian and we are on little endian
        uint32_t plaintext_len_be = htobe32(plaintext_len);

        // vector with the description hash concatenated with the reversed length
        std::vector<uint8_t> partial_data_to_hash_for_key(descriptionHash.toBytes());
        partial_data_to_hash_for_key.insert(partial_data_to_hash_for_key.end(),
                                            (uint8_t *)&plaintext_len_be,
                                            (uint8_t *)&plaintext_len_be + sizeof(plaintext_len_be));

        uint32_t plaintext_index = 0;
        for (uint32_t i = 0; i < num_xor_keys; i++) {
            uint8_t temp_ciphertext[HASHED_CIPHERTEXT_BLOCK_LENGTH];

            // reverse the count because we hash it in big endian and we are on little endian
            uint32_t count = i + 1;
            uint32_t count_be = htobe32(count);

            // concatenate the reversed count, the description hash and the reversed length
            std::vector<uint8_t> data_to_hash_for_key((uint8_t *)&count_be,
                                                      (uint8_t *)&count_be + sizeof(count_be));
            data_to_hash_for_key.insert(data_to_hash_for_key.end(),
                                        partial_data_to_hash_for_key.begin(),
                                        partial_data_to_hash_for_key.end());

            Hacl_HMAC_compute_sha2_256(temp_xor_key_bytes, key_bytes, sizeof(key_bytes),
                                       &data_to_hash_for_key.front(), data_to_hash_for_key.size());

            // XOR the key with the plaintext
            for (int j = 0; j < (int)sizeof(temp_ciphertext); j++) {
                temp_ciphertext[j] = plaintext_on_boundary[plaintext_index] ^ temp_xor_key_bytes[j];
                // advance the plaintext index
                plaintext_index++;
            }

            ciphertext.insert(ciphertext.end(), temp_ciphertext, temp_ciphertext + HASHED_CIPHERTEXT_BLOCK_LENGTH);
        }

        vector<uint8_t> zero_count({0, 0, 0, 0});
        std::vector<uint8_t> data_to_hash_mac_key(zero_count);
        data_to_hash_mac_key.insert(data_to_hash_mac_key.end(),
                                    partial_data_to_hash_for_key.begin(),
                                    partial_data_to_hash_for_key.end());

        // get the key for use in the mac
        uint8_t mac_key[HASHED_CIPHERTEXT_BLOCK_LENGTH];
        Hacl_HMAC_compute_sha2_256(mac_key, key_bytes, sizeof(key_bytes),
                                   &data_to_hash_mac_key.front(), data_to_hash_mac_key.size());

        // calculate the mac (c0 is g ^ r mod p and c1 is the ciphertext, they are concatenated)
        vector<uint8_t> c0_and_c1(g_to_r->toBytes());
        c0_and_c1.insert(c0_and_c1.end(), ciphertext.begin(), ciphertext.end());
        uint8_t mac_uint8[HASHED_CIPHERTEXT_BLOCK_LENGTH];
        Hacl_HMAC_compute_sha2_256(mac_uint8, mac_key, sizeof(mac_key), &c0_and_c1.front(),
                                   c0_and_c1.size());

        vector<uint8_t> mac_as_vector(mac_uint8, mac_uint8 + sizeof(mac_uint8));

        return make_unique<HashedElGamalCiphertext>(*g_to_r, ciphertext, mac_as_vector);
    }

    vector<uint8_t> HashedElGamalCiphertext::decrypt(const ElementModQ &secretKey,
                                                     const ElementModP &publicKey,
                                                     const ElementModQ &descriptionHash,
                                                     bool look_for_padding)
    {
        // Note this decryption method is primarily used for testing
        // and is only capable of decrypting boolean results (0/1)
        // it should be extended with a discrete_log search to decrypt
        // values other than 0 or 1
        uint8_t key_bytes[HASHED_CIPHERTEXT_BLOCK_LENGTH];
        uint8_t temp_xor_key_bytes[HASHED_CIPHERTEXT_BLOCK_LENGTH];
        vector<uint8_t> plaintext_with_padding;
        vector<uint8_t> plaintext;

        uint32_t ciphertext_len = ciphertext.size();
        uint32_t num_xor_keys = ciphertext_len / HASHED_CIPHERTEXT_BLOCK_LENGTH;
        if ((0 != (ciphertext_len % HASHED_CIPHERTEXT_BLOCK_LENGTH)) || (ciphertext_len == 0))  {
            throw invalid_argument("HashedElGamalCiphertext::decrypt the ciphertext "
                                   "is not a multiple of the block length 32");
        }

        auto publicKey_to_r = pow_mod_p(publicKey, secretKey);
        vector<uint8_t> publicKey_to_r_bytes = publicKey_to_r->toBytes();

        // now we need to hash the concatenation of g_to_r with publicKey_to_r
        // in order to get the base key
        std::vector<uint8_t> a_b(pad->toBytes());
        a_b.insert(a_b.end(), publicKey_to_r_bytes.begin(), publicKey_to_r_bytes.end());

        Hacl_Hash_SHA2_hash_256(&a_b.front(), a_b.size(), key_bytes);

        // reverse the length because we hash it in big endian and we are on little endian
        uint32_t ciphertext_len_be = htobe32(ciphertext_len);

        // vector with the description hash concatenated with the reversed length
        std::vector<uint8_t> partial_data_to_hash_for_key(descriptionHash.toBytes());
        partial_data_to_hash_for_key.insert(
          partial_data_to_hash_for_key.end(), (uint8_t *)&ciphertext_len_be,
          (uint8_t *)&ciphertext_len_be + sizeof(ciphertext_len_be));

        vector<uint8_t> zero_count({0, 0, 0, 0});
        std::vector<uint8_t> data_to_hash_mac_key(zero_count);
        data_to_hash_mac_key.insert(data_to_hash_mac_key.end(),
                                    partial_data_to_hash_for_key.begin(),
                                    partial_data_to_hash_for_key.end());

        // get the key for use in the mac
        uint8_t mac_key[HASHED_CIPHERTEXT_BLOCK_LENGTH];
        Hacl_HMAC_compute_sha2_256(mac_key, key_bytes, sizeof(key_bytes),
                                   &data_to_hash_mac_key.front(), data_to_hash_mac_key.size());

        // calculate the mac (c0 is g ^ r mod p and c1 is the ciphertext, they are concatenated)
        vector<uint8_t> c0_and_c1(pad->toBytes());
        c0_and_c1.insert(c0_and_c1.end(), ciphertext.begin(), ciphertext.end());
        uint8_t mac_uint8[HASHED_CIPHERTEXT_BLOCK_LENGTH];
        Hacl_HMAC_compute_sha2_256(mac_uint8, mac_key, sizeof(mac_key), &c0_and_c1.front(),
                                   c0_and_c1.size());
        vector<uint8_t> our_mac(mac_uint8, mac_uint8 + sizeof(mac_uint8));

        if (mac != our_mac) {
            throw runtime_error("HashedElGamalCiphertext::decrypt the calculated mac didn't match the passed in mac");
        }

        uint32_t plaintext_index = 0;
        for (uint32_t i = 0; i < num_xor_keys; i++) {
            uint8_t temp_plaintext[HASHED_CIPHERTEXT_BLOCK_LENGTH];

            // reverse the count because we hash it in big endian and we are on little endian
            uint32_t count = i + 1;
            uint32_t count_be = htobe32(count);

            // concatenate the reversed count, the description hash and the reversed length
            std::vector<uint8_t> data_to_hash_for_key((uint8_t *)&count_be,
                                                      (uint8_t *)&count_be + sizeof(count_be));
            data_to_hash_for_key.insert(data_to_hash_for_key.end(),
                                        partial_data_to_hash_for_key.begin(),
                                        partial_data_to_hash_for_key.end());

            Hacl_HMAC_compute_sha2_256(temp_xor_key_bytes, key_bytes, sizeof(key_bytes),
                                       &data_to_hash_for_key.front(), data_to_hash_for_key.size());

            // XOR the key with the plaintext
            for (int j = 0; j < (int)sizeof(temp_plaintext); j++) {
                temp_plaintext[j] = ciphertext[plaintext_index] ^ temp_xor_key_bytes[j];
                // advance the plaintext index
                plaintext_index++;
            }

            plaintext_with_padding.insert(plaintext_with_padding.end(), temp_plaintext,
                             temp_plaintext + HASHED_CIPHERTEXT_BLOCK_LENGTH);
        }

        if (look_for_padding) {
            // we have made sure that we are at least 1 block length
            uint32_t original_plaintext_len_be;
            memcpy(&original_plaintext_len_be, &plaintext_with_padding.front(),
                   sizeof(original_plaintext_len_be));
            uint32_t original_plaintext_len = be32toh(original_plaintext_len_be);
            if (original_plaintext_len > (plaintext_with_padding.size() - sizeof(original_plaintext_len))) {
                throw runtime_error("HashedElGamalCiphertext::decrypt the padding is incorrect, decrypt failed");
            }

            uint32_t padding_len = plaintext_with_padding.size() - (original_plaintext_len + sizeof(original_plaintext_len));
            if (padding_len > HASHED_CIPHERTEXT_BLOCK_LENGTH - 1) {
                throw runtime_error(
                  "HashedElGamalCiphertext::decrypt the padding is incorrect, decrypt failed");
            }

            // check that the end bytes are 0x00
            if (padding_len > 0)
            {
                for (int i = original_plaintext_len + sizeof(original_plaintext_len);
                     i < plaintext_with_padding.size(); i++) {
                    if (plaintext_with_padding[i] != 0x00) {
                        throw runtime_error("HashedElGamalCiphertext::decrypt the padding is "
                                            "incorrect, decrypt failed");
                    }
                }
            }

            plaintext.insert(plaintext.end(),
                               &plaintext_with_padding.front() + sizeof(original_plaintext_len),
                               &plaintext_with_padding.front() +
                                 (original_plaintext_len + sizeof(original_plaintext_len)));
        } else {
            plaintext = plaintext_with_padding;      
        }

        return plaintext;
    }

//    unique_ptr<HashedElGamalCiphertext> HashedElGamalCiphertext::clone() const
//    {
//        return make_unique<HashedElGamalCiphertext>(pimpl->->clone(), pimpl->data->clone());
//    }

#pragma endregion // HashedElGamalCiphertext

} // namespace electionguard
