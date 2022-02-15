#ifndef __ELECTIONGUARD_CPP_LOOKUP_TABLE_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_LOOKUP_TABLE_HPP_INCLUDED__

#include "async.hpp"
#include "utils.hpp"

#include <cstdint>
#include <electionguard/constants.h>
#include <electionguard/export.h>
#include <iomanip>
#include <iostream>
#include <memory>

using hacl::Bignum256;
using hacl::Bignum4096;
using hacl::CONTEXT_P;
using std::copy;
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
        constexpr LookupTable(uint64_t *base, uint64_t len)
        {
            _lookupTable = generateTable(base, len);
        }

        explicit LookupTable(uint64_t *base) { _lookupTable = generateTable(base, MAX_P_LEN); }

        /// <summary>
        /// calcuate a fixed based table.
        /// </summary>
        std::vector<uint64_t> pow_mod_p(uint64_t *exponent, uint64_t length) const
        {
            uint64_t result[MAX_P_LEN] = {1UL};
            uint8_t exponentBytes[MAX_Q_SIZE] = {};

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

                mul_mod_p(result, const_cast<uint64_t *>(_lookupTable[i][slice]), result);
            }

            // wrap in a vector for convenience
            std::vector<uint64_t> vec(begin(result), end(result));
            return vec;
        }

      protected:
        FixedBaseTable generateTable(uint64_t *base, uint64_t len)
        {
            // checking for power of two ensures the table is uniform
            if (!isPowerOfTwo(WindowSize)) {
                throw;
            }

            FixedBaseTable result{};
            uint64_t row_base[MAX_P_LEN] = {};
            uint64_t running_base[MAX_P_LEN] = {};

            // assign initial values
            copy(base, base + len, row_base);
            copy(base, base + len, running_base);

            // iterate over each m-row in the table
            for (uint64_t i = 0; i < TableLength; i++) {
                // iterate over each b-bit and compute the table values
                for (uint64_t j = 1; j < OrderBits; j++) {
                    copy(begin(running_base), end(running_base), begin(result[i][j]));
                    mul_mod_p(running_base, row_base, running_base);
                }
                copy(begin(running_base), end(running_base), begin(row_base));
            }

            return result;
        }

        void mul_mod_p(uint64_t *lhs, uint64_t *rhs, uint64_t *res) const
        {
            uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
            Bignum4096::mul(lhs, rhs, static_cast<uint64_t *>(mulResult));
            CONTEXT_P().mod(static_cast<uint64_t *>(mulResult), res);
        }

      private:
        FixedBaseTable _lookupTable;
    };

    typedef LookupTable<LUT_WINDOW_SIZE, LUT_ORDER_BITS, LUT_TABLE_LENGTH> LookupTableType;

    const auto generator_table =
      LookupTableType(const_cast<uint64_t *>(G_ARRAY_REVERSE), MAX_P_LEN);

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
        // static LookupTableContext *getInstance()
        // {
        //     LookupTableContext *sin = _instance.load();
        //     if (!sin) {
        //         task_lock.wait();
        //         sin = _instance.load();
        //         if (!sin) {
        //             sin = new LookupTableContext();
        //             _instance.store(sin);
        //         }
        //     }
        //     // volatile int dummy{};
        //     return sin;
        // }
        static LookupTableContext &getInstance()
        {
            static LookupTableContext instance;
            return instance;
        }

        LookupTableType *publicKeyTable(const string key, uint64_t (&publicKey)[MAX_P_LEN])
        {
            Log::debug("- publicKeyTable ENTER", key);

            auto const iter = key_map.find(key);
            if (iter != key_map.end()) {
                Log::debug("- publicKeyTable return existing");
                return key_map[key].get();
            }

            Log::debug("- publicKeyTable make new - key", key);

            LookupTableContext::key_map.emplace(
              std::pair(key, new LookupTableType(static_cast<uint64_t *>(publicKey))));
            return key_map[key].get();
        }

        static std::vector<uint64_t> pow_mod_p(string key, uint64_t (&publicKey)[MAX_P_LEN],
                                               uint64_t *exponent, uint64_t length)
        {
            LookupTableType *public_key_table = NULL;
            {
                Log::debug("- pow_mod_p ENTER");
                task_lock.wait();
                Log::debug("- pow_mod_p CONTINUE");
                public_key_table = getInstance().publicKeyTable(key, publicKey);
            }
            return public_key_table->pow_mod_p(exponent, MAX_Q_LEN);
        }

      private:
        static AsyncSemaphore task_lock;
        std::map<string, std::unique_ptr<LookupTableType>> key_map;
    };

    AsyncSemaphore LookupTableContext::task_lock;

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_LOOKUP_TABLE_HPP_INCLUDED__ */