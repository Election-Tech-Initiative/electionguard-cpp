#include "electionguard/ballot.hpp"
#include "electionguard/encrypt.hpp"

extern "C" {
#include "electionguard/encrypt.h"
}

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)

eg_encryption_mediator_t *eg_encryption_mediator_new()
{
    return AS_TYPE(eg_encryption_mediator_t, new electionguard::EncryptionMediator());
}

void eg_encryption_mediator_free(eg_encryption_mediator_t *mediator)
{
    if (mediator == nullptr) {
        return;
    }
    // TODO: safety
    delete AS_TYPE(electionguard::EncryptionMediator, mediator);
}

int eg_encryption_mediator_encrypt(eg_encryption_mediator_t *mediator)
{
    return AS_TYPE(electionguard::EncryptionMediator, mediator)->encrypt();
}

int eg_encrypt_selection(eg_plaintext_ballot_selection_t *plaintext,
                         eg_ciphertext_ballot_selection_t **ciphertext)
{
    auto *ciphertext_ =
      electionguard::encrypt_selection(AS_TYPE(electionguard::PlaintextBallotSelection, plaintext));

    *ciphertext = AS_TYPE(eg_ciphertext_ballot_selection_t, ciphertext_);

    return 1; // TODO: real return codes
}
