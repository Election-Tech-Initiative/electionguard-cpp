#include "electionguard/group.hpp"

#include "../kremlin/Hacl_HMAC_DRBG.h"
#include "../kremlin/Lib_Memzero0.h"
#include "../kremlin/Lib_RandomBuffer_System.h"
#include "convert.hpp"
#include "facades/Hacl_Bignum256.hpp"
#include "facades/Hacl_Bignum4096.hpp"
#include "log.hpp"
#include "lookup_table.hpp"
#include "random.hpp"
#include "utils.hpp"

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <vector>

using hacl::Bignum256;
using hacl::Bignum4096;
using hacl::CONTEXT_P;
using hacl::CONTEXT_Q;
using std::copy;
using std::get;
using std::holds_alternative;
using std::invalid_argument;
using std::make_unique;
using std::out_of_range;
using std::overflow_error;
using std::reference_wrapper;
using std::runtime_error;
using std::to_string;
using std::unique_ptr;

namespace electionguard
{

#pragma region Constants

    const ElementModP &R()
    {
        static ElementModP instance{R_ARRAY_REVERSE, true};
        return instance;
    }

    const ElementModP &G()
    {
        static ElementModP instance{G_ARRAY_REVERSE, true, true};
        return instance;
    }

    const ElementModP &P()
    {
        static ElementModP instance{P_ARRAY_REVERSE, true};
        return instance;
    }

    const ElementModP &ZERO_MOD_P()
    {
        static ElementModP instance{ZERO_MOD_P_ARRAY, true};
        return instance;
    }

    const ElementModP &ONE_MOD_P()
    {
        static ElementModP instance{ONE_MOD_P_ARRAY, true};
        return instance;
    }

    const ElementModP &TWO_MOD_P()
    {
        static ElementModP instance{TWO_MOD_P_ARRAY, true};
        return instance;
    }

    const ElementModQ &Q()
    {
        static ElementModQ instance{Q_ARRAY_REVERSE, true};
        return instance;
    }

    const ElementModQ &ZERO_MOD_Q()
    {
        static ElementModQ instance{ZERO_MOD_Q_ARRAY, true};
        return instance;
    }

    const ElementModQ &ONE_MOD_Q()
    {

        static ElementModQ instance{ONE_MOD_Q_ARRAY, true};
        return instance;
    }

    const ElementModQ &TWO_MOD_Q()
    {
        static ElementModQ instance{TWO_MOD_Q_ARRAY, true};
        return instance;
    }

#pragma endregion

#pragma region ElementModP

    struct ElementModP::Impl {

        bool isFixedBase = false;
        uint64_t data[MAX_P_LEN] = {};
        string hexRepresentation;

        Impl(const vector<uint64_t> &elem, bool unchecked, bool fixedBase)
        {
            uint64_t array[MAX_P_LEN] = {};
            copy(elem.begin(), elem.end(), static_cast<uint64_t *>(array));
            if (!unchecked && Bignum4096::lessThan(const_cast<uint64_t *>(P().get()),
                                                   static_cast<uint64_t *>(array)) > 0) {
                throw out_of_range("Value for ElementModP is greater than allowed");
            }
            isFixedBase = fixedBase;
            copy(begin(array), end(array), begin(data));
        };

        Impl(const uint64_t (&elem)[MAX_P_LEN], bool unchecked, bool fixedBase)
        {
            if (!unchecked && Bignum4096::lessThan(const_cast<uint64_t *>(P().get()),
                                                   const_cast<uint64_t *>(elem)) > 0) {
                throw out_of_range("Value for ElementModP is greater than allowed");
            }
            isFixedBase = fixedBase;
            copy(begin(elem), end(elem), begin(data));
        };

        ~Impl() { Lib_Memzero0_memzero(static_cast<uint64_t *>(data), MAX_P_LEN); };

        [[nodiscard]] unique_ptr<ElementModP::Impl> clone() const
        {
            return make_unique<ElementModP::Impl>(data, true, isFixedBase);
        }

        bool operator==(const Impl &other)
        {
            for (uint8_t i = 0; i < MAX_P_LEN; i++) {
                auto l = data[i];
                auto r = other.data[i];
                if (l != r) {
                    return false;
                }
            }
            return true;
        }

        bool operator<(const Impl &other)
        {
            auto other_ = other;
            return Bignum4096::lessThan(static_cast<uint64_t *>(data),
                                        static_cast<uint64_t *>(other_.data)) > 0;
        }
    };

    // Lifecycle Methods

    ElementModP::ElementModP(const ElementModP &other) : pimpl(other.pimpl->clone()) {}

    ElementModP::ElementModP(const vector<uint64_t> &elem, bool unchecked /* = false */,
                             bool fixedBase /* = false */)
        : pimpl(new Impl(elem, unchecked, fixedBase))
    {
    }

    ElementModP::ElementModP(const uint64_t (&elem)[MAX_P_LEN], bool unchecked /* = false */,
                             bool fixedBase /* = false */)
        : pimpl(new Impl(elem, unchecked, fixedBase))
    {
    }

    ElementModP::~ElementModP() = default;

    // Operator Overloads

    ElementModP &ElementModP::operator=(ElementModP other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    bool ElementModP::operator==(const ElementModP &other) { return *pimpl == *other.pimpl; }

    bool ElementModP::operator!=(const ElementModP &other) { return !(*this == other); }

    bool ElementModP::operator<(const ElementModP &other) { return *pimpl < *other.pimpl; }

    // Property Getters

    uint64_t *ElementModP::get() const { return static_cast<uint64_t *>(pimpl->data); }

    uint64_t (&ElementModP::ref() const)[MAX_P_LEN] { return pimpl->data; }

    uint64_t ElementModP::length() const { return MAX_P_LEN; }

    bool ElementModP::isFixedBase() const { return pimpl->isFixedBase; }

    bool ElementModP::isInBounds() const
    {
        return (const_cast<ElementModP &>(ZERO_MOD_P()) < *this) &&
               (const_cast<ElementModP &>(*this) < P());
    }
    bool ElementModP::isValidResidue() const
    {
        auto residue = (*pow_mod_p(*this, Q()) == const_cast<ElementModP &>(ONE_MOD_P()));
        return this->isInBounds() && residue;
    }

    vector<uint8_t> ElementModP::toBytes() const
    {
        uint8_t byteResult[MAX_P_SIZE] = {};
        // Use Hacl to convert the bignum to byte array
        Bignum4096::toBytes(static_cast<uint64_t *>(pimpl->data),
                            static_cast<uint8_t *>(byteResult));
        return vector<uint8_t>(begin(byteResult), end(byteResult));
    }

    string ElementModP::toHex() const
    {
        if (!pimpl->hexRepresentation.empty()) {
            return pimpl->hexRepresentation;
        }

        // Returned bytes array from Hacl needs to be pre-allocated to 512 bytes
        uint8_t byteResult[MAX_P_SIZE] = {};
        // Use Hacl to convert the bignum to byte array
        Bignum4096::toBytes(static_cast<uint64_t *>(pimpl->data),
                            static_cast<uint8_t *>(byteResult));
        pimpl->hexRepresentation = bytes_to_hex(byteResult);
        return pimpl->hexRepresentation;
    }

    std::unique_ptr<ElementModP> ElementModP::clone() const
    {
        return make_unique<ElementModP>(pimpl->data, true, pimpl->isFixedBase);
    }

    void ElementModP::setIsFixedBase(bool fixedBase) const { pimpl->isFixedBase = fixedBase; }

    // Static Methods

    unique_ptr<ElementModP> ElementModP::fromHex(const string &representation,
                                                 bool unchecked /* = false */)
    {
        auto sanitized = sanitize_hex_string(representation);
        auto bytes = hex_to_bytes(sanitized);
        auto element = bytes_to_p(bytes, unchecked);
        release(bytes);
        return element;
    }

    unique_ptr<ElementModP> ElementModP::fromUint64(uint64_t representation,
                                                    bool unchecked /* = false */)
    {
        const size_t bitWidth = 8U;
        uint64_t bigEndian = htobe64(representation);
        return bytes_to_p(reinterpret_cast<uint8_t *>(&bigEndian), bitWidth, unchecked);
    }

#pragma endregion

#pragma region ElementModQ

    struct ElementModQ::Impl {

        uint64_t data[MAX_Q_LEN] = {};

        Impl(const vector<uint64_t> &elem, bool unchecked)
        {
            uint64_t array[MAX_Q_LEN] = {};
            copy(elem.begin(), elem.end(), static_cast<uint64_t *>(array));
            if (!unchecked && Bignum256::lessThan(const_cast<uint64_t *>(Q().get()),
                                                  static_cast<uint64_t *>(array)) > 0) {
                throw out_of_range("Value for ElementModQ is greater than allowed");
            }
            copy(begin(array), end(array), begin(data));
        };

        Impl(const uint64_t (&elem)[MAX_Q_LEN], bool unchecked)
        {
            if (!unchecked && Bignum256::lessThan(const_cast<uint64_t *>(Q().get()),
                                                  const_cast<uint64_t *>(elem)) > 0) {
                throw out_of_range("Value for ElementModQ is greater than allowed");
            }
            copy(begin(elem), end(elem), begin(data));
        };

        ~Impl() { Lib_Memzero0_memzero(static_cast<uint64_t *>(data), MAX_Q_LEN); };

        [[nodiscard]] unique_ptr<ElementModQ::Impl> clone() const
        {
            return make_unique<ElementModQ::Impl>(data, true);
        }

        bool operator==(const Impl &other)
        {
            for (uint8_t i = 0; i < MAX_Q_LEN; i++) {
                auto l = data[i];
                auto r = other.data[i];
                if (l != r) {
                    return false;
                }
            }
            return true;
        }

        bool operator<(const Impl &other)
        {
            auto other_ = other;
            return Bignum256::lessThan(static_cast<uint64_t *>(data),
                                       static_cast<uint64_t *>(other_.data)) > 0;
        }
    };

    // Lifecycle Methods

    ElementModQ::ElementModQ(const ElementModQ &other) : pimpl(other.pimpl->clone()) {}

    ElementModQ::ElementModQ(const vector<uint64_t> &elem, bool unchecked /* = false */)
        : pimpl(new Impl(elem, unchecked))
    {
    }

    ElementModQ::ElementModQ(const uint64_t (&elem)[MAX_Q_LEN], bool unchecked /* = false*/)
        : pimpl(new Impl(elem, unchecked))
    {
    }
    ElementModQ::~ElementModQ() = default;

    // Operator Overloads

    ElementModQ &ElementModQ::operator=(ElementModQ other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    bool ElementModQ::operator==(const ElementModQ &other) { return *pimpl == *other.pimpl; }

    bool ElementModQ::operator!=(const ElementModQ &other) { return !(*this == other); }

    bool ElementModQ::operator<(const ElementModQ &other) { return *pimpl < *other.pimpl; }

    // Property Getters

    uint64_t *ElementModQ::get() const { return static_cast<uint64_t *>(pimpl->data); }

    uint64_t (&ElementModQ::ref() const)[MAX_Q_LEN] { return pimpl->data; }

    uint64_t ElementModQ::length() const { return MAX_Q_LEN; }

    bool ElementModQ::isInBounds() const
    {
        return (const_cast<ElementModQ &>(ZERO_MOD_Q()) < *this) &&
               (const_cast<ElementModQ &>(*this) < Q());
    }

    vector<uint8_t> ElementModQ::toBytes() const
    {
        uint8_t byteResult[MAX_Q_SIZE] = {};
        // Use Hacl to convert the bignum to byte array
        Bignum256::toBytes(static_cast<uint64_t *>(pimpl->data),
                           static_cast<uint8_t *>(byteResult));
        return vector<uint8_t>(begin(byteResult), end(byteResult));
    }

    string ElementModQ::toHex() const
    {
        // Returned bytes array from Hacl needs to be pre-allocated to 32 bytes
        uint8_t byteResult[MAX_Q_SIZE] = {};
        // Use Hacl to convert the bignum to byte array
        Bignum256::toBytes(static_cast<uint64_t *>(pimpl->data),
                           static_cast<uint8_t *>(byteResult));
        return bytes_to_hex(byteResult);
    }

    // Static Methods

    unique_ptr<ElementModQ> ElementModQ::fromHex(const string &representation,
                                                 bool unchecked /* = false */)
    {
        auto sanitized = sanitize_hex_string(representation);
        auto bytes = hex_to_bytes(sanitized);
        auto element = bytes_to_q(bytes, unchecked);
        release(bytes);
        return element;
    }

    unique_ptr<ElementModQ> ElementModQ::fromUint64(uint64_t representation,
                                                    bool unchecked /* = false */)
    {
        const size_t bitWidth = 8U;
        uint64_t bigEndian = htobe64(representation);
        return bytes_to_q(reinterpret_cast<uint8_t *>(&bigEndian), bitWidth, unchecked);
    }

    // Public Methods

    unique_ptr<ElementModP> ElementModQ::toElementModP() const
    {
        uint64_t p4096[MAX_P_LEN] = {};
        memcpy(static_cast<uint64_t *>(p4096), static_cast<uint64_t *>(pimpl->data), MAX_Q_SIZE);
        return make_unique<ElementModP>(p4096, true);
    }

    std::unique_ptr<ElementModQ> ElementModQ::clone() const
    {
        return make_unique<ElementModQ>(pimpl->data);
    }

#pragma endregion

#pragma region Utility Helpers

    /// <summary>
    /// Converts the binary value stored as a byte array
    /// to its big num representation stored as ElementModP
    /// </summary>
    unique_ptr<electionguard::ElementModP> bytes_to_p(const uint8_t *bytes, size_t size,
                                                      bool unchecked /* = false */)
    {
        if (bytes == nullptr || size > MAX_P_SIZE) {
            throw out_of_range("Cannot convert ElementModP with size: " + to_string(size));
        }

        auto *bigNum = Bignum4096::fromBytes(size, const_cast<uint8_t *>(bytes));
        if (bigNum == nullptr) {
            throw bad_alloc();
        }

        // The ElementModP constructor expects the bignum
        // to be a certain size, but there's no guarantee
        // that constraint is satisfied by new_bn_from_bytes_be
        // so copy it into a new element that is the correct size
        // and free the allocated resources
        uint64_t element[MAX_P_LEN] = {};
        memcpy(static_cast<uint64_t *>(element), bigNum, size);
        free(bigNum);
        return make_unique<electionguard::ElementModP>(element, unchecked);
    }

    /// <summary>
    /// Converts the binary value stored as a byte array
    /// to its big num representation stored as ElementModP
    /// </summary>
    unique_ptr<electionguard::ElementModP> bytes_to_p(vector<uint8_t> bytes,
                                                      bool unchecked /* = false */)
    {
        auto *array = new uint8_t[bytes.size()];
        copy(bytes.begin(), bytes.end(), array);

        auto element = bytes_to_p(array, bytes.size(), unchecked);
        delete[] array;
        return element;
    }

    /// <summary>
    /// Converts the binary value stored as a big-endian byte array
    /// to its big num representation stored as ElementModQ
    /// </summary>
    unique_ptr<electionguard::ElementModQ> bytes_to_q(const uint8_t *bytes, size_t size,
                                                      bool unchecked /* = false */)
    {
        if (bytes == nullptr || size > MAX_Q_SIZE) {
            throw out_of_range("Cannot convert ElementModQ with size: " + to_string(size));
        }

        auto *bigNum = Bignum256::fromBytes(size, const_cast<uint8_t *>(bytes));
        if (bigNum == nullptr) {
            throw bad_alloc();
        }

        // The ElementModQ constructor expects the bignum
        // to be a certain size, but there's no guarantee
        // that constraint is satisfied by new_bn_from_bytes_be
        // so copy it into a new element that is the correct size
        // and free the allocated resources
        uint64_t element[MAX_Q_LEN] = {};
        memcpy(static_cast<uint64_t *>(element), bigNum, size);
        free(bigNum);
        return make_unique<electionguard::ElementModQ>(element, unchecked);
    }

    unique_ptr<electionguard::ElementModQ> bytes_to_q(vector<uint8_t> bytes,
                                                      bool unchecked /* = false */)
    {
        auto *array = new uint8_t[bytes.size()];
        copy(bytes.begin(), bytes.end(), array);

        auto element = bytes_to_q(array, bytes.size(), unchecked);
        delete[] array;
        return element;
    }

#pragma endregion

#pragma region ElementModP Global Functions

    unique_ptr<ElementModP> add_mod_p(const ElementModP &lhs, const ElementModP &rhs)
    {
        const auto &p = P();
        uint64_t addResult[MAX_P_LEN_DOUBLE] = {};
        uint64_t carry =
          Bignum4096::add(const_cast<ElementModP &>(lhs).get(),
                          const_cast<ElementModP &>(rhs).get(), static_cast<uint64_t *>(addResult));

        // handle the specific case where the the sum == MAX_4096
        // but the carry value is not set.  We still need to offset.
        if (carry > 0 || isMax(addResult)) {

            auto big_a = (const_cast<ElementModP &>(p) < lhs);
            auto big_b = (const_cast<ElementModP &>(p) < rhs);

            if (big_a || big_b) {

                uint64_t offset[MAX_P_LEN] = {};
                if (big_a) {
                    // TODO: move the casts into the interface
                    Bignum4096::add(static_cast<uint64_t *>(offset),
                                    const_cast<uint64_t *>(P_ARRAY_INVERSE_OFFSET),
                                    static_cast<uint64_t *>(offset));
                    Bignum4096::add(static_cast<uint64_t *>(offset),
                                    const_cast<uint64_t *>(ONE_MOD_P_ARRAY),
                                    static_cast<uint64_t *>(offset));
                }

                if (big_b) {
                    Bignum4096::add(static_cast<uint64_t *>(offset),
                                    const_cast<uint64_t *>(P_ARRAY_INVERSE_OFFSET),
                                    static_cast<uint64_t *>(offset));
                    Bignum4096::add(static_cast<uint64_t *>(offset),
                                    const_cast<uint64_t *>(ONE_MOD_P_ARRAY),
                                    static_cast<uint64_t *>(offset));
                }

                // adjust
                Bignum4096::add(static_cast<uint64_t *>(addResult), static_cast<uint64_t *>(offset),
                                static_cast<uint64_t *>(addResult));

            } else {
                Bignum4096::add(static_cast<uint64_t *>(addResult),
                                const_cast<uint64_t *>(Q_ARRAY_INVERSE_OFFSET),
                                static_cast<uint64_t *>(addResult));
            }
        }

        uint64_t modResult[MAX_P_LEN] = {};
        CONTEXT_P().mod(static_cast<uint64_t *>(addResult), static_cast<uint64_t *>(modResult));
        return make_unique<ElementModP>(modResult, true);
    }

    std::unique_ptr<ElementModP> mod_p(const ElementModP &element)
    {
        uint64_t modResult[MAX_P_LEN] = {};
        CONTEXT_P().mod(element.get(), static_cast<uint64_t *>(modResult));
        return make_unique<ElementModP>(modResult, true);
    }

    unique_ptr<ElementModP> mul_mod_p(const ElementModP &lhs, const ElementModP &rhs)
    {
        const auto &p = P();
        uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
        Bignum4096::mul(const_cast<ElementModP &>(lhs).get(), const_cast<ElementModP &>(rhs).get(),
                        static_cast<uint64_t *>(mulResult));
        uint64_t modResult[MAX_P_LEN] = {};
        CONTEXT_P().mod(static_cast<uint64_t *>(mulResult), static_cast<uint64_t *>(modResult));
        return make_unique<ElementModP>(modResult, true);
    }

    unique_ptr<ElementModP> mul_mod_p(const vector<ElementModPOrQ> &elems)
    {
        auto product = ElementModP::fromUint64(1UL);
        for (auto x : elems) {
            if (holds_alternative<ElementModQ *>(x)) {
                auto elem = get<ElementModQ *>(x)->toElementModP();
                auto res = mul_mod_p(*product, *elem);
                product.swap(res);
            } else if (holds_alternative<ElementModP *>(x)) {
                auto *elem = get<ElementModP *>(x);
                auto res = mul_mod_p(*product, *elem);
                product.swap(res);
            } else {
                throw "invalid type";
            }
        }
        return make_unique<ElementModP>(*product);
    }

    unique_ptr<ElementModP> pow_mod_p(const ElementModP &base, const ElementModP &exponent)
    {
        // HACL's input constraints require the exponent to be greater than zero
        if (const_cast<ElementModP &>(exponent) == ZERO_MOD_P()) {
            return ElementModP::fromUint64(1UL);
        }

        uint64_t result[MAX_P_LEN] = {};
        CONTEXT_P().modExp(base.get(), MAX_P_SIZE, exponent.get(), static_cast<uint64_t *>(result));
        return make_unique<ElementModP>(result, true);
    }

    unique_ptr<ElementModP> pow_mod_p(const ElementModP &base, const ElementModQ &exponent)
    {
        // HACL's input constraints require the exponent to be greater than zero
        if (const_cast<ElementModQ &>(exponent) == ZERO_MOD_Q()) {
            return ElementModP::fromUint64(1UL);
        }

        // check if we have a lookup table initialized for this element
        if (base.isFixedBase()) {
            // TODO: use a smaller key
            auto hex = base.toHex();
            auto exp_ptr = exponent.get();
            auto result = LookupTableContext::pow_mod_p(hex, base.ref(), exponent.ref());

            return make_unique<ElementModP>(result, true);
        }

        // if none exists, execute the modular exponentiation directly
        return pow_mod_p(base, *exponent.toElementModP());
    }

    unique_ptr<ElementModP> g_pow_p(const ElementModP &exponent)
    {
        return pow_mod_p(G(), exponent);
    }

    unique_ptr<ElementModP> g_pow_p(const ElementModQ &exponent)
    {
        return pow_mod_p(G(), exponent);
    }

#pragma endregion

#pragma region ElementModQ Global Functions

    unique_ptr<ElementModQ> add_mod_q(const ElementModQ &lhs, const ElementModQ &rhs)
    {
        const auto &q = Q();
        uint64_t addResult[MAX_Q_LEN_DOUBLE] = {};
        uint64_t carry =
          Bignum256::add(const_cast<ElementModQ &>(lhs).get(), const_cast<ElementModQ &>(rhs).get(),
                         static_cast<uint64_t *>(addResult));

        // handle the specific case where the the sum == MAX_256
        // but the carry value is not set.  We still need to offset.
        if (carry > 0 || isMax(addResult)) {

            auto big_a = (const_cast<ElementModQ &>(q) < lhs);
            auto big_b = (const_cast<ElementModQ &>(q) < rhs);

            if (big_a || big_b) {

                uint64_t offset[MAX_Q_LEN] = {};
                if (big_a) {
                    // TODO: move the casts into the interface
                    Bignum256::add(static_cast<uint64_t *>(offset),
                                   const_cast<uint64_t *>(Q_ARRAY_INVERSE_OFFSET),
                                   static_cast<uint64_t *>(offset));
                    Bignum256::add(static_cast<uint64_t *>(offset),
                                   const_cast<uint64_t *>(ONE_MOD_Q_ARRAY),
                                   static_cast<uint64_t *>(offset));
                }

                if (big_b) {
                    Bignum256::add(static_cast<uint64_t *>(offset),
                                   const_cast<uint64_t *>(Q_ARRAY_INVERSE_OFFSET),
                                   static_cast<uint64_t *>(offset));
                    Bignum256::add(static_cast<uint64_t *>(offset),
                                   const_cast<uint64_t *>(ONE_MOD_Q_ARRAY),
                                   static_cast<uint64_t *>(offset));
                }

                // adjust
                Bignum256::add(static_cast<uint64_t *>(addResult), static_cast<uint64_t *>(offset),
                               static_cast<uint64_t *>(addResult));

            } else {
                Bignum256::add(static_cast<uint64_t *>(addResult),
                               const_cast<uint64_t *>(Q_ARRAY_INVERSE_OFFSET),
                               static_cast<uint64_t *>(addResult));
            }
        }

        uint64_t result[MAX_Q_LEN] = {};
        CONTEXT_Q().mod(static_cast<uint64_t *>(addResult), static_cast<uint64_t *>(result));
        return make_unique<ElementModQ>(result, true);
    }

    unique_ptr<ElementModQ> add_mod_q(const vector<reference_wrapper<ElementModQ>> &elements)
    {
        if (elements.empty()) {
            throw invalid_argument("must have one or more elements");
        }

        auto result = ElementModQ::fromUint64(0UL, true);
        for (auto element : elements) {
            auto sum = add_mod_q(*result, element.get());
            result.swap(sum);
        }
        return result;
    }

    unique_ptr<ElementModQ> sub_mod_q(const ElementModQ &a, const ElementModQ &b)
    {
        const auto &q = Q();
        uint64_t subResult[MAX_Q_LEN_DOUBLE] = {};
        uint64_t carry =
          Bignum256::sub(const_cast<ElementModQ &>(a).get(), const_cast<ElementModQ &>(b).get(),
                         static_cast<uint64_t *>(subResult));

        if (carry > 0) {

            auto big_a = (const_cast<ElementModQ &>(q) < a);
            auto big_b = (const_cast<ElementModQ &>(q) < b);

            if (big_a || big_b) {

                // TODO: ISSUE #135: precompute

                uint64_t offset[MAX_Q_LEN] = {};

                Bignum256::add(static_cast<uint64_t *>(offset),
                               const_cast<uint64_t *>(Q_ARRAY_INVERSE_OFFSET),
                               static_cast<uint64_t *>(offset));
                Bignum256::add(static_cast<uint64_t *>(offset),
                               const_cast<uint64_t *>(ONE_MOD_Q_ARRAY),
                               static_cast<uint64_t *>(offset));

                // adjust
                Bignum256::sub(static_cast<uint64_t *>(subResult), static_cast<uint64_t *>(offset),
                               static_cast<uint64_t *>(subResult));

            } else {
                // if there is no carry then offset by the inverse
                Bignum256::sub(static_cast<uint64_t *>(subResult),
                               const_cast<uint64_t *>(Q_ARRAY_INVERSE_OFFSET),
                               static_cast<uint64_t *>(subResult));
            }
        }

        uint64_t resModQ[MAX_Q_LEN] = {};
        CONTEXT_Q().mod(static_cast<uint64_t *>(subResult), static_cast<uint64_t *>(resModQ));

        return make_unique<ElementModQ>(resModQ, true);
    }

    unique_ptr<ElementModQ> a_plus_bc_mod_q(const ElementModQ &a, const ElementModQ &b,
                                            const ElementModQ &c)
    {
        // since the result of b*c can be larger than Q's 256,
        // use P's length and 4096 Hacl for rest of calculation

        uint64_t resultBC[MAX_P_LEN] = {}; // init to MAX_P_LEN
        Bignum256::mul(const_cast<ElementModQ &>(b).get(), const_cast<ElementModQ &>(c).get(),
                       static_cast<uint64_t *>(resultBC));

        auto bc_as_p = make_unique<ElementModP>(resultBC, true);
        auto a_as_p = a.toElementModP();

        uint64_t a_plus_bc_result[MAX_P_LEN_DOUBLE] = {};
        uint64_t carry =
          Bignum4096::add(a_as_p->get(), bc_as_p->get(), static_cast<uint64_t *>(a_plus_bc_result));

        // we should never overflow P space since our max size
        // is resultBC[MAX_Q_LEN_DOUBLE] + a[MAX_Q_LEN]
        if (carry > 0) {
            throw overflow_error("a_plus_bc_mod_q add operation out of bounds");
        }

        const auto &q = Q();
        auto q_as_p = q.toElementModP();
        uint64_t resModQ[MAX_P_LEN] = {};
        bool modSuccess = Bignum4096::mod(q_as_p->get(), static_cast<uint64_t *>(a_plus_bc_result),
                                          static_cast<uint64_t *>(resModQ));
        if (!modSuccess) {
            throw runtime_error("a_plus_bc_mod_q mod operation failed");
        }
        uint64_t result[MAX_Q_LEN] = {};
        memcpy(static_cast<uint64_t *>(result), resModQ, MAX_Q_SIZE);

        return make_unique<ElementModQ>(result, true);
    }

    unique_ptr<ElementModQ> sub_from_q(const ElementModQ &a)
    {
        uint64_t result[MAX_Q_LEN] = {};
        Bignum256::sub(const_cast<ElementModQ &>(Q()).get(), const_cast<ElementModQ &>(a).get(),
                       static_cast<uint64_t *>(result));
        // TODO: python version doesn't perform % Q on results,
        // but we still need to handle the overflow values between (Q, MAX_256]
        return make_unique<ElementModQ>(result, true);
    }

    unique_ptr<ElementModP> rand_p()
    {
        auto bytes = Random::getBytes();

        auto *bigNum = Bignum4096::fromBytes(MAX_P_SIZE, const_cast<uint8_t *>(bytes.data()));
        if (bigNum == nullptr) {
            throw bad_alloc();
        }

        uint64_t element[MAX_P_LEN] = {0};
        memcpy(static_cast<uint64_t *>(element), bigNum, MAX_P_SIZE);
        free(bigNum);

        auto random_p = make_unique<ElementModP>(element, true);
        return add_mod_p(*random_p, ZERO_MOD_P());
    }

    unique_ptr<ElementModQ> rand_q()
    {
        auto bytes = Random::getBytes();

        auto *bigNum = Bignum256::fromBytes(MAX_Q_SIZE, const_cast<uint8_t *>(bytes.data()));
        if (bigNum == nullptr) {
            throw bad_alloc();
        }

        // first index of Q cannot cannod exceed 0xFFFFFFFFFFFFFF43
        bigNum[0] = bigNum[0] & 0xFFFFFFFFFFFFFF43;

        uint64_t element[MAX_Q_LEN] = {0};
        memcpy(static_cast<uint64_t *>(element), bigNum, MAX_Q_SIZE);
        free(bigNum);

        auto random_q = make_unique<ElementModQ>(element, true);
        return add_mod_q(*random_q, ZERO_MOD_Q());
    }

#pragma endregion

    string vector_uint8_t_to_hex(const vector<uint8_t> &bytes) { return bytes_to_hex(bytes); }

} // namespace electionguard
