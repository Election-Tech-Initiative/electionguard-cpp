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

// G in Hacl_Bignum4096 format
uint64_t g_array[MAX_P_LEN] = {
  0xb0c73abf03e8e0aa, 0x94b38ac417cddf40, 0x40d9992197d80a6e, 0x82dc54775ab83538,
  0x10c136f8691101ad, 0x8e61369ba0ddbadb, 0xe8a317ae1e1d82,   0x288a89c931cf564f,
  0xdba8a12c5ca82757, 0xd4809ef34abeb83f, 0x76ebd2d95a3dee96, 0x8f299bcb29005200,
  0xe9f61c11a07bf421, 0xc3b1cbdb50f759d0, 0xbf7cd02d9661385a, 0x801328b2c6a60526,
  0xf6bc80662ba06207, 0xcaec94c95e96d628, 0x76c94b2008926b38, 0x93ad97b8fe09274e,
  0x9af5e71114a005f,  0xc0cdbdb76b6ad45f, 0x5a5d1cedeee01e4b, 0xc7e71fcb2255254,
  0xbaad96aac96b0dbe, 0xc9b11ab6c59bce62, 0x72ceb313fdab069c, 0xeb276fb3685f3716,
  0xdf23f1db8355bde1, 0x501fa2342b728263, 0xfb1918b2a2a30e69, 0xf7f72da39a2284fd,
  0x8090c35da9355e6c, 0x322b37c50af18215, 0x86d24900742062c1, 0xf6932825bf45de83,
  0x7aaa0e9fd8785702, 0xd8e41422b64d2cba, 0x1e221c41f55bba31, 0xa6750e7c8750b514,
  0xf09d40ac20c2ab74, 0x872e62b5d1b13a98, 0xb3c39aa08b829b28, 0xcc3eda17dbfc47b8,
  0xfdebeef58570669a, 0x7f3dabcbad8eb9d8, 0xfa7b6c87f4554cb5, 0x26e050af50a03c65,
  0xd7f736fcba6c032e, 0x6b0e2a05be6af78f, 0xdd4b795f78de07d8, 0x4e65633480b396e1,
  0x25ef356fe5bb5931, 0xd3b308da86d1c3a5, 0x4a62dcbb93b1ddd8, 0xb706fa55d5038ffb,
  0x6387b98ee0e3def1, 0xcfb0cfb65363b256, 0x840ec7d4ec804794, 0xa142cdf33f6ef853,
  0x750d6798f5196cf2, 0xd45ec4cc64cfd15e, 0x8d2a3141825b33d5, 0x37de384f98f6e03,
};

uint64_t one[MAX_P_LEN] = {1};

void hex_to_bytes(string hex, uint8_t *bytesOut)
{
    for (size_t i(0); i < hex.size(); i += 2) {
        string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t)stoi(byteString.c_str(), NULL, 16);
        bytesOut[i / 2] = byte;
    }
}

namespace electionguard
{
    const ElementModP P = ElementModP(p_array, true);
    const ElementModQ Q = ElementModQ(q_array, true);
    const ElementModP G = ElementModP(g_array, true);

    // param elem is expected to be allocated to uint64_t[MAX_P_LEN]
    ElementModP::ElementModP(uint64_t *elem, bool unchecked /* = false */) : data()
    {
        if (!unchecked && !Hacl_Bignum4096_lt(elem, const_cast<uint64_t *>(P.data.elem))) {
            // TODO: throw exception or just ignore it and set intance to a nullptr?
            // this = nullptr;
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

        Log::debug(byteResult, MAX_P_LEN * sizeof(data.elem[0]),
                   " : byteResult from Hacl_Bignum4096_bn_to_bytes_be = ");
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
        return add_mod_p(lhs->get(), rhs->get());
    }

    ElementModP *add_mod_p(uint64_t *lhs, uint64_t *rhs)
    {
        uint64_t res[MAX_P_LEN] = {};
        uint64_t carry = Hacl_Bignum4096_add(lhs, rhs, res);
        if (carry > 0) {
            // just bypass compiler error
        }
        // TODO: % P
        return new ElementModP(res, true);
    }

    ElementModP *mul_mod_p(ElementModP *lhs, ElementModP *rhs)
    {
        uint64_t res[128] = {};
        Hacl_Bignum4096_mul(lhs->get(), rhs->get(), res);
        // TODO: % P
        return new ElementModP(res, true);
    }

    ElementModP *pow_mod_p(ElementModP *b, ElementModP *e)
    {
        // Log::debug(b, MAX_P_LEN, " : b = ");
        // Log::debug(e, MAX_P_LEN, " : e = ");
        // Log::debug(one, MAX_P_LEN, " : one = ");

        uint64_t resultModP[MAX_P_LEN] = {};
        Hacl_Bignum4096_mod_exp(e->get(), b->get(), MAX_P_LEN, one, resultModP);

        // Log::debug(resultModP, MAX_P_LEN, " : resultModP = ");

        return new ElementModP(resultModP, true);
    }

    ElementModP *g_pow_p(ElementModP *e) { return pow_mod_p(const_cast<ElementModP *>(&G), e); }

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
                throw "Value for ElementModQ is greater than allowed";
            }
        }
        memcpy(data.elem, elem, MAX_Q_LEN * sizeof(elem[0]));
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
        free(bn); // free Hacl's bignum since we've copied it to ElementModQ
        // since we're allocating the array to exactly the Max of Q,
        // we can initialize ElementModQ as unchecked
        return new ElementModQ(q, true);
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
        // Hacl expects the bignum uint64_t array to be exactly 4096-bits
        // so convert q to p
        auto pL = lhs->toElementModP();
        // Log::debug(pL->get(), MAX_P_LEN,
        //            " : rhs->toHex = " + lhs->toHex() + " : lhs->toElementModP->get() = ");
        auto pR = rhs->toElementModP();
        // Log::debug(pR->get(), MAX_P_LEN,
        //            " : rhs->toHex = " + rhs->toHex() + " : rhs->toElementModP->get() = ");
        auto sumP = add_mod_p(pL, pR);
        // Log::debug(sumP->get(), MAX_P_LEN,
        //            " : sumP->toHex = " + sumP->toHex() + " : sumP->get() = ");

        // TODO: % Q isn't working
        // uint64_t resModQ[MAX_P_LEN] = {};
        // Hacl_Bignum4096_mod_exp(q_array, sumP->get(), MAX_P_LEN, one, resModQ);
        // Log::debug(resModQ, MAX_P_LEN, " : resModQ = ");
        // Log::debug(q_array, MAX_P_LEN, " : q_array = ");
        // Log::debug(one, MAX_P_LEN, " : one = ");

        // return new ElementModQ(resModQ, true);
        return new ElementModQ(sumP->get(), true);
    }

    ElementModQ *add_mod_q(uint64_t *lhs, uint64_t *rhs)
    {
        return add_mod_q(new ElementModQ(lhs, true), new ElementModQ(rhs, true));
    }
} // namespace electionguard