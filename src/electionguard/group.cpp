#include "electionguard/group.hpp"

#include "../kremlin/Hacl_Bignum256.h"
#include "../kremlin/Hacl_Bignum4096.h"
#include "../kremlin/Lib_Memzero0.h"
#include "log.hpp"
#include "serialize.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <vector>

#pragma region UtilityHelpers

/// <summary>
/// Converts the binary value stored as a byte array
/// to its big num representation stored as ElementModP
/// </summary>
unique_ptr<electionguard::ElementModP> bytes_to_p(uint8_t *bytes, size_t size)
{
    // TODO: validate inputs are within range
    auto *bigNum = Hacl_Bignum4096_new_bn_from_bytes_be(size, bytes);
    if (bigNum == nullptr) {
        throw out_of_range("bytes_to_p could not allocate");
    }

    // The ElementModP constructor expects the bignum
    // to be a certain size, but there's no guarantee
    // that constraint is satisfied by new_bn_from_bytes_be
    // so copy it into a new element that is the correct size
    // and free the allocated resources
    uint64_t element[MAX_P_LEN] = {};
    memcpy(static_cast<uint64_t *>(element), bigNum, size);
    free(bigNum);
    return make_unique<electionguard::ElementModP>(element);
}

/// <summary>
/// Converts the binary value stored as a byte array
/// to its big num representation stored as ElementModP
/// </summary>
unique_ptr<electionguard::ElementModP> bytes_to_p(vector<uint8_t> bytes)
{
    auto *array = new uint8_t[bytes.size()];
    copy(bytes.begin(), bytes.end(), array);

    auto element = bytes_to_p(array, bytes.size());
    delete[] array;
    return element;
}

/// <summary>
/// Converts the binary value stored as a big-endian byte array
/// to its big num representation stored as ElementModQ
/// </summary>
unique_ptr<electionguard::ElementModQ> bytes_to_q(const uint8_t *bytes, size_t size)
{
    // TODO: validate inputs are within range
    auto *bigNum = Hacl_Bignum256_new_bn_from_bytes_be(size, const_cast<uint8_t *>(bytes));
    if (bigNum == nullptr) {
        throw out_of_range("bytes_to_q could not allocate");
    }

    // The ElementModQ constructor expects the bignum
    // to be a certain size, but there's no guarantee
    // that constraint is satisfied by new_bn_from_bytes_be
    // so copy it into a new element that is the correct size
    // and free the allocated resources
    uint64_t element[MAX_Q_LEN] = {};
    memcpy(static_cast<uint64_t *>(element), bigNum, size);
    free(bigNum);
    return make_unique<electionguard::ElementModQ>(element);
}

unique_ptr<electionguard::ElementModQ> bytes_to_q(vector<uint8_t> bytes)
{
    auto *array = new uint8_t[bytes.size()];
    copy(bytes.begin(), bytes.end(), array);

    auto element = bytes_to_q(array, bytes.size());
    delete[] array;
    return element;
}

bool isMax(const uint64_t (&array)[MAX_Q_LEN_DOUBLE])
{
    const uint64_t max = 0xffffffffffffffff;
    for (uint32_t i = 0; i < MAX_Q_LEN; i++) {
        if (array[i] != max) {
            return false;
        }
    }
    return true;
}

bool isMax(const uint64_t (&array)[MAX_P_LEN_DOUBLE])
{
    const uint64_t max = 0xffffffffffffffff;
    for (uint32_t i = 0; i < MAX_P_LEN; i++) {
        if (array[i] != max) {
            return false;
        }
    }
    return true;
}

#pragma endregion

namespace electionguard
{

#pragma region Constants

    const ElementModP &G()
    {
        static ElementModP instance{G_ARRAY_REVERSE, true};
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

        Impl(const vector<uint64_t> &elem, bool unchecked)
        {
            uint64_t array[MAX_P_LEN] = {};
            copy(elem.begin(), elem.end(), static_cast<uint64_t *>(array));
            if (!unchecked && Hacl_Bignum4096_lt_mask(const_cast<uint64_t *>(P_ARRAY_REVERSE),
                                                      static_cast<uint64_t *>(array)) > 0) {
                throw out_of_range("Value for ElementModP is greater than allowed");
            }
            memcpy(static_cast<uint64_t *>(data), static_cast<uint64_t *>(array), MAX_P_SIZE);
        };

        Impl(const uint64_t (&elem)[MAX_P_LEN], bool unchecked)
        {
            if (!unchecked && Hacl_Bignum4096_lt_mask(const_cast<uint64_t *>(P_ARRAY_REVERSE),
                                                      const_cast<uint64_t *>(elem)) > 0) {
                throw out_of_range("Value for ElementModP is greater than allowed");
            }
            memcpy(static_cast<uint64_t *>(data), const_cast<uint64_t *>(elem), MAX_P_SIZE);
        };

        ~Impl() { Lib_Memzero0_memzero(static_cast<uint64_t *>(data), MAX_P_LEN); };

        bool operator==(const Impl &other)
        {
            // TODO: safety, specifically when the object underflows its max size
            // e.g. if ((l == (uint64_t)0U) && (r == (uint64_t)0U))
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
            return Hacl_Bignum4096_lt_mask(static_cast<uint64_t *>(data),
                                           static_cast<uint64_t *>(other_.data)) > 0;
        }

        uint64_t data[MAX_P_LEN] = {};
    };

    // Lifecycle Methods

    ElementModP::ElementModP(const ElementModP &other) : pimpl(new Impl(*other.pimpl)) {}

    ElementModP::ElementModP(const vector<uint64_t> &elem, bool unchecked /* = false */)
        : pimpl(new Impl(elem, unchecked))
    {
    }

    ElementModP::ElementModP(const uint64_t (&elem)[MAX_P_LEN], bool unchecked /* = false */)
        : pimpl(new Impl(elem, unchecked))
    {
    }

    ElementModP::~ElementModP() = default;

    // Property Getters

    uint64_t *ElementModP::get() const { return static_cast<uint64_t *>(pimpl->data); }

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

    // Methods

    string ElementModP::toHex() const
    {
        // Returned bytes array from Hacl needs to be pre-allocated to 512 bytes
        uint8_t byteResult[MAX_P_SIZE] = {};
        // Use Hacl to convert the bignum to byte array
        Hacl_Bignum4096_bn_to_bytes_be(static_cast<uint64_t *>(pimpl->data),
                                       static_cast<uint8_t *>(byteResult));
        return bytes_to_hex(byteResult);
    }

    unique_ptr<ElementModP> ElementModP::fromHex(const string &representation)
    {
        auto sanitized = sanitize_hex_string(representation);
        auto bytes = hex_to_bytes(sanitized);
        auto element = bytes_to_p(bytes);
        release(bytes);
        return element;
    }

    unique_ptr<ElementModP> ElementModP::fromUint64(uint64_t representation)
    {
        const size_t bitWidth = 8U;
        uint64_t bigEndian = htobe64(representation);
        return bytes_to_p(reinterpret_cast<uint8_t *>(&bigEndian), bitWidth);
    }

    // Operator Overloads

    ElementModP &ElementModP::operator=(ElementModP other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    bool ElementModP::operator==(const ElementModP &other) { return *pimpl == *other.pimpl; }

    bool ElementModP::operator!=(const ElementModP &other) { return !(*this == other); }

    bool ElementModP::operator<(const ElementModP &other) { return *pimpl < *other.pimpl; }

#pragma endregion

#pragma region ElementModQ

    struct ElementModQ::Impl {

        Impl(const vector<uint64_t> &elem, bool unchecked)
        {
            uint64_t array[MAX_Q_LEN] = {};
            copy(elem.begin(), elem.end(), static_cast<uint64_t *>(array));
            if (!unchecked && Hacl_Bignum4096_lt_mask(const_cast<uint64_t *>(Q_ARRAY_REVERSE),
                                                      static_cast<uint64_t *>(array)) > 0) {
                throw out_of_range("Value for ElementModQ is greater than allowed");
            }
            memcpy(static_cast<uint64_t *>(data), static_cast<uint64_t *>(array), MAX_Q_SIZE);
        };

        Impl(const uint64_t (&elem)[MAX_Q_LEN], bool unchecked)
        {
            if (!unchecked && Hacl_Bignum256_lt_mask(const_cast<uint64_t *>(Q_ARRAY_REVERSE),
                                                     const_cast<uint64_t *>(elem)) > 0) {
                throw out_of_range("Value for ElementModQ is greater than allowed");
            }
            memcpy(static_cast<uint64_t *>(data), const_cast<uint64_t *>(elem), MAX_Q_SIZE);
        };

        ~Impl() { Lib_Memzero0_memzero(static_cast<uint64_t *>(data), MAX_Q_LEN); };

        bool operator==(const Impl &other)
        {
            // TODO: safety, specifically when the object underflows its max size
            // e.g. if ((l == (uint64_t)0U) && (r == (uint64_t)0U))
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
            return Hacl_Bignum256_lt_mask(static_cast<uint64_t *>(data),
                                          static_cast<uint64_t *>(other_.data)) > 0;
        }

        uint64_t data[MAX_Q_LEN] = {};
    };

    // Lifecycle Methods

    ElementModQ::ElementModQ(const ElementModQ &other) : pimpl(new Impl(*other.pimpl)) {}

    ElementModQ::ElementModQ(const vector<uint64_t> &elem, bool unchecked /* = false */)
        : pimpl(new Impl(elem, unchecked))
    {
    }

    ElementModQ::ElementModQ(const uint64_t (&elem)[MAX_Q_LEN], bool unchecked /* = false*/)
        : pimpl(new Impl(elem, unchecked))
    {
    }
    ElementModQ::~ElementModQ() = default;

    // Property Getters

    uint64_t *ElementModQ::get() const { return static_cast<uint64_t *>(pimpl->data); }

    // Public Methods

    bool ElementModQ::isInBounds() const
    {
        return (const_cast<ElementModQ &>(ZERO_MOD_Q()) < *this) &&
               (const_cast<ElementModQ &>(*this) < Q());
    }

    string ElementModQ::toHex() const
    {
        // Returned bytes array from Hacl needs to be pre-allocated to 32 bytes
        uint8_t byteResult[MAX_Q_SIZE] = {};
        // Use Hacl to convert the bignum to byte array
        Hacl_Bignum256_bn_to_bytes_be(static_cast<uint64_t *>(pimpl->data),
                                      static_cast<uint8_t *>(byteResult));
        return bytes_to_hex(byteResult);
    }

    unique_ptr<ElementModP> ElementModQ::toElementModP() const
    {
        uint64_t p4096[MAX_P_LEN] = {};
        memcpy(static_cast<uint64_t *>(p4096), static_cast<uint64_t *>(pimpl->data), MAX_Q_SIZE);
        return make_unique<ElementModP>(p4096, true);
    }

    unique_ptr<ElementModQ> ElementModQ::fromHex(const string &representation)
    {
        auto sanitized = sanitize_hex_string(representation);
        auto bytes = hex_to_bytes(sanitized);
        auto element = bytes_to_q(bytes);
        release(bytes);
        return element;
    }

    unique_ptr<ElementModQ> ElementModQ::fromUint64(uint64_t representation)
    {
        const size_t bitWidth = 8U;
        uint64_t bigEndian = htobe64(representation);
        return bytes_to_q(reinterpret_cast<uint8_t *>(&bigEndian), bitWidth);
    }

    // Operator Overloads

    ElementModQ &ElementModQ::operator=(ElementModQ other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    bool ElementModQ::operator==(const ElementModQ &other) { return *pimpl == *other.pimpl; }

    bool ElementModQ::operator!=(const ElementModQ &other) { return !(*this == other); }

    bool ElementModQ::operator<(const ElementModQ &other) { return *pimpl < *other.pimpl; }

#pragma endregion

#pragma region ElementModP Global Functions

    unique_ptr<ElementModP> add_mod_p(const ElementModP &lhs, const ElementModP &rhs)
    {
        const auto &p = P();
        uint64_t result[MAX_P_LEN_DOUBLE] = {};
        uint64_t carry = Hacl_Bignum4096_add(const_cast<ElementModP &>(lhs).get(),
                                             const_cast<ElementModP &>(rhs).get(),
                                             static_cast<uint64_t *>(result));

        // handle the specific case where the the sum == MAX_4096
        // but the carry value is not set.  We still need to offset.
        if (carry > 0 || isMax(result)) {

            // TODO: handle carry like in ElementModQ
        }

        uint64_t modResult[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod(p.get(), static_cast<uint64_t *>(result),
                                           static_cast<uint64_t *>(modResult));
        if (!success) {
            throw runtime_error(" add_mod_p mod operation failed");
        }
        return make_unique<ElementModP>(modResult);
    }

    unique_ptr<ElementModP> mul_mod_p(const ElementModP &lhs, const ElementModP &rhs)
    {
        const auto &p = P();
        uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
        Hacl_Bignum4096_mul(const_cast<ElementModP &>(lhs).get(),
                            const_cast<ElementModP &>(rhs).get(),
                            static_cast<uint64_t *>(mulResult));
        uint64_t modResult[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod(p.get(), static_cast<uint64_t *>(mulResult),
                                           static_cast<uint64_t *>(modResult));
        if (!success) {
            throw runtime_error(" mul_mod_p mod operation failed");
        }
        return make_unique<ElementModP>(modResult);
    }

    unique_ptr<ElementModP> mul_mod_p(const vector<ElementModPOrQ> &elems)
    {
        auto product = ElementModP::fromUint64(1);
        for (auto x : elems) {
            ElementModP *elem = nullptr;
            if (holds_alternative<ElementModQ *>(x)) {
                auto elem_as_p = get<ElementModQ *>(x)->toElementModP();
                elem = elem_as_p.get();
            } else if (holds_alternative<ElementModP *>(x)) {
                elem = get<ElementModP *>(x);
            } else {
                throw "invalid type";
            }

            auto res = mul_mod_p(*product, *elem);
            product = move(res);
        }
        return make_unique<ElementModP>(*product);
    }

    unique_ptr<ElementModP> pow_mod_p(const ElementModP &base, const ElementModP &exponent)
    {
        const auto &p = P();

        // HACL's input constraints require the exponent to be greater than zero
        if (const_cast<ElementModP &>(exponent) == ZERO_MOD_P()) {
            return ElementModP::fromUint64(1);
        }

        uint64_t result[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod_exp(p.get(), base.get(), MAX_P_SIZE, exponent.get(),
                                               static_cast<uint64_t *>(result));
        if (!success) {
            throw runtime_error(" pow_mod_p mod operation failed");
        }
        return make_unique<ElementModP>(result, true);
    }

    unique_ptr<ElementModP> pow_mod_p(const ElementModP &base, const ElementModQ &exponent)
    {
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
        uint64_t carry = Hacl_Bignum256_add(const_cast<ElementModQ &>(lhs).get(),
                                            const_cast<ElementModQ &>(rhs).get(),
                                            static_cast<uint64_t *>(addResult));

        // handle the specific case where the the sum == MAX_256
        // but the carry value is not set.  We still need to offset.
        if (carry > 0 || isMax(addResult)) {

            auto big_a = (const_cast<ElementModQ &>(q) < lhs);
            auto big_b = (const_cast<ElementModQ &>(q) < rhs);

            if (big_a || big_b) {

                uint64_t offset[MAX_Q_LEN] = {};
                // TODO: precompute?
                if (big_a) {
                    Hacl_Bignum256_add(static_cast<uint64_t *>(offset),
                                       const_cast<uint64_t *>(Q_ARRAY_INVERSE_REVERSE),
                                       static_cast<uint64_t *>(offset));
                    Hacl_Bignum256_add(static_cast<uint64_t *>(offset),
                                       const_cast<uint64_t *>(ONE_MOD_Q_ARRAY),
                                       static_cast<uint64_t *>(offset));
                }

                if (big_b) {
                    Hacl_Bignum256_add(static_cast<uint64_t *>(offset),
                                       const_cast<uint64_t *>(Q_ARRAY_INVERSE_REVERSE),
                                       static_cast<uint64_t *>(offset));
                    Hacl_Bignum256_add(static_cast<uint64_t *>(offset),
                                       const_cast<uint64_t *>(ONE_MOD_Q_ARRAY),
                                       static_cast<uint64_t *>(offset));
                }

                // adjust
                Hacl_Bignum256_add(static_cast<uint64_t *>(addResult),
                                   static_cast<uint64_t *>(offset),
                                   static_cast<uint64_t *>(addResult));

            } else {
                const uint64_t offset[MAX_Q_LEN] = {0x00000000000000bd};
                Hacl_Bignum256_add(static_cast<uint64_t *>(addResult),
                                   const_cast<uint64_t *>(offset),
                                   static_cast<uint64_t *>(addResult));
            }
        }

        uint64_t result[MAX_Q_LEN] = {};
        bool modSuccess = Hacl_Bignum256_mod(q.get(), static_cast<uint64_t *>(addResult),
                                             static_cast<uint64_t *>(result));
        if (!modSuccess) {
            throw runtime_error("add_mod_q mod operation failed");
        }
        return make_unique<ElementModQ>(result);
    }

    unique_ptr<ElementModQ> sub_mod_q(const ElementModQ &a, const ElementModQ &b)
    {
        const auto &q = Q();
        uint64_t subResult[MAX_Q_LEN_DOUBLE] = {};
        uint64_t carry =
          Hacl_Bignum256_sub(const_cast<ElementModQ &>(a).get(), const_cast<ElementModQ &>(b).get(),
                             static_cast<uint64_t *>(subResult));

        if (carry > 0) {

            auto big_a = (const_cast<ElementModQ &>(q) < a);
            auto big_b = (const_cast<ElementModQ &>(q) < b);

            if (big_a || big_b) {

                // TODO: precompute

                uint64_t offset[MAX_Q_LEN] = {};

                Hacl_Bignum256_add(static_cast<uint64_t *>(offset),
                                   const_cast<uint64_t *>(Q_ARRAY_INVERSE_REVERSE),
                                   static_cast<uint64_t *>(offset));
                Hacl_Bignum256_add(static_cast<uint64_t *>(offset),
                                   const_cast<uint64_t *>(ONE_MOD_Q_ARRAY),
                                   static_cast<uint64_t *>(offset));

                // adjust
                Hacl_Bignum256_sub(static_cast<uint64_t *>(subResult),
                                   static_cast<uint64_t *>(offset),
                                   static_cast<uint64_t *>(subResult));

            } else {
                const uint64_t offset[MAX_Q_LEN] = {0x00000000000000bd};
                Hacl_Bignum256_sub(static_cast<uint64_t *>(subResult),
                                   const_cast<uint64_t *>(offset),
                                   static_cast<uint64_t *>(subResult));
            }
        }

        uint64_t resModQ[MAX_Q_LEN] = {};
        bool modSuccess = Hacl_Bignum256_mod(q.get(), static_cast<uint64_t *>(subResult),
                                             static_cast<uint64_t *>(resModQ));
        if (!modSuccess) {
            throw runtime_error(" a_minus_b_mod_q mod operation failed");
        }
        return make_unique<ElementModQ>(resModQ);
    }

    unique_ptr<ElementModQ> a_plus_bc_mod_q(const ElementModQ &a, const ElementModQ &b,
                                            const ElementModQ &c)
    {
        // since the result of b*c can be larger than Q's 256,
        // use P's length and 4096 Hacl for rest of calculation

        uint64_t resultBC[MAX_P_LEN] = {}; // init to MAX_P_LEN
        Hacl_Bignum256_mul(const_cast<ElementModQ &>(b).get(), const_cast<ElementModQ &>(c).get(),
                           static_cast<uint64_t *>(resultBC));

        auto bc_as_p = make_unique<ElementModP>(resultBC);
        auto a_as_p = a.toElementModP();

        uint64_t a_plus_bc_result[MAX_P_LEN_DOUBLE] = {};
        uint64_t carry = Hacl_Bignum4096_add(a_as_p->get(), bc_as_p->get(),
                                             static_cast<uint64_t *>(a_plus_bc_result));

        // we should never overflow P space since our max size
        // is resultBC[MAX_Q_LEN_DOUBLE] + a[MAX_Q_LEN]
        if (carry > 0) {
            throw overflow_error("a_plus_bc_mod_q add operation out of bounds");
        }

        // Even though the method can overlow, the result does not
        // since we take the inputs mod Q
        uint64_t resModQ[MAX_Q_LEN] = {};

        auto q_as_p = Q().toElementModP();
        bool modSuccess =
          Hacl_Bignum4096_mod(q_as_p->get(), static_cast<uint64_t *>(a_plus_bc_result),
                              static_cast<uint64_t *>(resModQ));
        if (!modSuccess) {
            throw runtime_error("a_plus_bc_mod_q mod operation failed");
        }
        return make_unique<ElementModQ>(resModQ);
    }

    unique_ptr<ElementModQ> sub_from_q(const ElementModQ &a)
    {
        uint64_t result[MAX_Q_LEN] = {};
        Hacl_Bignum256_sub(const_cast<ElementModQ &>(Q()).get(), const_cast<ElementModQ &>(a).get(),
                           static_cast<uint64_t *>(result));
        // TODO: python version doesn't perform % Q on results,
        // but we still need to handle the overflow values between (Q, MAX_256]
        return make_unique<ElementModQ>(result, true);
    }

    unique_ptr<ElementModQ> rand_q()
    {
        // TODO: choose a better random generator
        random_device rd;
        uniform_int_distribution<uint64_t> dist(0, 0xFFFFFFFFFFFFFFFF);

        uint64_t result[MAX_Q_LEN];
        for (size_t i(0); i < MAX_Q_LEN; i++) {
            if (i == 0) {
                // first index of Q cannot cannod exceed 0xFFFFFFFFFFFFFF43
                uniform_int_distribution<uint64_t> firstDist(0, 0xFFFFFFFFFFFFFF43);
                result[i] = firstDist(rd);
            } else {
                result[i] = dist(rd);
            }
        }

        return make_unique<ElementModQ>(result);
    }

#pragma endregion

} // namespace electionguard
