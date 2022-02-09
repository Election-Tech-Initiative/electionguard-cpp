#ifndef __ELECTIONGUARD_CPP_LOOKUP_TABLE_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_LOOKUP_TABLE_HPP_INCLUDED__

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
        constexpr LookupTable(uint64_t *base, uint64_t len) : _base(base), _len(len)
        {
            _lookupTable = generateTable();
        }

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
            for (auto i = 0; i < _lookupTable.size(); i++) {
                auto slice = exponentBytes[i];

                // skip zero bytes
                if (slice == 0) {
                    continue;
                }

                uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
                Bignum4096::mul(result, const_cast<uint64_t *>(_lookupTable[i][slice]),
                                static_cast<uint64_t *>(mulResult));
                CONTEXT_P().mod(static_cast<uint64_t *>(mulResult),
                                static_cast<uint64_t *>(result));
            }

            // wrap in a vector for convenience
            std::vector<uint64_t> vec(begin(result), end(result));
            return vec;
        }

      protected:
        constexpr FixedBaseTable generateTable()
        {
            // checking for power of two ensures the table is uniform
            if (!isPowerOfTwo(WindowSize)) {
                throw;
            }

            FixedBaseTable result{};
            uint64_t row_base[MAX_P_LEN] = {};
            uint64_t running_base[MAX_P_LEN] = {};

            // assign initial values
            copy(_base, _base + _len, row_base);
            copy(_base, _base + _len, running_base);

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

        void mul_mod_p(uint64_t *lhs, uint64_t *rhs, uint64_t *res)
        {
            uint64_t mulResult[MAX_P_LEN_DOUBLE] = {};
            Bignum4096::mul(lhs, rhs, static_cast<uint64_t *>(mulResult));
            CONTEXT_P().mod(static_cast<uint64_t *>(mulResult), res);
        }

      private:
        uint64_t *_base;
        uint64_t _len;
        FixedBaseTable _lookupTable;
    };

    const auto generator_table = LookupTable<LUT_WINDOW_SIZE, LUT_ORDER_BITS, LUT_TABLE_LENGTH>(
      const_cast<uint64_t *>(G_ARRAY_REVERSE), MAX_P_LEN);

    const auto publicKeyTable(uint64_t *publicKey)
    {
        static auto public_key_table =
          LookupTable<LUT_WINDOW_SIZE, LUT_ORDER_BITS, LUT_TABLE_LENGTH>(publicKey, MAX_P_LEN);
        return public_key_table;
    }
} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_LOOKUP_TABLE_HPP_INCLUDED__ */