#include "electionguard/encrypt.hpp"

#include "async.hpp"
#include "electionguard/ballot_code.hpp"
#include "electionguard/elgamal.hpp"
#include "electionguard/hash.hpp"
#include "log.hpp"
#include "nonces.hpp"
#include "utils.hpp"

#include <algorithm>
#include <future>
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

using electionguard::getSystemTimestamp;

namespace electionguard
{
#pragma region EncryptionDevice

    struct EncryptionDevice::Impl {
        uint64_t deviceUuid;
        uint64_t sessionUuid;
        uint64_t launchCode;
        string location;

        Impl(const uint64_t deviceUuid, const uint64_t sessionUuid, const uint64_t launchCode,
             const string &location)
        {
            this->deviceUuid = deviceUuid;
            this->sessionUuid = sessionUuid;
            this->launchCode = launchCode;
            this->location = location;
        }
    };

    EncryptionDevice::EncryptionDevice(const uint64_t deviceUuid, const uint64_t sessionUuid,
                                       const uint64_t launchCode, const string &location)
        : pimpl(new Impl(deviceUuid, sessionUuid, launchCode, location))
    {
        Log::trace("EncryptionDevice: Created: UUID: " + to_string(deviceUuid) +
                   " at: " + location);
    }
    EncryptionDevice::~EncryptionDevice() = default;

    EncryptionDevice &EncryptionDevice::operator=(EncryptionDevice other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    unique_ptr<ElementModQ> EncryptionDevice::getHash() const
    {
        return BallotCode::getHashForDevice(pimpl->deviceUuid, pimpl->sessionUuid,
                                            pimpl->launchCode, pimpl->location);
    }

    uint64_t EncryptionDevice::getTimestamp() const { return getSystemTimestamp(); }

#pragma endregion

#pragma region EncryptionMediator

    struct EncryptionMediator::Impl {
        const InternalManifest &internalManifest;
        const CiphertextElectionContext &context;
        const EncryptionDevice &encryptionDevice;
        unique_ptr<ElementModQ> ballotCodeSeed;

        Impl(const InternalManifest &internalManifest, const CiphertextElectionContext &context,
             const EncryptionDevice &encryptionDevice)
            : internalManifest(internalManifest), context(context),
              encryptionDevice(encryptionDevice)

        {
        }
    };

    EncryptionMediator::EncryptionMediator(const InternalManifest &internalManifest,
                                           const CiphertextElectionContext &context,
                                           const EncryptionDevice &encryptionDevice)
        : pimpl(new Impl(internalManifest, context, encryptionDevice))
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
        Log::trace("encrypt: objectId: " + ballot.getObjectId());

        // this implementation chains each ballot encrypted by the mediator
        // to every subsequent ballot creating a linked list data structure
        // that can be used to prove there are no gaps in the election record
        // but this is not required as part of the specification
        if (!pimpl->ballotCodeSeed) {
            auto deviceHash = pimpl->encryptionDevice.getHash();
            pimpl->ballotCodeSeed.swap(deviceHash);
            Log::debug("encrypt: instantiated ballotCodeSeed", pimpl->ballotCodeSeed->toHex());
        }

        auto encryptedBallot =
          encryptBallot(ballot, pimpl->internalManifest, pimpl->context, *pimpl->ballotCodeSeed,
                        nullptr, pimpl->encryptionDevice.getTimestamp(), shouldVerifyProofs);

        Log::trace("encrypt: ballot encrypted");
        pimpl->ballotCodeSeed = make_unique<ElementModQ>(*encryptedBallot->getBallotCode());
        return encryptedBallot;
    }

    unique_ptr<CompactCiphertextBallot>
    EncryptionMediator::compactEncrypt(const PlaintextBallot &ballot,
                                       bool shouldVerifyProofs /* = true */) const
    {
        Log::trace("encrypt: objectId:" + ballot.getObjectId());

        // this implementation chains each ballot encrypted by the mediator
        // to every subsequent ballot creating a linked list data structure
        // that can be used to prove there are no gaps in the election record
        // but this is not required as part of the specification.
        if (!pimpl->ballotCodeSeed) {
            auto deviceHash = pimpl->encryptionDevice.getHash();
            pimpl->ballotCodeSeed.swap(deviceHash);
            Log::debug("encrypt: instantiated ballotCodeSeed:", pimpl->ballotCodeSeed->toHex());
        }

        auto encryptedBallot = encryptCompactBallot(
          ballot, pimpl->internalManifest, pimpl->context, *pimpl->ballotCodeSeed, nullptr,
          pimpl->encryptionDevice.getTimestamp(), shouldVerifyProofs);

        Log::trace("encrypt: ballot encrypted");
        pimpl->ballotCodeSeed = make_unique<ElementModQ>(*encryptedBallot->getBallotCode());
        return encryptedBallot;
    }

#pragma endregion

    unique_ptr<PlaintextBallotSelection> selectionFrom(const SelectionDescription &description,
                                                       bool isPlaceholder = false,
                                                       bool isAffirmative = false)
    {
        return make_unique<PlaintextBallotSelection>(description.getObjectId(),
                                                     isAffirmative ? 1UL : 0UL, isPlaceholder);
    }

    unique_ptr<PlaintextBallotContest> contestFrom(const ContestDescription &description)
    {
        vector<unique_ptr<PlaintextBallotSelection>> selections;
        for (const auto &selectionDescription : description.getSelections()) {
            selections.push_back(selectionFrom(selectionDescription));
        }

        return make_unique<PlaintextBallotContest>(description.getObjectId(), move(selections));
    }

    unique_ptr<PlaintextBallotContest> emplaceMissingValues(const PlaintextBallotContest &contest,
                                                            const ContestDescription &description)
    {
        vector<unique_ptr<PlaintextBallotSelection>> selections;
        // loop through the selections for the contest
        for (const auto &selectionDescription : description.getSelections()) {
            bool hasSelection = false;
            // loop through all the existing contests to find an existing value
            for (const auto &selection : contest.getSelections()) {
                if (selection.get().getObjectId() == selectionDescription.get().getObjectId()) {
                    hasSelection = true;
                    selections.push_back(selection.get().clone());
                    break;
                }
            }

            // no selections provided for the contest, so create a placeholder contest
            if (!hasSelection) {
                selections.push_back(selectionFrom(selectionDescription));
            }
        }

        return make_unique<PlaintextBallotContest>(description.getObjectId(), move(selections));
    }

    unique_ptr<PlaintextBallot> emplaceMissingValues(const PlaintextBallot &ballot,
                                                     const InternalManifest &manifest)
    {
        auto *style = manifest.getBallotStyle(ballot.getStyleId());

        vector<unique_ptr<PlaintextBallotContest>> contests;
        // loop through the contests for the ballot style
        for (const auto &description : manifest.getContestsFor(style->getObjectId())) {
            bool hasContest = false;
            // loop through all the existing contests to find an existing value
            for (const auto &contest : ballot.getContests()) {
                if (contest.get().getObjectId() == description.get().getObjectId()) {
                    hasContest = true;
                    contests.push_back(emplaceMissingValues(contest.get(), description.get()));
                    break;
                }
            }
            // no selections provided for the contest, so create a placeholder contest
            if (!hasContest) {
                contests.push_back(contestFrom(description));
            }
        }
        return make_unique<PlaintextBallot>(ballot.getObjectId(), ballot.getStyleId(),
                                            move(contests));
    }

#pragma region Encryption Functions

    unique_ptr<CiphertextBallotSelection>
    encryptSelection(const PlaintextBallotSelection &selection,
                     const SelectionDescription &description, const ElementModP &elgamalPublicKey,
                     const ElementModQ &cryptoExtendedBaseHash, const ElementModQ &nonceSeed,
                     bool isPlaceholder /* = false */, bool shouldVerifyProofs /* = true */)
    {
        // Validate Input
        if (!selection.isValid(description.getObjectId())) {
            // todo: include plaintext data in log output
            throw invalid_argument("malformed input selection " + selection.getObjectId());
        }

        // Configure the crypto input values
        auto descriptionHash = description.crypto_hash();
        auto nonceSequence =
          make_unique<Nonces>(*descriptionHash, &const_cast<ElementModQ &>(nonceSeed));
        auto selectionNonce = nonceSequence->get(description.getSequenceOrder());
        auto proofNonce = nonceSequence->next();

        Log::trace("encryptSelection: for " + description.getObjectId() + " hash: ",
                   descriptionHash->toHex());

        // Generate the encryption
        auto ciphertext = elgamalEncrypt(selection.getVote(), *selectionNonce, elgamalPublicKey);
        if (ciphertext == nullptr) {
            throw runtime_error("encryptSelection:: Error generating ciphertext");
        }

        // TODO: ISSUE #134: encrypt/decrypt: encrypt the extended_data field

        auto encrypted = CiphertextBallotSelection::make(
          selection.getObjectId(), *descriptionHash, move(ciphertext), elgamalPublicKey,
          cryptoExtendedBaseHash, *proofNonce, selection.getVote(), isPlaceholder,
          move(selectionNonce));

        if (encrypted == nullptr || encrypted->getProof() == nullptr) {
            throw runtime_error("encryptSelection:: Error constructing encrypted selection");
        }

        // optionally, skip the verification step
        if (!shouldVerifyProofs) {
            return encrypted;
        }

        // verify the selection.
        if (encrypted->isValidEncryption(*descriptionHash, elgamalPublicKey,
                                         cryptoExtendedBaseHash)) {
            return encrypted;
        }
        throw runtime_error("encryptSelection failed validity check");
    }

    unique_ptr<CiphertextBallotContest>
    encryptContest(const PlaintextBallotContest &contest,
                   const ContestDescriptionWithPlaceholders &description,
                   const ElementModP &elgamalPublicKey, const ElementModQ &cryptoExtendedBaseHash,
                   const ElementModQ &nonceSeed, bool shouldVerifyProofs /* = true */)

    {
        // Validate Input
        if (!contest.isValid(description.getObjectId(), description.getSelections().size(),
                             description.getNumberElected(), description.getVotesAllowed())) {
            throw invalid_argument("the plaintext contest was invalid");
        }

        // TODO: validate the description inpout

        // account for sequence id
        auto descriptionHash = description.crypto_hash();
        auto nonceSequence =
          make_unique<Nonces>(*descriptionHash, &const_cast<ElementModQ &>(nonceSeed));
        auto contestNonce = nonceSequence->get(description.getSequenceOrder());
        auto chaumPedersenNonce = nonceSequence->next();
        std::shared_ptr<ElementModQ> sharedNonce(move(contestNonce));

        vector<future<unique_ptr<CiphertextBallotSelection>>> tasks;
        vector<unique_ptr<CiphertextBallotSelection>> encryptedSelections;

        // TODO: ISSUE #36: this code could be inefficient if we had a contest
        // with a lot of choices, although the O(n^2) iteration here is small
        // compared to the huge cost of doing the cryptography.
        uint64_t selectionCount = 0;

        // iterate over the actual selections for each contest description
        // and apply the selected value if it exists.  If it does not, an explicit
        // false is entered instead and the selection_count is not incremented
        // this allows consumers to only pass in the relevant selections made by a voter
        auto normalizedContest = emplaceMissingValues(contest, description);
        auto normalizedSelections = normalizedContest->getSelections();
        for (const auto &selectionDescription : description.getSelections()) {
            auto description_id = selectionDescription.get().getObjectId();
            if (auto selection =
                  std::find_if(normalizedSelections.begin(), normalizedSelections.end(),
                               [description_id](const PlaintextBallotSelection &item) {
                                   return item.getObjectId() == description_id;
                               });
                selection != normalizedSelections.end()) {

                // track the selection count so we can append the
                // appropriate number of true placeholder votes
                const auto &selectionRef = selection->get();
                selectionCount += selection->get().getVote();

                tasks.push_back(Scheduler::submit([=] {
                    auto encrypted = encryptSelection(
                      selection->get(), selectionDescription.get(), elgamalPublicKey,
                      cryptoExtendedBaseHash, *sharedNonce.get(), false, shouldVerifyProofs);
                    return encrypted;
                }));

            } else {
                // Should never happen since the contest is normalized by emplaceMissingValues
                throw runtime_error("encryptedContest:: Error constructing encrypted selection");
            }
        }

        // Handle Placeholder selections
        // After we loop through all of the real selections on the ballot,
        // we loop through each placeholder value and determine if it should be filled in
        for (const auto &placeholder : description.getPlaceholders()) {
            // for undervotes, select the placeholder value as true for each available seat
            // note this pattern is used since DisjunctiveChaumPedersen expects a 0 or 1
            // so each seat can only have a maximum value of 1 in the current implementation
            bool selectPlaceholder = false;
            if (selectionCount < description.getNumberElected()) {
                selectPlaceholder = true;
                selectionCount += 1;
            }

            tasks.push_back(Scheduler::submit([=] {
                auto placeholderSelection = selectionFrom(placeholder, true, selectPlaceholder);
                auto encrypted = encryptSelection(*placeholderSelection, placeholder,
                                                  elgamalPublicKey, cryptoExtendedBaseHash,
                                                  *sharedNonce.get(), false, shouldVerifyProofs);
                return encrypted;
            }));
        }

        // wait for all tasks to complete
        encryptedSelections = wait_all(tasks);

        // TODO: ISSUE #33: support other cases such as cumulative voting
        // (individual selections being an encryption of > 1)
        if (selectionCount < description.getVotesAllowed()) {
            Log::warn(
              "mismatching selection count: only n-of-m style elections are currently supported");
        }

        // Create the return object
        auto encryptedContest = CiphertextBallotContest::make(
          contest.getObjectId(), *descriptionHash, move(encryptedSelections), elgamalPublicKey,
          cryptoExtendedBaseHash, *chaumPedersenNonce, description.getNumberElected(),
          sharedNonce->clone());

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

    vector<unique_ptr<CiphertextBallotContest>>
    encryptContests(const PlaintextBallot &ballot, const InternalManifest &internalManifest,
                    const CiphertextElectionContext &context, const ElementModQ &nonceSeed,
                    bool shouldVerifyProofs /* = true */)
    {
        auto *style = internalManifest.getBallotStyle(ballot.getStyleId());
        vector<unique_ptr<CiphertextBallotContest>> encryptedContests;
        auto normalizedBallot = emplaceMissingValues(ballot, internalManifest);

        // only iterate on contests for this specific ballot style
        for (const auto &description : internalManifest.getContestsFor(style->getObjectId())) {
            bool hasContest = false;
            for (const auto &contest : normalizedBallot->getContests()) {
                if (contest.get().getObjectId() == description.get().getObjectId()) {
                    hasContest = true;
                    auto encrypted = encryptContest(
                      contest.get(), description.get(), *context.getElGamalPublicKey(),
                      *context.getCryptoExtendedBaseHash(), nonceSeed, shouldVerifyProofs);

                    encryptedContests.push_back(move(encrypted));
                    break;
                }
            }

            if (!hasContest) {
                // Should never happen since the ballot is normalized by emplacing missing values
                throw runtime_error("The ballot was malformed");
            }
        }
        return encryptedContests;
    }

    unique_ptr<CiphertextBallot>
    encryptBallot(const PlaintextBallot &ballot, const InternalManifest &manifest,
                  const CiphertextElectionContext &context, const ElementModQ &encryptionSeed,
                  unique_ptr<ElementModQ> nonce /* = nullptr */, uint64_t timestamp /* = 0 */,
                  bool shouldVerifyProofs /* = true */)
    {
        auto *style = manifest.getBallotStyle(ballot.getStyleId());

        // Validate Input
        if (style == nullptr) {
            throw runtime_error("could not find a ballot style: " + ballot.getStyleId());
        }

        // Generate a random seed nonce to use for the contest and selection nonce's on the ballot
        if (nonce == nullptr) {
            nonce = rand_q();
        }

        // Include a representation of the election and the external Id in the nonce's used
        // to derive other nonce values on the ballot
        auto nonceSeed =
          CiphertextBallot::nonceSeed(*manifest.getManifestHash(), ballot.getObjectId(), *nonce);

        Log::trace("manifestHash   : ", manifest.getManifestHash()->toHex());
        Log::trace("encryptionSeed          :", encryptionSeed.toHex());
        Log::trace("timestamp         : " + to_string(timestamp));

        // encrypt contests
        auto encryptedContests =
          encryptContests(ballot, manifest, context, *nonceSeed, shouldVerifyProofs);

        // Get the system time
        if (timestamp == 0) {
            timestamp = getSystemTimestamp();
        }

        // make the Ciphertext Ballot object
        auto encryptedBallot =
          CiphertextBallot::make(ballot.getObjectId(), ballot.getStyleId(),
                                 *manifest.getManifestHash(), move(encryptedContests), move(nonce),
                                 timestamp, make_unique<ElementModQ>(encryptionSeed), nullptr);

        if (!encryptedBallot) {
            throw runtime_error("encryptedBallot:: Error constructing encrypted ballot");
        }

        if (!shouldVerifyProofs) {
            return encryptedBallot;
        }

        // verify the ballot.
        if (encryptedBallot->isValidEncryption(*manifest.getManifestHash(),
                                               *context.getElGamalPublicKey(),
                                               *context.getCryptoExtendedBaseHash())) {
            return encryptedBallot;
        }

        throw runtime_error("encryptedBallot: failed validity check");
    }

    unique_ptr<CompactCiphertextBallot>
    encryptCompactBallot(const PlaintextBallot &ballot, const InternalManifest &manifest,
                         const CiphertextElectionContext &context,
                         const ElementModQ &ballotCodeSeed,
                         unique_ptr<ElementModQ> nonceSeed /* = nullptr */,
                         uint64_t timestamp /* = 0 */, bool shouldVerifyProofs /* = true*/)
    {
        auto normalized = emplaceMissingValues(ballot, manifest);
        auto ciphertext = encryptBallot(*normalized, manifest, context, ballotCodeSeed,
                                        move(nonceSeed), timestamp, shouldVerifyProofs);
        return CompactCiphertextBallot::make(*normalized, *ciphertext);
    }

#pragma endregion

} // namespace electionguard
