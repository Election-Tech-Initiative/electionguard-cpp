#include "electionguard/ballot.hpp"

#include "../log.hpp"
#include "electionguard/group.hpp"
#include "memory_cache.hpp"
#include "variant_cast.hpp"

#include <iomanip>
#include <iostream>

extern "C" {
#include "electionguard/ballot.h"
}

using electionguard::Cache;
using electionguard::CiphertextBallot;
using electionguard::CiphertextBallotSelection;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::Log;
using electionguard::PlaintextBallot;
using electionguard::PlaintextBallotSelection;

static Cache<PlaintextBallot> cache_plaintext_ballot;
static Cache<CiphertextBallot> cache_ciphertext_ballot;

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

#pragma region PlaintextBallotContest

#pragma endregion

#pragma region CiphertextBallotContest

#pragma endregion

#pragma region PlaintextBallot

eg_plaintext_ballot_t *eg_plaintext_ballot_from_json(char *data)
{
    auto data_string = string(data);
    auto serialized = PlaintextBallot::fromJson(data_string);
    auto *reference = cache_plaintext_ballot.retain(move(serialized));
    return AS_TYPE(eg_plaintext_ballot_t, reference);
}
eg_plaintext_ballot_t *eg_plaintext_ballot_from_bson(uint8_t *data, uint64_t length)
{
    auto data_bytes = vector<uint8_t>(data, data + length);
    auto serialized = PlaintextBallot::fromBson(data_bytes);
    auto *reference = cache_plaintext_ballot.retain(move(serialized));
    return AS_TYPE(eg_plaintext_ballot_t, reference);
}

// returns the size and fills out_data
uint64_t eg_plaintext_ballot_to_json(eg_plaintext_ballot_t *plaintext, char **out_data)
{
    auto domain_type = AS_TYPE(PlaintextBallot, plaintext);
    auto data_string = domain_type->toJson();

    auto data_size = data_string.length() + 1;
    auto *data_array = new char[data_size];
    strncpy(data_array, data_string.c_str(), data_size);
    *out_data = data_array;
    return data_size;
}

uint64_t eg_plaintext_ballot_to_bson(eg_plaintext_ballot_t *plaintext, uint8_t **out_data)
{
    auto domain_type = AS_TYPE(PlaintextBallot, plaintext);
    auto data_bytes = domain_type->toBson();

    auto *data_array = new uint8_t[data_bytes.size()];
    copy(data_bytes.begin(), data_bytes.end(), data_array);
    *out_data = data_array;
    return data_bytes.size();
}

#pragma endregion

#pragma region CiphertextBallot

eg_ciphertext_ballot_t *eg_ciphertext_ballot_from_json(char *data)
{
    auto data_string = string(data);
    auto serialized = CiphertextBallot::fromJson(data_string);
    auto *reference = cache_ciphertext_ballot.retain(move(serialized));
    return AS_TYPE(eg_ciphertext_ballot_t, reference);
}
eg_ciphertext_ballot_t *eg_ciphertext_ballot_from_bson(uint8_t *data, uint64_t length)
{
    auto data_bytes = vector<uint8_t>(data, data + length);
    auto serialized = CiphertextBallot::fromBson(data_bytes);
    auto *reference = cache_ciphertext_ballot.retain(move(serialized));
    return AS_TYPE(eg_ciphertext_ballot_t, reference);
}

// returns the size and fills out_data
uint64_t eg_ciphertext_ballot_to_json(eg_ciphertext_ballot_t *ciphertext, char **out_data)
{
    auto domain_type = AS_TYPE(CiphertextBallot, ciphertext);
    auto data_string = domain_type->toJson();

    auto data_size = data_string.length() + 1;
    auto data_array = new char[data_size];
    strncpy(data_array, data_string.c_str(), data_size);
    *out_data = data_array;
    return data_size;
}

uint64_t eg_ciphertext_ballot_to_bson(eg_ciphertext_ballot_t *ciphertext, uint8_t **out_data)
{
    auto domain_type = AS_TYPE(CiphertextBallot, ciphertext);
    auto data_bytes = domain_type->toBson();

    auto *data_array = new uint8_t[data_bytes.size()];
    copy(data_bytes.begin(), data_bytes.end(), data_array);
    *out_data = data_array;
    return data_bytes.size();
}

#pragma endregion