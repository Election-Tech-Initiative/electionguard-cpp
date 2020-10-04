#include "electionguard/encrypt.hpp"

#include "../log.hpp"
#include "variant_cast.hpp"

#include <cerrno>
#include <exception>

extern "C" {
#include "electionguard/encrypt.h"
}

using electionguard::CiphertextBallot;
using electionguard::CiphertextBallotSelection;
using electionguard::CiphertextElectionContext;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::encryptBallot;
using electionguard::EncryptionDevice;
using electionguard::EncryptionMediator;
using electionguard::InternalElectionDescription;
using electionguard::Log;
using electionguard::PlaintextBallot;
using electionguard::PlaintextBallotSelection;
using electionguard::SelectionDescription;

#pragma region EncryptionDevice

eg_electionguard_status_t eg_encryption_device_new(const uint64_t in_uuid, const char *in_location,
                                                   eg_encryption_device_t **out_handle)
{
    try {
        auto device = make_unique<EncryptionDevice>(in_uuid, string(in_location));
        *out_handle = AS_TYPE(eg_encryption_device_t, device.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_encryption_device_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_encryption_device_free(eg_encryption_device_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(EncryptionDevice, handle);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t eg_encryption_device_get_hash(eg_encryption_device_t *handle,
                                                        eg_element_mod_q_t **out_hash)
{
    try {
        auto hash = AS_TYPE(EncryptionDevice, handle)->getHash();
        *out_hash = AS_TYPE(eg_element_mod_q_t, hash.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_encryption_device_get_hash", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region EncryptionMediator

eg_electionguard_status_t eg_encryption_mediator_new(
  eg_internal_election_description_t *in_metadata, eg_ciphertext_election_context_t *in_context,
  eg_encryption_device_t *in_encryption_device, eg_encryption_mediator_t **out_handle)
{
    try {
        auto *metadata = AS_TYPE(InternalElectionDescription, in_metadata);
        auto *context = AS_TYPE(CiphertextElectionContext, in_context);
        auto *device = AS_TYPE(EncryptionDevice, in_encryption_device);
        auto mediator = make_unique<EncryptionMediator>(*metadata, *context, *device);

        *out_handle = AS_TYPE(eg_encryption_mediator_t, mediator.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_encryption_mediator_new", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_encryption_mediator_free(eg_encryption_mediator_t *handle)
{
    if (handle == nullptr) {
        return ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT;
    }

    delete AS_TYPE(EncryptionMediator, handle);
    return ELECTIONGUARD_STATUS_SUCCESS;
}

eg_electionguard_status_t
eg_encryption_mediator_encrypt_ballot(eg_encryption_mediator_t *handle,
                                      eg_plaintext_ballot_t *in_plaintext,
                                      eg_ciphertext_ballot_t **out_ciphertext_handle)
{
    try {
        auto *plaintext = AS_TYPE(PlaintextBallot, in_plaintext);
        auto ciphertext = AS_TYPE(EncryptionMediator, handle)->encrypt(*plaintext);

        *out_ciphertext_handle = AS_TYPE(eg_ciphertext_ballot_t, ciphertext.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_encryption_mediator_encrypt_ballot", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region EncryptSelection

eg_electionguard_status_t eg_encrypt_selection(eg_plaintext_ballot_selection_t *in_plaintext,
                                               eg_selection_description_t *in_description,
                                               eg_element_mod_p_t *in_public_key,
                                               eg_element_mod_q_t *in_crypto_extended_base_hash,
                                               eg_element_mod_q_t *in_nonce_seed,
                                               bool in_is_placeholder, bool in_should_verify_proofs,
                                               eg_ciphertext_ballot_selection_t **out_handle)
{
    try {
        auto *plaintext = AS_TYPE(PlaintextBallotSelection, in_plaintext);
        auto *description = AS_TYPE(SelectionDescription, in_description);
        auto *public_key = AS_TYPE(ElementModP, in_public_key);
        auto *crypto_extended_base_hash = AS_TYPE(ElementModQ, in_crypto_extended_base_hash);
        auto *nonce_seed_ = AS_TYPE(ElementModQ, in_nonce_seed);

        auto ciphertext =
          encryptSelection(*plaintext, *description, *public_key, *crypto_extended_base_hash,
                           *nonce_seed_, in_is_placeholder, in_should_verify_proofs);

        *out_handle = AS_TYPE(eg_ciphertext_ballot_selection_t, ciphertext.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_encrypt_selection", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion

#pragma region EncryptContest

#pragma endregion

#pragma region EncryptBallot

eg_electionguard_status_t eg_encrypt_ballot(eg_plaintext_ballot_t *in_plaintext,
                                            eg_internal_election_description_t *in_metadata,
                                            eg_ciphertext_election_context_t *in_context,
                                            eg_element_mod_q_t *in_seed_hash,
                                            bool in_should_verify_proofs,
                                            eg_ciphertext_ballot_t **out_handle)
{
    try {
        auto *plaintext = AS_TYPE(PlaintextBallot, in_plaintext);
        auto *metadata = AS_TYPE(InternalElectionDescription, in_metadata);
        auto *context = AS_TYPE(CiphertextElectionContext, in_context);
        auto *seed_hash = AS_TYPE(ElementModQ, in_seed_hash);

        auto ciphertext = encryptBallot(*plaintext, *metadata, *context, *seed_hash, nullptr,
                                        in_should_verify_proofs);
        *out_handle = AS_TYPE(eg_ciphertext_ballot_t, ciphertext.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_encrypt_ballot", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

eg_electionguard_status_t eg_encrypt_ballot_with_nonce(
  eg_plaintext_ballot_t *in_plaintext, eg_internal_election_description_t *in_metadata,
  eg_ciphertext_election_context_t *in_context, eg_element_mod_q_t *in_seed_hash,
  eg_element_mod_q_t *in_nonce, bool in_should_verify_proofs, eg_ciphertext_ballot_t **out_handle)
{
    try {
        auto *plaintext = AS_TYPE(PlaintextBallot, in_plaintext);
        auto *metadata = AS_TYPE(InternalElectionDescription, in_metadata);
        auto *context = AS_TYPE(CiphertextElectionContext, in_context);
        auto *seed_hash = AS_TYPE(ElementModQ, in_seed_hash);
        auto *nonce = AS_TYPE(ElementModQ, in_nonce);
        unique_ptr<ElementModQ> nonce_ptr{nonce};

        auto ciphertext = encryptBallot(*plaintext, *metadata, *context, *seed_hash,
                                        move(nonce_ptr), in_should_verify_proofs);
        *out_handle = AS_TYPE(eg_ciphertext_ballot_t, ciphertext.release());
        return ELECTIONGUARD_STATUS_SUCCESS;
    } catch (const exception &e) {
        Log::error(": eg_encrypt_ballot_with_nonce", e);
        return ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC;
    }
}

#pragma endregion
