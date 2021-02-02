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

using std::invalid_argument;
using std::make_unique;
using std::runtime_error;
using std::to_string;
using std::unique_ptr;
using std::vector;

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

        Log::debug(": encrypt: ballot encrypted");
        pimpl->trackerHashSeed = make_unique<ElementModQ>(*encryptedBallot->getTrackingHash());
        return encryptedBallot;
    }

#pragma endregion

    unique_ptr<PlaintextBallotSelection> selectionFrom(const SelectionDescription &description,
                                                       bool isPlaceholder = false,
                                                       bool isAffirmative = false)
    {
        return make_unique<PlaintextBallotSelection>(description.getObjectId(),
                                                     isAffirmative ? "1" : "0", isPlaceholder);
    }

    unique_ptr<PlaintextBallotContest> contestFrom(const ContestDescription &description)
    {
        vector<unique_ptr<PlaintextBallotSelection>> selections;
        for (const auto &selectionDescription : description.getSelections()) {
            selections.push_back(selectionFrom(selectionDescription));
        }

        return make_unique<PlaintextBallotContest>(description.getObjectId(), move(selections));
    }

    unique_ptr<CiphertextBallotSelection>
    encryptSelection(const PlaintextBallotSelection &plaintext,
                     const SelectionDescription &description, const ElementModP &elgamalPublicKey,
                     const ElementModQ &cryptoExtendedBaseHash, const ElementModQ &nonceSeed,
                     bool isPlaceholder /* = false */, bool shouldVerifyProofs /* = true */)
    {
        // TODO: ISSUE #119: Validate Input

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

        // TODO: ISSUE #134: encrypt/decrypt: encrypt the extended_data field

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
    encryptContest(const PlaintextBallotContest &plaintext,
                   const ContestDescriptionWithPlaceholders &contestDescription,
                   const ElementModP &elgamalPublicKey, const ElementModQ &cryptoExtendedBaseHash,
                   const ElementModQ &nonceSeed, bool shouldVerifyProofs /* = true */)

    {
        // TODO: ISSUE #119: validate input

        // account for sequence id
        auto contestDescriptionHash = contestDescription.crypto_hash();
        auto nonceSequence =
          make_unique<Nonces>(*contestDescriptionHash, &const_cast<ElementModQ &>(nonceSeed));
        auto contestNonce = nonceSequence->get(contestDescription.getSequenceOrder());
        auto chaumPedersenNonce = nonceSequence->next();

        vector<unique_ptr<CiphertextBallotSelection>> encryptedSelections;

        // TODO: ISSUE #36: this code could be inefficient if we had a contest
        // with a lot of choices, although the O(n^2) iteration here is small
        // compared to the huge cost of doing the cryptography.
        uint64_t selectionCount = 0;

        for (const auto &selectionDescription : contestDescription.getSelections()) {
            bool hasSelection = false;
            unique_ptr<CiphertextBallotSelection> encryptedSelection = nullptr;

            // iterate over the actual selections for each contest description
            // and apply the selected value if it exists.  If it does not, an explicit
            // false is entered instead and the selection_count is not incremented
            // this allows consumers to only pass in the relevant selections made by a voter
            for (const auto &selection : plaintext.getSelections()) {
                if (selection.get().getObjectId() == selectionDescription.get().getObjectId()) {
                    // track the selection count so we can append the
                    // appropriate number of true placeholder votes
                    hasSelection = true;
                    selectionCount += selection.get().toInt();

                    auto encrypted = encryptSelection(selection.get(), selectionDescription.get(),
                                                      elgamalPublicKey, cryptoExtendedBaseHash,
                                                      *contestNonce, false, shouldVerifyProofs);
                    encryptedSelection = move(encrypted);
                    break;
                }
            }

            if (!hasSelection) {
                // No selection was made for this possible value
                // so we explicitly set it to false
                auto emptySelection = selectionFrom(selectionDescription);
                auto encrypted = encryptSelection(*emptySelection, selectionDescription.get(),
                                                  elgamalPublicKey, cryptoExtendedBaseHash,
                                                  *contestNonce, false, shouldVerifyProofs);
                encryptedSelection = move(encrypted);
            }

            if (encryptedSelection == nullptr) {
                throw runtime_error("encryptedContest:: Error constructing encrypted selection");
            }
            encryptedSelections.push_back(move(encryptedSelection));
        }

        // Handle Placeholder selections
        // After we loop through all of the real selections on the ballot,
        // we loop through each placeholder value and determine if it should be filled in
        for (const auto &placeholder : contestDescription.getPlaceholders()) {
            // for undervotes, select the placeholder value as true for each available seat
            // note this pattern is used since DisjunctiveChaumPedersen expects a 0 or 1
            // so each seat can only have a maximum value of 1 in the current implementation
            bool selectPlaceholder = false;
            if (selectionCount < contestDescription.getNumberElected()) {
                selectPlaceholder = true;
                selectionCount += 1;
            }

            auto placeholderSelection = selectionFrom(placeholder, true, selectPlaceholder);
            auto encryptedSelection =
              encryptSelection(*placeholderSelection, placeholder, elgamalPublicKey,
                               cryptoExtendedBaseHash, *contestNonce, false, shouldVerifyProofs);

            if (encryptedSelection == nullptr) {
                throw runtime_error(
                  "encryptedContest:: Error constructing encrypted placeholder selection");
            }

            encryptedSelections.push_back(move(encryptedSelection));
        }

        // TODO: ISSUE #33: support other cases such as cumulative voting
        // (individual selections being an encryption of > 1)
        if (selectionCount < contestDescription.getVotesAllowed()) {
            Log::debug(
              "mismatching selection count: only n-of-m style elections are currently supported");
        }

        // Create the return object
        auto encryptedContest = CiphertextBallotContest::make(
          plaintext.getObjectId(), *contestDescriptionHash, move(encryptedSelections),
          elgamalPublicKey, cryptoExtendedBaseHash, *chaumPedersenNonce,
          contestDescription.getNumberElected(), move(contestNonce));

        if (encryptedContest == nullptr || encryptedContest->getProof() == nullptr) {
            throw runtime_error("encryptedContest:: Error constructing encrypted constest");
        }

        // optionally, skip the verification step
        if (!shouldVerifyProofs) {
            return encryptedContest;
        }

        // verify the contest.
        if (encryptedContest->isValidEncryption(*contestDescriptionHash, elgamalPublicKey,
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
        // TODO: ISSUE #36: finish implementation
        // this implementation currently only works with elections that have one ballot style
        // (e.g. where all contests in the election are included on every ballot.)
        // similarly it expects that the fully hydrated representation of the ballot is provided.

        auto randomMasterNonce = nonce ? move(nonce) : rand_q();
        auto nonceSeed = CiphertextBallot::nonceSeed(metadata.getDescriptionHash(),
                                                     plaintext.getObjectId(), *randomMasterNonce);

        vector<unique_ptr<CiphertextBallotContest>> encryptedContests;

        Log::debugHex(": encryptBallot: for descriptionHash: ",
                      metadata.getDescriptionHash().toHex());

        // TODO: ISSUE #36: improve performance, complete features
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
                Log::debug(": missing contest: " + contestDescription.get().getObjectId());
                throw invalid_argument("caller must include all contests");
            }
        }

        // TODO: ISSUE #125: timestamp should be an optional param on the interface
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
