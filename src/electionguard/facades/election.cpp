#include "electionguard/election.hpp"

#include "memory_cache.hpp"
#include "variant_cast.hpp"

#include <memory>

extern "C" {

#include "electionguard/election.h"
}

using electionguard::Cache;
using electionguard::CiphertextElectionContext;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::SelectionDescription;

// TODO: safe initialization
static Cache<ElementModQ> cache_element_mod_q;
static Cache<SelectionDescription> cache_selection_description;
static Cache<CiphertextElectionContext> cache_ciphertext_election_context;

#pragma region SelectionDescription

eg_selection_description_t *eg_selection_description_create(const char *object_id,
                                                            const char *candidate_id,
                                                            uint64_t sequence_order)
{
    auto objectId = string(object_id);
    auto candidateId = string(candidate_id);
    auto description = make_unique<SelectionDescription>(objectId, candidateId, sequence_order);
    auto *reference = cache_selection_description.retain(move(description));
    return AS_TYPE(eg_selection_description_t, reference);
}

const char *eg_selection_description_get_object_id(eg_selection_description_t *selection)
{
    auto objectId = AS_TYPE(SelectionDescription, selection)->getObjectId();
    const auto *cstr = objectId.c_str();
    return cstr;
}

const char *eg_selection_description_get_candidate_id(eg_selection_description_t *selection)
{
    auto candidateId = AS_TYPE(SelectionDescription, selection)->getCandidateId();
    const auto *cstr = candidateId.c_str();
    return cstr;
}

uint64_t eg_selection_description_get_sequence_order(eg_selection_description_t *selection)
{
    return AS_TYPE(SelectionDescription, selection)->getSequenceOrder();
}

eg_element_mod_q_t *eg_selection_description_crypto_hash(eg_selection_description_t *selection)
{
    auto cryptoHash = AS_TYPE(SelectionDescription, selection)->crypto_hash();
    auto *reference = cache_element_mod_q.retain(move(cryptoHash));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

#pragma endregion

#pragma region CiphertextElectionContext

eg_ciphertext_election_context_t *
eg_ciphertext_election_context_make(uint64_t number_of_guardians, uint64_t quorum,
                                    eg_element_mod_p_t *elgamal_public_key,
                                    eg_element_mod_q_t *description_hash)
{

    auto *publicKeyPtr = AS_TYPE(ElementModP, elgamal_public_key);
    auto *hashPtr = AS_TYPE(ElementModQ, description_hash);

    unique_ptr<ElementModP> elGamalPublicKey{publicKeyPtr};
    unique_ptr<ElementModQ> descriptionHash{hashPtr};

    auto context = CiphertextElectionContext::make(number_of_guardians, quorum,
                                                   move(elGamalPublicKey), move(descriptionHash));
    auto *reference = cache_ciphertext_election_context.retain(move(context));
    return AS_TYPE(eg_ciphertext_election_context_t, reference);
}

eg_ciphertext_election_context_t *
eg_ciphertext_election_context_make_from_hex(uint64_t number_of_guardians, uint64_t quorum,
                                             const char *elgamal_public_key,
                                             const char *description_hash)
{
    auto elGamalPublicKey = string(elgamal_public_key);
    auto descriptionHash = string(description_hash);

    auto context = CiphertextElectionContext::make(number_of_guardians, quorum, elGamalPublicKey,
                                                   descriptionHash);
    auto *reference = cache_ciphertext_election_context.retain(move(context));
    return AS_TYPE(eg_ciphertext_election_context_t, reference);
}

#pragma endregion
