#ifndef __ELECTIONGUARD_CPP_LOOKUP_TABLE_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_LOOKUP_TABLE_HPP_INCLUDED__

#include "async.hpp"
#include "facades/Hacl_Bignum256.hpp"
#include "facades/Hacl_Bignum4096.hpp"
#include "utils.hpp"

#include <array>
#include <cstdint>
#include <electionguard/constants.h>
#include <electionguard/export.h>
#include <iomanip>
#include <iostream>
#include <memory>

using hacl::Bignum256;
using hacl::Bignum4096;
using hacl::CONTEXT_P;
using std::begin;
using std::copy;
using std::end;
using std::unique_ptr;

namespace electionguard
{
    /// <summary>
    /// A fixed-base lookup tables used to precompute components for exponentiation.
    ///
    /// For a given base, precompute `b` bit tables over a `k` window size
    /// with a specific `m` table length.
    ///
    /// when executing a `pow_mod_p` operation, the exponent is sliced into k-bits
    /// and each slice is multiplied together using the values precomputed in the lookup table
    ///
    /// note this implementation is tuned sepcifically for the following values:
    /// b order bits = 256
    /// k window size = 8
    /// m table length = 32
    ///
    /// any changes to these values may impact the internal operation of the functions.
    /// </summary>
    template <uint64_t WindowSize, uint64_t OrderBits, uint64_t TableLength>
    class EG_INTERNAL_API LookupTable
    {
        typedef std::array<std::array<uint64_t[MAX_P_LEN], OrderBits>, TableLength> FixedBaseTable;

      public:
        explicit LookupTable(uint64_t *base) { generateTable(base, MAX_P_LEN); }

        /// <summary>
        /// calcuate pow_mod_p using the precomputed fixed base.
        /// </summary>
        std::vector<uint64_t> pow_mod_p(uint64_t (&exponent)[MAX_Q_LEN]) const
        {
            uint64_t montgomery_result[MAX_P_LEN] = {};
            uint64_t result[MAX_P_LEN] = {};
            uint8_t exponentBytes[MAX_Q_SIZE] = {};

            // copy the 1 in montgomery form into montgomery_result to start
            copy((uint64_t *)one_in_montgomery_form, (uint64_t *)one_in_montgomery_form + MAX_P_LEN,
                 montgomery_result);

            // convert the bignum to byte array
            // which aligns with the k window size
            Bignum256::toBytes(static_cast<uint64_t *>(exponent),
                               static_cast<uint8_t *>(exponentBytes));
            // reverse the byte order to account for the way hacl offsets its limbs
            std::reverse(std::begin(exponentBytes), std::end(exponentBytes));

            // iterate over rows-m slicing each segment of the exponent
            // and lookup the table values before executing a mul_mod_p operation
            for (uint64_t i = 0; i < _lookupTable.size(); i++) {
                auto slice = exponentBytes[i];

                // skip zero bytes
                if (slice == 0) {
                    continue;
                }

                mul_mod_p_mont(montgomery_result, const_cast<uint64_t *>(_lookupTable[i][slice]),
                          montgomery_result);
            }

            // convert from montogomery form
            CONTEXT_P().from_montgomery_form(montgomery_result, result);

            // wrap in a vector for convenience
            std::vector<uint64_t> vec(begin(result), end(result));
            return vec;
        }

      protected:
        void generateTable(uint64_t *base, uint64_t len)
        {
            // checking for power of two ensures the table is uniform
            if (!isPowerOfTwo(WindowSize)) {
                throw;
            }

            uint64_t row_base[MAX_P_LEN] = {};
            uint64_t running_base[MAX_P_LEN] = {};
            uint64_t one[MAX_P_LEN] = {1UL};


            // convert base to montgomery form
            CONTEXT_P().to_montgomery_form(base, row_base);

            // assign initial values
            copy(row_base, row_base + len, running_base);

            // iterate over each m-row in the table
            for (uint64_t i = 0; i < TableLength; i++) {
                // iterate over each b-bit and compute the table values
                for (uint64_t j = 1; j < OrderBits; j++) {
                    copy(begin(running_base), end(running_base), begin(_lookupTable[i][j]));
                    mul_mod_p_mont(running_base, row_base, running_base);
                }
                copy(begin(running_base), end(running_base), begin(row_base));
            }

            // also convert 1 to montgomery form and store it because we use it to
            // start every table based exponentiation and there is no point in
            // computing it each time
            CONTEXT_P().to_montgomery_form(one, one_in_montgomery_form);
        }

        void mul_mod_p_mont(uint64_t *lhs, uint64_t *rhs, uint64_t *res) const
        {
            CONTEXT_P().montgomery_mod_mul_stay_in_mont_form(lhs, rhs, res);
        }
      private:
        FixedBaseTable _lookupTable = {};
        uint64_t one_in_montgomery_form[MAX_P_LEN] = {};
    };

    typedef LookupTable<LUT_WINDOW_SIZE, LUT_ORDER_BITS, LUT_TABLE_LENGTH> LookupTableType;

    /// <summary>
    /// A singleton context for a collection of fixed base lookup tables.
    /// </summary>
    class EG_INTERNAL_API LookupTableContext
    {
      public:
        LookupTableContext(const LookupTableContext &) = delete;
        LookupTableContext(LookupTableContext &&) = delete;
        LookupTableContext &operator=(const LookupTableContext &) = delete;
        LookupTableContext &operator=(LookupTableContext &&) = delete;

      private:
        LookupTableContext() {}
        ~LookupTableContext() {}

      public:
        static LookupTableContext &getInstance()
        {
            static LookupTableContext instance;
            return instance;
        }

        /// <summary>
        /// calcuate pow_mod_p using the provided fixed base.
        /// </summary>
        static std::vector<uint64_t> pow_mod_p(string key, uint64_t (&base)[MAX_P_LEN],
                                               uint64_t (&exponent)[MAX_Q_LEN])
        {
            LookupTableType *public_key_table = NULL;
            {
                getInstance().task_lock.wait();
                public_key_table = getInstance().getBaseLookupTable(key, base);
            }
            return public_key_table->pow_mod_p(exponent);
        }

      private:
        AsyncSemaphore task_lock;
        std::map<string, std::unique_ptr<LookupTableType>> key_map;

        LookupTableType *getBaseLookupTable(const string key, uint64_t (&base)[MAX_P_LEN])
        {
            auto const iter = key_map.find(key);
            if (iter != key_map.end()) {
                return key_map[key].get();
            }

            key_map.emplace(std::pair(key, new LookupTableType(static_cast<uint64_t *>(base))));
            return key_map[key].get();
        }
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_LOOKUP_TABLE_HPP_INCLUDED__ */
