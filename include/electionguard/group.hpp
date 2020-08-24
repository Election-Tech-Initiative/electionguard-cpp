#ifndef __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__
#include "export.h"

#include <cstdint>

namespace electionguard
{
    struct ElementModPData {
        uint64_t elem[64];
    };

    /// <summary>
    /// An element of the larger `mod p` space, i.e., in [0, P), where P is a 4096-bit prime.
    /// </summary>
    class EG_API ElementModP
    {
      public:
        ElementModP(uint64_t *elem);
        ~ElementModP();

        uint64_t *get();

      private:
        ElementModPData data;
    };

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod P
    /// </summary>
    EG_API ElementModP add_mod_p(ElementModP *lhs, ElementModP *rhs);

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod P
    /// </summary>
    EG_API ElementModP add_mod_p(uint64_t *lhs, uint64_t *rhs);

    struct ElementModQData {
        uint64_t elem[4];
    };

    /// <summary>
    /// An element of the smaller `mod q` space, i.e., in [0, Q), where Q is a 256-bit prime.
    /// </summary>
    class EG_API ElementModQ
    {
      public:
        ElementModQ(uint64_t *elem);
        ~ElementModQ();

        uint64_t *get();

      private:
        ElementModQData data;
    };

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod Q
    /// </summary>
    EG_API ElementModQ add_mod_q(ElementModQ *lhs, ElementModQ *rhs);

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod Q
    /// </summary>
    EG_API ElementModQ add_mod_q(uint64_t *lhs, uint64_t *rhs);

} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__ */