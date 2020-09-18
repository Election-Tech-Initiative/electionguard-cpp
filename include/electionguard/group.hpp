#ifndef __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__
#include "constants.h"
#include "export.h"

#include <cstdint>
#include <string>
#include <variant>
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
        bool isInBounds();
        bool isValidResidue();
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

        bool isInBounds();

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

        // TODO: overload math operators and redirect to functions

        bool operator==(const ElementModQ &other);
        bool operator!=(const ElementModQ &other);

        bool operator<(const ElementModQ &other);

      private:
        ElementModQData data;
    };

    // Common constants

    constexpr ElementModP *G() { return new ElementModP(G_ARRAY_REVERSE, true); };

    constexpr ElementModP *P() { return new ElementModP(P_ARRAY_REVERSE, true); };

    constexpr ElementModP *ZERO_MOD_P() { return new ElementModP(ZERO_MOD_P_ARRAY, true); };

    constexpr ElementModP *ONE_MOD_P() { return new ElementModP(ONE_MOD_P_ARRAY, true); };

    constexpr ElementModP *TWO_MOD_P() { return new ElementModP(TWO_MOD_P_ARRAY, true); };

    constexpr ElementModQ *Q() { return new ElementModQ(Q_ARRAY_REVERSE, true); };

    constexpr ElementModQ *ZERO_MOD_Q() { return new ElementModQ(ZERO_MOD_Q_ARRAY, true); };

    constexpr ElementModQ *ONE_MOD_Q() { return new ElementModQ(ONE_MOD_Q_ARRAY, true); };

    constexpr ElementModQ *TWO_MOD_Q() { return new ElementModQ(TWO_MOD_Q_ARRAY, true); };

    using ElementModPOrQ = variant<ElementModP *, ElementModQ *>;

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod P
    /// </summary>
    EG_API ElementModP *add_mod_p(ElementModP *lhs, ElementModP *rhs);

    /// <summary>
    /// Multplies together the left hand side and right hand side and returns the product mod P
    /// </summary>
    EG_API ElementModP *mul_mod_p(ElementModP *lhs, ElementModP *rhs);

    EG_API ElementModP *mul_mod_p(vector<ElementModPOrQ> elems);

    /// <summary>
    /// Computes b^e mod p.
    /// </summary>
    EG_API ElementModP *pow_mod_p(ElementModPOrQ b, ElementModPOrQ e);

    /// <summary>
    /// Computes g^e mod p.
    /// </summary>
    EG_API ElementModP *g_pow_p(ElementModPOrQ e);

    //EG_API ElementModP *mul_inv_mod_p

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod Q
    /// </summary>
    EG_API ElementModQ *add_mod_q(ElementModQ *lhs, ElementModQ *rhs);

    /// <summary>
    /// Computes (a-b) mod q.
    /// </summary>
    EG_API ElementModQ *sub_mod_q(ElementModQ *a, ElementModQ *b);

    /// <summary>
    /// Computes (a + b * c) mod q.
    /// </summary>
    EG_API ElementModQ *a_plus_bc_mod_q(ElementModQ *a, ElementModQ *b, ElementModQ *c);

    /// <summary>
    /// Computes (Q - a) mod q.
    /// </summary>
    EG_API ElementModQ *sub_from_q(ElementModQ *a);

    /// <summary>
    /// Generate random number between 0 and Q
    /// </summary>
    EG_API ElementModQ *rand_q();

} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__ */