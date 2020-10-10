#include "electionguard/encrypt.hpp"

#include "electionguard/elgamal.hpp"
#include "electionguard/hash.hpp"
#include "electionguard/tracker.hpp"
#include "log.hpp"
#include "nonces.hpp"

#include <iostream>

extern "C" {
#include "../kremlin/Hacl_Bignum4096.h"
}

namespace electionguard
{
#pragma region EncryptionDevice

    struct EncryptionDevice::Impl {
        uint64_t uuid;
        string location;
        Impl(const uint64_t uuid, const string &location)
        {
            this->uuid = uuid;
            this->location = location;
        }
    };

    EncryptionDevice::EncryptionDevice(const uint64_t uuid, const string &location)
        : pimpl(new Impl(uuid, location))
    {
        Log::debug(": EncryptionDevice: Created: UUID: " + to_string(uuid) + " at: " + location);
    }
    EncryptionDevice::~EncryptionDevice() = default;

    EncryptionDevice &EncryptionDevice::operator=(EncryptionDevice other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    unique_ptr<ElementModQ> EncryptionDevice::getHash() const
    {
        return Tracker::getHashForDevice(pimpl->uuid, pimpl->location);
    }

#pragma endregion

#pragma region EncryptionMediator

    struct EncryptionMediator::Impl {
        const InternalElectionDescription &metadata;
        const CiphertextElectionContext &context;
        const EncryptionDevice &encryptionDevice;
        unique_ptr<ElementModQ> trackerHashSeed;

        Impl(const InternalElectionDescription &metadata, const CiphertextElectionContext &context,
             const EncryptionDevice &encryptionDevice)
            : metadata(metadata), context(context), encryptionDevice(encryptionDevice)

        {
        }
    };

    EncryptionMediator::EncryptionMediator(const InternalElectionDescription &metadata,
                                           const CiphertextElectionContext &context,
                                           const EncryptionDevice &encryptionDevice)
        : pimpl(new Impl(metadata, context, encryptionDevice))
    {
    }

    EncryptionMediator::~EncryptionMediator() = default;

    EncryptionMediator &EncryptionMediator::operator=(EncryptionMediator other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    unique_ptr<CiphertextBallot>
    EncryptionMediator::encrypt(const PlaintextBallot &ballot,
                                bool shouldVerifyProofs /* = true */) const
    {
        Log::debug(" encrypt: objectId: " + ballot.getObjectId());

        if (!pimpl->trackerHashSeed) {
            auto trackerHashSeed = pimpl->encryptionDevice.getHash();
            pimpl->trackerHashSeed.swap(trackerHashSeed);
            Log::debugHex(": encrypt: instantiated tacking hash: ",
                          pimpl->trackerHashSeed->toHex());
        }

        Log::debugHex(": encrypt: encrypting with previous hash: ",
                      pimpl->trackerHashSeed->toHex());
        auto encryptedBallot = encryptBallot(ballot, pimpl->metadata, pimpl->context,
                                             *pimpl->trackerHashSeed, nullptr, shouldVerifyProofs);

        pimpl->trackerHashSeed = make_unique<ElementModQ>(*encryptedBallot->getTrackingHash());
        return encryptedBallot;
    }

#pragma endregion

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

        Log::debugHex(": encryptSelection: for " + description.getObjectId() + " hash: ",
                      descriptionHash->toHex());

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
        throw "encryptSelection failed validity check";
    }

    unique_ptr<CiphertextBallotContest>
    encryptContest(const PlaintextBallotContest &plaintext, const ContestDescription &description,
                   const ElementModP &elgamalPublicKey, const ElementModQ &cryptoExtendedBaseHash,
                   const ElementModQ &nonceSeed, bool shouldVerifyProofs /* = true */)

    {
        // TODO: validate input

        auto descriptionHash = description.crypto_hash();
        auto nonceSequence =
          make_unique<Nonces>(*descriptionHash, &const_cast<ElementModQ &>(nonceSeed));
        auto contestNonce = nonceSequence->get(description.getSequenceOrder());
        auto proofNonce = nonceSequence->next();

        vector<unique_ptr<CiphertextBallotSelection>> encryptedSelections;

        Log::debugHex(": encryptContest: for descriptionHash: ", descriptionHash->toHex());

        // TODO: improve performance, complete features
        // note this is a simplified version of the loop in python
        // where the caller must supply all values and placeholder selections are not handled
        uint64_t max_sequenceId = 0;
        for (const auto &selectionDescription : description.getSelections()) {
            bool hasSelection = false;
            for (const auto &selection : plaintext.getSelections()) {
                if (selection.get().getObjectId() == selectionDescription.get().getObjectId()) {
                    hasSelection = true;
                    auto encrypted = encryptSelection(selection.get(), selectionDescription.get(),
                                                      elgamalPublicKey, cryptoExtendedBaseHash,
                                                      *contestNonce, false, shouldVerifyProofs);
                    encryptedSelections.push_back(move(encrypted));
                    break;
                }
            }

            if (selectionDescription.get().getSequenceOrder() > max_sequenceId) {
                max_sequenceId = selectionDescription.get().getSequenceOrder();
            }

            if (!hasSelection) {
                throw invalid_argument("caller must include all selections");
            }
        }

        // since we know the above will meet the selection limit,
        // just append the number elected count of placeholders with the proper description hashes
        for (uint64_t i = 0; i < description.getNumberElected(); i++) {
            auto sequence = max_sequenceId + 1 + i;
            auto placeholderId =
              description.getObjectId() + "-" + to_string(sequence) + "-placeholder";
            auto candidateId = description.getObjectId() + "-" + to_string(sequence) + "-candidate";
            auto placeholder =
              make_unique<SelectionDescription>(placeholderId, candidateId, sequence);
            auto plaintext = make_unique<PlaintextBallotSelection>(placeholderId, "0", true);

            auto encrypted =
              encryptSelection(*plaintext, *placeholder, elgamalPublicKey, cryptoExtendedBaseHash,
                               *contestNonce, true, shouldVerifyProofs);
            encryptedSelections.push_back(move(encrypted));
        }

        auto encryptedContest = CiphertextBallotContest::make(
          plaintext.getObjectId(), *descriptionHash, move(encryptedSelections), elgamalPublicKey,
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

        throw runtime_error("encryptContest failed validity check");
    }

    unique_ptr<CiphertextBallot> encryptBallot(const PlaintextBallot &plaintext,
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
        auto nonceSeed = CiphertextBallot::nonceSeed(metadata.getDescriptionHash(),
                                                     plaintext.getObjectId(), *randomMasterNonce);

        vector<unique_ptr<CiphertextBallotContest>> encryptedContests;

        Log::debugHex(": encryptBallot: for descriptionHash: ",
                      metadata.getDescriptionHash().toHex());

        // TODO: improve performance, complete features
        for (const auto &contestDescription : metadata.getContests()) {
            bool hasContest = false;
            for (const auto &contest : plaintext.getContests()) {
                if (contest.get().getObjectId() == contestDescription.get().getObjectId()) {
                    hasContest = true;
                    auto encrypted = encryptContest(
                      contest.get(), contestDescription.get(), *context.getElGamalPublicKey(),
                      *context.getCryptoExtendedBaseHash(), *nonceSeed, shouldVerifyProofs);

                    encryptedContests.push_back(move(encrypted));
                    break;
                }
            }
            if (!hasContest) {
                throw invalid_argument("caller must include all contests");
            }
        }

        const uint64_t defaultTimestamp = 0;
        auto encryptedBallot = CiphertextBallot::make(
          plaintext.getObjectId(), plaintext.getBallotStyle(), metadata.getDescriptionHash(),
          move(encryptedContests), move(nonceSeed), defaultTimestamp,
          make_unique<ElementModQ>(seedHash));

        if (!encryptedBallot) {
            throw runtime_error("encryptedBallot:: Error constructing encrypted ballot");
        }

        Log::debug(": Encrypted Ballot: encrypted: " + encryptedBallot->getObjectId());
        Log::debug(encryptedBallot->toJson());

        if (!shouldVerifyProofs) {
            return encryptedBallot;
        }

        // verify the ballot.
        if (encryptedBallot->isValidEncryption(metadata.getDescriptionHash(),
                                               *context.getElGamalPublicKey(),
                                               *context.getCryptoExtendedBaseHash())) {
            return encryptedBallot;
        }

        throw runtime_error("encryptedBallot: failed validity check");
    }

} // namespace electionguard
