#ifndef __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__
#include "export.h"

#include <cstdint>
#include <string>

using namespace std;

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
        ElementModP(uint64_t *elem, bool unchecked = false);
        ~ElementModP();

        uint64_t *get();
        string toHex();

      private:
        ElementModPData data;
    };

    /// <summary>
    /// Converts the binary value stored as a byte array
    /// to its big num representation stored as ElementModP
    /// </summary>
    EG_API ElementModP *bytes_to_p(uint8_t *b, size_t bLen);

    /// <summary>
    /// Converts the binary value stored by the hex string
    /// to its big num representation stored as ElementModP
    /// </summary>
    EG_API ElementModP *hex_to_p(string h);

    /// <summary>
    /// Converts an unsigned long integer value (that is no larger than an unsigned long int)
    /// to its big num representation stored as ElementModP
    /// </summary>
    EG_API ElementModP *uint64_to_p(uint64_t i);

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod P
    /// </summary>
    EG_API ElementModP *add_mod_p(ElementModP *lhs, ElementModP *rhs);

    struct ElementModQData {
        uint64_t elem[4];
    };

    /// <summary>
    /// An element of the smaller `mod q` space, i.e., in [0, Q), where Q is a 256-bit prime.
    /// </summary>
    class EG_API ElementModQ
    {
      public:
        ElementModQ(uint64_t *elem, bool unchecked = false);
        ~ElementModQ();

        uint64_t *get();
        ElementModP *toElementModP();
        string toHex();

        bool operator==(const ElementModQ &other);
        bool operator!=(const ElementModQ &other);

      private:
        ElementModQData data;
    };

    /// <summary>
    /// Converts the binary value stored as a big-endian byte array
    /// to its big num representation stored as ElementModQ
    /// </summary>
    EG_API ElementModQ *bytes_to_q(uint8_t *b, size_t bLen);

    /// <summary>
    /// Converts the binary value stored by the hex string
    /// to its big num representation stored as ElementModQ
    /// </summary>
    EG_API ElementModQ *hex_to_q(string h);

    /// <summary>
    /// Converts an unsigned long integer value (that is no larger than an unsigned long int)
    /// to its big num representation stored as ElementModQ
    /// </summary>
    EG_API ElementModQ *uint64_to_q(uint64_t i);

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod Q
    /// </summary>
    EG_API ElementModQ *add_mod_q(ElementModQ *lhs, ElementModQ *rhs);

    // mul_mod_p, pow_mod_p, g_pow_p, rand_q, a_minus_b_mod_q, a_plus_bc_mod_q, negate_mod_q

} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__ */