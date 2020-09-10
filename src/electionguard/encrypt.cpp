#include "electionguard/encrypt.hpp"

#include "electionguard/elgamal.hpp"
#include "electionguard/hash.hpp"
#include "log.hpp"
#include "nonces.hpp"

#include <iostream>

extern "C" {
#include "../kremlin/Hacl_Bignum4096.h"
}

namespace electionguard
{
    EncryptionMediator::EncryptionMediator() { Log::debug(" : Creating EncryptionCompositor[]"); }
    EncryptionMediator::~EncryptionMediator()
    {
        Log::debug(" : Destroying EncryptionCompositor[]");
    }

    int EncryptionMediator::encrypt()
    {
        Log::debug(" : encrypting by instance");
        return 9;
    }

    //todo: renmame using the cpp style, and maybe move into a class as static
    CiphertextBallotSelection *
    encrypt_selection(PlaintextBallotSelection *selection, SelectionDescription *description,
                      ElementModP *elgamalPublicKey, ElementModQ *cryptoExtendedBaseHash,
                      ElementModQ *nonceSeed, bool isPlaceholder, bool shouldVerifyProofs)
    {
        // Validate Input
        // TODO:

        auto *descriptionHash = description->crypto_hash();
        auto *nonceSequence = new Nonces(descriptionHash, nonceSeed);
        auto *selectionNonce = nonceSequence->get(description->getSequenceOrder());
        auto *proofNonce = nonceSequence->next();

        uint64_t plaintext = selection->toInt();

        // Generate the encryption
        auto *ciphertext = elgamalEncrypt(plaintext, selectionNonce, elgamalPublicKey);
        if (ciphertext == nullptr) {
            Log::debug("encryptSelection:: Error generating ciphertext");
            return nullptr;
        }

        // TODO: encrypt/decrypt: encrypt the extended_data field

        auto *encryptedSelection = CiphertextBallotSelection::make(
          selection->getObjectId(), descriptionHash, ciphertext, elgamalPublicKey,
          cryptoExtendedBaseHash, proofNonce, plaintext, isPlaceholder, selectionNonce);

        // TODO: cleanup
        if (encryptedSelection == nullptr || encryptedSelection->getProof() == nullptr) {
            Log::debug("encryptSelection:: Error constructing encrypted selection");
            return nullptr;
        }

        // optionally, skip the verification step
        if (!shouldVerifyProofs) {
            return encryptedSelection;
        }

        // verify the selection.
        if (encryptedSelection->isValidEncryption(descriptionHash, elgamalPublicKey,
                                                  cryptoExtendedBaseHash)) {
            return encryptedSelection;
        }

        Log::debug("encryptSelection:: failed validity check");

        // TODO: cleanup
        return nullptr;
    }

} // namespace electionguard
