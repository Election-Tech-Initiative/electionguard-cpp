#ifndef __ELECTIONGUARD_CORE_EXPORT_H_INCLUDED__
#define __ELECTIONGUARD_CORE_EXPORT_H_INCLUDED__

/* Use the windows pattern to export suymbols */

#ifndef EG_API
#    ifdef _WIN32
#        if defined(ELECTIONGUARD_BUILD_SHARED) /* build dll */
#            define EG_API __declspec(dllexport)
#            if defined(DEBUG)
#                define EG_INTERNAL_API __declspec(dllexport)
#            endif
#        elif !defined(ELECTIONGUARD_BUILD_STATIC) /* use dll */
#            define EG_API __declspec(dllimport)
#            ifdef DEBUG
#                define EG_INTERNAL_API __declspec(dllimport)
#            endif
#        else /* static library */
#            define EG_API
#        endif
#        ifndef EG_INTERNAL_API
#            define EG_INTERNAL_API
#        endif
#    else
#        if __GNUC__ >= 4
#            define EG_API __attribute__((visibility("default")))
#            ifdef DEBUG
#                define EG_INTERNAL_API __attribute__((visibility("default")))
#            else
#                define EG_INTERNAL_API
#            endif
#        else
#            define EG_API
#            define EG_INTERNAL_API
#        endif
#    endif
#endif

#endif /* __ELECTIONGUARD_CORE_EXPORT_H_INCLUDED__ */