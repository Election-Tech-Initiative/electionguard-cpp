#ifndef __ELECTIONGUARD_CORE_H_INCLUDED__
#define __ELECTIONGUARD_CORE_H_INCLUDED__

#ifndef EG_API
#  ifdef _WIN32
#     if defined(ELECTIONGUARD_BUILD_SHARED) /* build dll */
#         define EG_API __declspec(dllexport)
#     elif !defined(ELECTIONGUARD_BUILD_STATIC) /* use dll */
#         define EG_API __declspec(dllimport)
#     else /* static library */
#         define EG_API
#     endif
#  else
#     if __GNUC__ >= 4
#         define EG_API __attribute__((visibility("default")))
#     else
#         define EG_API
#     endif
#  endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

struct eg_encryption_compositor_s;
typedef struct eg_encryption_compositor_s eg_encryption_compositor_t;

EG_API eg_encryption_compositor_t *eg_encryption_compositor_new();
EG_API void eg_encryption_compositor_free(eg_encryption_compositor_t *composer);
EG_API int eg_encryption_compositor_encrypt(eg_encryption_compositor_t *composer);

EG_API int eg_encrypt_ballot();

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_H_INCLUDED__ */
