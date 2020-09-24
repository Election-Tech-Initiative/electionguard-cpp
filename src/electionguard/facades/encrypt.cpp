#include "electionguard/encrypt.hpp"

#include "memory_cache.hpp"
#include "variant_cast.hpp"

#include <cerrno>
#include <exception>

extern "C" {
#include "electionguard/encrypt.h"
}

using electionguard::Cache;
using electionguard::CiphertextBallotSelection;
using electionguard::ElementModP;
using electionguard::ElementModQ;
using electionguard::EncryptionMediator;
using electionguard::PlaintextBallotSelection;
using electionguard::SelectionDescription;

// TODO: safe initialization
static Cache<CiphertextBallotSelection> cache_ciphertext_ballot_selection;

eg_encryption_mediator_t *eg_encryption_mediator_new()
{
    return AS_TYPE(eg_encryption_mediator_t, new EncryptionMediator());
}

void eg_encryption_mediator_free(eg_encryption_mediator_t *mediator)
{
    if (mediator == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(EncryptionMediator, mediator);
}

int eg_encryption_mediator_encrypt(eg_encryption_mediator_t *mediator)
{
    return AS_TYPE(EncryptionMediator, mediator)->encrypt();
}

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
