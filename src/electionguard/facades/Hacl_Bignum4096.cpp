#include "Hacl_Bignum4096.hpp"

#include "../../kremlin/Hacl_Bignum4096.h"
#include "../log.hpp"

using electionguard::Log;

namespace hacl
{
    Bignum4096::Bignum4096(uint64_t *elem) { context = Hacl_Bignum4096_mont_ctx_init(elem); }
    Bignum4096::~Bignum4096() { Hacl_Bignum4096_mont_ctx_free(context); }

    uint64_t Bignum4096::add(uint64_t *a, uint64_t *b, uint64_t *res)
    {
        return Hacl_Bignum4096_add(a, b, res);
    }

    uint64_t Bignum4096::sub(uint64_t *a, uint64_t *b, uint64_t *res)
    {
        return Hacl_Bignum4096_sub(a, b, res);
    }

    void Bignum4096::mul(uint64_t *a, uint64_t *b, uint64_t *res)
    {
        Hacl_Bignum4096_mul(a, b, res);
    }

    bool Bignum4096::mod(uint64_t *n, uint64_t *a, uint64_t *res)
    {
        return Hacl_Bignum4096_mod(n, a, res);
    }

    bool Bignum4096::modExp(uint64_t *n, uint64_t *a, uint32_t bBits, uint64_t *b, uint64_t *res,
                            bool useConstTime /* = true */)
    {
        if (bBits <= 0) {
            Log::trace("Bignum4096::modExp:: bbits <= 0");
            return false;
        }
        if (useConstTime) {
            return Hacl_Bignum4096_mod_exp_consttime(n, a, bBits, b, res);
        }
        return Hacl_Bignum4096_mod_exp_vartime(n, a, bBits, b, res);
    }

    uint64_t *Bignum4096::fromBytes(uint32_t len, uint8_t *bytes)
    {
        return Hacl_Bignum4096_new_bn_from_bytes_be(len, bytes);
    }

    void Bignum4096::toBytes(uint64_t *bytes, uint8_t *res)
    {
        return Hacl_Bignum4096_bn_to_bytes_be(bytes, res);
    }

    uint64_t Bignum4096::lessThan(uint64_t *a, uint64_t *b)
    {
        return Hacl_Bignum4096_lt_mask(a, b);
    }

    void Bignum4096::mod(uint64_t *a, uint64_t *res) const
    {
        Hacl_Bignum4096_mod_precomp(context, a, res);
    }

    void Bignum4096::modExp(uint64_t *a, uint32_t bBits, uint64_t *b, uint64_t *res,
                            bool useConstTime /* = true */) const
    {
        if (bBits <= 0) {
            Log::trace("Bignum4096::modExp:: bbits <= 0");
            return throw;
        }
        if (useConstTime) {
            return Hacl_Bignum4096_mod_exp_consttime_precomp(context, a, bBits, b, res);
        }
        return Hacl_Bignum4096_mod_exp_vartime_precomp(context, a, bBits, b, res);
    }

    const Bignum4096 &CONTEXT_P()
    {
        static Bignum4096 instance{const_cast<uint64_t *>(P_ARRAY_REVERSE)};
        return instance;
    }
} // namespace hacl
