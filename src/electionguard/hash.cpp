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
    template <typename T> string hash_inner_vector(vector<T> inner_vector);
    void push_hash_update(Hacl_Streaming_Functor_state_s___uint32_t____ *p, CryptoHashableType a);

    const char delimiter_char = '|';
    const string null_string = "null";

    uint8_t delimiter[1] = {delimiter_char};

    unique_ptr<ElementModQ> hash_elems(const vector<CryptoHashableType> &a)
    {
        uint8_t output[MAX_Q_SIZE] = {};
        Hacl_Streaming_Functor_state_s___uint32_t____ *p = Hacl_Streaming_SHA2_create_in_256();
        Hacl_Streaming_SHA2_update_256(p, static_cast<uint8_t *>(delimiter), sizeof(delimiter));

        if (a.empty()) {
            push_hash_update(p, null_string);
        } else {
            for (const CryptoHashableType &item : a) {
                push_hash_update(p, item);
            }
        }

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
        uint64_t element[MAX_Q_LEN] = {};
        memcpy(static_cast<uint64_t *>(element), bigNum, sizeof(output));
        Hacl_Streaming_SHA2_free_256(p);
        free(bigNum);

        // TODO: take the result mod Q - 1
        // to produce a result that is [0,q-1]
        return make_unique<ElementModQ>(element);
    }

    unique_ptr<ElementModQ> hash_elems(CryptoHashableType a)
    {
        return hash_elems(vector<CryptoHashableType>{move(a)});
    }

    template <typename T> string hash_inner_vector(vector<T> inner_vector)
    {
        vector<CryptoHashableType> hashable_vector(inner_vector.begin(), inner_vector.end());
        return hash_elems(hashable_vector)->toHex();
    }

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
        VECTOR_STRING = 22
    };

    void push_hash_update(Hacl_Streaming_Functor_state_s___uint32_t____ *p, CryptoHashableType a)
    {
        string input_string;
        switch (a.index()) {
            case NULL_PTR: // nullptr_t
            {
                input_string = null_string;
                break;
            }
            case CRYPTOHASHABLE_PTR: // CryptoHashable *
            {
                auto hashable = get<CryptoHashable *>(a)->crypto_hash();
                input_string = hashable->toHex();
                break;
            }
            case ELEMENTMODP_PTR: // ElementModP *
            {
                input_string = get<ElementModP *>(a)->toHex();
                break;
            }
            case ELEMENTMODQ_PTR: // ElementModQ *
            {
                input_string = get<ElementModQ *>(a)->toHex();
                break;
            }
            case CRYPTOHASHABLE_REF: // reference_wrapper<CryptoHashable>
            {
                auto hashable = get<reference_wrapper<CryptoHashable>>(a).get().crypto_hash();
                input_string = hashable->toHex();
                //Log::debug("input string: " + input_string);
                break;
            }
            case ELEMENTMODP_REF: // reference_wrapper<ElementModP>
            {
                input_string = get<reference_wrapper<ElementModP>>(a).get().toHex();
                break;
            }
            case ELEMENTMODQ_REF: // reference_wrapper<ElementModQ>
            {
                input_string = get<reference_wrapper<ElementModQ>>(a).get().toHex();
                break;
            }
            case CRYPTOHASHABLE_CONST_REF: // reference_wrapper<const CryptoHashable>
            {
                auto hashable = get<reference_wrapper<const CryptoHashable>>(a).get().crypto_hash();
                input_string = hashable->toHex();
                break;
            }
            case ELEMENTMODP_CONST_REF: // reference_wrapper<const ElementModP>
            {
                input_string = get<reference_wrapper<const ElementModP>>(a).get().toHex();
                break;
            }
            case ELEMENTMODQ_CONST_REF: // reference_wrapper<const ElementModQ>
            {
                input_string = get<reference_wrapper<const ElementModQ>>(a).get().toHex();
                break;
            }
            case UINT64_T: // uint64_t
            {
                uint64_t i = get<uint64_t>(a);
                if (i != 0) {
                    input_string = to_string(i);
                }
                break;
            }
            case STRING: // string
            {
                input_string = get<string>(a);
                break;
            }
            case VECTOR_CRYPTOHASHABLE_PTR: // vector<CryptoHashable *>
            {
                input_string =
                  hash_inner_vector<CryptoHashable *>(get<vector<CryptoHashable *>>(a));
                break;
            }
            case VECTOR_ELEMENTMODP_PTR: // vector<ElementModP *>
            {
                input_string = hash_inner_vector<ElementModP *>(get<vector<ElementModP *>>(a));
                break;
            }
            case VECTOR_ELEMENTMODQ_PTR: // vector<ElementModQ *>
            {
                input_string = hash_inner_vector<ElementModQ *>(get<vector<ElementModQ *>>(a));
                break;
            }
            case VECTOR_CRYPTOHASHABLE_REF: // vector<reference_wrapper<CryptoHashable>>
            {
                input_string = hash_inner_vector<reference_wrapper<CryptoHashable>>(
                  get<vector<reference_wrapper<CryptoHashable>>>(a));
                break;
            }
            case VECTOR_ELEMENTMODP_REF: // vector<reference_wrapper<ElementModP>>
            {
                input_string = hash_inner_vector<reference_wrapper<ElementModP>>(
                  get<vector<reference_wrapper<ElementModP>>>(a));
                break;
            }
            case VECTOR_ELEMENTMODQ_REF: // vector<reference_wrapper<ElementModQ>>
            {
                input_string = hash_inner_vector<reference_wrapper<ElementModQ>>(
                  get<vector<reference_wrapper<ElementModQ>>>(a));
                break;
            }
            case VECTOR_CRYPTOHASHABLE_CONST_REF: // vector<reference_wrapper<const CryptoHashable>>
            {
                input_string = hash_inner_vector<reference_wrapper<const CryptoHashable>>(
                  get<vector<reference_wrapper<const CryptoHashable>>>(a));
                break;
            }
            case VECTOR_ELEMENTMODP_CONST_REF: // vector<reference_wrapper<const ElementModP>>
            {
                input_string = hash_inner_vector<reference_wrapper<const ElementModP>>(
                  get<vector<reference_wrapper<const ElementModP>>>(a));
                break;
            }
            case VECTOR_ELEMENTMODQ_CONST_REF: // vector<reference_wrapper<const ElementModQ>>
            {
                input_string = hash_inner_vector<reference_wrapper<const ElementModQ>>(
                  get<vector<reference_wrapper<const ElementModQ>>>(a));
                break;
            }
            case VECTOR_UINT64_T: // vector<uint64_t>
            {
                input_string = hash_inner_vector<uint64_t>(get<vector<uint64_t>>(a));
                break;
            }
            case VECTOR_STRING: // vector<string>
            {
                input_string = hash_inner_vector<string>(get<vector<string>>(a));
                break;
            }
        }

        if (input_string.empty()) {
            input_string = null_string;
        }

        const auto *input = reinterpret_cast<const uint8_t *>(input_string.c_str());
        Hacl_Streaming_SHA2_update_256(p, const_cast<uint8_t *>(input), input_string.size());
        Hacl_Streaming_SHA2_update_256(p, static_cast<uint8_t *>(delimiter), sizeof(delimiter));
    }
} // namespace electionguard
