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
    // TODO: implement
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
    unique_ptr<CiphertextBallotSelection>
    encryptSelection(const PlaintextBallotSelection &plaintext,
                     const SelectionDescription &description, const ElementModP &elgamalPublicKey,
                     const ElementModQ &cryptoExtendedBaseHash, const ElementModQ &nonceSeed,
                     bool isPlaceholder /* = false */, bool shouldVerifyProofs /* = true */)
    {
        // TODO: Validate Input

        auto descriptionHash = description.crypto_hash();
        auto nonceSequence =
          make_unique<Nonces>(*descriptionHash, &const_cast<ElementModQ &>(nonceSeed));
        auto selectionNonce = nonceSequence->get(description.getSequenceOrder());
        auto proofNonce = nonceSequence->next();

        uint64_t plaintext_ = plaintext.toInt();

        // Generate the encryption
        auto ciphertext = elgamalEncrypt(plaintext_, *selectionNonce, elgamalPublicKey);
        if (ciphertext == nullptr) {
            throw runtime_error("encryptSelection:: Error generating ciphertext");
        }

        // TODO: encrypt/decrypt: encrypt the extended_data field

        auto encryptedSelection = CiphertextBallotSelection::make(
          plaintext.getObjectId(), *descriptionHash, move(ciphertext), elgamalPublicKey,
          cryptoExtendedBaseHash, *proofNonce, plaintext_, isPlaceholder, move(selectionNonce));

        if (encryptedSelection == nullptr || encryptedSelection->getProof() == nullptr) {
            throw runtime_error("encryptSelection:: Error constructing encrypted selection");
        }

        // optionally, skip the verification step
        if (!shouldVerifyProofs) {
            return encryptedSelection;
        }

        // verify the selection.
        if (encryptedSelection->isValidEncryption(*descriptionHash, elgamalPublicKey,
                                                  cryptoExtendedBaseHash)) {
            return encryptedSelection;
        }

        Log::debug("encryptSelection:: failed validity check");
        return nullptr;
    }

    unique_ptr<CiphertextBallotContest>
    encryptContest(const PlaintextBallotContest &contest, const ContestDescription &description,
                   const ElementModP &elgamalPublicKey, const ElementModQ &cryptoExtendedBaseHash,
                   const ElementModQ &nonceSeed, bool isPlaceholder /* = false */,
                   bool shouldVerifyProofs /* = true */)

    {
        // TODO: validate input

        auto descriptionHash = description.crypto_hash();
        auto nonceSequence =
          make_unique<Nonces>(*descriptionHash, &const_cast<ElementModQ &>(nonceSeed));
        auto contestNonce = nonceSequence->get(description.getSequenceOrder());
        auto proofNonce = nonceSequence->next();

        vector<unique_ptr<CiphertextBallotSelection>> encryptedSelections;

        // TODO: improve performance, complete features
        // note this is a simplified version of the loop in python
        // where the caller must supply all values and placeholder selections are not handled
        for (const auto &selectionDescription : description.getSelections()) {
            bool hasSelection = false;
            for (const auto &selection : contest.getSelections()) {
                if (selection.get().getObjectId() == selectionDescription.get().getObjectId()) {
                    hasSelection = true;
                    auto encrypted = encryptSelection(selection.get(), selectionDescription.get(),
                                                      elgamalPublicKey, cryptoExtendedBaseHash,
                                                      *contestNonce, false, true);
                    encryptedSelections.push_back(move(encrypted));
                    break;
                }
            }

            if (!hasSelection) {
                throw invalid_argument("caller must include all selections");
            }
        }

        auto encryptedContest = CiphertextBallotContest::make(
          contest.getObjectId(), *descriptionHash, move(encryptedSelections), elgamalPublicKey,
          cryptoExtendedBaseHash, *proofNonce, description.getNumberElected(), move(contestNonce));

        if (encryptedContest == nullptr || encryptedContest->getProof() == nullptr) {
            throw runtime_error("encryptedContest:: Error constructing encrypted constest");
        }

        // optionally, skip the verification step
        if (!shouldVerifyProofs) {
            return encryptedContest;
        }

        // verify the selection.
        if (encryptedContest->isValidEncryption(*descriptionHash, elgamalPublicKey,
                                                cryptoExtendedBaseHash)) {
            return encryptedContest;
        }

        Log::debug("encryptedContest:: failed validity check");
        return nullptr;
    }

} // namespace electionguard
