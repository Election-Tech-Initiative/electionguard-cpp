#ifndef __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__
#include "export.h"

#include <cstdint>

namespace electionguard
{
    struct ElementModPData {
        uint64_t elem[64];
    };

    class ElementModP
    {
      public:
        ElementModP(uint64_t *elem);
        ~ElementModP();

        uint64_t *get();
        uint64_t to_int();

      private:
        ElementModPData data;
    };

    ElementModP add_mod_p(ElementModP *lhs, ElementModP *rhs);
    ElementModP add_mod_p(uint64_t *lhs, uint64_t *rhs);

    struct ElementModQData {
        uint64_t elem[4];
    };

    class ElementModQ
    {
      public:
        ElementModQ(uint64_t *elem);
        ~ElementModQ();

        uint64_t *get();
        uint64_t to_int();

      private:
        ElementModQData data;
    };

    ElementModQ add_mod_q(ElementModQ *lhs, ElementModQ *rhs);
    ElementModQ add_mod_q(uint64_t *lhs, uint64_t *rhs);

} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__ */