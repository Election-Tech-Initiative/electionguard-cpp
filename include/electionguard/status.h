#ifndef __ELECTIONGUARD_CORE_STATUS_H_INCLUDED__
#define __ELECTIONGUARD_CORE_STATUS_H_INCLUDED__

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum eg_electionguard_status_e {
    ELECTIONGUARD_STATUS_SUCCESS = 0,
    ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT,
    ELECTIONGUARD_STATUS_ERROR_OUT_OF_RANGE,
    ELECTIONGUARD_STATUS_ERROR_IO_ERROR,
    ELECTIONGUARD_STATUS_ERROR_BAD_ACCESS,
    ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC,
    ELECTIONGUARD_STATUS_ERROR_ALREADY_EXISTS,
    ELECTIONGUARD_STATUS_ERROR_RUNTIME_ERROR,

    /// This code should always be the last code in the collection
    // so that the status codes string can be correctly derived
    ELECTIONGUARD_STATUS_UNKNOWN
} eg_electionguard_status_t;

static const char *const eg_electionguard_status_codes[] = {
  "ELECTIONGUARD_STATUS_SUCCESS",
  "ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT",
  "ELECTIONGUARD_STATUS_ERROR_OUT_OF_RANGE",
  "ELECTIONGUARD_STATUS_ERROR_IO_ERROR",
  "ELECTIONGUARD_STATUS_ERROR_BAD_ACCESS",
  "ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC",
  "ELECTIONGUARD_STATUS_ERROR_ALREADY_EXISTS",
  "ELECTIONGUARD_STATUS_ERROR_RUNTIME_ERROR",
  "ELECTIONGUARD_STATUS_UNKNOWN"};

static const char *eg_electionguard_get_status(eg_electionguard_status_t status)
{
    if (status >= ELECTIONGUARD_STATUS_UNKNOWN) {
        eg_electionguard_status_codes[ELECTIONGUARD_STATUS_UNKNOWN];
    }
    return eg_electionguard_status_codes[status];
}

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_STATUS_H_INCLUDED__ */
