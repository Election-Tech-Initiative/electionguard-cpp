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

#pragma endregion

namespace electionguard
{

#pragma region Constants
    // todo: compile time constants

    ElementModP *G() { return new ElementModP(G_ARRAY, true); };

    ElementModP *P() { return new ElementModP(P_ARRAY, true); };

    ElementModP *ZERO_MOD_P() { return new ElementModP(ZERO_MOD_P_ARRAY, true); };

    ElementModP *ONE_MOD_P() { return new ElementModP(ONE_MOD_P_ARRAY, true); };

    ElementModP *TWO_MOD_P() { return new ElementModP(TWO_MOD_P_ARRAY, true); };

    ElementModQ *Q() { return new ElementModQ(Q_ARRAY, true); };

    ElementModQ *ZERO_MOD_Q() { return new ElementModQ(ZERO_MOD_Q_ARRAY, true); };

    ElementModQ *ONE_MOD_Q() { return new ElementModQ(ONE_MOD_Q_ARRAY, true); };

    ElementModQ *TWO_MOD_Q() { return new ElementModQ(TWO_MOD_Q_ARRAY, true); };

#pragma endregion

#pragma region ElementModP

    // Lifecycle Methods

    ElementModP::ElementModP(const vector<uint64_t> &elem, bool unchecked /* = false */) : data()
    {
        Lib_Memzero0_memzero(static_cast<uint64_t *>(data.elem), MAX_P_LEN);

        uint64_t array[MAX_P_LEN] = {};
        copy(elem.begin(), elem.end(), static_cast<uint64_t *>(array));
        if (!unchecked && Hacl_Bignum4096_lt_mask(const_cast<uint64_t *>(P_ARRAY),
                                                  static_cast<uint64_t *>(array)) > 0) {
            throw out_of_range("Value for ElementModP is greater than allowed");
        }
        memcpy(static_cast<uint64_t *>(data.elem), static_cast<uint64_t *>(array), MAX_P_SIZE);
    }

    // param elem is expected to be allocated to uint64_t[MAX_P_LEN]
    ElementModP::ElementModP(const uint64_t *elem, bool unchecked /* = false */) : data()
    {
        Lib_Memzero0_memzero(static_cast<uint64_t *>(data.elem), MAX_P_LEN);
        if (!unchecked && Hacl_Bignum4096_lt_mask(const_cast<uint64_t *>(P_ARRAY),
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

#pragma region ElementModP Global Functions

    ElementModP *add_mod_p(ElementModP *lhs, ElementModP *rhs)
    {
        uint64_t result[MAX_P_LEN] = {};
        uint64_t carry =
          Hacl_Bignum4096_add(lhs->get(), rhs->get(), static_cast<uint64_t *>(result));
        if (carry > 0) {
            // just bypass compiler error
        }

        // TODO: temporarily using a mul operation to expand the data
        uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
        Hacl_Bignum4096_mul(static_cast<uint64_t *>(result), ONE_MOD_P()->get(),
                            static_cast<uint64_t *>(mulResult));

        uint64_t modResult[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod(P()->get(), static_cast<uint64_t *>(mulResult),
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

    ElementModP *pow_mod_p(ElementModP *b, ElementModP *e)
    {
        // HACL's input constraints require the exponent to be greater than zero
        if (*e == *ZERO_MOD_P()) {
            return ElementModP::fromUint64(1);
        }

        uint64_t result[MAX_P_LEN] = {};
        bool success = Hacl_Bignum4096_mod_exp(P()->get(), b->get(), MAX_P_SIZE, e->get(),
                                               static_cast<uint64_t *>(result));
        if (!success) {
            throw runtime_error(" pow_mod_p mod operation failed");
        }

        return new ElementModP(result, true);
    }

    ElementModP *g_pow_p(ElementModP *e) { return pow_mod_p(G(), e); }

#pragma endregion

#pragma region ElementModQ

    // Lifecycle Methods

    ElementModQ::ElementModQ(const vector<uint64_t> &elem, bool unchecked /* = false */) : data()
    {
        Lib_Memzero0_memzero(static_cast<uint64_t *>(data.elem), MAX_P_LEN);

        uint64_t array[MAX_Q_LEN] = {};
        copy(elem.begin(), elem.end(), static_cast<uint64_t *>(array));
        if (!unchecked && Hacl_Bignum4096_lt_mask(const_cast<uint64_t *>(Q_ARRAY),
                                                  static_cast<uint64_t *>(array)) > 0) {
            throw out_of_range("Value for ElementModQ is greater than allowed");
        }
        memcpy(static_cast<uint64_t *>(data.elem), static_cast<uint64_t *>(array), MAX_Q_SIZE);
    }

    // param elem is expected to be allocated to uint64_t[MAX_Q_LEN]
    ElementModQ::ElementModQ(const uint64_t *elem, bool unchecked /* = false*/) : data()
    {
        if (!unchecked && Hacl_Bignum256_lt_mask(const_cast<uint64_t *>(Q_ARRAY),
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

#pragma region ElementModQ Global Functions

    ElementModQ *add_mod_q(ElementModQ *lhs, ElementModQ *rhs)
    {
        uint64_t addResult[MAX_Q_LEN] = {};
        uint64_t carry =
          Hacl_Bignum256_add(lhs->get(), rhs->get(), static_cast<uint64_t *>(addResult));
        if (carry > 0) {
            // just bypass compiler error
        }

        // TODO: Temporarily executing a mul operation to widen the result
        uint64_t mulResult[MAX_Q_LEN_DOUBLE] = {};
        Hacl_Bignum256_mul(static_cast<uint64_t *>(addResult), ONE_MOD_Q()->get(),
                           static_cast<uint64_t *>(mulResult));

        uint64_t resModQ[MAX_Q_LEN] = {};
        bool modSuccess = Hacl_Bignum256_mod(Q()->get(), static_cast<uint64_t *>(mulResult),
                                             static_cast<uint64_t *>(resModQ));
        if (!modSuccess) {
            throw runtime_error(" add_mod_q mod operation failed");
        }

        return new ElementModQ(static_cast<uint64_t *>(resModQ));
    }

    ElementModQ *a_minus_b_mod_q(ElementModQ *a, ElementModQ *b)
    {
        uint64_t subResult[MAX_Q_LEN] = {};
        Hacl_Bignum256_sub(a->get(), b->get(), static_cast<uint64_t *>(subResult));

        // TODO: Temporarily executing a mul operation to widen the result
        uint64_t mulResult[MAX_Q_LEN_DOUBLE] = {};
        Hacl_Bignum256_mul(static_cast<uint64_t *>(subResult), ONE_MOD_Q()->get(),
                           static_cast<uint64_t *>(mulResult));

        uint64_t resModQ[MAX_Q_LEN] = {};
        bool modSuccess = Hacl_Bignum256_mod(Q()->get(), static_cast<uint64_t *>(mulResult),
                                             static_cast<uint64_t *>(resModQ));
        if (!modSuccess) {
            throw runtime_error(" a_minus_b_mod_q mod operation failed");
        }

        return new ElementModQ(static_cast<uint64_t *>(resModQ));
    }

    ElementModQ *a_plus_bc_mod_q(ElementModQ *a, ElementModQ *b, ElementModQ *c)
    {
        uint64_t resultBC[MAX_Q_LEN_DOUBLE] = {};
        Hacl_Bignum256_mul(b->get(), c->get(), static_cast<uint64_t *>(resultBC));

        // TODO: Temprarily using P address space
        // since the result of b*c can be larger than Q's 256,
        // use P's length and 4096 Hacl for rest of calculation
        uint64_t bc4096[MAX_P_LEN] = {};
        memcpy(static_cast<uint64_t *>(bc4096), static_cast<uint64_t *>(resultBC),
               MAX_Q_LEN_DOUBLE);

        uint64_t a_plus_bc_result[MAX_P_LEN] = {};
        uint64_t carry =
          Hacl_Bignum4096_add(a->toElementModP()->get(), static_cast<uint64_t *>(bc4096),
                              static_cast<uint64_t *>(a_plus_bc_result));
        if (carry > 0) {
            // just bypass compiler error
        }

        // TODO: Temporarily executing a mul operation to widen the result
        uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
        Hacl_Bignum256_mul(static_cast<uint64_t *>(a_plus_bc_result), ONE_MOD_P()->get(),
                           static_cast<uint64_t *>(mulResult));

        uint64_t resModQ[MAX_P_LEN] = {};
        bool modSuccess =
          Hacl_Bignum4096_mod(Q()->toElementModP()->get(), static_cast<uint64_t *>(mulResult),
                              static_cast<uint64_t *>(resModQ));
        if (!modSuccess) {
            throw runtime_error(" a_plus_bc_mod_q mod operation failed");
        }

        return new ElementModQ(static_cast<uint64_t *>(resModQ));
    }

    ElementModQ *negate_mod_q(ElementModQ *a)
    {
        uint64_t result[MAX_Q_LEN] = {};
        Hacl_Bignum256_sub(Q()->get(), a->get(), static_cast<uint64_t *>(result));
        // TODO: python version doesn't perform % Q on results...
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