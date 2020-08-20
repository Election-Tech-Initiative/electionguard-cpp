#include "electionguard/group.hpp"

extern "C" {
#include "../kremlin/Hacl_Bignum4096.h"
}

uint64_t p_array[64] = {
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

uint64_t q_array[4] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                       0xFFFFFFFFFFFFFF43};

uint64_t one[64] = {1};

namespace electionguard
{
    const ElementModP P = ElementModP(p_array);
    const ElementModQ Q = ElementModQ(q_array);

    ElementModP::ElementModP(uint64_t *elem) : data()
    {
        // TODO : safety
        for (int i = 0; i < 64; i++) {
            data.elem[i] = elem[i];
        }
    }
    ElementModP::~ElementModP() {}

    uint64_t *ElementModP::get() { return data.elem; }

    uint64_t ElementModP::to_int()
    {
        // TODO: convert bignum pointer to uint64_t
        return data.elem[0]; // temp work around
    }

    ElementModP add_mod_p(ElementModP *lhs, ElementModP *rhs)
    {
        uint64_t res[64] = {};
        uint64_t carry = Hacl_Bignum4096_add(lhs->get(), rhs->get(), res);
        if (carry > 0) {
            // just bypass compiler error
        }

        return ElementModP(res);
    }

    ElementModP add_mod_p(uint64_t *lhs, uint64_t *rhs)
    {
        uint64_t res[64] = {};
        uint64_t carry = Hacl_Bignum4096_add(lhs, rhs, res);
        if (carry > 0) {
            // just bypass compiler error
        }

        return ElementModP(res);
    }

    ElementModQ::ElementModQ(uint64_t *elem) : data()
    {
        // TODO : safety
        for (int i = 0; i < 4; i++) {
            data.elem[i] = elem[i];
        }
    }
    ElementModQ::~ElementModQ() {}

    uint64_t *ElementModQ::get() { return data.elem; }

    uint64_t ElementModQ::to_int()
    {
        // TODO: convert uint64_t pointer to uint64_t
        return data.elem[0]; // temp work around
    }

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