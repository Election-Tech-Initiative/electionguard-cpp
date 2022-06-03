#ifndef __FACADES__Hacl_Bignum4096_H_INCLUDED__
#define __FACADES__Hacl_Bignum4096_H_INCLUDED__

#ifdef _WIN32
#include "../../karamel/Hacl_Bignum4096_32.h"
#endif // _WIN32
#include "../../karamel/Hacl_Bignum4096.h"
#include "electionguard/export.h"

#include <cstdint>
#include <memory>

namespace hacl
{
    /// <summary>
    /// wrapper class for calling 4096-bit hacl functions.
    //
    /// Instantiating this class creates a montgomery context
    /// that can be cached and reused to improve performance of mod and modexp functions
    /// </summary>
    class EG_INTERNAL_API Bignum4096
    {
      public:
#ifdef _WIN32
        explicit Bignum4096(uint32_t *elem);
#else
        explicit Bignum4096(uint64_t *elem);
#endif // _WIN32
        ~Bignum4096();

        static uint64_t add(uint64_t *a, uint64_t *b, uint64_t *res);

        static uint64_t sub(uint64_t *a, uint64_t *b, uint64_t *res);

        static void mul(uint64_t *a, uint64_t *b, uint64_t *res);

        /// <ummary>
        /// Write `a mod n` in `res`.
        ///
        ///  The argument a is meant to be a 8192-bit bignum, i.e. uint64_t[128].
        ///  The argument n and the outparam res are meant to be 4096-bit bignums, i.e. uint64_t[64].
        ///
        ///  The function returns false if any of the following preconditions are violated,
        ///  true otherwise.
        ///  • 1 < n
        ///  • n % 2 = 1
        /// </summary>
        static bool mod(uint64_t *n, uint64_t *a, uint64_t *res);

        static bool modExp(uint64_t *n, uint64_t *a, uint32_t bBits, uint64_t *b, uint64_t *res,
                           bool useConstTime = false);

        static uint64_t *fromBytes(uint32_t len, uint8_t *bytes);

        static void toBytes(uint64_t *bytes, uint8_t *res);

        static uint64_t lessThan(uint64_t *a, uint64_t *b);

        void mod(uint64_t *a, uint64_t *res) const;

        void modExp(uint64_t *a, uint32_t bBits, uint64_t *b, uint64_t *res,
                    bool useConstTime = false) const;

        void to_montgomery_form(uint64_t *a, uint64_t *aM) const;

        void from_montgomery_form(uint64_t *aM, uint64_t *a) const;

        void montgomery_mod_mul_stay_in_mont_form(uint64_t *aM, uint64_t *bM,
                                                         uint64_t *cM) const;

      private:
        struct handle_destructor {
#ifdef _WIN32
            void operator()(Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *handle) const
            {
                Hacl_Bignum4096_32_mont_ctx_free(handle);
            }
#else
            void operator()(Hacl_Bignum_MontArithmetic_bn_mont_ctx_u64 *handle) const
            {
                Hacl_Bignum4096_mont_ctx_free(handle);
            }
#endif // _WIN32
        };
#ifdef _WIN32
        typedef std::unique_ptr<Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32, handle_destructor>
          HaclBignumContext4096;
#else
        typedef std::unique_ptr<Hacl_Bignum_MontArithmetic_bn_mont_ctx_u64, handle_destructor>
          HaclBignumContext4096;
#endif // _WIN32
        HaclBignumContext4096 context;
    };

    /// <summary>
    /// A Bignum4096 instance initialized with the large prime montgomery context
    /// </summary>
    const EG_INTERNAL_API Bignum4096 &CONTEXT_P();
} // namespace hacl

#endif /* __FACADES__Hacl_Bignum256_H_INCLUDED__ */
