#include "electionguard/election.hpp"

#include "memory_cache.hpp"
#include "variant_cast.hpp"

#include <memory>

extern "C" {

#include "electionguard/election.h"
}

using electionguard::Cache;
using electionguard::ElementModQ;
using electionguard::SelectionDescription;

// TODO: safe initialization
static Cache<ElementModQ> cache_element_mod_q;
static Cache<SelectionDescription> cache_selection_description;

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

char *eg_selection_description_get_candidate_id(eg_selection_description_t *selection)
{
    return AS_TYPE(SelectionDescription, selection)->getCandidateId();
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
