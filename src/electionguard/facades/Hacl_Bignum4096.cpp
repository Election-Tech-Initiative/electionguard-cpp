#include "Hacl_Bignum4096.hpp"

#include "../../kremlin/Hacl_Bignum4096.h"

namespace hacl
{
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

    bool Bignum4096::modExp(uint64_t *n, uint64_t *a, uint32_t bBits, uint64_t *b, uint64_t *res)
    {
        if (bBits <= 0) {
            return false;
        }
        return Hacl_Bignum4096_mod_exp_consttime(n, a, bBits, b, res);
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
} // namespace hacl