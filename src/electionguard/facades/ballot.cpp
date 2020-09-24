#include "electionguard/ballot.hpp"

#include "electionguard/group.hpp"
#include "variant_cast.hpp"

extern "C" {
#include "electionguard/ballot.h"
}

using electionguard::CiphertextBallotSelection;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::PlaintextBallotSelection;

#pragma region PlaintextBallotSelection

eg_plaintext_ballot_selection_t *eg_plaintext_ballot_selection_create(const char *object_id,
                                                                      const char *vote)
{
    return AS_TYPE(eg_plaintext_ballot_selection_t, new PlaintextBallotSelection(object_id, vote));
}

const char *eg_plaintext_ballot_selection_get_object_id(eg_plaintext_ballot_selection_t *selection)
{
    auto objectId = AS_TYPE(PlaintextBallotSelection, selection)->getObjectId();
    const auto *cstr = objectId.c_str();
    return cstr;
}

#pragma endregion

#pragma region CiphertextBallotSelection

const char *
eg_ciphertext_ballot_selection_get_object_id(eg_ciphertext_ballot_selection_t *selection)
{
    auto objectId = AS_TYPE(CiphertextBallotSelection, selection)->getObjectId();
    const auto *cstr = objectId.c_str();
    return cstr;
}

eg_element_mod_q_t *
eg_ciphertext_ballot_selection_get_description_hash(eg_ciphertext_ballot_selection_t *selection)
{
    auto *descriptionHash = AS_TYPE(CiphertextBallotSelection, selection)->getDescriptionHash();
    return AS_TYPE(eg_element_mod_q_t, descriptionHash);
}

eg_elgamal_ciphertext_t *
eg_ciphertext_ballot_selection_get_ciphertext(eg_ciphertext_ballot_selection_t *selection)
{
    auto *ciphertext = AS_TYPE(CiphertextBallotSelection, selection)->getCiphertext();
    return AS_TYPE(eg_elgamal_ciphertext_t, ciphertext);
}

eg_disjunctive_chaum_pedersen_proof_t *
eg_ciphertext_ballot_selection_get_proof(eg_ciphertext_ballot_selection_t *selection)
{
    auto *proof = AS_TYPE(CiphertextBallotSelection, selection)->getProof();
    return AS_TYPE(eg_disjunctive_chaum_pedersen_proof_t, proof);
}

bool eg_ciphertext_ballot_selection_is_valid_encryption(
  eg_ciphertext_ballot_selection_t *selection, eg_element_mod_q_t *seed_hash,
  eg_element_mod_p_t *public_key, eg_element_mod_q_t *crypto_extended_base_hash)
{
    auto *seedHash = AS_TYPE(ElementModQ, seed_hash);
    auto *publicKey = AS_TYPE(ElementModP, public_key);
    auto *cryptoExtendedBaseHash = AS_TYPE(ElementModQ, crypto_extended_base_hash);
    return AS_TYPE(CiphertextBallotSelection, selection)
      ->isValidEncryption(*seedHash, *publicKey, *cryptoExtendedBaseHash);
}

#pragma endregion