#include "electionguard/hash.hpp"

#include "../kremlin/Hacl_Bignum256.h"
#include "../kremlin/Hacl_Streaming_SHA2.h"
#include "log.hpp"

#include <iomanip>
#include <iostream>

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

        auto *bigNum = Hacl_Bignum256_new_bn_from_bytes_be(sizeof(output), output);
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

    void push_hash_update(Hacl_Streaming_Functor_state_s___uint32_t____ *p, CryptoHashableType a)
    {
        string input_string;
        switch (a.index()) {
            case 0: // nullptr_t
            {
                input_string = null_string;
                break;
            }
            case 1: // CryptoHashable *
            {
                auto hashable = get<CryptoHashable *>(a)->crypto_hash();
                input_string = hashable->toHex();
                break;
            }
            case 2: // ElementModP *
            {
                input_string = get<ElementModP *>(a)->toHex();
                break;
            }
            case 3: // ElementModQ *
            {
                input_string = get<ElementModQ *>(a)->toHex();
                break;
            }
            case 4: // uint64_t
            {
                uint64_t i = get<uint64_t>(a);
                if (i != 0) {
                    input_string = to_string(i);
                }
                break;
            }
            case 5: // string
            {
                input_string = get<string>(a);
                break;
            }
            case 6: // vector<CryptoHashable *>
            {
                input_string =
                  hash_inner_vector<CryptoHashable *>(get<vector<CryptoHashable *>>(a));
                break;
            }
            case 7: // vector<ElementModP *>
            {
                input_string = hash_inner_vector<ElementModP *>(get<vector<ElementModP *>>(a));
                break;
            }
            case 8: // vector<ElementModQ *>
            {
                input_string = hash_inner_vector<ElementModQ *>(get<vector<ElementModQ *>>(a));
                break;
            }
            case 9: // vector<uint64_t>
            {
                input_string = hash_inner_vector<uint64_t>(get<vector<uint64_t>>(a));
                break;
            }
            case 10: // vector<string>
            {
                input_string = hash_inner_vector<string>(get<vector<string>>(a));
                break;
            }
        }

        if (input_string.empty()) {
            input_string = null_string;
        }

        const uint8_t *input = reinterpret_cast<const uint8_t *>(input_string.c_str());
        Hacl_Streaming_SHA2_update_256(p, (uint8_t *)input, input_string.size());
        Hacl_Streaming_SHA2_update_256(p, static_cast<uint8_t *>(delimiter), sizeof(delimiter));
    }
} // namespace electionguard
