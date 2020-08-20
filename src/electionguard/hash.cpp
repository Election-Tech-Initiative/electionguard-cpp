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
    ElementModQ *hash_elems() { return hash_elems("null"); }
    ElementModQ *hash_elems(nullptr_t p) { return hash_elems("null"); }

    ElementModQ *hash_elems(vector<CryptoHashable> v)
    {
        if (v.empty()) {
            return hash_elems("null");
        }

        stringstream ss;
        for (auto a = v.begin(); a != v.end(); ++a) {
            // TODO: not sure if this is correct
            ss << to_string(hash_elems(&(*a))->to_int());
        }
        return hash_elems(ss.str());
    }

    ElementModQ *hash_elems(vector<ElementModP> v)
    {
        if (v.empty()) {
            return hash_elems("null");
        }

        stringstream ss;
        for (auto a = v.begin(); a != v.end(); ++a) {
            // TODO: not sure if this is correct
            ss << to_string(hash_elems(&(*a))->to_int());
        }
        return hash_elems(ss.str());
    }

    ElementModQ *hash_elems(vector<ElementModQ> v)
    {
        if (v.empty()) {
            return hash_elems("null");
        }

        stringstream ss;
        for (auto a = v.begin(); a != v.end(); ++a) {
            // TODO: not sure if this is correct
            ss << to_string(hash_elems(&(*a))->to_int());
        }
        return hash_elems(ss.str());
    }

    ElementModQ *hash_elems(vector<uint64_t> v)
    {
        if (v.empty()) {
            return hash_elems("null");
        }

        stringstream ss;
        for (auto a = v.begin(); a != v.end(); ++a) {
            // TODO: not sure if this is correct
            ss << to_string(hash_elems(*a)->to_int());
        }
        return hash_elems(ss.str());
    }

    ElementModQ *hash_elems(CryptoHashable *a) { return hash_elems(a->crypto_hash()); }

    ElementModQ *hash_elems(ElementModP *a) { return hash_elems(a->to_int()); }

    ElementModQ *hash_elems(ElementModQ *a) { return hash_elems(a->to_int()); }

    ElementModQ *hash_elems(uint64_t const &a) { return hash_elems(to_string(a)); }

    ElementModQ *hash_elems(string const &a)
    {
        if (a.empty()) {
            return hash_elems("null");
        }

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
