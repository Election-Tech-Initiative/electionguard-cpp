#include "electionguard/group.hpp"

#include "../kremlin/Hacl_Bignum256.h"
#include "../kremlin/Hacl_Bignum4096.h"
#include "electionguard/constants.h"
#include "log.hpp"

#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdint.h>
#include <vector>

void hex_to_bytes(string hex, uint8_t *bytesOut)
{
    for (size_t i(0); i < hex.size(); i += 2) {
        string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t)stoi(byteString.c_str(), NULL, 16);
        bytesOut[i / 2] = byte;
    }
}

string bytes_to_hex(uint8_t *bytes, size_t bLen)
{
    // Iterate through the returned bytes to convert to Hex representation
    // while ignoring any initial 0-bytes
    bool detectedFirstNonZeroBytes = false;
    stringstream ss;
    ss << hex;
    for (size_t i(0); i < bLen; ++i) {
        int b = bytes[i];
        if (!detectedFirstNonZeroBytes && b != 0) {
            detectedFirstNonZeroBytes = true;
        }
        if (detectedFirstNonZeroBytes) {
            ss << setw(2) << setfill('0') << (int)b;
        }
    }
    return detectedFirstNonZeroBytes ? (ss.str()) : "00";
}

namespace electionguard
{
    // param elem is expected to be allocated to uint64_t[MAX_P_LEN]
    ElementModP::ElementModP(uint64_t *elem, bool unchecked /* = false */) : data()
    {
        if (!unchecked && Hacl_Bignum4096_lt(const_cast<uint64_t *>(P_ARRAY), elem)) {
            throw "Value for ElementModP is greater than allowed";
        }
        memcpy(data.elem, elem, MAX_P_LEN * sizeof(elem[0]));
    }
    ElementModP::~ElementModP() {}

    uint64_t *ElementModP::get() { return data.elem; }

    string ElementModP::toHex()
    {
        // Returned bytes array from Hacl needs to be pre-allocated to 512 bytes
        uint8_t byteResult[MAX_P_LEN * sizeof(data.elem[0])] = {};
        // Use Hacl to convert the bignum to byte array
        Hacl_Bignum4096_bn_to_bytes_be(data.elem, byteResult);
        return bytes_to_hex(byteResult, sizeof(byteResult));
    }

    bool ElementModP::operator==(const ElementModP &other)
    {
        // TODO: safety, specifically when the object underflows its max size
        // e.g. if ((l == (uint64_t)0U) && (r == (uint64_t)0U))
        for (int i = 0; i < MAX_P_LEN; i++) {
            auto l = data.elem[i];
            auto r = other.data.elem[i];
            if (l != r) {
                return false;
            }
        }
        return true;
    }

    bool ElementModP::operator!=(const ElementModP &other) { return !(*this == other); }

    ElementModP *bytes_to_p(uint8_t *b, size_t bLen)
    {
        auto bn = Hacl_Bignum4096_new_bn_from_bytes_be(bLen, b);
        uint64_t p[MAX_P_LEN] = {};
        memcpy(p, bn, bLen);
        free(bn); // free Hacl's bignum since we've copied it to p
        // since we're allocating the array to exactly the Max of P,
        // we can initialize ElementModP as unchecked
        return new ElementModP(p, true);
    }

    ElementModP *hex_to_p(string h)
    {
        bool isOdd = h.size() % 2;
        if (isOdd) {
            h.insert(0, 1, '0');
        }
        size_t len = h.size() / 2;
        uint8_t *bytes = new uint8_t[len];
        hex_to_bytes(h, bytes);
        return bytes_to_p(bytes, len);
    }

    ElementModP *uint64_to_p(uint64_t i)
    {
        uint64_t bigEndian = htobe64(i);
        return bytes_to_p((uint8_t *)&bigEndian, 8U);
    }

    ElementModP *add_mod_p(ElementModP *lhs, ElementModP *rhs)
    {
        uint64_t result[MAX_P_LEN] = {};
        uint64_t carry = Hacl_Bignum4096_add(lhs->get(), rhs->get(), result);
        if (carry > 0) {
            // just bypass compiler error
        }
        // TODO: % P
        return new ElementModP(result, true);
    }

    ElementModP *mul_mod_p(ElementModP *lhs, ElementModP *rhs)
    {
        uint64_t result[128UL] = {};
        Hacl_Bignum4096_mul(lhs->get(), rhs->get(), result);
        // TODO: % P
        return new ElementModP(result, true);
    }

    ElementModP *pow_mod_p(ElementModP *b, ElementModP *e)
    {
        // Log::debug(b, MAX_P_LEN, " : b = ");
        // Log::debug(e, MAX_P_LEN, " : e = ");

        uint64_t result[MAX_P_LEN] = {};
        Hacl_Bignum4096_mod_exp(const_cast<uint64_t *>(P_ARRAY), b->get(),
                                MAX_P_LEN * sizeof(e->get()[0]), e->get(), result);

        // Log::debug(result, MAX_P_LEN, " : result = ");

        return new ElementModP(result, true);
    }

    ElementModP *g_pow_p(ElementModP *e)
    {
        auto g = new ElementModP(const_cast<uint64_t *>(G_ARRAY), true);
        return pow_mod_p(g, e);
    }

    // param elem is expected to be allocated to uint64_t[MAX_Q_LEN]
    ElementModQ::ElementModQ(uint64_t *elem, bool unchecked /* = false*/) : data()
    {
        if (!unchecked && Hacl_Bignum256_lt(const_cast<uint64_t *>(Q_ARRAY), elem)) {
            throw "Value for ElementModQ is greater than allowed";
        }
        memcpy(data.elem, elem, MAX_Q_LEN * sizeof(elem[0]));
    }
    ElementModQ::~ElementModQ() {}

    uint64_t *ElementModQ::get() { return data.elem; }

    string ElementModQ::toHex()
    {
        // Returned bytes array from Hacl needs to be pre-allocated to 32 bytes
        uint8_t byteResult[MAX_Q_LEN * sizeof(data.elem[0])] = {};
        // Use Hacl to convert the bignum to byte array
        Hacl_Bignum256_bn_to_bytes_be(data.elem, byteResult);
        return bytes_to_hex(byteResult, sizeof(byteResult));
    }

    bool ElementModQ::operator==(const ElementModQ &other)
    {
        // TODO: safety, specifically when the object underflows its max size
        // e.g. if ((l == (uint64_t)0U) && (r == (uint64_t)0U))
        for (int i = 0; i < MAX_Q_LEN; i++) {
            auto l = data.elem[i];
            auto r = other.data.elem[i];
            if (l != r) {
                return false;
            }
        }
        return true;
    }

    bool ElementModQ::operator!=(const ElementModQ &other) { return !(*this == other); }

    ElementModQ *bytes_to_q(uint8_t *b, size_t bLen)
    {
        auto bn = Hacl_Bignum256_new_bn_from_bytes_be(bLen, b);
        uint64_t q[MAX_Q_LEN] = {};
        memcpy(q, bn, bLen);
        free(bn); // free Hacl's bignum since we've copied it to ElementModQ
        return new ElementModQ(q);
    }

    ElementModQ *hex_to_q(string h)
    {
        bool isOdd = h.size() % 2;
        if (isOdd) {
            h.insert(0, 1, '0');
        }
        size_t len = h.size() / 2;
        uint8_t *bytes = new uint8_t[len];
        hex_to_bytes(h, bytes);
        return bytes_to_q(bytes, len);
    }

    ElementModQ *uint64_to_q(uint64_t i)
    {
        uint64_t bigEndian = htobe64(i);
        return bytes_to_q((uint8_t *)&bigEndian, 8U);
    }

    ElementModQ *add_mod_q(ElementModQ *lhs, ElementModQ *rhs)
    {
        uint64_t result[MAX_Q_LEN] = {};
        uint64_t carry = Hacl_Bignum256_add(lhs->get(), rhs->get(), result);
        if (carry > 0) {
            // just bypass compiler error
        }
        // Log::debug(result->get(), MAX_Q_LEN,
        //            " : result->toHex = " + result->toHex() + " : result->get() = ");

        // TODO: % Q isn't working
        // uint64_t resModQ[MAX_Q_LEN] = {};
        // Hacl_Bignum256_mod_exp(const_cast<uint64_t *>(Q_ARRAY), result->get(), MAX_Q_LEN, one, resModQ);
        // Log::debug(resModQ, MAX_Q_LEN, " : resModQ = ");
        // Log::debug(const_cast<uint64_t *>(Q_ARRAY), MAX_Q_LEN, " : const_cast<uint64_t *>(Q_ARRAY) = ");
        // Log::debug(one, MAX_Q_LEN, " : one = ");

        // return new ElementModQ(resModQ, true);
        return new ElementModQ(result, true);
    }

    ElementModQ *a_minus_b_mod_q(ElementModQ *a, ElementModQ *b)
    {
        uint64_t result[MAX_Q_LEN] = {};
        Hacl_Bignum256_sub(a->get(), b->get(), result);
        // TODO: % Q
        return new ElementModQ(result, true);
    }

    ElementModQ *a_plus_bc_mod_q(ElementModQ *a, ElementModQ *b, ElementModQ *c)
    {
        uint64_t resultBC[512UL] = {};
        Hacl_Bignum256_mul(b->get(), c->get(), resultBC);

        // since the result of b*c can be larger than Q's 256, use P's length and 4096 Hacl for rest of calculation
        uint64_t a4096[MAX_P_LEN] = {};
        uint64_t bc4096[MAX_P_LEN] = {};
        memcpy(a4096, a, MAX_Q_LEN * sizeof(a[0]));
        memcpy(bc4096, resultBC, 512UL);

        uint64_t result[MAX_P_LEN] = {};
        uint64_t carry = Hacl_Bignum4096_add(a4096, bc4096, result);

        // TODO: % Q
        return new ElementModQ(result, true);
    }

    ElementModQ *negate_mod_q(ElementModQ *a)
    {
        uint64_t result[MAX_Q_LEN] = {};
        Hacl_Bignum256_sub(const_cast<uint64_t *>(Q_ARRAY), a->get(), result);
        // TODO: python version doesn't perform % Q on results...
        return new ElementModQ(result, true);
    }

    ElementModQ *rand_q()
    {
        // TODO: choose a better random generator?
        random_device rd;
        uniform_int_distribution<uint64_t> dist(0, 0xFFFFFFFFFFFFFFFF);

        uint64_t result[MAX_Q_LEN];
        for (size_t i(0); i < MAX_Q_LEN; i++) {
            if (i == 0) {
                // first index of Q cannot cannod exceed 0xFFFFFFFFFFFFFF43
                uniform_int_distribution<uint64_t> firstDist(0, 0xFFFFFFFFFFFFFF43);
                result[i] = firstDist(rd);
            } else {
                result[i] = dist(rd);
            }
        }

        return new ElementModQ(result);
    }
} // namespace electionguard