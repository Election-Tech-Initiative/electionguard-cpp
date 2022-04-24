#include "electionguard/elgamal.hpp"
#include "electionguard/hash.hpp"
#include "electionguard/hmac.hpp"

#include "../kremlin/Hacl_HMAC.h"
#include "../kremlin/Lib_Memzero0.h"
#include "../kremlin/Hacl_Bignum4096.h"
#include "log.hpp"
#include <electionguard/hash.hpp>


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
    
    struct HashedElGamalCiphertext::Impl {
        unique_ptr<ElementModP> pad;
        vector<uint8_t> data;
        vector<uint8_t> mac;

        Impl(unique_ptr<ElementModP> pad, vector<uint8_t> data, vector<uint8_t> mac)
            : pad(move(pad)), data(data), mac(mac)
        {
        }

        [[nodiscard]] unique_ptr<HashedElGamalCiphertext::Impl> clone() const
        {
            auto _pad = make_unique<ElementModP>(*pad);
            return make_unique<HashedElGamalCiphertext::Impl>(move(_pad), data, mac);
        }

        bool operator==(const Impl &other)
        {
            return *pad == *other.pad && data == other.data && mac == other.mac;
        }
    };

    // Lifecycle Methods

    HashedElGamalCiphertext::HashedElGamalCiphertext(const HashedElGamalCiphertext &other)
            : pimpl(other.pimpl->clone())
    {
    }

    HashedElGamalCiphertext::HashedElGamalCiphertext(std::unique_ptr<ElementModP> pad,
                                                     std::vector<uint8_t> data,
                                                     std::vector<uint8_t> mac)
        : pimpl(new Impl(move(pad), data, mac))
    {
    }

    HashedElGamalCiphertext::~HashedElGamalCiphertext() = default;

    // Operator Overloads

    HashedElGamalCiphertext &HashedElGamalCiphertext::operator=(HashedElGamalCiphertext rhs)
    {
        swap(pimpl, rhs.pimpl);
        return *this;
    }

    bool HashedElGamalCiphertext::operator==(const HashedElGamalCiphertext &other)
    {
        return *pimpl == *other.pimpl;
    }

    bool HashedElGamalCiphertext::operator!=(const HashedElGamalCiphertext &other)
    {
        return !(*this == other);
    }

    // Property Getters

    ElementModP *HashedElGamalCiphertext::getPad() { return pimpl->pad.get(); }
    ElementModP *HashedElGamalCiphertext::getPad() const { return pimpl->pad.get(); }
    vector<uint8_t> HashedElGamalCiphertext::getData() { return pimpl->data; }
    vector<uint8_t> HashedElGamalCiphertext::getData() const { return pimpl->data; }
    vector<uint8_t> HashedElGamalCiphertext::getMac() { return pimpl->mac; }
    vector<uint8_t> HashedElGamalCiphertext::getMac() const { return pimpl->mac; }

    // Interface Overrides

    unique_ptr<ElementModQ> HashedElGamalCiphertext::crypto_hash()
    { 
        return local_crypto_hash();
    }

    unique_ptr<ElementModQ> HashedElGamalCiphertext::crypto_hash() const
    {
        return local_crypto_hash();
    }

    unique_ptr<ElementModQ> HashedElGamalCiphertext::local_crypto_hash() const
    {
        // concatenate c0 | c1 | c2 (pad | ciphertext | mac)
        std::vector<uint8_t> c0_c1_c2(pimpl->pad->toBytes());
        c0_c1_c2.insert(c0_c1_c2.end(), pimpl->data.begin(), pimpl->data.end());
        c0_c1_c2.insert(c0_c1_c2.end(), pimpl->mac.begin(), pimpl->mac.end());

        uint8_t temp_hash[32];
        Hacl_Hash_SHA2_hash_256(&c0_c1_c2.front(), c0_c1_c2.size(), temp_hash);

        // reverse the bytes
        uint64_t temp_hash_reversed[4];
        uint8_t *ptemp = (uint8_t *)temp_hash_reversed;
        for (int i = 0; i < (int)sizeof(temp_hash); i++) {
            ptemp[i] = temp_hash[sizeof(temp_hash) - (i + 1)];
        }
        unique_ptr<ElementModQ> hash = make_unique<ElementModQ>(temp_hash_reversed);

        return hash;
    }

    // Public Methods

    vector<uint8_t> HashedElGamalCiphertext::decrypt(const ElementModQ &nonce,
                                                     const ElementModP &publicKey,
                                                     const ElementModQ &descriptionHash,
                                                     bool look_for_padding)
    {
        // Note this decryption method is primarily used for testing
        // and is only capable of decrypting boolean results (0/1)
        // it should be extended with a discrete_log search to decrypt
        // values other than 0 or 1
        // 
        //uint8_t temp_xor_key_bytes[HASHED_CIPHERTEXT_BLOCK_LENGTH];
        vector<uint8_t> plaintext_with_padding;
        vector<uint8_t> plaintext;

        uint32_t ciphertext_len = pimpl->data.size();
        uint32_t num_xor_keys = ciphertext_len / HASHED_CIPHERTEXT_BLOCK_LENGTH;
        if ((0 != (ciphertext_len % HASHED_CIPHERTEXT_BLOCK_LENGTH)) || (ciphertext_len == 0))  {
            throw invalid_argument("HashedElGamalCiphertext::decrypt the ciphertext "
                                   "is not a multiple of the block length 32");
        }

        auto g_to_r = g_pow_p(nonce);

        auto publicKey_to_r = pow_mod_p(publicKey, nonce);

        // now we need to hash the concatenation of g_to_r with publicKey_to_r
        // in order to get the base key
        vector<ElementModP *> elems{g_to_r.get(), publicKey_to_r.get()};
        auto master_key = hash_elems(elems);

        vector<uint8_t> mac_key =
          get_hmac(master_key->toBytes(), descriptionHash.toBytes(), descriptionHash.toBytes().size(), 0);

        // calculate the mac (c0 is g ^ r mod p and c1 is the ciphertext, they are concatenated)
        vector<uint8_t> c0_and_c1(pimpl->pad->toBytes());
        c0_and_c1.insert(c0_and_c1.end(), pimpl->data.begin(),
                         pimpl->data.end());
        vector<uint8_t> our_mac = get_hmac(mac_key, c0_and_c1, 0, 0);
        Lib_Memzero0_memzero(&mac_key.front(), mac_key.size());

        if (pimpl->mac != our_mac) {
            throw runtime_error("HashedElGamalCiphertext::decrypt the calculated mac didn't match the passed in mac");
        }

        uint32_t plaintext_index = 0;
        for (uint32_t i = 0; i < num_xor_keys; i++) {
            vector<int8_t> temp_plaintext(HASHED_CIPHERTEXT_BLOCK_LENGTH, 0);

            vector<uint8_t> xor_key =
              get_hmac(master_key->toBytes(), descriptionHash.toBytes(),
                       descriptionHash.toBytes().size(), i + 1);
            
            // XOR the key with the plaintext
            for (int j = 0; j < (int)HASHED_CIPHERTEXT_BLOCK_LENGTH; j++) {
                temp_plaintext[j] = pimpl->data[plaintext_index] ^ xor_key[j];
                // advance the plaintext index
                plaintext_index++;
            }
            Lib_Memzero0_memzero(&xor_key.front(), xor_key.size());

            plaintext_with_padding.insert(plaintext_with_padding.end(),
                                          temp_plaintext.begin(), temp_plaintext.end());
            Lib_Memzero0_memzero(&temp_plaintext.front(), temp_plaintext.size());
        }

        if (look_for_padding) {
            uint16_t pad_len_be;
            memcpy(&pad_len_be, &plaintext_with_padding.front(), sizeof(pad_len_be));
            uint16_t pad_len = be16toh(pad_len_be);

            if (pad_len > (plaintext_with_padding.size() - sizeof(pad_len))) {
                throw runtime_error(
                  "HashedElGamalCiphertext::decrypt the padding is incorrect, decrypt failed");
            }

            // check that the end bytes are 0x00
            if (pad_len > 0) {
                for (int i = 1; i <= (int)pad_len; i++) {
                    if (plaintext_with_padding[plaintext_with_padding.size() - i] != 0x00) {
                        throw runtime_error("HashedElGamalCiphertext::decrypt the padding is "
                                            "incorrect, decrypt failed");
                    }
                }
            }

            plaintext.insert(plaintext.end(), &plaintext_with_padding.front() + sizeof(pad_len),
                             &plaintext_with_padding.front() +
                               (plaintext_with_padding.size() - pad_len));
        } else {
            plaintext = plaintext_with_padding;
        }

        return plaintext;
    }

    unique_ptr<HashedElGamalCiphertext> HashedElGamalCiphertext::clone() const
    {
        return make_unique<HashedElGamalCiphertext>(pimpl->pad->clone(), pimpl->data, pimpl->mac);
    }

#pragma endregion // HashedElGamalCiphertext

    unique_ptr<HashedElGamalCiphertext> hashedElgamalEncrypt(std::vector<uint8_t> plaintext,
                                                             const ElementModQ &nonce,
                                                             const ElementModP &publicKey,
                                                             const ElementModQ &descriptionHash,
                                                             bool apply_padding, uint32_t max_len)
    {
        vector<uint8_t> ciphertext;
        vector<uint8_t> plaintext_on_boundary;

        // padding scheme is to concatenate [length of the padding][plaintext][padding bytes of 0x00]
        // padding bytes 0x00 are padded out to the first HASHED_CIPHERTEXT_BLOCK_LENGTH boundary
        // past max_len. So if max_len is 62 then it will pad to the 64 byte boundary
        if (apply_padding) {
            if ((max_len == 0) ||
                (0 != ((max_len + sizeof(uint16_t)) % HASHED_CIPHERTEXT_BLOCK_LENGTH)) ||
                (max_len > 65534)) {
                throw invalid_argument("HashedElGamalCiphertext::encrypt the max_len must be a "
                                       "multiple of the block length 32");
            }
            if (plaintext.size() > max_len) {
                throw invalid_argument(
                  "HashedElGamalCiphertext::encrypt the plaintext is greater than max_len");
            }

            //uint32_t original_plaintext_len = plaintext.size();
            uint16_t pad_len = max_len - plaintext.size();
            uint16_t pad_len_be = htobe16(pad_len);

            std::vector<uint8_t> padding(pad_len, 0);

            // insert length in big endian form
            plaintext_on_boundary.insert(plaintext_on_boundary.end(), (uint8_t *)&pad_len_be,
                                         (uint8_t *)&pad_len_be + sizeof(pad_len_be));
            // insert plaintext
            plaintext_on_boundary.insert(plaintext_on_boundary.end(), plaintext.begin(),
                                         plaintext.end());

            // we dont pad 0x00s if the length field plus plaintext length falls on a block length boundary
            if (pad_len > 0) {
                // insert padding
                plaintext_on_boundary.insert(plaintext_on_boundary.end(), padding.begin(),
                                             padding.end());
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

        auto g_to_r = g_pow_p(nonce);

        auto publicKey_to_r = pow_mod_p(publicKey, nonce);

        // now we need to hash the concatenation of g_to_r with publicKey_to_r
        // in order to get the base key
        vector<ElementModP *> elems{g_to_r.get(), publicKey_to_r.get()};
        auto master_key = hash_elems(elems);

        uint32_t plaintext_index = 0;
        for (uint32_t i = 0; i < num_xor_keys; i++) {
            vector<uint8_t> temp_ciphertext(HASHED_CIPHERTEXT_BLOCK_LENGTH, 0);

            vector<uint8_t> xor_key = get_hmac(master_key->toBytes(), descriptionHash.toBytes(),
                                               descriptionHash.toBytes().size(), i + 1);

            // XOR the key with the plaintext
            for (int j = 0; j < (int)HASHED_CIPHERTEXT_BLOCK_LENGTH; j++) {
                temp_ciphertext[j] = plaintext_on_boundary[plaintext_index] ^ xor_key[j];
                // advance the plaintext index
                plaintext_index++;
            }
            Lib_Memzero0_memzero(&xor_key.front(), xor_key.size());

            ciphertext.insert(ciphertext.end(), temp_ciphertext.begin(),
                              temp_ciphertext.end());
            Lib_Memzero0_memzero(&temp_ciphertext.front(), temp_ciphertext.size());
        }

        vector<uint8_t> mac_key =
          get_hmac(master_key->toBytes(), descriptionHash.toBytes(), descriptionHash.toBytes().size(), 0);

        // calculate the mac (c0 is g ^ r mod p and c1 is the ciphertext, they are concatenated)
        vector<uint8_t> c0_and_c1(g_to_r->toBytes());
        c0_and_c1.insert(c0_and_c1.end(), ciphertext.begin(), ciphertext.end());
        vector<uint8_t> mac = get_hmac(mac_key, c0_and_c1, 0, 0);
        Lib_Memzero0_memzero(&mac_key.front(), mac_key.size());

        return make_unique<HashedElGamalCiphertext>(move(g_to_r), ciphertext, mac);
    }

} // namespace electionguard
