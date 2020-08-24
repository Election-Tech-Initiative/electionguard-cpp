#include "electionguard/group.hpp"

#include <iostream>
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

namespace electionguard
{
    const ElementModP P = ElementModP(p_array);
    const ElementModQ Q = ElementModQ(q_array);

    ElementModP::ElementModP(uint64_t *elem) : data()
    {
        // TODO : safety
        for (int i = 0; i < MAX_P_LEN; i++) {
            data.elem[i] = elem[i];
        }
    }
    ElementModP::~ElementModP() {}

    uint64_t *ElementModP::get() { return data.elem; }

    string ElementModP::toBigIntString()
    {
        // TODO: convert uint64_t pointer to big int string
        return to_string(data.elem[0]); // temp work around
    }

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
        // TODO : safety
        for (int i = 0; i < MAX_Q_LEN; i++) {
            data.elem[i] = elem[i];
        }
    }
    ElementModQ::~ElementModQ() {}

    uint64_t *ElementModQ::get() { return data.elem; }

    string ElementModQ::toBigIntString()
    {
        // TODO: convert uint64_t pointer to big int string
        return to_string(data.elem[0]); // temp work around
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