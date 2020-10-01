#include "electionguard/encrypt.hpp"

#include "memory_cache.hpp"
#include "variant_cast.hpp"

#include <cerrno>
#include <exception>

extern "C" {
#include "electionguard/encrypt.h"
}

using electionguard::Cache;
using electionguard::CiphertextBallot;
using electionguard::CiphertextBallotSelection;
using electionguard::CiphertextElectionContext;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::encryptBallot;
using electionguard::EncryptionDevice;
using electionguard::EncryptionMediator;
using electionguard::InternalElectionDescription;
using electionguard::PlaintextBallot;
using electionguard::PlaintextBallotSelection;
using electionguard::SelectionDescription;

// TODO: safe initialization
static Cache<CiphertextBallotSelection> cache_ciphertext_ballot_selection;
static Cache<CiphertextBallot> cache_ciphertext_ballot;
static Cache<EncryptionDevice> cache_encryption_device;
static Cache<EncryptionMediator> cache_encryption_mediator;
static Cache<ElementModQ> cache_element_mod_q;

#pragma region EncryptionDevice

eg_encryption_device_t *eg_encryption_device_create(const uint64_t uuid, const char *location)
{
    auto device = make_unique<EncryptionDevice>(uuid, string(location));
    auto *reference = cache_encryption_device.retain(move(device));
    return AS_TYPE(eg_encryption_device_t, reference);
}

eg_element_mod_q_t *eg_encryption_device_get_hash(eg_encryption_device_t *device)
{
    auto hash = AS_TYPE(EncryptionDevice, device)->getHash();
    auto *reference = cache_element_mod_q.retain(move(hash));
    return AS_TYPE(eg_element_mod_q_t, reference);
}

#pragma endregion

#pragma region EncryptionMediator

eg_encryption_mediator_t *
eg_encryption_mediator_create(eg_internal_election_description_t *metadata,
                              eg_ciphertext_election_context_t *context,
                              eg_encryption_device_t *encryption_device)
{
    auto *metadata_ = AS_TYPE(InternalElectionDescription, metadata);
    auto *context_ = AS_TYPE(CiphertextElectionContext, context);
    auto *device_ = AS_TYPE(EncryptionDevice, encryption_device);
    auto mediator = make_unique<EncryptionMediator>(*metadata_, *context_, *device_);

    auto *reference = cache_encryption_mediator.retain(move(mediator));
    return AS_TYPE(eg_encryption_mediator_t, reference);
}

eg_ciphertext_ballot_t *eg_encryption_mediator_encrypt(eg_encryption_mediator_t *mediator,
                                                       eg_plaintext_ballot_t *ballot)
{
    auto *plaintext = AS_TYPE(PlaintextBallot, ballot);
    auto ciphertext = AS_TYPE(EncryptionMediator, mediator)->encrypt(*plaintext);

    auto *reference = cache_ciphertext_ballot.retain(move(ciphertext));
    return AS_TYPE(eg_ciphertext_ballot_t, reference);
}

#pragma endregion

#pragma region EncryptSelection

eg_ciphertext_ballot_selection_t *
eg_encrypt_selection(eg_plaintext_ballot_selection_t *plaintext,
                     eg_selection_description_t *description, eg_element_mod_p_t *public_key,
                     eg_element_mod_q_t *crypto_extended_base_hash, eg_element_mod_q_t *nonce_seed,
                     bool is_placeholder, bool should_verify_proofs)
{
    try {
        auto *plaintext_ = AS_TYPE(PlaintextBallotSelection, plaintext);
        auto *description_ = AS_TYPE(SelectionDescription, description);
        auto *public_key_ = AS_TYPE(ElementModP, public_key);
        auto *crypto_extended_base_hash_ = AS_TYPE(ElementModQ, crypto_extended_base_hash);
        auto *nonce_seed_ = AS_TYPE(ElementModQ, nonce_seed);

        auto ciphertext =
          encryptSelection(*plaintext_, *description_, *public_key_, *crypto_extended_base_hash_,
                           *nonce_seed_, is_placeholder, should_verify_proofs);

        auto *reference = cache_ciphertext_ballot_selection.retain(move(ciphertext));
        return AS_TYPE(eg_ciphertext_ballot_selection_t, reference);
    } catch (const exception &) {
        // TODO: real return codes
        errno = EIO;
        return nullptr;
    }
}

#pragma endregion

#pragma region EncryptContest

eg_ciphertext_ballot_contest_t *
eg_encrypt_contest(eg_plaintext_ballot_contest_t *plaintext, eg_contest_description_t *description,
                   eg_element_mod_p_t *public_key, eg_element_mod_q_t *crypto_extended_base_hash,
                   eg_element_mod_q_t *nonce_seed, bool should_verify_proofs)
{
    // TODO: Implement
    return nullptr;
}

#pragma endregion

#pragma region EncryptBallot

eg_ciphertext_ballot_t *eg_encrypt_ballot(eg_plaintext_ballot_t *plaintext,
                                          eg_internal_election_description_t *metadata,
                                          eg_ciphertext_election_context_t *context,
                                          eg_element_mod_q_t *seed_hash, bool should_verify_proofs)
{
    auto *plaintext_ = AS_TYPE(PlaintextBallot, plaintext);
    auto *metadata_ = AS_TYPE(InternalElectionDescription, metadata);
    auto *context_ = AS_TYPE(CiphertextElectionContext, context);
    auto *seed_hash_ = AS_TYPE(ElementModQ, seed_hash);

    auto ciphertext =
      encryptBallot(*plaintext_, *metadata_, *context_, *seed_hash_, nullptr, should_verify_proofs);
    auto *reference = cache_ciphertext_ballot.retain(move(ciphertext));
    return AS_TYPE(eg_ciphertext_ballot_t, reference);
}

eg_ciphertext_ballot_t *eg_encrypt_ballot_with_nonce(eg_plaintext_ballot_t *plaintext,
                                                     eg_internal_election_description_t *metadata,
                                                     eg_ciphertext_election_context_t *context,
                                                     eg_element_mod_q_t *seed_hash,
                                                     eg_element_mod_q_t *nonce,
                                                     bool should_verify_proofs)
{
    auto *plaintext_ = AS_TYPE(PlaintextBallot, plaintext);
    auto *metadata_ = AS_TYPE(InternalElectionDescription, metadata);
    auto *context_ = AS_TYPE(CiphertextElectionContext, context);
    auto *seed_hash_ = AS_TYPE(ElementModQ, seed_hash);
    auto *nonce_ = AS_TYPE(ElementModQ, nonce);

    unique_ptr<ElementModQ> nonce_ptr{nonce_};

    auto ciphertext = encryptBallot(*plaintext_, *metadata_, *context_, *seed_hash_,
                                    move(nonce_ptr), should_verify_proofs);
    auto *reference = cache_ciphertext_ballot.retain(move(ciphertext));
    return AS_TYPE(eg_ciphertext_ballot_t, reference);
}

#pragma endregion
