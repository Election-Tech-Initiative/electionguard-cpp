#include "electionguard/group.hpp"

#include "../kremlin/Hacl_Bignum256.h"
#include "../kremlin/Hacl_Bignum4096.h"
#include "electionguard/constants.h"
#include "log.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
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
        if (!unchecked && Hacl_Bignum4096_lt_mask(const_cast<uint64_t *>(P_ARRAY), elem) > 0) {
            throw out_of_range("Value for ElementModP is greater than allowed");
        }
        memcpy(data.elem, elem, MAX_P_SIZE);
    }
    ElementModP::~ElementModP() {}

    uint64_t *ElementModP::get() { return data.elem; }

    string ElementModP::toHex()
    {
        // Returned bytes array from Hacl needs to be pre-allocated to 512 bytes
        uint8_t byteResult[MAX_P_SIZE] = {};
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
        auto *bn = Hacl_Bignum4096_new_bn_from_bytes_be(bLen, b);
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

        // TODO: temporarily using a mul operation to expand the data
        uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
        Hacl_Bignum4096_mul(result, ONE_MOD_P()->get(), mulResult);

        uint64_t modResult[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod(P()->get(), mulResult, modResult);
        if (!success) {
            throw runtime_error(" add_mod_p mod operation failed");
        }
        return new ElementModP(result);
    }

    ElementModP *mul_mod_p(ElementModP *lhs, ElementModP *rhs)
    {
        uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
        Hacl_Bignum4096_mul(lhs->get(), rhs->get(), mulResult);
        uint64_t modResult[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod(P()->get(), mulResult, modResult);
        if (!success) {
            throw runtime_error(" mul_mod_p mod operation failed");
        }
        return new ElementModP(modResult);
    }

    ElementModP *pow_mod_p(ElementModP *b, ElementModP *e)
    {
        // HACL's input constraints require the exponent to be greater than zero
        if (*e == *ZERO_MOD_P()) {
            return uint64_to_p(1);
        }

        uint64_t result[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod_exp(P()->get(), b->get(), MAX_P_SIZE, e->get(), result);
        if (!success) {
            throw runtime_error(" pow_mod_p mod operation failed");
        }

        return new ElementModP(result, true);
    }

    ElementModP *g_pow_p(ElementModP *e) { return pow_mod_p(G(), e); }

    // param elem is expected to be allocated to uint64_t[MAX_Q_LEN]
    ElementModQ::ElementModQ(uint64_t *elem, bool unchecked /* = false*/) : data()
    {
        if (!unchecked && Hacl_Bignum256_lt_mask(const_cast<uint64_t *>(Q_ARRAY), elem) > 0) {
            throw out_of_range("Value for ElementModQ is greater than allowed");
        }
        memcpy(data.elem, elem, MAX_Q_SIZE);
    }
    ElementModQ::~ElementModQ() {}

    uint64_t *ElementModQ::get() { return data.elem; }

    string ElementModQ::toHex()
    {
        // Returned bytes array from Hacl needs to be pre-allocated to 32 bytes
        uint8_t byteResult[MAX_Q_SIZE] = {};
        // Use Hacl to convert the bignum to byte array
        Hacl_Bignum256_bn_to_bytes_be(data.elem, byteResult);
        return bytes_to_hex(byteResult, sizeof(byteResult));
    }

    ElementModP *ElementModQ::toElementModP()
    {
        uint64_t p4096[MAX_P_LEN] = {};
        memcpy(p4096, data.elem, MAX_Q_SIZE);
        return new ElementModP(p4096, true);
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

    bool ElementModQ::operator<(const ElementModQ &other)
    {
        auto other_ = other;
        return Hacl_Bignum256_lt_mask(data.elem, other_.data.elem) > 0;
    }

    ElementModQ *bytes_to_q(uint8_t *b, size_t bLen)
    {
        auto *bn = Hacl_Bignum256_new_bn_from_bytes_be(bLen, b);
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
        uint64_t addResult[MAX_Q_LEN] = {};
        uint64_t carry = Hacl_Bignum256_add(lhs->get(), rhs->get(), addResult);
        if (carry > 0) {
            // just bypass compiler error
        }

        // TODO: Temporarily executing a mul operation to widen the result
        uint64_t mulResult[MAX_Q_LEN_DOUBLE] = {};
        Hacl_Bignum256_mul(addResult, ONE_MOD_Q()->get(), mulResult);

        uint64_t resModQ[MAX_Q_LEN] = {};
        bool modSuccess = Hacl_Bignum256_mod(Q()->get(), mulResult, resModQ);
        if (!modSuccess) {
            throw runtime_error(" add_mod_q mod operation failed");
        }

        return new ElementModQ(resModQ);
    }

    ElementModQ *a_minus_b_mod_q(ElementModQ *a, ElementModQ *b)
    {
        uint64_t subResult[MAX_Q_LEN] = {};
        Hacl_Bignum256_sub(a->get(), b->get(), subResult);

        // TODO: Temporarily executing a mul operation to widen the result
        uint64_t mulResult[MAX_Q_LEN_DOUBLE] = {};
        Hacl_Bignum256_mul(subResult, ONE_MOD_Q()->get(), mulResult);

        uint64_t resModQ[MAX_Q_LEN] = {};
        bool modSuccess = Hacl_Bignum256_mod(Q()->get(), mulResult, resModQ);
        if (!modSuccess) {
            throw runtime_error(" a_minus_b_mod_q mod operation failed");
        }

        return new ElementModQ(resModQ);
    }

    ElementModQ *a_plus_bc_mod_q(ElementModQ *a, ElementModQ *b, ElementModQ *c)
    {
        uint64_t resultBC[MAX_Q_LEN_DOUBLE] = {};
        Hacl_Bignum256_mul(b->get(), c->get(), resultBC);

        // TODO: Temprarily using P address space
        // since the result of b*c can be larger than Q's 256,
        // use P's length and 4096 Hacl for rest of calculation
        uint64_t bc4096[MAX_P_LEN] = {};
        memcpy(bc4096, resultBC, MAX_Q_LEN_DOUBLE);

        uint64_t a_plus_bc_result[MAX_P_LEN] = {};
        uint64_t carry = Hacl_Bignum4096_add(a->toElementModP()->get(), bc4096, a_plus_bc_result);

        // TODO: Temporarily executing a mul operation to widen the result
        uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
        Hacl_Bignum256_mul(a_plus_bc_result, ONE_MOD_P()->get(), mulResult);

        uint64_t resModQ[MAX_P_LEN] = {};
        bool modSuccess = Hacl_Bignum4096_mod(Q()->toElementModP()->get(), mulResult, resModQ);
        if (!modSuccess) {
            throw runtime_error(" a_plus_bc_mod_q mod operation failed");
        }

        return new ElementModQ(resModQ);
    }

    ElementModQ *negate_mod_q(ElementModQ *a)
    {
        uint64_t result[MAX_Q_LEN] = {};
        Hacl_Bignum256_sub(Q()->get(), a->get(), result);
        // TODO: python version doesn't perform % Q on results...
        return new ElementModQ(result, true);
    }

    ElementModQ *rand_q()
    {
        // TODO: choose a better random generator
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