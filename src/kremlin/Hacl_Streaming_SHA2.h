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


#ifndef __Hacl_Streaming_SHA2_H
#define __Hacl_Streaming_SHA2_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "evercrypt_targetconfig.h"
#include "libintvector.h"
#include "kremlin/internal/types.h"
#include "kremlin/lowstar_endianness.h"
#include <string.h>
#include "kremlin/internal/target.h"


#include "Hacl_Kremlib.h"
#include "Hacl_Hash.h"

typedef struct Hacl_Streaming_Functor_state_s___uint32_t_____s
Hacl_Streaming_Functor_state_s___uint32_t____;

Hacl_Streaming_Functor_state_s___uint32_t____ *Hacl_Streaming_SHA2_create_in_224();

void Hacl_Streaming_SHA2_init_224(Hacl_Streaming_Functor_state_s___uint32_t____ *s);

void
Hacl_Streaming_SHA2_update_224(
  Hacl_Streaming_Functor_state_s___uint32_t____ *p,
  uint8_t *data,
  uint32_t len
);

void
Hacl_Streaming_SHA2_finish_224(Hacl_Streaming_Functor_state_s___uint32_t____ *p, uint8_t *dst);

void Hacl_Streaming_SHA2_free_224(Hacl_Streaming_Functor_state_s___uint32_t____ *s);

Hacl_Streaming_Functor_state_s___uint32_t____ *Hacl_Streaming_SHA2_create_in_256();

void Hacl_Streaming_SHA2_init_256(Hacl_Streaming_Functor_state_s___uint32_t____ *s);

void
Hacl_Streaming_SHA2_update_256(
  Hacl_Streaming_Functor_state_s___uint32_t____ *p,
  uint8_t *data,
  uint32_t len
);

void
Hacl_Streaming_SHA2_finish_256(Hacl_Streaming_Functor_state_s___uint32_t____ *p, uint8_t *dst);

void Hacl_Streaming_SHA2_free_256(Hacl_Streaming_Functor_state_s___uint32_t____ *s);

typedef struct Hacl_Streaming_Functor_state_s___uint64_t_____s
Hacl_Streaming_Functor_state_s___uint64_t____;

Hacl_Streaming_Functor_state_s___uint64_t____ *Hacl_Streaming_SHA2_create_in_384();

void Hacl_Streaming_SHA2_init_384(Hacl_Streaming_Functor_state_s___uint64_t____ *s);

void
Hacl_Streaming_SHA2_update_384(
  Hacl_Streaming_Functor_state_s___uint64_t____ *p,
  uint8_t *data,
  uint32_t len
);

void
Hacl_Streaming_SHA2_finish_384(Hacl_Streaming_Functor_state_s___uint64_t____ *p, uint8_t *dst);

void Hacl_Streaming_SHA2_free_384(Hacl_Streaming_Functor_state_s___uint64_t____ *s);

Hacl_Streaming_Functor_state_s___uint64_t____ *Hacl_Streaming_SHA2_create_in_512();

void Hacl_Streaming_SHA2_init_512(Hacl_Streaming_Functor_state_s___uint64_t____ *s);

void
Hacl_Streaming_SHA2_update_512(
  Hacl_Streaming_Functor_state_s___uint64_t____ *p,
  uint8_t *data,
  uint32_t len
);

void
Hacl_Streaming_SHA2_finish_512(Hacl_Streaming_Functor_state_s___uint64_t____ *p, uint8_t *dst);

void Hacl_Streaming_SHA2_free_512(Hacl_Streaming_Functor_state_s___uint64_t____ *s);

#if defined(__cplusplus)
}
#endif

#define __Hacl_Streaming_SHA2_H_DEFINED
#endif
