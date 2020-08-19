#include "hash.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

extern "C" {
#include "../kremlin/Hacl_Streaming_SHA2_256.h"
}

namespace electionguard
{
    string hex_str(uint8_t *data, int len)
    {
        stringstream ss;
        ss << hex;
        for (int i(0); i < len; ++i)
            ss << setw(2) << setfill('0') << (int)data[i];
        return ss.str();
    }

    ElementModQ *hash_elems(string const &a)
    {
        // TODO: support other params to hash
        auto *input = reinterpret_cast<const uint8_t *>(a.c_str());
        uint8_t output[32] = {};
        uint8_t pipe[1] = {'|'};

        Hacl_Streaming_Functor_state_s___uint32_t____ *p = Hacl_Streaming_SHA2_256_create_in();
        Hacl_Streaming_SHA2_256_update(p, pipe, sizeof(pipe));
        Hacl_Streaming_SHA2_256_update(p, (uint8_t *)input, a.size());
        Hacl_Streaming_SHA2_256_update(p, pipe, sizeof(pipe));
        Hacl_Streaming_SHA2_256_finish(p, output);

        // auto hex_output = hex_str(output, sizeof(output));
        // cout << __func__ << " : hashed output as hex : " << hex_output << endl;

        auto *bn = Hacl_Bignum4096_new_bn_from_bytes_be(sizeof(output), output);

        Hacl_Streaming_SHA2_256_free(p);

        return new ElementModQ(bn);
    }
} // namespace electionguard
