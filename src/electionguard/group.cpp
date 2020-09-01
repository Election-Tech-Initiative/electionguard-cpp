#include "electionguard/group.hpp"

#include "log.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <vector>

extern "C" {
#include "../kremlin/Hacl_Bignum4096.h"
}

constexpr uint8_t MAX_P_LEN = 64;
constexpr uint8_t MAX_Q_LEN = 4;

// TODO: is this the correct max P value? It's not matching python's max p
// Max P value in Hacl_Bignum4096 format
uint64_t p_array[MAX_P_LEN] = {
  0x1DB502994F24DFB1, 0xFE0175E30B1B0E79, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFFFFFFFFFFFFFFba, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
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
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};

// Max Q value in Hacl_Bignum4096 format
uint64_t q_array[MAX_P_LEN] = {
  0xFFFFFFFFFFFFFF43, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
  0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};

void hex_to_bytes(string hex, uint8_t *bytesOut)
{
    for (size_t i(0); i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t)stoi(byteString.c_str(), NULL, 16);
        bytesOut[i] = byte;
    }
}

namespace electionguard
{
    const ElementModP P = ElementModP(p_array, true);
    const ElementModQ Q = ElementModQ(q_array, true);

    // param elem is expected to be allocated to uint64_t[MAX_P_LEN]
    ElementModP::ElementModP(uint64_t *elem, bool unchecked /* = false */) : data()
    {
        if (!unchecked && !Hacl_Bignum4096_lt(elem, const_cast<uint64_t *>(P.data.elem))) {
            // TODO: throw exception or just ignore it and set intance to a nullptr?
            // this = nullptr;
            throw "Value for ElementModP is greater than allowed";
        }
        memcpy(data.elem, elem, MAX_P_LEN * sizeof(elem[0]));
        // cout << "ElementModP :: value of data.elem = ";
        for (size_t i(0); i < MAX_P_LEN; i++) {
            // cout << "[" << hex << setw(2) << setfill('0') << data.elem[i] << "]";
        }
        // cout << endl;
    }
    ElementModP::~ElementModP() {}

    uint64_t *ElementModP::get() { return data.elem; }

    string ElementModP::toHex()
    {
        // cout << "\nElementModP::toHex value of data.elem = ";
        for (size_t i(0); i < MAX_P_LEN; i++) {
            // cout << "[" << hex << setw(2) << setfill('0') << data.elem[i] << "]";
        }
        // cout << endl;

        // Returned bytes array from Hacl needs to be pre-allocated to 512 bytes
        uint8_t byteResult[MAX_P_LEN * sizeof(data.elem[0])] = {};

        // Use Hacl to convert the bignum to byte array
        Hacl_Bignum4096_bn_to_bytes_be(data.elem, byteResult);

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
                ss << setw(2) << setfill('0') << (int)b;
            }
        }
        return detectedFirstNonZeroBytes ? (ss.str()) : "00";
    }

    ElementModP *bytes_to_p(uint8_t *b, size_t bLen)
    {
        auto bn = Hacl_Bignum4096_new_bn_from_bytes_be(bLen, b);
        uint64_t p[MAX_P_LEN] = {};
        memcpy(p, bn, bLen);
        // since we're allocating the array to exactly the Max of P,
        // we can initialize ElementModP as unchecked
        return new ElementModP(p, true);
    }

    ElementModP *hex_to_p(string h)
    {
        size_t len = h.size() / 2;
        uint8_t *bytes = new uint8_t[len];
        hex_to_bytes(h, bytes);
        return bytes_to_p(bytes, len);
    }

    ElementModP *uint64_to_p(uint64_t i)
    {
        uint64_t bigEndian = htobe64(i);
        return bytes_to_p((uint8_t *)&bigEndian, 8);
    }

    ElementModP *add_mod_p(ElementModP *lhs, ElementModP *rhs)
    {
        uint64_t res[MAX_P_LEN] = {};
        uint64_t carry = Hacl_Bignum4096_add(lhs->get(), rhs->get(), res);
        if (carry > 0) {
            // just bypass compiler error
        }
        // cout << "\nElementModP::add_mod_p value of res = ";
        for (size_t i(0); i < MAX_P_LEN; i++) {
            // cout << "[" << hex << setw(2) << setfill('0') << res[i] << "]";
        }
        // cout << endl;
        // TODO: % P?
        return new ElementModP(res);
    }

    // param elem is expected to be allocated to uint64_t[MAX_Q_LEN]
    ElementModQ::ElementModQ(uint64_t *elem, bool unchecked /* = false*/) : data()
    {
        if (!unchecked) {
            // convert elem to 4096-bits to perform bignum comparison
            uint64_t q4096[MAX_P_LEN] = {};
            memcpy(q4096, elem, MAX_Q_LEN * sizeof(elem[0]));
            if (!Hacl_Bignum4096_lt(q4096, const_cast<uint64_t *>(Q.data.elem))) {
                // TODO: throw exception or just ignore it and set intance to a nullptr?
                // this = nullptr;
                // cout << "value of q4096 = ";
                for (size_t i(0); i < MAX_P_LEN; i++) {
                    // cout << "[" << hex << setw(2) << setfill('0') << (int)q4096[i] << "]";
                }
                // cout << endl;
                throw "Value for ElementModQ is greater than allowed";
            }
        }

        memcpy(data.elem, elem, MAX_Q_LEN * sizeof(elem[0]));
        // cout << "ElementModQ :: value of data.elem = ";
        for (size_t i(0); i < MAX_Q_LEN; i++) {
            // cout << "[" << hex << setw(2) << setfill('0') << data.elem[i] << "]";
        }
        // cout << endl;
    }
    ElementModQ::~ElementModQ() {}

    uint64_t *ElementModQ::get() { return data.elem; }

    // Hacl can only perform work on 4096-bit numbers
    // Since ElementModP is already 4096-bits, have ElementModQ convert to
    // ElementModP when it needs to use Hacl methods
    ElementModP *ElementModQ::toElementModP()
    {
        // convert to 4096-bits to use Hacl methods
        uint64_t p[MAX_P_LEN] = {};
        memcpy(p, data.elem, MAX_Q_LEN * sizeof(data.elem[0]));
        // cout << "\nElementModQ::toElementModP :: value of p = ";
        for (size_t i(0); i < MAX_P_LEN; i++) {
            // cout << "[" << hex << setw(2) << setfill('0') << (int)p[i] << "]";
        }
        // cout << endl;
        return new ElementModP(p, true);
    }

    string ElementModQ::toHex() { return toElementModP()->toHex(); }

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
        auto bn = Hacl_Bignum4096_new_bn_from_bytes_be(bLen, b);
        uint64_t q[MAX_Q_LEN] = {};
        memcpy(q, bn, bLen);
        // cout << "bytes_to_q = ";
        for (size_t i(0); i < MAX_Q_LEN; i++) {
            // cout << "[" << hex << setw(2) << setfill('0') << (int)q[i] << "]";
        }
        // cout << endl;
        // since we're allocating the array to exactly the Max of Q,
        // we can initialize ElementModQ as unchecked
        return new ElementModQ(q, true);
    }

    ElementModQ *hex_to_q(string h)
    {
        size_t len = h.size() / 2;
        uint8_t *bytes = new uint8_t[len];
        hex_to_bytes(h, bytes);
        return bytes_to_q(bytes, len);
    }

    ElementModQ *uint64_to_q(uint64_t i)
    {
        uint64_t bigEndian = htobe64(i);
        return bytes_to_q((uint8_t *)&bigEndian, 8);
    }

    ElementModQ *add_mod_q(ElementModQ *lhs, ElementModQ *rhs)
    {
        // cout << "--- in add_mod_q ---" << endl;
        // Hacl expects the bignum uint64_t array to be exactly 4096-bits
        // so convert q to p
        auto pL = lhs->toElementModP();
        // cout << "pL = " << pL->toHex() << endl;
        auto pR = rhs->toElementModP();
        // cout << "pR = " << pR->toHex() << endl;

        auto sumP = add_mod_p(pL, pR);

        Log::debug(" : sumP = " + sumP->toHex());

        // cout << "sumP = ";
        for (size_t i(0); i < MAX_P_LEN; i++) {
            // cout << "[" << hex << setw(2) << setfill('0') << (sumP->get())[i] << "]";
        }
        // cout << endl;
        // TODO: % P
        return new ElementModQ(sumP->get());
    }
} // namespace electionguard