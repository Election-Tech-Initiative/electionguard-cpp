#ifndef __ELECTIONGUARD_CORE_ELECTION_H_INCLUDED__
#define __ELECTIONGUARD_CORE_ELECTION_H_INCLUDED__

#include "export.h"
#include "group.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// SelectionDescription
struct eg_selection_description_s;
typedef struct eg_selection_description_s eg_selection_description_t;

EG_API eg_selection_description_t *eg_selection_description_new(const char *object_id,
                                                                const char *candidate_id,
                                                                uint64_t sequence_order);
EG_API void eg_selection_description_free(eg_selection_description_t *selection);

EG_API char *eg_selection_description_get_object_id(eg_selection_description_t *selection);
EG_API char *eg_selection_description_get_candidate_id(eg_selection_description_t *selection);
EG_API uint64_t eg_selection_description_get_sequence_order(eg_selection_description_t *selection);

EG_API eg_element_mod_q_t *
eg_selection_description_crypto_hash(eg_selection_description_t *selection);

#ifdef __cplusplus
}
#endif
#endif /* __ELECTIONGUARD_CORE_ELECTION_H_INCLUDED__ */