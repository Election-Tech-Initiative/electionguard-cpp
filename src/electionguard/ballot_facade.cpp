#include "electionguard/ballot.hpp"

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