#ifndef __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__

#include "constants.h"
#include "export.h"

#include <cstdint>
#include <memory>
#include <string>
#include <variant>
#include <vector>

using namespace std;

namespace electionguard
{

    /// <summary>
    /// An element of the larger `mod p` space, i.e., in [0, P), where P is a 4096-bit prime.
    /// </summary>
    class EG_API ElementModP
    {
      public:
        ElementModP(const ElementModP &other);
        ElementModP(ElementModP &&other);
        ElementModP(const vector<uint64_t> &elem, bool unchecked = false);
        ElementModP(const uint64_t (&elem)[MAX_P_LEN], bool unchecked = false);
        ~ElementModP();

        ElementModP &operator=(ElementModP other);
        ElementModP &operator=(ElementModP &&other);
        bool operator==(const ElementModP &other);
        bool operator!=(const ElementModP &other);

        bool operator<(const ElementModP &other);

        // TODO: math operators

        uint64_t *get() const;
        bool isInBounds() const;
        bool isValidResidue() const;
        vector<uint8_t> toBytes() const;
        string toHex() const;

        /// <summary>
        /// Converts the binary value stored by the hex string
        /// to its big num representation stored as ElementModQ
        /// </summary>
        static unique_ptr<ElementModP> fromHex(const string &representation);

        /// <summary>
        /// Converts an unsigned long integer value
        /// (that is no larger than an unsigned long int)
        /// to its big num representation stored as ElementModQ
        /// </summary>
        static unique_ptr<ElementModP> fromUint64(uint64_t representation);

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// An element of the smaller `mod q` space, i.e., in [0, Q), where Q is a 256-bit prime.
    /// </summary>
    class EG_API ElementModQ
    {
      public:
        ElementModQ(const ElementModQ &other);
        ElementModQ(ElementModQ &&other);
        ElementModQ(const vector<uint64_t> &elem, bool unchecked = false);
        ElementModQ(const uint64_t (&elem)[MAX_Q_LEN], bool unchecked = false);
        ~ElementModQ();

        ElementModQ &operator=(ElementModQ other);
        ElementModQ &operator=(ElementModQ &&other);
        bool operator==(const ElementModQ &other);
        bool operator!=(const ElementModQ &other);

        bool operator<(const ElementModQ &other);

        // TODO: overload math operators and redirect to functions

        uint64_t *get() const;
        bool isInBounds() const;
        vector<uint8_t> toBytes() const;
        string toHex() const;

        /// <summary>
        /// Converts the binary value stored by the hex string
        /// to its big num representation stored as ElementModQ
        /// </summary>
        static unique_ptr<ElementModQ> fromHex(const string &representation);

        /// <summary>
        /// Converts an unsigned long integer value
        /// (that is no larger than an unsigned long int)
        /// to its big num representation stored as ElementModQ
        /// </summary>
        static unique_ptr<ElementModQ> fromUint64(uint64_t representation);

        unique_ptr<ElementModP> toElementModP() const;

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
    };

    // Common constants

    const ElementModP &G();
    const ElementModP &P();
    const ElementModP &ZERO_MOD_P();
    const ElementModP &ONE_MOD_P();
    const ElementModP &TWO_MOD_P();
    const ElementModQ &Q();
    const ElementModQ &ZERO_MOD_Q();
    const ElementModQ &ONE_MOD_Q();
    const ElementModQ &TWO_MOD_Q();

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod P
    /// </summary>
    EG_API unique_ptr<ElementModP> add_mod_p(const ElementModP &lhs, const ElementModP &rhs);

    /// <summary>
    /// Multplies together the left hand side and right hand side and returns the product mod P
    /// </summary>
    EG_API unique_ptr<ElementModP> mul_mod_p(const ElementModP &lhs, const ElementModP &rhs);

    using ElementModPOrQ = variant<ElementModP *, ElementModQ *>;

    EG_API unique_ptr<ElementModP> mul_mod_p(const vector<ElementModPOrQ> &elems);

    /// <summary>
    /// Computes b^e mod p.
    /// </summary>
    EG_API unique_ptr<ElementModP> pow_mod_p(const ElementModP &base, const ElementModP &exponent);

    /// <summary>
    /// Computes b^e mod p.
    /// </summary>
    EG_API unique_ptr<ElementModP> pow_mod_p(const ElementModP &base, const ElementModQ &exponent);

    /// <summary>
    /// Computes g^e mod p.
    /// </summary>
    EG_API unique_ptr<ElementModP> g_pow_p(const ElementModP &exponent);

    /// <summary>
    /// Computes g^e mod p.
    /// </summary>
    EG_API unique_ptr<ElementModP> g_pow_p(const ElementModQ &exponent);

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod Q
    /// </summary>
    EG_API unique_ptr<ElementModQ> add_mod_q(const ElementModQ &lhs, const ElementModQ &rhs);

    EG_API unique_ptr<ElementModQ>
    add_mod_q(const vector<reference_wrapper<ElementModQ>> &elements);

    /// <summary>
    /// Computes (a-b) mod q.
    /// </summary>
    EG_API unique_ptr<ElementModQ> sub_mod_q(const ElementModQ &a, const ElementModQ &b);

    /// <summary>
    /// Computes (a + b * c) mod q.
    /// </summary>
    EG_API unique_ptr<ElementModQ> a_plus_bc_mod_q(const ElementModQ &a, const ElementModQ &b,
                                                   const ElementModQ &c);

    /// <summary>
    /// Computes (Q - a) mod q.
    /// </summary>
    EG_API unique_ptr<ElementModQ> sub_from_q(const ElementModQ &a);

    /// <summary>
    /// Generate random number between 0 and Q
    /// </summary>
    EG_API unique_ptr<ElementModQ> rand_q();

} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_GROUP_HPP_INCLUDED__ */