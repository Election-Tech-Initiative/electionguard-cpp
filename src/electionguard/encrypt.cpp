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

        // verify the contest.
        if (encryptedContest->isValidEncryption(*descriptionHash, elgamalPublicKey,
                                                cryptoExtendedBaseHash)) {
            return encryptedContest;
        }

        Log::debug("encryptedContest:: failed validity check");
        return nullptr;
    }

    unique_ptr<CiphertextBallot> encryptBallot(const PlaintextBallot &ballot,
                                               const InternalElectionDescription &metadata,
                                               const CiphertextElectionContext &context,
                                               const ElementModQ &seedHash,
                                               unique_ptr<ElementModQ> nonce /* = nullptr */,
                                               bool shouldVerifyProofs /* = true */)
    {
        // TODO: finish implementation
        // this implementation currently only works with elections that have one ballot style
        // (e.g. where all contests in the election are included on every ballot.)
        // similarly it expects that the fully hydrated representation of the ballot is provided.

        auto randomMasterNonce = nonce ? move(nonce) : rand_q();

        auto nonceSeed = CiphertextBallot::nonceSeed(*metadata.getDescriptionHash(),
                                                     ballot.getObjectId(), *randomMasterNonce);

        vector<unique_ptr<CiphertextBallotContest>> encryptedContests;

        // TODO: improve performance, complete features
        for (const auto &contestDescription : metadata.getContests()) {
            bool hasContest = false;
            for (const auto &contest : ballot.getContests()) {
                if (contest.get().getObjectId() == contestDescription.get().getObjectId()) {
                    hasContest = true;
                    auto encrypted = encryptContest(
                      contest.get(), contestDescription.get(), *context.getElGamalPublicKey(),
                      *context.getCryptoExtendedBaseHash(), *nonceSeed);
                    encryptedContests.push_back(move(encrypted));
                    break;
                }
            }

            if (!hasContest) {
                throw invalid_argument("caller must include all contests");
            }
        }

        auto encryptedBallot = CiphertextBallot::make(ballot.getObjectId(), ballot.getBallotStyle(),
                                                      *metadata.getDescriptionHash(),
                                                      move(encryptedContests), move(nonceSeed));

        if (!encryptedBallot) {
            throw runtime_error("encryptedBallot:: Error constructing encrypted ballot");
        }

        if (!shouldVerifyProofs) {
            return encryptedBallot;
        }

        // verify the ballot.
        if (encryptedBallot->isValidEncryption(*metadata.getDescriptionHash(),
                                               *context.getElGamalPublicKey(),
                                               *context.getCryptoExtendedBaseHash())) {
            return encryptedBallot;
        }

        Log::debug("encryptedBallot:: failed validity check");
        return nullptr;
    }

} // namespace electionguard
