#include "hash.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

extern "C" {
#include "../kremlin/Hacl_Streaming_SHA2_256.h"
}

// helper to output the hex value of the resulting SHA-256 hash
string hex_str(uint8_t *data, int len)
{
    stringstream ss;
    ss << hex;
    for (int i(0); i < len; ++i)
        ss << setw(2) << setfill('0') << (int)data[i];
    return ss.str();
}

namespace electionguard
{
    const char delimiter_char = '|';
    const string null_string = "null";

    uint8_t delimiter[1] = {delimiter_char};

    void push_hash_update(Hacl_Streaming_Functor_state_s___uint32_t____ *p, CryptoHashableType a)
    {
        string input_string;
        switch (a.index()) {
            case 0: // nullptr_t
                input_string = null_string;
                break;
            case 1: // CryptoHashable
                input_string = to_string(get<CryptoHashable *>(a)->crypto_hash()->to_int());
                break;
            case 2: // ElementModP
                input_string = to_string(get<ElementModP *>(a)->to_int());
                break;
            case 3: // ElementModQ
                input_string = to_string(get<ElementModQ *>(a)->to_int());
                break;
            case 4: // uint64_t
                input_string = to_string(get<uint64_t>(a));
                break;
            default: // string
                input_string = get<string>(a);
                break;
                // TODO: equivalent of hashing a Sequence in Python?
                // Need `ElementModQ.to_int` implementation to validate crunching the recursive Q output works
        }

        if (input_string.empty()) {
            input_string = null_string;
        }

        const uint8_t *input = reinterpret_cast<const uint8_t *>(input_string.c_str());
        Hacl_Streaming_SHA2_256_update(p, (uint8_t *)input, input_string.size());
        Hacl_Streaming_SHA2_256_update(p, delimiter, sizeof(delimiter));

        cout << __func__ << " : input variant index : " << a.index()
             << " : input_string : " << input_string << endl;
    }

    ElementModQ *hash_elems(initializer_list<CryptoHashableType> a)
    {
        uint8_t output[32] = {};
        Hacl_Streaming_Functor_state_s___uint32_t____ *p = Hacl_Streaming_SHA2_256_create_in();
        Hacl_Streaming_SHA2_256_update(p, delimiter, sizeof(delimiter));

        if (a.size() == 0) {
            push_hash_update(p, null_string);
        } else {
            for (auto it = a.begin(); it != a.end(); ++it) {
                push_hash_update(p, *it);
            }
        }

        Hacl_Streaming_SHA2_256_finish(p, output);

        auto hex_output = hex_str(output, sizeof(output));
        cout << __func__ << " : output as hex : " << hex_output << endl;

        auto *bn = Hacl_Bignum4096_new_bn_from_bytes_be(sizeof(output), output);

        Hacl_Streaming_SHA2_256_free(p);

        return new ElementModQ(bn);
    }

    ElementModQ *hash_elems(CryptoHashableType a)
    {
        return hash_elems(initializer_list<CryptoHashableType>{a});
    }
} // namespace electionguard
