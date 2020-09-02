/* MIT License
 *
 * Copyright (c) 2016-2020 INRIA, CMU and Microsoft Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef __Hacl_Bignum256_H
#define __Hacl_Bignum256_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "evercrypt_targetconfig.h"
#include "lib_intrinsics.h"
#include "libintvector.h"
#include "kremlin/internal/types.h"
#include "kremlin/lowstar_endianness.h"
#include <string.h>
#include "kremlin/internal/target.h"


#include "Hacl_Kremlib.h"
#include "Hacl_Bignum.h"

/************************/
/* Arithmetic functions */
/************************/


/*
Write `a + b mod 2^256` in `res`.

  This functions returns the carry.

  The arguments a, b and res are meant to be 256-bit bignums, i.e. uint64_t[4]
*/
uint64_t Hacl_Bignum256_add(uint64_t *a, uint64_t *b, uint64_t *res);

/*
Write `a - b mod 2^256` in `res`.

  This functions returns the carry.

  The arguments a, b and res are meant to be 256-bit bignums, i.e. uint64_t[4]
*/
uint64_t Hacl_Bignum256_sub(uint64_t *a, uint64_t *b, uint64_t *res);

/*
Write `a * b` in `res`.

  The arguments a and b are meant to be 256-bit bignums, i.e. uint64_t[4].
  The outparam res is meant to be a 512-bit bignum, i.e. uint64_t[8].
*/
void Hacl_Bignum256_mul(uint64_t *a, uint64_t *b, uint64_t *res);

/*
Write `a ^ b mod n1` in `res`.

  The arguments a, n1 and the outparam res are meant to be 256-bit bignums, i.e. uint64_t[4].
  The argument b is a bignum of any size, and bBits is an upper bound on the
  number of significant bits of b. For instance, if b is a 256-bit bignum,
  bBits should be 256.
*/
void
Hacl_Bignum256_mod_exp(uint64_t *n, uint64_t *a, uint32_t bBits, uint64_t *b, uint64_t *res);


/********************/
/* Loads and stores */
/********************/


/*
Load a bid-endian bignum from memory.

  The argument b points to len bytes of valid memory.
  The function returns a heap-allocated bignum of size sufficient to hold the
    result of loading b, or NULL if the amount of required memory would exceed 4GB.

  If the return value is non-null, clients must eventually call free(3) on it to
  avoid memory leaks.
*/
uint64_t *Hacl_Bignum256_new_bn_from_bytes_be(uint32_t len, uint8_t *b);

/*
Serialize a bignum into big-endian memory.

  The argument b points to a 256-bit bignum.
  The outparam res points to 32 bytes of valid memory.
*/
void Hacl_Bignum256_bn_to_bytes_be(uint64_t *b, uint8_t *res);


/***************/
/* Comparisons */
/***************/


/*
Returns true if and only if argument a is strictly less than the argument b.
*/
bool Hacl_Bignum256_lt(uint64_t *a, uint64_t *b);

#if defined(__cplusplus)
}
#endif

#define __Hacl_Bignum256_H_DEFINED
#endif
