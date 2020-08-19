#include "electionguard/ballot.hpp"
#include "electionguard/group.hpp"

extern "C" {
#include "electionguard/ballot.h"
}

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)

eg_plaintext_ballot_selection_t *eg_plaintext_ballot_selection_new(const char *object_id,
                                                                   char *vote)
{
    return AS_TYPE(eg_plaintext_ballot_selection_t,
                   new electionguard::PlaintextBallotSelection(object_id, vote));
}

void eg_plaintext_ballot_selection_free(eg_plaintext_ballot_selection_t *selection)
{
    if (!selection)
        return;
    delete AS_TYPE(electionguard::PlaintextBallotSelection, selection);
}

eg_ciphertext_ballot_selection_t *
eg_ciphertext_ballot_selection_new(const char *object_id, eg_element_mod_q_t *description_hash)
{
    auto descriptionHash = AS_TYPE(electionguard::ElementModQ, description_hash);
    return AS_TYPE(eg_ciphertext_ballot_selection_t,
                   new electionguard::CiphertextBallotSelection(object_id, descriptionHash));
}

void eg_ciphertext_ballot_selection_free(eg_ciphertext_ballot_selection_t *selection)
{
    if (!selection)
        return;
    delete AS_TYPE(electionguard::CiphertextBallotSelection, selection);
}