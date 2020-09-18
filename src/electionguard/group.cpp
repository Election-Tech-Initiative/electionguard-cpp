#include "electionguard/group.hpp"

#include "../kremlin/Hacl_Bignum256.h"
#include "../kremlin/Hacl_Bignum4096.h"
#include "../kremlin/Lib_Memzero0.h"
#include "electionguard/constants.h"
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
electionguard::ElementModP *bytes_to_p(uint8_t *bytes, size_t size)
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
    // TODO: validate that we actually want to copy 'size' elements from bignum
    memcpy(static_cast<uint64_t *>(element), bigNum, size);
    free(bigNum);
    return new electionguard::ElementModP(static_cast<uint64_t *>(element));
}

/// <summary>
/// Converts the binary value stored as a byte array
/// to its big num representation stored as ElementModP
/// </summary>
electionguard::ElementModP *bytes_to_p(vector<uint8_t> bytes)
{
    auto *array = new uint8_t[bytes.size()];
    copy(bytes.begin(), bytes.end(), array);

    auto *element = bytes_to_p(array, bytes.size());
    delete[] array;
    return element;
}

/// <summary>
/// Converts the binary value stored as a big-endian byte array
/// to its big num representation stored as ElementModQ
/// </summary>
electionguard::ElementModQ *bytes_to_q(const uint8_t *bytes, size_t size)
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
    return new electionguard::ElementModQ(static_cast<uint64_t *>(element));
}

electionguard::ElementModQ *bytes_to_q(vector<uint8_t> bytes)
{
    auto *array = new uint8_t[bytes.size()];
    copy(bytes.begin(), bytes.end(), array);

    auto *element = bytes_to_q(array, bytes.size());
    delete[] array;
    return element;
}

bool isMax(uint64_t (&array)[MAX_Q_LEN_DOUBLE])
{
    for (uint32_t i = 0; i < MAX_Q_LEN; i++) {
        if (array[i] != 0xffffffffffffffff) {
            return false;
        }
    }
    return true;
}

bool isMax(uint64_t (&array)[MAX_P_LEN_DOUBLE])
{
    for (uint32_t i = 0; i < MAX_P_LEN; i++) {
        if (array[i] != 0xffffffffffffffff) {
            return false;
        }
    }
    return true;
}

#pragma endregion

namespace electionguard
{
#pragma region ElementModP

    // Lifecycle Methods

    ElementModP::ElementModP(const vector<uint64_t> &elem, bool unchecked /* = false */) : data()
    {
        Lib_Memzero0_memzero(static_cast<uint64_t *>(data.elem), MAX_P_LEN);

        uint64_t array[MAX_P_LEN] = {};
        copy(elem.begin(), elem.end(), static_cast<uint64_t *>(array));
        if (!unchecked && Hacl_Bignum4096_lt_mask(const_cast<uint64_t *>(P_ARRAY_REVERSE),
                                                  static_cast<uint64_t *>(array)) > 0) {
            throw out_of_range("Value for ElementModP is greater than allowed");
        }
        memcpy(static_cast<uint64_t *>(data.elem), static_cast<uint64_t *>(array), MAX_P_SIZE);
    }

    // param elem is expected to be allocated to uint64_t[MAX_P_LEN]
    ElementModP::ElementModP(const uint64_t *elem, bool unchecked /* = false */) : data()
    {
        Lib_Memzero0_memzero(static_cast<uint64_t *>(data.elem), MAX_P_LEN);
        if (!unchecked && Hacl_Bignum4096_lt_mask(const_cast<uint64_t *>(P_ARRAY_REVERSE),
                                                  const_cast<uint64_t *>(elem)) > 0) {
            throw out_of_range("Value for ElementModP is greater than allowed");
        }
        memcpy(static_cast<uint64_t *>(data.elem), elem, MAX_P_SIZE);
    }

    ElementModP::~ElementModP()
    {
        Lib_Memzero0_memzero(static_cast<uint64_t *>(data.elem), MAX_P_LEN);
    }

    // Property Getters

    uint64_t *ElementModP::get() { return static_cast<uint64_t *>(data.elem); }

    bool ElementModP::isInBounds() { return (*ZERO_MOD_P() < *this) && (*this < *P()); }
    bool ElementModP::isValidResidue()
    {
        auto residue = (*pow_mod_p(this, Q()) == *ONE_MOD_P());
        return this->isInBounds() && residue;
    }

    string ElementModP::toHex()
    {
        // Returned bytes array from Hacl needs to be pre-allocated to 512 bytes
        uint8_t byteResult[MAX_P_SIZE] = {};
        // Use Hacl to convert the bignum to byte array
        Hacl_Bignum4096_bn_to_bytes_be(static_cast<uint64_t *>(data.elem),
                                       static_cast<uint8_t *>(byteResult));
        return bytes_to_hex(byteResult);
    }

    ElementModP *ElementModP::fromHex(const string &representation)
    {
        auto sanitized = sanitize_hex_string(representation);
        auto bytes = hex_to_bytes(sanitized);
        auto *element = bytes_to_p(bytes);
        release(bytes);
        return element;
    }

    ElementModP *ElementModP::fromUint64(uint64_t representation)
    {
        const size_t bitWidth = 8U;
        uint64_t bigEndian = htobe64(representation);
        return bytes_to_p(reinterpret_cast<uint8_t *>(&bigEndian), bitWidth);
    }

    // Operator Overloads

    bool ElementModP::operator==(const ElementModP &other)
    {
        // TODO: safety, specifically when the object underflows its max size
        // e.g. if ((l == (uint64_t)0U) && (r == (uint64_t)0U))
        for (uint8_t i = 0; i < MAX_P_LEN; i++) {
            auto l = data.elem[i];
            auto r = other.data.elem[i];
            if (l != r) {
                return false;
            }
        }
        return true;
    }

    bool ElementModP::operator!=(const ElementModP &other) { return !(*this == other); }

    bool ElementModP::operator<(const ElementModP &other)
    {
        auto other_ = other;
        return Hacl_Bignum4096_lt_mask(static_cast<uint64_t *>(data.elem),
                                       static_cast<uint64_t *>(other_.data.elem)) > 0;
    }

#pragma endregion

#pragma region ElementModQ

    // Lifecycle Methods

    ElementModQ::ElementModQ(const vector<uint64_t> &elem, bool unchecked /* = false */) : data()
    {
        Lib_Memzero0_memzero(static_cast<uint64_t *>(data.elem), MAX_P_LEN);

        uint64_t array[MAX_Q_LEN] = {};
        copy(elem.begin(), elem.end(), static_cast<uint64_t *>(array));
        if (!unchecked && Hacl_Bignum4096_lt_mask(const_cast<uint64_t *>(Q_ARRAY_REVERSE),
                                                  static_cast<uint64_t *>(array)) > 0) {
            throw out_of_range("Value for ElementModQ is greater than allowed");
        }
        memcpy(static_cast<uint64_t *>(data.elem), static_cast<uint64_t *>(array), MAX_Q_SIZE);
    }

    // param elem is expected to be allocated to uint64_t[MAX_Q_LEN]
    ElementModQ::ElementModQ(const uint64_t *elem, bool unchecked /* = false*/) : data()
    {
        if (!unchecked && Hacl_Bignum256_lt_mask(const_cast<uint64_t *>(Q_ARRAY_REVERSE),
                                                 const_cast<uint64_t *>(elem)) > 0) {
            throw out_of_range("Value for ElementModQ is greater than allowed");
        }
        memcpy(static_cast<uint64_t *>(data.elem), elem, MAX_Q_SIZE);
    }
    ElementModQ::~ElementModQ()
    {
        Lib_Memzero0_memzero(static_cast<uint64_t *>(data.elem), MAX_Q_LEN);
    }

    // Property Getters

    uint64_t *ElementModQ::get() { return static_cast<uint64_t *>(data.elem); }

    // Public Methods

    bool ElementModQ::isInBounds() { return (*ZERO_MOD_Q() < *this) && (*this < *Q()); }

    string ElementModQ::toHex()
    {
        // Returned bytes array from Hacl needs to be pre-allocated to 32 bytes
        uint8_t byteResult[MAX_Q_SIZE] = {};
        // Use Hacl to convert the bignum to byte array
        Hacl_Bignum256_bn_to_bytes_be(static_cast<uint64_t *>(data.elem),
                                      static_cast<uint8_t *>(byteResult));
        return bytes_to_hex(byteResult);
    }

    ElementModQ *ElementModQ::fromHex(const string &representation)
    {
        auto sanitized = sanitize_hex_string(representation);
        auto bytes = hex_to_bytes(sanitized);
        auto *element = bytes_to_q(bytes);
        release(bytes);
        return element;
    }

    ElementModQ *ElementModQ::fromUint64(uint64_t representation)
    {
        const size_t bitWidth = 8U;
        uint64_t bigEndian = htobe64(representation);
        return bytes_to_q(reinterpret_cast<uint8_t *>(&bigEndian), bitWidth);
    }

    ElementModP *ElementModQ::toElementModP()
    {
        uint64_t p4096[MAX_P_LEN] = {};
        memcpy(static_cast<uint64_t *>(p4096), static_cast<uint64_t *>(data.elem), MAX_Q_SIZE);
        return new ElementModP(static_cast<uint64_t *>(p4096), true);
    }

    // Operator Overloads

    bool ElementModQ::operator==(const ElementModQ &other)
    {
        // TODO: safety, specifically when the object underflows its max size
        // e.g. if ((l == (uint64_t)0U) && (r == (uint64_t)0U))
        for (int i = 0; i < MAX_Q_LEN; i++) {
            auto l = data.elem[i];
            auto r = other.data.elem[i];
            if (l != r) {
                return false;
            }
        }
        return true;
    }

    bool ElementModQ::operator!=(const ElementModQ &other) { return !(*this == other); }

    bool ElementModQ::operator<(const ElementModQ &other)
    {
        auto other_ = other;
        return Hacl_Bignum256_lt_mask(static_cast<uint64_t *>(data.elem),
                                      static_cast<uint64_t *>(other_.data.elem)) > 0;
    }

#pragma endregion

#pragma region ElementModP Global Functions

    ElementModP *add_mod_p(ElementModP *lhs, ElementModP *rhs)
    {

        uint64_t result[MAX_P_LEN_DOUBLE] = {};
        uint64_t carry =
          Hacl_Bignum4096_add(lhs->get(), rhs->get(), static_cast<uint64_t *>(result));

        // handle the specific case where the the sum == MAX_4096
        // but the carry value is not set.  We still need to offset.
        if (carry > 0 || isMax(result)) {

            auto big_a = (*P() < *lhs);
            auto big_b = (*P() < *rhs);

            // TODO: genericize this in a HACL wrapper
            // the single byte shifts belong in the wrapper
            // but the Q array shifts belong in this class
            if (big_a || big_b) {

                // TODO: handle carry like in ElementModQ

                // uint64_t offset[MAX_Q_LEN] = {};

                // if (big_a) {
                //     Hacl_Bignum4096_add(offset, const_cast<uint64_t *>(P_ARRAY_INVERSE_REVERSE),
                //                         offset);
                //     Hacl_Bignum4096_add(offset, const_cast<uint64_t *>(ONE_MOD_P_ARRAY), offset);
                // }

                // if (big_b) {
                //     Hacl_Bignum4096_add(offset, const_cast<uint64_t *>(P_ARRAY_INVERSE_REVERSE),
                //                         offset);
                //     Hacl_Bignum4096_add(offset, const_cast<uint64_t *>(ONE_MOD_P_ARRAY), offset);
                // }

                // // adjust
                // Hacl_Bignum4096_add(result, offset, static_cast<uint64_t *>(result));

            } else {
                // uint64_t offset[MAX_P_LEN] = {0x00000000000000bd};
                // Hacl_Bignum4096_add(result, offset, static_cast<uint64_t *>(result));
            }
        }

        uint64_t modResult[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod(P()->get(), static_cast<uint64_t *>(result),
                                           static_cast<uint64_t *>(modResult));
        if (!success) {
            throw runtime_error(" add_mod_p mod operation failed");
        }
        return new ElementModP(static_cast<uint64_t *>(result));
    }

    ElementModP *mul_mod_p(ElementModP *lhs, ElementModP *rhs)
    {
        uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
        Hacl_Bignum4096_mul(lhs->get(), rhs->get(), static_cast<uint64_t *>(mulResult));
        uint64_t modResult[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod(P()->get(), static_cast<uint64_t *>(mulResult),
                                           static_cast<uint64_t *>(modResult));
        if (!success) {
            throw runtime_error(" mul_mod_p mod operation failed");
        }
        return new ElementModP(static_cast<uint64_t *>(modResult));
    }

    ElementModP *mul_mod_p(vector<ElementModPOrQ> elems)
    {
        auto *product = ElementModP::fromUint64(1);
        for (auto x : elems) {
            ElementModP *elem;
            if (holds_alternative<ElementModQ *>(x)) {
                elem = get<ElementModQ *>(x)->toElementModP();

            } else if (holds_alternative<ElementModP *>(x)) {
                elem = get<ElementModP *>(x);
            } else {
                throw "invalid type";
            }

            auto *res = mul_mod_p(product, elem);
            product = res;
        }
        return product;
    }

    ElementModP *pow_mod_p(ElementModPOrQ b, ElementModPOrQ e)
    {
        ElementModP *base;
        ElementModP *exponent;

        if (holds_alternative<ElementModQ *>(b)) {
            base = get<ElementModQ *>(b)->toElementModP();
        } else if (holds_alternative<ElementModP *>(b)) {
            base = get<ElementModP *>(b);
        } else {
            throw "unsupported type";
        }

        if (holds_alternative<ElementModQ *>(e)) {
            exponent = get<ElementModQ *>(e)->toElementModP();
        } else if (holds_alternative<ElementModP *>(e)) {
            exponent = get<ElementModP *>(e);
        } else {
            throw "unsupported type";
        }

        // HACL's input constraints require the exponent to be greater than zero
        if (*exponent == *ZERO_MOD_P()) {
            return ElementModP::fromUint64(1);
        }

        uint64_t result[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod_exp(P()->get(), base->get(), MAX_P_SIZE, exponent->get(),
                                               static_cast<uint64_t *>(result));
        if (!success) {
            throw runtime_error(" pow_mod_p mod operation failed");
        }

        return new ElementModP(result, true);
    }

    ElementModP *g_pow_p(ElementModPOrQ e) { return pow_mod_p(G(), e); }

#pragma endregion

#pragma region ElementModQ Global Functions

    ElementModQ *add_mod_q(ElementModQ *lhs, ElementModQ *rhs)
    {
        uint64_t addResult[MAX_Q_LEN_DOUBLE] = {};
        uint64_t carry =
          Hacl_Bignum256_add(lhs->get(), rhs->get(), static_cast<uint64_t *>(addResult));

        // handle the specific case where the the sum == MAX_256
        // but the carry value is not set.  We still need to offset.
        if (carry > 0 || isMax(addResult)) {

            auto big_a = (*Q() < *lhs);
            auto big_b = (*Q() < *rhs);

            // TODO: genericize this in a HACL wrapper
            // the single byte shifts belong in the wrapper
            // but the Q array shifts belong in this class
            if (big_a || big_b) {

                uint64_t offset[MAX_Q_LEN] = {};

                if (big_a) {
                    Hacl_Bignum256_add(offset, const_cast<uint64_t *>(Q_ARRAY_INVERSE_REVERSE),
                                       offset);
                    Hacl_Bignum256_add(offset, const_cast<uint64_t *>(ONE_MOD_Q_ARRAY), offset);
                }

                if (big_b) {
                    Hacl_Bignum256_add(offset, const_cast<uint64_t *>(Q_ARRAY_INVERSE_REVERSE),
                                       offset);
                    Hacl_Bignum256_add(offset, const_cast<uint64_t *>(ONE_MOD_Q_ARRAY), offset);
                }

                // adjust
                Hacl_Bignum256_add(addResult, offset, static_cast<uint64_t *>(addResult));

            } else {
                uint64_t offset[MAX_Q_LEN] = {0x00000000000000bd};
                Hacl_Bignum256_add(addResult, offset, static_cast<uint64_t *>(addResult));
            }
        }

        uint64_t resModQ[MAX_Q_LEN] = {};
        bool modSuccess = Hacl_Bignum256_mod(Q()->get(), static_cast<uint64_t *>(addResult),
                                             static_cast<uint64_t *>(resModQ));
        if (!modSuccess) {
            throw runtime_error("add_mod_q mod operation failed");
        }
        return new ElementModQ(static_cast<uint64_t *>(resModQ));
    }

    ElementModQ *sub_mod_q(ElementModQ *a, ElementModQ *b)
    {
        uint64_t subResult[MAX_Q_LEN_DOUBLE] = {};
        uint64_t carry = Hacl_Bignum256_sub(a->get(), b->get(), static_cast<uint64_t *>(subResult));

        if (carry > 0) {

            auto big_a = (*Q() < *a);
            auto big_b = (*Q() < *b);

            if (big_a || big_b) {

                // TODO: genericize and precompute

                uint64_t offset[MAX_Q_LEN] = {};

                Hacl_Bignum256_add(offset, const_cast<uint64_t *>(Q_ARRAY_INVERSE_REVERSE), offset);
                Hacl_Bignum256_add(offset, const_cast<uint64_t *>(ONE_MOD_Q_ARRAY), offset);

                // adjust
                Hacl_Bignum256_sub(subResult, offset, static_cast<uint64_t *>(subResult));

            } else {
                uint64_t offset[MAX_Q_LEN] = {0x00000000000000bd};
                Hacl_Bignum256_sub(subResult, offset, static_cast<uint64_t *>(subResult));
            }
        }

        uint64_t resModQ[MAX_Q_LEN] = {};
        bool modSuccess = Hacl_Bignum256_mod(Q()->get(), static_cast<uint64_t *>(subResult),
                                             static_cast<uint64_t *>(resModQ));
        if (!modSuccess) {
            throw runtime_error(" a_minus_b_mod_q mod operation failed");
        }
        return new ElementModQ(static_cast<uint64_t *>(resModQ));
    }

    ElementModQ *a_plus_bc_mod_q(ElementModQ *a, ElementModQ *b, ElementModQ *c)
    {
        // since the result of b*c can be larger than Q's 256,
        // use P's length and 4096 Hacl for rest of calculation

        uint64_t resultBC[MAX_P_LEN] = {}; // init to MAX_P_LEN
        Hacl_Bignum256_mul(b->get(), c->get(), static_cast<uint64_t *>(resultBC));

        auto *bc_as_p = new ElementModP(resultBC);
        auto *a_as_p = a->toElementModP()->get();

        uint64_t a_plus_bc_result[MAX_P_LEN_DOUBLE] = {};
        uint64_t carry =
          Hacl_Bignum4096_add(a_as_p, bc_as_p->get(), static_cast<uint64_t *>(a_plus_bc_result));

        // we should never overflow P space since our max size
        // is resultBC[MAX_Q_LEN_DOUBLE] + a[MAX_Q_LEN]
        if (carry > 0) {
            throw overflow_error("a_plus_bc_mod_q add operation out of bounds");
        }

        uint64_t resModQ[MAX_P_LEN] = {};
        bool modSuccess = Hacl_Bignum4096_mod(Q()->toElementModP()->get(),
                                              static_cast<uint64_t *>(a_plus_bc_result),
                                              static_cast<uint64_t *>(resModQ));
        if (!modSuccess) {
            throw runtime_error("a_plus_bc_mod_q mod operation failed");
        }

        return new ElementModQ(static_cast<uint64_t *>(resModQ));
    }

    ElementModQ *sub_from_q(ElementModQ *a)
    {
        uint64_t result[MAX_Q_LEN] = {};
        Hacl_Bignum256_sub(Q()->get(), a->get(), static_cast<uint64_t *>(result));
        // TODO: python version doesn't perform % Q on results,
        // but we still needto handle the overflow values between (Q, MAX_256]
        return new ElementModQ(static_cast<uint64_t *>(result), true);
    }

    ElementModQ *rand_q()
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

        return new ElementModQ(static_cast<uint64_t *>(result));
    }

#pragma endregion

} // namespace electionguard