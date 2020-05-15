#ifndef __ELECTIONGUARD_CORE_H_INCLUDED__
#define __ELECTIONGUARD_CORE_H_INCLUDED__

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

struct eg_encryption_compositor_s;
typedef struct eg_encryption_compositor_s eg_encryption_compositor_t;

EG_API eg_encryption_compositor_t *eg_encryption_compositor_new();
EG_API void eg_encryption_compositor_free(eg_encryption_compositor_t *composer);
EG_API int
eg_encryption_compositor_encrypt(eg_encryption_compositor_t *composer);

EG_API int eg_encrypt_ballot();

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_H_INCLUDED__ */
