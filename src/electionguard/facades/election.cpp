#include "electionguard/election.hpp"

#include "memory_cache.hpp"
#include "variant_cast.hpp"

#include <memory>
#include <string>
#include <vector>

extern "C" {

#include "electionguard/election.h"
}

using electionguard::Cache;
using electionguard::CiphertextElectionContext;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::InternalElectionDescription;
using electionguard::SelectionDescription;

using std::string;
using std::vector;

// TODO: safe initialization
static Cache<ElementModQ> cache_element_mod_q;
static Cache<ElementModP> cache_element_mod_p;
static Cache<SelectionDescription> cache_selection_description;
static Cache<InternalElectionDescription> cache_internal_election_description;
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

#pragma region InternalElectionDescription

eg_element_mod_q_t *
eg_internal_election_description_get_description_hash(eg_internal_election_description_t *metadata)
{
    auto const_ref = AS_TYPE(InternalElectionDescription, metadata)->getDescriptionHash();

    auto pointer = make_unique<ElementModQ>(const_ref);
    auto *reference = cache_element_mod_q.retain(move(pointer));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

eg_internal_election_description_t *eg_internal_election_description_from_json(char *data)
{
    auto data_string = string(data);
    auto serialized = InternalElectionDescription::fromJson(data_string);
    auto *reference = cache_internal_election_description.retain(move(serialized));
    return AS_TYPE(eg_internal_election_description_t, reference);
}

eg_internal_election_description_t *eg_internal_election_description_from_bson(uint8_t *data,
                                                                               uint64_t length)
{
    auto data_bytes = vector<uint8_t>(data, data + length);
    auto serialized = InternalElectionDescription::fromBson(data_bytes);
    auto *reference = cache_internal_election_description.retain(move(serialized));
    return AS_TYPE(eg_internal_election_description_t, reference);
}

// returns the size and fills out_data
uint64_t eg_internal_election_description_to_json(eg_internal_election_description_t *metadata,
                                                  char **out_data)
{
    auto domain_type = AS_TYPE(InternalElectionDescription, metadata);
    auto data_string = domain_type->toJson();

    auto data_size = data_string.length() + 1;
    auto data_array = new char[data_size];
    strncpy(data_array, data_string.c_str(), data_size);
    *out_data = data_array;
    return data_size;
}

uint64_t eg_internal_election_description_to_bson(eg_internal_election_description_t *metadata,
                                                  uint8_t **out_data)
{
    auto domain_type = AS_TYPE(InternalElectionDescription, metadata);
    auto data_bytes = domain_type->toBson();

    auto *data_array = new uint8_t[data_bytes.size()];
    copy(data_bytes.begin(), data_bytes.end(), data_array);
    *out_data = data_array;

    // TODO: cleanup data_array
    return data_bytes.size();
}

#pragma endregion

#pragma region CiphertextElectionContext

eg_element_mod_p_t *
eg_ciphertext_election_context_get_elgamal_public_key(eg_ciphertext_election_context_t *context)
{
    auto *pointer = AS_TYPE(CiphertextElectionContext, context)->getElGamalPublicKey();
    unique_ptr<ElementModP> local_reference{const_cast<ElementModP *>(pointer)};
    auto *reference = cache_element_mod_p.retain(move(local_reference));
    return AS_TYPE(eg_element_mod_p_t, reference);
}

eg_element_mod_q_t *
eg_ciphertext_election_context_get_description_hash(eg_ciphertext_election_context_t *context)
{
    auto *pointer = AS_TYPE(CiphertextElectionContext, context)->getDescriptionHash();
    unique_ptr<ElementModQ> local_reference{const_cast<ElementModQ *>(pointer)};
    auto *reference = cache_element_mod_q.retain(move(local_reference));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

eg_element_mod_q_t *
eg_ciphertext_election_context_get_crypto_base_hash(eg_ciphertext_election_context_t *context)
{
    auto *pointer = AS_TYPE(CiphertextElectionContext, context)->getCryptoBaseHash();
    unique_ptr<ElementModQ> local_reference{const_cast<ElementModQ *>(pointer)};
    auto *reference = cache_element_mod_q.retain(move(local_reference));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

eg_element_mod_q_t *eg_ciphertext_election_context_get_crypto_extended_base_hash(
  eg_ciphertext_election_context_t *context)
{
    auto *pointer = AS_TYPE(CiphertextElectionContext, context)->getCryptoExtendedBaseHash();
    unique_ptr<ElementModQ> local_reference{const_cast<ElementModQ *>(pointer)};
    auto *reference = cache_element_mod_q.retain(move(local_reference));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

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

eg_ciphertext_election_context_t *eg_ciphertext_election_context_from_json(char *data)
{
    auto data_string = string(data);
    auto serialized = CiphertextElectionContext::fromJson(data_string);
    auto *reference = cache_ciphertext_election_context.retain(move(serialized));
    return AS_TYPE(eg_ciphertext_election_context_t, reference);
}
eg_ciphertext_election_context_t *eg_ciphertext_election_context_from_bson(uint8_t *data,
                                                                           uint64_t length)
{
    auto data_bytes = vector<uint8_t>(data, data + length);
    auto serialized = CiphertextElectionContext::fromBson(data_bytes);
    auto *reference = cache_ciphertext_election_context.retain(move(serialized));
    return AS_TYPE(eg_ciphertext_election_context_t, reference);
}

// returns the size and fills out_data
uint64_t eg_ciphertext_election_context_to_json(eg_ciphertext_election_context_t *context,
                                                char **out_data)
{
    auto domain_type = AS_TYPE(CiphertextElectionContext, context);
    auto data_string = domain_type->toJson();

    auto data_size = data_string.length() + 1;
    auto data_array = new char[data_size];
    strncpy(data_array, data_string.c_str(), data_size);
    *out_data = data_array;
    return data_size;
}

uint64_t eg_ciphertext_election_context_to_bson(eg_ciphertext_election_context_t *context,
                                                uint8_t **out_data)
{
    auto domain_type = AS_TYPE(CiphertextElectionContext, context);
    auto data_bytes = domain_type->toBson();

    auto *data_array = new uint8_t[data_bytes.size()];
    copy(data_bytes.begin(), data_bytes.end(), data_array);
    *out_data = data_array;
    return data_bytes.size();
}

#pragma endregion
