#ifndef __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__
#define __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

struct eg_plaintext_ballot_selection_s;
typedef struct eg_plaintext_ballot_selection_s eg_plaintext_ballot_selection_t;

EG_API eg_plaintext_ballot_selection_t *eg_plaintext_ballot_selection_new(const char *object_id,
                                                                          char *vote);
EG_API void eg_plaintext_ballot_selection_free(eg_plaintext_ballot_selection_t *selection);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_BALLOT_H_INCLUDED__ */