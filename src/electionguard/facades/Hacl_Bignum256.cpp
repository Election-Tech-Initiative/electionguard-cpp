#include "Hacl_Bignum256.hpp"

#include "../../kremlin/Hacl_Bignum256.h"

namespace hacl
{
    uint64_t Bignum256::add(uint64_t *a, uint64_t *b, uint64_t *res)
    {
        return Hacl_Bignum256_add(a, b, res);
    }

    uint64_t Bignum256::sub(uint64_t *a, uint64_t *b, uint64_t *res)
    {
        return Hacl_Bignum256_sub(a, b, res);
    }

    void Bignum256::mul(uint64_t *a, uint64_t *b, uint64_t *res) { Hacl_Bignum256_mul(a, b, res); }

    bool Bignum256::mod(uint64_t *n, uint64_t *a, uint64_t *res)
    {
        return Hacl_Bignum256_mod(n, a, res);
    }

    bool Bignum256::modExp(uint64_t *n, uint64_t *a, uint32_t bBits, uint64_t *b, uint64_t *res)
    {
        if (bBits <= 0) {
            return false;
        }
        return Hacl_Bignum256_mod_exp_consttime(n, a, bBits, b, res);
    }

    uint64_t *Bignum256::fromBytes(uint32_t len, uint8_t *bytes)
    {
        return Hacl_Bignum256_new_bn_from_bytes_be(len, bytes);
    }

    void Bignum256::toBytes(uint64_t *bytes, uint8_t *res)
    {
        return Hacl_Bignum256_bn_to_bytes_be(bytes, res);
    }

    uint64_t Bignum256::lessThan(uint64_t *a, uint64_t *b) { return Hacl_Bignum256_lt_mask(a, b); }
} // namespace hacl