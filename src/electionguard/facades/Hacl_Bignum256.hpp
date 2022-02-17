#ifndef __FACADES__Hacl_Bignum256_H_INCLUDED__
#define __FACADES__Hacl_Bignum256_H_INCLUDED__

#include "../../kremlin/Hacl_Bignum256.h"
#include "electionguard/export.h"

#include <cstdint>
#include <vector>

namespace hacl
{
    /// <summary>
    /// wrapper class for calling 256-bit hacl functions.
    //
    /// Instantiating this class creates a montgomery context
    /// that can be cached and reused to improve performance of mod and modexp functions
    /// </summary>
    class EG_INTERNAL_API Bignum256
    {
      public:
        explicit Bignum256(uint64_t *elem);
        ~Bignum256();

        static uint64_t add(uint64_t *a, uint64_t *b, uint64_t *res);

        static uint64_t sub(uint64_t *a, uint64_t *b, uint64_t *res);

        static void mul(uint64_t *a, uint64_t *b, uint64_t *res);

        static bool mod(uint64_t *n, uint64_t *a, uint64_t *res);

        static bool modExp(uint64_t *n, uint64_t *a, uint32_t bBits, uint64_t *b, uint64_t *res,
                           bool useConstTime = false);

        static uint64_t *fromBytes(uint32_t len, uint8_t *bytes);

        static std::vector<uint8_t> toBytes(uint64_t *bytes);
        static void toBytes(uint64_t *bytes, uint8_t *res);

        static uint64_t lessThan(uint64_t *a, uint64_t *b);

        void mod(uint64_t *a, uint64_t *res) const;

        void modExp(uint64_t *a, uint32_t bBits, uint64_t *b, uint64_t *res,
                    bool useConstTime = false) const;

      private:
        Hacl_Bignum_MontArithmetic_bn_mont_ctx_u64 *context;
    };

    /// <summary>
    /// A Bignum256 instance initialized with the small prime montgomery context
    /// </summary>
    const EG_INTERNAL_API Bignum256 &CONTEXT_Q();
} // namespace hacl

#endif /* __FACADES__Hacl_Bignum256_H_INCLUDED__ */
