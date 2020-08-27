#include "electionguard/election.hpp"

extern "C" {

#include "electionguard/election.h"
}

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)

EG_API eg_selection_description_t *
eg_selection_description_new(const char *object_id, char *candidate_id, uint64_t sequence_order)
{
    return AS_TYPE(eg_selection_description_t, new electionguard::SelectionDescription(
                                                 object_id, candidate_id, sequence_order));
}

EG_API void eg_selection_description_free(eg_selection_description_t *selection)
{
    if (selection == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(electionguard::SelectionDescription, selection);
}

EG_API char *eg_selection_description_get_object_id(eg_selection_description_t *selection)
{
    return AS_TYPE(electionguard::SelectionDescription, selection)->getObjectId();
}

EG_API char *eg_selection_description_get_candidate_id(eg_selection_description_t *selection)
{
    return AS_TYPE(electionguard::SelectionDescription, selection)->getCandidateId();
}

EG_API uint64_t eg_selection_description_get_sequence_order(eg_selection_description_t *selection)
{
    return AS_TYPE(electionguard::SelectionDescription, selection)->getSequenceOrder();
}

EG_API eg_element_mod_q_t *
eg_selection_description_crypto_hash(eg_selection_description_t *selection)
{
    auto *crypto_hash = AS_TYPE(electionguard::SelectionDescription, selection)->crypto_hash();
    return AS_TYPE(eg_element_mod_q_t, crypto_hash);
}