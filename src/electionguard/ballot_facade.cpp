#include "electionguard/ballot.hpp"
#include "electionguard/group.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/ballot.h"
}

using namespace electionguard;

/// PlaintextBallotSelection
eg_plaintext_ballot_selection_t *eg_plaintext_ballot_selection_new(const char *object_id,
                                                                   const char *vote)
{
    return AS_TYPE(eg_plaintext_ballot_selection_t, new PlaintextBallotSelection(object_id, vote));
}

void eg_plaintext_ballot_selection_free(eg_plaintext_ballot_selection_t *selection)
{
    if (selection == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(PlaintextBallotSelection, selection);
}

char *eg_plaintext_ballot_selection_get_object_id(eg_plaintext_ballot_selection_t *selection)
{
    return AS_TYPE(PlaintextBallotSelection, selection)->getObjectId();
}

/// CiphertextBallotSelection
eg_ciphertext_ballot_selection_t *
eg_ciphertext_ballot_selection_new(const char *object_id, eg_element_mod_q_t *description_hash,
                                   bool is_placeholder, eg_element_mod_q_t *nonce,
                                   eg_element_mod_q_t *crypto_hash)
{
    // TODO: finish
    auto *descriptionHash = AS_TYPE(ElementModQ, description_hash);

    auto *fake_elgamal = new ElGamalCiphertext(nullptr, nullptr);

    auto *nonce_ = AS_TYPE(ElementModQ, nonce);
    auto cryptoHash = AS_TYPE(ElementModQ, crypto_hash);
    return AS_TYPE(eg_ciphertext_ballot_selection_t,
                   new CiphertextBallotSelection(object_id, descriptionHash, fake_elgamal,
                                                 is_placeholder, nonce_, cryptoHash, nullptr,
                                                 nullptr));
}

void eg_ciphertext_ballot_selection_free(eg_ciphertext_ballot_selection_t *selection)
{
    if (selection == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(CiphertextBallotSelection, selection);
}

char *eg_ciphertext_ballot_selection_get_object_id(eg_ciphertext_ballot_selection_t *selection)
{
    auto *ciphertext = AS_TYPE(CiphertextBallotSelection, selection);
    return ciphertext->getObjectId();
}

EG_API eg_element_mod_q_t *
eg_ciphertext_ballot_selection_get_description_hash(eg_ciphertext_ballot_selection_t *selection)
{
    auto *descriptionHash = AS_TYPE(CiphertextBallotSelection, selection)->getDescriptionHash();
    return AS_TYPE(eg_element_mod_q_t, descriptionHash);
}