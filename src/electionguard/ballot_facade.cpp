#include "electionguard/ballot.hpp"
#include "electionguard/group.hpp"

extern "C" {
#include "electionguard/ballot.h"
}

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)

/// PlaintextBallotSelection
eg_plaintext_ballot_selection_t *eg_plaintext_ballot_selection_new(const char *object_id,
                                                                   const char *vote)
{
    return AS_TYPE(eg_plaintext_ballot_selection_t,
                   new electionguard::PlaintextBallotSelection(object_id, vote));
}

void eg_plaintext_ballot_selection_free(eg_plaintext_ballot_selection_t *selection)
{
    if (selection == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(electionguard::PlaintextBallotSelection, selection);
}

char *eg_plaintext_ballot_selection_get_object_id(eg_plaintext_ballot_selection_t *selection)
{
    return AS_TYPE(electionguard::PlaintextBallotSelection, selection)->getObjectId();
}

/// CiphertextBallotSelection
eg_ciphertext_ballot_selection_t *
eg_ciphertext_ballot_selection_new(const char *object_id, eg_element_mod_q_t *description_hash)
{
    auto *descriptionHash = AS_TYPE(electionguard::ElementModQ, description_hash);
    return AS_TYPE(eg_ciphertext_ballot_selection_t,
                   new electionguard::CiphertextBallotSelection(object_id, descriptionHash));
}

void eg_ciphertext_ballot_selection_free(eg_ciphertext_ballot_selection_t *selection)
{
    if (selection == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(electionguard::CiphertextBallotSelection, selection);
}

char *eg_ciphertext_ballot_selection_get_object_id(eg_ciphertext_ballot_selection_t *selection)
{
    auto *ciphertext = AS_TYPE(electionguard::CiphertextBallotSelection, selection);
    return ciphertext->getObjectId();
}

EG_API eg_element_mod_q_t *
eg_ciphertext_ballot_selection_get_description_hash(eg_ciphertext_ballot_selection_t *selection)
{
    auto *descriptionHash =
      AS_TYPE(electionguard::CiphertextBallotSelection, selection)->getDescriptionHash();
    return AS_TYPE(eg_element_mod_q_t, descriptionHash);
}