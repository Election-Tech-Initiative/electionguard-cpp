#ifndef __ELECTIONGUARD_CPP_GROUP_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_GROUP_HPP_INCLUDED__

#include "constants.h"
#include "export.h"

#include <cstdint>
#include <memory>
#include <string>
#include <variant>
#include <vector>

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
        ElementModP(const std::vector<uint64_t> &elem, bool unchecked = false,
                    bool fixedBase = false);
        ElementModP(const uint64_t (&elem)[MAX_P_LEN], bool unchecked = false,
                    bool fixedBase = false);
        ~ElementModP();

        ElementModP &operator=(ElementModP other);
        ElementModP &operator=(ElementModP &&other);
        bool operator==(const ElementModP &other);
        bool operator!=(const ElementModP &other);

        bool operator<(const ElementModP &other);

        // TODO: ISSUE #130: math operators

        /// <Summary>
        /// Get the integer representation of the element
        /// Note the Element is stored in HACL format
        /// <returns> a pointer to the first limb. </returns>
        /// </Summary>
        uint64_t *get() const;

        uint64_t length() const;

        bool isFixedBase() const;

        /// <Summary>
        /// Validates that the element is actually within the bounds of [0,P).
        /// </Summary>
        bool isInBounds() const;

        /// <Summary>
        /// Validates that this element is in Z^r_p.
        /// </Summary>
        bool isValidResidue() const;

        /// <Summary>
        /// exports a bytes representation of the integer value in Big Endian format
        /// </Summary>
        std::vector<uint8_t> toBytes() const;

        /// <Summary>
        /// exports a hex representation of the integer value in Big Endian format
        /// </Summary>
        std::string toHex() const;

        /// <Summary>
        /// clone the object by making a deep copy
        /// </Summary>
        std::unique_ptr<ElementModP> clone() const;

        void setIsFixedBase(bool fixedBase) const;

        /// <summary>
        /// Converts the binary value stored by the hex string in Big Endian format
        /// to its big num representation stored as ElementModP
        /// </summary>
        static std::unique_ptr<ElementModP> fromHex(const std::string &representation,
                                                    bool unchecked = false);

        /// <summary>
        /// Converts an unsigned long integer value
        /// (that is no larger than an unsigned long int)
        /// to its big num representation stored as ElementModP
        /// </summary>
        static std::unique_ptr<ElementModP> fromUint64(uint64_t representation,
                                                       bool unchecked = false);

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// An element of the smaller `mod q` space, i.e., in [0, Q), where Q is a 256-bit prime.
    /// </summary>
    class EG_API ElementModQ
    {
      public:
        ElementModQ(const ElementModQ &other);
        ElementModQ(ElementModQ &&other);
        ElementModQ(const std::vector<uint64_t> &elem, bool unchecked = false);
        ElementModQ(const uint64_t (&elem)[MAX_Q_LEN], bool unchecked = false);
        ~ElementModQ();

        ElementModQ &operator=(ElementModQ other);
        ElementModQ &operator=(ElementModQ &&other);
        bool operator==(const ElementModQ &other);
        bool operator!=(const ElementModQ &other);

        bool operator<(const ElementModQ &other);

        // TODO: ISSUE #130: overload math operators and redirect to functions

        /// <Summary>
        /// Get the integer representation of the element
        /// Note the Element is stored in HACL format
        /// <returns> a pointer to the first limb</returns>
        /// </Summary>
        uint64_t *get() const;

        uint64_t length() const;

        /// <Summary>
        /// Validates that the element is actually within the bounds of [0,Q).
        /// </Summary>
        bool isInBounds() const;

        /// <Summary>
        /// exports a bytes representation of the integer value in Big Endian format
        /// </Summary>
        std::vector<uint8_t> toBytes() const;

        /// <Summary>
        /// exports a hex representation of the integer value in Big Endian format
        /// </Summary>
        std::string toHex() const;

        /// <Summary>
        /// clone the object by making a deep copy
        /// </Summary>
        std::unique_ptr<ElementModQ> clone() const;

        /// <summary>
        /// Converts the binary value stored by the hex string
        /// to its big num representation stored as ElementModQ
        /// </summary>
        static std::unique_ptr<ElementModQ> fromHex(const std::string &representation,
                                                    bool unchecked = false);

        /// <summary>
        /// Converts an unsigned long integer value
        /// (that is no larger than an unsigned long int)
        /// to its big num representation stored as ElementModQ
        /// </summary>
        static std::unique_ptr<ElementModQ> fromUint64(uint64_t representation,
                                                       bool unchecked = false);

        /// <Summary>
        /// create a copy of the Element in ElementModP space
        /// </Summary>
        std::unique_ptr<ElementModP> toElementModP() const;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    // Common constants

    EG_API const ElementModP &R();
    EG_API const ElementModP &G();
    EG_API const ElementModP &P();
    EG_API const ElementModP &ZERO_MOD_P();
    EG_API const ElementModP &ONE_MOD_P();
    EG_API const ElementModP &TWO_MOD_P();
    EG_API const ElementModQ &Q();
    EG_API const ElementModQ &ZERO_MOD_Q();
    EG_API const ElementModQ &ONE_MOD_Q();
    EG_API const ElementModQ &TWO_MOD_Q();

    // utility helpers

    EG_API std::unique_ptr<electionguard::ElementModP> bytes_to_p(const uint8_t *bytes, size_t size,
                                                                  bool unchecked = false);

    EG_API std::unique_ptr<electionguard::ElementModP> bytes_to_p(std::vector<uint8_t> bytes,
                                                                  bool unchecked = false);

    EG_API std::unique_ptr<electionguard::ElementModQ> bytes_to_q(const uint8_t *bytes, size_t size,
                                                                  bool unchecked = false);

    EG_API std::unique_ptr<electionguard::ElementModQ> bytes_to_q(std::vector<uint8_t> bytes,
                                                                  bool unchecked = false);

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod P
    /// </summary>
    EG_API std::unique_ptr<ElementModP> add_mod_p(const ElementModP &lhs, const ElementModP &rhs);

    /// <summary>
    /// Multplies together the left hand side and right hand side and returns the product mod P
    /// </summary>
    EG_API std::unique_ptr<ElementModP> mul_mod_p(const ElementModP &lhs, const ElementModP &rhs);

    using ElementModPOrQ = std::variant<ElementModP *, ElementModQ *>;

    /// <summary>
    /// Multplies together the collection and returns the product mod P
    /// </summary>
    EG_API std::unique_ptr<ElementModP> mul_mod_p(const std::vector<ElementModPOrQ> &elems);

    /// <summary>
    /// computes element mod p
    /// </summary>
    EG_API std::unique_ptr<ElementModP> mod_p(const ElementModP &element);

    /// <summary>
    /// Computes b^e mod p.
    /// </summary>
    EG_API std::unique_ptr<ElementModP> pow_mod_p(const ElementModP &base,
                                                  const ElementModP &exponent);

    /// <summary>
    /// Computes b^e mod p.
    /// </summary>
    EG_API std::unique_ptr<ElementModP> pow_mod_p(const ElementModP &base,
                                                  const ElementModQ &exponent);

    /// <summary>
    /// Computes g^e mod p.
    /// </summary>
    EG_API std::unique_ptr<ElementModP> g_pow_p(const ElementModP &exponent);

    /// <summary>
    /// Computes g^e mod p.
    /// </summary>
    EG_API std::unique_ptr<ElementModP> g_pow_p(const ElementModQ &exponent);

    /// <summary>
    /// Adds together the left hand side and right hand side and returns the sum mod Q
    /// </summary>
    EG_API std::unique_ptr<ElementModQ> add_mod_q(const ElementModQ &lhs, const ElementModQ &rhs);

    /// <summary>
    /// Adds together the collection and returns the sum mod Q
    /// </summary>
    EG_API std::unique_ptr<ElementModQ>
    add_mod_q(const std::vector<std::reference_wrapper<ElementModQ>> &elements);

    /// <summary>
    /// Computes (a-b) mod q.
    /// </summary>
    EG_API std::unique_ptr<ElementModQ> sub_mod_q(const ElementModQ &a, const ElementModQ &b);

    /// <summary>
    /// Computes (a + b * c) mod q.
    /// </summary>
    EG_API std::unique_ptr<ElementModQ> a_plus_bc_mod_q(const ElementModQ &a, const ElementModQ &b,
                                                        const ElementModQ &c);

    /// <summary>
    /// Computes (Q - a) mod q.
    /// </summary>
    EG_API std::unique_ptr<ElementModQ> sub_from_q(const ElementModQ &a);

    /// <summary>
    /// Generate random number between 0 and P
    /// </summary>
    EG_API std::unique_ptr<ElementModP> rand_p();

    /// <summary>
    /// Generate random number between 0 and Q
    /// </summary>
    EG_API std::unique_ptr<ElementModQ> rand_q();

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_GROUP_HPP_INCLUDED__ */
