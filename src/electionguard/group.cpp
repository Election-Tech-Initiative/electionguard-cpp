#include "electionguard/group.hpp"

#include "log.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

extern "C" {
#include "../kremlin/Hacl_Bignum4096.h"
}

constexpr uint8_t MAX_P_LEN = 64;
constexpr uint8_t MAX_Q_LEN = 4;

uint64_t p_array[MAX_P_LEN] = {
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFba,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFE0175E30B1B0E79, 0x1DB502994F24DFB1};

uint64_t q_array[MAX_Q_LEN] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                               0xFFFFFFFFFFFFFF43};

uint64_t one[MAX_P_LEN] = {1};

string bignum_to_hex(uint64_t *bignumSource, size_t bignumLength)
{
    // Hacl expects the bignum uint64_t array to be exactly 4096-bits
    uint64_t bignum4096[64] = {};

    // Copy relevent bytes from our source to our 4096-bit bignum
    memcpy(bignum4096, bignumSource, bignumLength * sizeof(bignumSource));

    // Returned bytes array from Hacl needs to be pre-allocated to 512-bits
    uint8_t byteResult[64] = {};

    // Use Hacl to convert the bignum to byte array
    Hacl_Bignum4096_bn_to_bytes_be(bignum4096, byteResult);

    // Iterate through the returned bytes to convert to Hex representation
    // while ignoring any initial 0-bytes
    bool detectedFirstNonZeroBytes = false;
    stringstream ss;
    ss << hex;
    for (size_t i(0); i < sizeof(byteResult); ++i) {
        int b = byteResult[i];
        if (!detectedFirstNonZeroBytes && b != 0) {
            detectedFirstNonZeroBytes = true;
        }
        if (detectedFirstNonZeroBytes) {
            ss << setw(2) << setfill('0') << b;
        }
    }
    return detectedFirstNonZeroBytes ? (ss.str()) : "00";
}

namespace electionguard
{
    const ElementModP P = ElementModP(p_array);
    const ElementModQ Q = ElementModQ(q_array);

    ElementModP::ElementModP(uint64_t *elem) : data()
    {
        memcpy(data.elem, elem, MAX_P_LEN * sizeof(elem));
    }
    ElementModP::~ElementModP() {}

    uint64_t *ElementModP::get() { return data.elem; }

    string ElementModP::toHex() { return bignum_to_hex(data.elem, MAX_P_LEN); }

    ElementModP add_mod_p(ElementModP *lhs, ElementModP *rhs)
    {
        uint64_t res[MAX_P_LEN] = {};
        uint64_t carry = Hacl_Bignum4096_add(lhs->get(), rhs->get(), res);
        if (carry > 0) {
            // just bypass compiler error
        }

        return ElementModP(res);
    }

    ElementModP add_mod_p(uint64_t *lhs, uint64_t *rhs)
    {
        uint64_t res[MAX_P_LEN] = {};
        uint64_t carry = Hacl_Bignum4096_add(lhs, rhs, res);
        if (carry > 0) {
            // just bypass compiler error
        }

        return ElementModP(res);
    }

    ElementModQ::ElementModQ(uint64_t *elem) : data()
    {
        memcpy(data.elem, elem, MAX_Q_LEN * sizeof(elem));
    }
    ElementModQ::~ElementModQ() {}

    uint64_t *ElementModQ::get() { return data.elem; }

    string ElementModQ::toHex() { return bignum_to_hex(data.elem, MAX_Q_LEN); }

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

    ElementModQ add_mod_q(ElementModQ *lhs, ElementModQ *rhs)
    {
        uint64_t res[64] = {};
        uint64_t carry = Hacl_Bignum4096_add(lhs->get(), rhs->get(), res);
        if (carry > 0) {
            // just bypass compiler error
        }
        return ElementModQ(res);
    }
    ElementModQ add_mod_q(uint64_t *lhs, uint64_t *rhs)
    {
        uint64_t res[64] = {};
        uint64_t carry = Hacl_Bignum4096_add(lhs, rhs, res);
        if (carry > 0) {
            // just bypass compiler error
        }

        return ElementModQ(res);
    }
} // namespace electionguard