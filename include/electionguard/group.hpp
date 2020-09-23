#ifndef __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__
#include "constants.h"
#include "export.h"

#include <cstdint>
#include <string>
#include <vector>

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
        ElementModP(const vector<uint64_t> &elem, bool unchecked = false);
        ElementModP(const uint64_t *elem, bool unchecked = false);
        ~ElementModP();

        uint64_t *get();
        string toHex();

        /// <summary>
        /// Converts the binary value stored by the hex string
        /// to its big num representation stored as ElementModQ
        /// </summary>
        static ElementModP *fromHex(const string &representation);

        /// <summary>
        /// Converts an unsigned long integer value
        /// (that is no larger than an unsigned long int)
        /// to its big num representation stored as ElementModQ
        /// </summary>
        static ElementModP *fromUint64(uint64_t representation);

        bool operator==(const ElementModP &other);
        bool operator!=(const ElementModP &other);

        bool operator<(const ElementModP &other);

      private:
        ElementModPData data;
    };

    ElementModP *G();

    ElementModP *P();

    ElementModP *ZERO_MOD_P();

    ElementModP *ONE_MOD_P();

    ElementModP *TWO_MOD_P();

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod P
    /// </summary>
    EG_API ElementModP *add_mod_p(ElementModP *lhs, ElementModP *rhs);

    /// <summary>
    /// Multplies together the left hand side and right hand side and returns the product mod P
    /// </summary>
    EG_API ElementModP *mul_mod_p(ElementModP *lhs, ElementModP *rhs);

    /// <summary>
    /// Computes b^e mod p.
    /// </summary>
    EG_API ElementModP *pow_mod_p(ElementModP *b, ElementModP *e);

    /// <summary>
    /// Computes g^e mod p.
    /// </summary>
    EG_API ElementModP *g_pow_p(ElementModP *e);

    struct ElementModQData {
        uint64_t elem[4];
    };

    /// <summary>
    /// An element of the smaller `mod q` space, i.e., in [0, Q), where Q is a 256-bit prime.
    /// </summary>
    class EG_API ElementModQ
    {
      public:
        ElementModQ(const vector<uint64_t> &elem, bool unchecked = false);
        ElementModQ(const uint64_t *elem, bool unchecked = false);
        ~ElementModQ();

        uint64_t *get();
        string toHex();
        ElementModP *toElementModP();

        /// <summary>
        /// Converts the binary value stored by the hex string
        /// to its big num representation stored as ElementModQ
        /// </summary>
        static ElementModQ *fromHex(const string &representation);

        /// <summary>
        /// Converts an unsigned long integer value
        /// (that is no larger than an unsigned long int)
        /// to its big num representation stored as ElementModQ
        /// </summary>
        static ElementModQ *fromUint64(uint64_t representation);

        bool operator==(const ElementModQ &other);
        bool operator!=(const ElementModQ &other);

        bool operator<(const ElementModQ &other);

      private:
        ElementModQData data;
    };

    ElementModQ *Q();

    ElementModQ *ZERO_MOD_Q();

    ElementModQ *ONE_MOD_Q();

    ElementModQ *TWO_MOD_Q();

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod Q
    /// </summary>
    EG_API ElementModQ *add_mod_q(ElementModQ *lhs, ElementModQ *rhs);

    /// <summary>
    /// Computes (a-b) mod q.
    /// </summary>
    EG_API ElementModQ *a_minus_b_mod_q(ElementModQ *a, ElementModQ *b);

    /// <summary>
    /// Computes (a + b * c) mod q.
    /// </summary>
    EG_API ElementModQ *a_plus_bc_mod_q(ElementModQ *a, ElementModQ *b, ElementModQ *c);

    /// <summary>
    /// Computes (Q - a) mod q.
    /// </summary>
    EG_API ElementModQ *negate_mod_q(ElementModQ *a);

    /// <summary>
    /// Generate random number between 0 and Q
    /// </summary>
    EG_API ElementModQ *rand_q();

} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__ */