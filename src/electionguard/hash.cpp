#include "electionguard/hash.hpp"

#include "../kremlin/Hacl_Bignum256.h"
#include "../kremlin/Hacl_Streaming_SHA2.h"
#include "log.hpp"

#include <iomanip>
#include <iostream>

using std::get;
using std::make_unique;
using std::nullptr_t;
using std::out_of_range;
using std::reference_wrapper;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

namespace electionguard
{
    string get_hash_string(CryptoHashableType a);
    template <typename T> string hash_inner_vector(vector<T> inner_vector);
    void push_hash_update(Hacl_Streaming_SHA2_state_sha2_224 *p, CryptoHashableType a);
    Hacl_Streaming_SHA2_state_sha2_224 *hash_open();
    unique_ptr<ElementModQ> hash_close(Hacl_Streaming_SHA2_state_sha2_224 *p);

    enum CryptoHashableTypeEnum {
        NULL_PTR = 0,
        CRYPTOHASHABLE_PTR = 1,
        ELEMENTMODP_PTR = 2,
        ELEMENTMODQ_PTR = 3,
        CRYPTOHASHABLE_REF = 4,
        ELEMENTMODP_REF = 5,
        ELEMENTMODQ_REF = 6,
        CRYPTOHASHABLE_CONST_REF = 7,
        ELEMENTMODP_CONST_REF = 8,
        ELEMENTMODQ_CONST_REF = 9,
        UINT64_T = 10,
        STRING = 11,
        VECTOR_CRYPTOHASHABLE_PTR = 12,
        VECTOR_ELEMENTMODP_PTR = 13,
        VECTOR_ELEMENTMODQ_PTR = 14,
        VECTOR_CRYPTOHASHABLE_REF = 15,
        VECTOR_ELEMENTMODP_REF = 16,
        VECTOR_ELEMENTMODQ_REF = 17,
        VECTOR_CRYPTOHASHABLE_CONST_REF = 18,
        VECTOR_ELEMENTMODP_CONST_REF = 19,
        VECTOR_ELEMENTMODQ_CONST_REF = 20,
        VECTOR_UINT64_T = 21,
        VECTOR_STRING = 22,
        VECTOR_UINT8_T = 23
    };

    const char delimiter_char = '|';
    const string null_string = "null";

    uint8_t delimiter[1] = {delimiter_char};

    unique_ptr<ElementModQ> hash_elems(const vector<CryptoHashableType> &a)
    {
        uint8_t output[MAX_Q_SIZE] = {};
        Hacl_Streaming_SHA2_state_sha2_224 *p = hash_open();

        if (a.empty()) {
            push_hash_update(p, nullptr);
        } else {
            for (const CryptoHashableType &item : a) {
                push_hash_update(p, item);
            }
        }
        return hash_close(p);
    }

    unique_ptr<ElementModQ> hash_elems(CryptoHashableType a)
    {
        Hacl_Streaming_SHA2_state_sha2_224 *p = hash_open();
        push_hash_update(p, a);
        return hash_close(p);
    }

    Hacl_Streaming_SHA2_state_sha2_224 *hash_open()
    {
        Hacl_Streaming_SHA2_state_sha2_224 *p = Hacl_Streaming_SHA2_create_in_256();
        Hacl_Streaming_SHA2_update_256(p, static_cast<uint8_t *>(delimiter), sizeof(delimiter));
        return p;
    }

    unique_ptr<ElementModQ> hash_close(Hacl_Streaming_SHA2_state_sha2_224 *p)
    {
        uint8_t output[MAX_Q_SIZE] = {};
        Hacl_Streaming_SHA2_finish_256(p, static_cast<uint8_t *>(output));

        auto *bigNum =
          Hacl_Bignum256_new_bn_from_bytes_be(sizeof(output), static_cast<uint8_t *>(output));
        if (bigNum == nullptr) {
            throw out_of_range("bytes_to_p could not allocate");
        }

        // The ElementModQ constructor expects the bignum
        // to be a certain size, but there's no guarantee
        // that constraint is satisfied by new_bn_from_bytes_be
        // so copy it into a new element that is the correct size
        // and free the allocated resources
        uint64_t normalized[MAX_Q_LEN] = {};
        memcpy(static_cast<uint64_t *>(normalized), bigNum, sizeof(output));
        Hacl_Streaming_SHA2_free_256(p);
        free(bigNum);

        auto element = make_unique<ElementModQ>(normalized, true);

        // TODO: take the result mod Q - 1
        // to produce a result that is [0,q-1]
        return add_mod_q(*element, ZERO_MOD_Q());
    }

    template <typename T> string hash_inner_vector(vector<T> inner_vector)
    {
        if (inner_vector.empty()) {
            return null_string;
        }
        vector<CryptoHashableType> hashable_vector(inner_vector.begin(), inner_vector.end());
        return hash_elems(hashable_vector)->toHex();
    }

    string get_hash_string(CryptoHashableType a)
    {
        switch (a.index()) {
            case NULL_PTR: // nullptr_t
            {
                return null_string;
            }
            case CRYPTOHASHABLE_PTR: // CryptoHashable *
            {
                auto hashable = get<CryptoHashable *>(a)->crypto_hash();
                return hashable->toHex();
            }
            case ELEMENTMODP_PTR: // ElementModP *
            {
                return get<ElementModP *>(a)->toHex();
            }
            case ELEMENTMODQ_PTR: // ElementModQ *
            {
                return get<ElementModQ *>(a)->toHex();
            }
            case CRYPTOHASHABLE_REF: // reference_wrapper<CryptoHashable>
            {
                auto hashable = get<reference_wrapper<CryptoHashable>>(a).get().crypto_hash();
                return hashable->toHex();
                //Log::debug("input string: " + input_string);
            }
            case ELEMENTMODP_REF: // reference_wrapper<ElementModP>
            {
                return get<reference_wrapper<ElementModP>>(a).get().toHex();
            }
            case ELEMENTMODQ_REF: // reference_wrapper<ElementModQ>
            {
                return get<reference_wrapper<ElementModQ>>(a).get().toHex();
            }
            case CRYPTOHASHABLE_CONST_REF: // reference_wrapper<const CryptoHashable>
            {
                auto hashable = get<reference_wrapper<const CryptoHashable>>(a).get().crypto_hash();
                return hashable->toHex();
            }
            case ELEMENTMODP_CONST_REF: // reference_wrapper<const ElementModP>
            {
                return get<reference_wrapper<const ElementModP>>(a).get().toHex();
            }
            case ELEMENTMODQ_CONST_REF: // reference_wrapper<const ElementModQ>
            {
                return get<reference_wrapper<const ElementModQ>>(a).get().toHex();
            }
            case UINT64_T: // uint64_t
            {
                uint64_t i = get<uint64_t>(a);
                if (i != 0) {
                    return to_string(i);
                }
                return null_string;
            }
            case STRING: // string
            {
                auto hashable = get<string>(a);
                if (hashable.empty()) {
                    return null_string;
                }
                return hashable;
            }
            case VECTOR_CRYPTOHASHABLE_PTR: // vector<CryptoHashable *>
            {
                return hash_inner_vector<CryptoHashable *>(get<vector<CryptoHashable *>>(a));
            }
            case VECTOR_ELEMENTMODP_PTR: // vector<ElementModP *>
            {
                return hash_inner_vector<ElementModP *>(get<vector<ElementModP *>>(a));
            }
            case VECTOR_ELEMENTMODQ_PTR: // vector<ElementModQ *>
            {
                return hash_inner_vector<ElementModQ *>(get<vector<ElementModQ *>>(a));
            }
            case VECTOR_CRYPTOHASHABLE_REF: // vector<reference_wrapper<CryptoHashable>>
            {
                return hash_inner_vector<reference_wrapper<CryptoHashable>>(
                  get<vector<reference_wrapper<CryptoHashable>>>(a));
            }
            case VECTOR_ELEMENTMODP_REF: // vector<reference_wrapper<ElementModP>>
            {
                return hash_inner_vector<reference_wrapper<ElementModP>>(
                  get<vector<reference_wrapper<ElementModP>>>(a));
            }
            case VECTOR_ELEMENTMODQ_REF: // vector<reference_wrapper<ElementModQ>>
            {
                return hash_inner_vector<reference_wrapper<ElementModQ>>(
                  get<vector<reference_wrapper<ElementModQ>>>(a));
            }
            case VECTOR_CRYPTOHASHABLE_CONST_REF: // vector<reference_wrapper<const CryptoHashable>>
            {
                return hash_inner_vector<reference_wrapper<const CryptoHashable>>(
                  get<vector<reference_wrapper<const CryptoHashable>>>(a));
            }
            case VECTOR_ELEMENTMODP_CONST_REF: // vector<reference_wrapper<const ElementModP>>
            {
                return hash_inner_vector<reference_wrapper<const ElementModP>>(
                  get<vector<reference_wrapper<const ElementModP>>>(a));
            }
            case VECTOR_ELEMENTMODQ_CONST_REF: // vector<reference_wrapper<const ElementModQ>>
            {
                return hash_inner_vector<reference_wrapper<const ElementModQ>>(
                  get<vector<reference_wrapper<const ElementModQ>>>(a));
            }
            case VECTOR_UINT64_T: // vector<uint64_t>
            {
                return hash_inner_vector<uint64_t>(get<vector<uint64_t>>(a));
            }
            case VECTOR_STRING: // vector<string>
            {
                return hash_inner_vector<string>(get<vector<string>>(a));
            }
            case VECTOR_UINT8_T: // vector<uint8_t>
            {
                return hash_inner_vector<uint8_t>(get<vector<uint8_t>>(a));
            }
        }

        return null_string;
    }

    void push_hash_update(Hacl_Streaming_SHA2_state_sha2_224 *p, CryptoHashableType a)
    {
        string input_string = get_hash_string(a);
        const auto *input = reinterpret_cast<const uint8_t *>(input_string.c_str());
        Hacl_Streaming_SHA2_update_256(p, const_cast<uint8_t *>(input), input_string.size());
        Hacl_Streaming_SHA2_update_256(p, static_cast<uint8_t *>(delimiter), sizeof(delimiter));
    }
} // namespace electionguard
