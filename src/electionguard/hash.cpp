#include "hash.hpp"

#include "log.hpp"

#include <iomanip>
#include <iostream>

extern "C" {
#include "../kremlin/Hacl_Streaming_SHA2_256.h"
}

namespace electionguard
{
    template <typename T> string hash_inner_vector(vector<T> inner_vector);
    void push_hash_update(Hacl_Streaming_Functor_state_s___uint32_t____ *p, CryptoHashableType a);

    const char delimiter_char = '|';
    const string null_string = "null";

    uint8_t delimiter[1] = {delimiter_char};

    ElementModQ *hash_elems(vector<CryptoHashableType> a)
    {
        uint8_t output[32] = {};
        Hacl_Streaming_Functor_state_s___uint32_t____ *p = Hacl_Streaming_SHA2_256_create_in();
        Hacl_Streaming_SHA2_256_update(p, delimiter, sizeof(delimiter));

        if (a.size() == 0) {
            push_hash_update(p, null_string);
        } else {
            for (CryptoHashableType item : a) {
                push_hash_update(p, item);
            }
        }

        Hacl_Streaming_SHA2_256_finish(p, output);

        auto *bn = Hacl_Bignum4096_new_bn_from_bytes_be(sizeof(output), output);

        Hacl_Streaming_SHA2_256_free(p);

        return new ElementModQ(bn);
    }

    ElementModQ *hash_elems(CryptoHashableType a)
    {
        return hash_elems(vector<CryptoHashableType>{a});
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
                input_string = get<CryptoHashable *>(a)->crypto_hash()->toHex();
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
        Hacl_Streaming_SHA2_256_update(p, (uint8_t *)input, input_string.size());
        Hacl_Streaming_SHA2_256_update(p, delimiter, sizeof(delimiter));
    }
} // namespace electionguard
