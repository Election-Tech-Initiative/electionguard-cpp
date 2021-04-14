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


#include "Hacl_HMAC.h"

void
Hacl_HMAC_legacy_compute_sha1(
  uint8_t *dst,
  uint8_t *key,
  uint32_t key_len,
  uint8_t *data,
  uint32_t data_len
)
{
  uint32_t l = (uint32_t)64U;
  KRML_CHECK_SIZE(sizeof (uint8_t), l);
  {
    uint8_t *key_block = (uint8_t *)alloca(l * sizeof (uint8_t));
    memset(key_block, 0U, l * sizeof (uint8_t));
    {
      uint32_t i0;
      if (key_len <= (uint32_t)64U)
      {
        i0 = key_len;
      }
      else
      {
        i0 = (uint32_t)20U;
      }
      {
        uint8_t *nkey = key_block;
        if (key_len <= (uint32_t)64U)
        {
          memcpy(nkey, key, key_len * sizeof (uint8_t));
        }
        else
        {
          Hacl_Hash_SHA1_legacy_hash(key, key_len, nkey);
        }
        KRML_CHECK_SIZE(sizeof (uint8_t), l);
        {
          uint8_t *ipad = (uint8_t *)alloca(l * sizeof (uint8_t));
          memset(ipad, (uint8_t)0x36U, l * sizeof (uint8_t));
          {
            uint32_t i;
            for (i = (uint32_t)0U; i < l; i++)
            {
              uint8_t xi = ipad[i];
              uint8_t yi = key_block[i];
              ipad[i] = xi ^ yi;
            }
          }
          KRML_CHECK_SIZE(sizeof (uint8_t), l);
          {
            uint8_t *opad = (uint8_t *)alloca(l * sizeof (uint8_t));
            memset(opad, (uint8_t)0x5cU, l * sizeof (uint8_t));
            {
              uint32_t scrut[5];
              uint32_t *s;
              uint8_t *dst1;
              uint8_t *hash1;
              {
                uint32_t i;
                for (i = (uint32_t)0U; i < l; i++)
                {
                  uint8_t xi = opad[i];
                  uint8_t yi = key_block[i];
                  opad[i] = xi ^ yi;
                }
              }
              scrut[0U] = (uint32_t)0x67452301U;
              scrut[1U] = (uint32_t)0xefcdab89U;
              scrut[2U] = (uint32_t)0x98badcfeU;
              scrut[3U] = (uint32_t)0x10325476U;
              scrut[4U] = (uint32_t)0xc3d2e1f0U;
              s = scrut;
              dst1 = ipad;
              Hacl_Hash_Core_SHA1_legacy_init(s);
              if (data_len == (uint32_t)0U)
              {
                Hacl_Hash_SHA1_legacy_update_last(s, (uint64_t)0U, ipad, (uint32_t)64U);
              }
              else
              {
                Hacl_Hash_SHA1_legacy_update_multi(s, ipad, (uint32_t)1U);
                Hacl_Hash_SHA1_legacy_update_last(s, (uint64_t)(uint32_t)64U, data, data_len);
              }
              Hacl_Hash_Core_SHA1_legacy_finish(s, dst1);
              hash1 = ipad;
              Hacl_Hash_Core_SHA1_legacy_init(s);
              if ((uint32_t)20U == (uint32_t)0U)
              {
                Hacl_Hash_SHA1_legacy_update_last(s, (uint64_t)0U, opad, (uint32_t)64U);
              }
              else
              {
                Hacl_Hash_SHA1_legacy_update_multi(s, opad, (uint32_t)1U);
                Hacl_Hash_SHA1_legacy_update_last(s, (uint64_t)(uint32_t)64U, hash1, (uint32_t)20U);
              }
              Hacl_Hash_Core_SHA1_legacy_finish(s, dst);
            }
          }
        }
      }
    }
  }
}

void
Hacl_HMAC_compute_sha2_256(
  uint8_t *dst,
  uint8_t *key,
  uint32_t key_len,
  uint8_t *data,
  uint32_t data_len
)
{
  uint32_t l = (uint32_t)64U;
  KRML_CHECK_SIZE(sizeof (uint8_t), l);
  {
    uint8_t *key_block = (uint8_t *)alloca(l * sizeof (uint8_t));
    memset(key_block, 0U, l * sizeof (uint8_t));
    {
      uint32_t i0;
      if (key_len <= (uint32_t)64U)
      {
        i0 = key_len;
      }
      else
      {
        i0 = (uint32_t)32U;
      }
      {
        uint8_t *nkey = key_block;
        if (key_len <= (uint32_t)64U)
        {
          memcpy(nkey, key, key_len * sizeof (uint8_t));
        }
        else
        {
          Hacl_Hash_SHA2_hash_256(key, key_len, nkey);
        }
        KRML_CHECK_SIZE(sizeof (uint8_t), l);
        {
          uint8_t *ipad = (uint8_t *)alloca(l * sizeof (uint8_t));
          memset(ipad, (uint8_t)0x36U, l * sizeof (uint8_t));
          {
            uint32_t i;
            for (i = (uint32_t)0U; i < l; i++)
            {
              uint8_t xi = ipad[i];
              uint8_t yi = key_block[i];
              ipad[i] = xi ^ yi;
            }
          }
          KRML_CHECK_SIZE(sizeof (uint8_t), l);
          {
            uint8_t *opad = (uint8_t *)alloca(l * sizeof (uint8_t));
            memset(opad, (uint8_t)0x5cU, l * sizeof (uint8_t));
            {
              uint32_t scrut[8];
              uint32_t *s;
              uint8_t *dst1;
              uint8_t *hash1;
              {
                uint32_t i;
                for (i = (uint32_t)0U; i < l; i++)
                {
                  uint8_t xi = opad[i];
                  uint8_t yi = key_block[i];
                  opad[i] = xi ^ yi;
                }
              }
              scrut[0U] = (uint32_t)0x6a09e667U;
              scrut[1U] = (uint32_t)0xbb67ae85U;
              scrut[2U] = (uint32_t)0x3c6ef372U;
              scrut[3U] = (uint32_t)0xa54ff53aU;
              scrut[4U] = (uint32_t)0x510e527fU;
              scrut[5U] = (uint32_t)0x9b05688cU;
              scrut[6U] = (uint32_t)0x1f83d9abU;
              scrut[7U] = (uint32_t)0x5be0cd19U;
              s = scrut;
              dst1 = ipad;
              Hacl_Hash_Core_SHA2_init_256(s);
              if (data_len == (uint32_t)0U)
              {
                Hacl_Hash_SHA2_update_last_256(s, (uint64_t)0U, ipad, (uint32_t)64U);
              }
              else
              {
                Hacl_Hash_SHA2_update_multi_256(s, ipad, (uint32_t)1U);
                Hacl_Hash_SHA2_update_last_256(s, (uint64_t)(uint32_t)64U, data, data_len);
              }
              Hacl_Hash_Core_SHA2_finish_256(s, dst1);
              hash1 = ipad;
              Hacl_Hash_Core_SHA2_init_256(s);
              if ((uint32_t)32U == (uint32_t)0U)
              {
                Hacl_Hash_SHA2_update_last_256(s, (uint64_t)0U, opad, (uint32_t)64U);
              }
              else
              {
                Hacl_Hash_SHA2_update_multi_256(s, opad, (uint32_t)1U);
                Hacl_Hash_SHA2_update_last_256(s, (uint64_t)(uint32_t)64U, hash1, (uint32_t)32U);
              }
              Hacl_Hash_Core_SHA2_finish_256(s, dst);
            }
          }
        }
      }
    }
  }
}

void
Hacl_HMAC_compute_sha2_384(
  uint8_t *dst,
  uint8_t *key,
  uint32_t key_len,
  uint8_t *data,
  uint32_t data_len
)
{
  uint32_t l = (uint32_t)128U;
  KRML_CHECK_SIZE(sizeof (uint8_t), l);
  {
    uint8_t *key_block = (uint8_t *)alloca(l * sizeof (uint8_t));
    memset(key_block, 0U, l * sizeof (uint8_t));
    {
      uint32_t i0;
      if (key_len <= (uint32_t)128U)
      {
        i0 = key_len;
      }
      else
      {
        i0 = (uint32_t)48U;
      }
      {
        uint8_t *nkey = key_block;
        if (key_len <= (uint32_t)128U)
        {
          memcpy(nkey, key, key_len * sizeof (uint8_t));
        }
        else
        {
          Hacl_Hash_SHA2_hash_384(key, key_len, nkey);
        }
        KRML_CHECK_SIZE(sizeof (uint8_t), l);
        {
          uint8_t *ipad = (uint8_t *)alloca(l * sizeof (uint8_t));
          memset(ipad, (uint8_t)0x36U, l * sizeof (uint8_t));
          {
            uint32_t i;
            for (i = (uint32_t)0U; i < l; i++)
            {
              uint8_t xi = ipad[i];
              uint8_t yi = key_block[i];
              ipad[i] = xi ^ yi;
            }
          }
          KRML_CHECK_SIZE(sizeof (uint8_t), l);
          {
            uint8_t *opad = (uint8_t *)alloca(l * sizeof (uint8_t));
            memset(opad, (uint8_t)0x5cU, l * sizeof (uint8_t));
            {
              uint64_t scrut[8];
              uint64_t *s;
              uint8_t *dst1;
              uint8_t *hash1;
              {
                uint32_t i;
                for (i = (uint32_t)0U; i < l; i++)
                {
                  uint8_t xi = opad[i];
                  uint8_t yi = key_block[i];
                  opad[i] = xi ^ yi;
                }
              }
              scrut[0U] = (uint64_t)0xcbbb9d5dc1059ed8U;
              scrut[1U] = (uint64_t)0x629a292a367cd507U;
              scrut[2U] = (uint64_t)0x9159015a3070dd17U;
              scrut[3U] = (uint64_t)0x152fecd8f70e5939U;
              scrut[4U] = (uint64_t)0x67332667ffc00b31U;
              scrut[5U] = (uint64_t)0x8eb44a8768581511U;
              scrut[6U] = (uint64_t)0xdb0c2e0d64f98fa7U;
              scrut[7U] = (uint64_t)0x47b5481dbefa4fa4U;
              s = scrut;
              dst1 = ipad;
              Hacl_Hash_Core_SHA2_init_384(s);
              if (data_len == (uint32_t)0U)
              {
                Hacl_Hash_SHA2_update_last_384(s,
                  FStar_UInt128_uint64_to_uint128((uint64_t)0U),
                  ipad,
                  (uint32_t)128U);
              }
              else
              {
                Hacl_Hash_SHA2_update_multi_384(s, ipad, (uint32_t)1U);
                Hacl_Hash_SHA2_update_last_384(s,
                  FStar_UInt128_uint64_to_uint128((uint64_t)(uint32_t)128U),
                  data,
                  data_len);
              }
              Hacl_Hash_Core_SHA2_finish_384(s, dst1);
              hash1 = ipad;
              Hacl_Hash_Core_SHA2_init_384(s);
              if ((uint32_t)48U == (uint32_t)0U)
              {
                Hacl_Hash_SHA2_update_last_384(s,
                  FStar_UInt128_uint64_to_uint128((uint64_t)0U),
                  opad,
                  (uint32_t)128U);
              }
              else
              {
                Hacl_Hash_SHA2_update_multi_384(s, opad, (uint32_t)1U);
                Hacl_Hash_SHA2_update_last_384(s,
                  FStar_UInt128_uint64_to_uint128((uint64_t)(uint32_t)128U),
                  hash1,
                  (uint32_t)48U);
              }
              Hacl_Hash_Core_SHA2_finish_384(s, dst);
            }
          }
        }
      }
    }
  }
}

void
Hacl_HMAC_compute_sha2_512(
  uint8_t *dst,
  uint8_t *key,
  uint32_t key_len,
  uint8_t *data,
  uint32_t data_len
)
{
  uint32_t l = (uint32_t)128U;
  KRML_CHECK_SIZE(sizeof (uint8_t), l);
  {
    uint8_t *key_block = (uint8_t *)alloca(l * sizeof (uint8_t));
    memset(key_block, 0U, l * sizeof (uint8_t));
    {
      uint32_t i0;
      if (key_len <= (uint32_t)128U)
      {
        i0 = key_len;
      }
      else
      {
        i0 = (uint32_t)64U;
      }
      {
        uint8_t *nkey = key_block;
        if (key_len <= (uint32_t)128U)
        {
          memcpy(nkey, key, key_len * sizeof (uint8_t));
        }
        else
        {
          Hacl_Hash_SHA2_hash_512(key, key_len, nkey);
        }
        KRML_CHECK_SIZE(sizeof (uint8_t), l);
        {
          uint8_t *ipad = (uint8_t *)alloca(l * sizeof (uint8_t));
          memset(ipad, (uint8_t)0x36U, l * sizeof (uint8_t));
          {
            uint32_t i;
            for (i = (uint32_t)0U; i < l; i++)
            {
              uint8_t xi = ipad[i];
              uint8_t yi = key_block[i];
              ipad[i] = xi ^ yi;
            }
          }
          KRML_CHECK_SIZE(sizeof (uint8_t), l);
          {
            uint8_t *opad = (uint8_t *)alloca(l * sizeof (uint8_t));
            memset(opad, (uint8_t)0x5cU, l * sizeof (uint8_t));
            {
              uint64_t scrut[8];
              uint64_t *s;
              uint8_t *dst1;
              uint8_t *hash1;
              {
                uint32_t i;
                for (i = (uint32_t)0U; i < l; i++)
                {
                  uint8_t xi = opad[i];
                  uint8_t yi = key_block[i];
                  opad[i] = xi ^ yi;
                }
              }
              scrut[0U] = (uint64_t)0x6a09e667f3bcc908U;
              scrut[1U] = (uint64_t)0xbb67ae8584caa73bU;
              scrut[2U] = (uint64_t)0x3c6ef372fe94f82bU;
              scrut[3U] = (uint64_t)0xa54ff53a5f1d36f1U;
              scrut[4U] = (uint64_t)0x510e527fade682d1U;
              scrut[5U] = (uint64_t)0x9b05688c2b3e6c1fU;
              scrut[6U] = (uint64_t)0x1f83d9abfb41bd6bU;
              scrut[7U] = (uint64_t)0x5be0cd19137e2179U;
              s = scrut;
              dst1 = ipad;
              Hacl_Hash_Core_SHA2_init_512(s);
              if (data_len == (uint32_t)0U)
              {
                Hacl_Hash_SHA2_update_last_512(s,
                  FStar_UInt128_uint64_to_uint128((uint64_t)0U),
                  ipad,
                  (uint32_t)128U);
              }
              else
              {
                Hacl_Hash_SHA2_update_multi_512(s, ipad, (uint32_t)1U);
                Hacl_Hash_SHA2_update_last_512(s,
                  FStar_UInt128_uint64_to_uint128((uint64_t)(uint32_t)128U),
                  data,
                  data_len);
              }
              Hacl_Hash_Core_SHA2_finish_512(s, dst1);
              hash1 = ipad;
              Hacl_Hash_Core_SHA2_init_512(s);
              if ((uint32_t)64U == (uint32_t)0U)
              {
                Hacl_Hash_SHA2_update_last_512(s,
                  FStar_UInt128_uint64_to_uint128((uint64_t)0U),
                  opad,
                  (uint32_t)128U);
              }
              else
              {
                Hacl_Hash_SHA2_update_multi_512(s, opad, (uint32_t)1U);
                Hacl_Hash_SHA2_update_last_512(s,
                  FStar_UInt128_uint64_to_uint128((uint64_t)(uint32_t)128U),
                  hash1,
                  (uint32_t)64U);
              }
              Hacl_Hash_Core_SHA2_finish_512(s, dst);
            }
          }
        }
      }
    }
  }
}

