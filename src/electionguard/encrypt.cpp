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
        InternalElectionDescription &metadata;
        CiphertextElectionContext &context;
        EncryptionDevice &encryptionDevice;
        unique_ptr<ElementModQ> trackerHashSeed;

        Impl(InternalElectionDescription &metadata, CiphertextElectionContext &context,
             EncryptionDevice &encryptionDevice)
            : metadata(metadata), context(context), encryptionDevice(encryptionDevice)

        {
        }
    };

    EncryptionMediator::EncryptionMediator(InternalElectionDescription &metadata,
                                           CiphertextElectionContext &context,
                                           EncryptionDevice &encryptionDevice)
        : pimpl(new Impl(metadata, context, encryptionDevice))
    {
    }

    EncryptionMediator::~EncryptionMediator() = default;

    EncryptionMediator &EncryptionMediator::operator=(EncryptionMediator other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    unique_ptr<CiphertextBallot> EncryptionMediator::encrypt(const PlaintextBallot &ballot) const
    {

        if (!pimpl->trackerHashSeed) {
            auto trackerHashSeed = pimpl->encryptionDevice.getHash();
            pimpl->trackerHashSeed.swap(trackerHashSeed);
        }

        auto encryptedBallot =
          encryptBallot(ballot, pimpl->metadata, pimpl->context, *pimpl->trackerHashSeed);

        pimpl->trackerHashSeed = make_unique<ElementModQ>(*encryptedBallot->getTrackingHash());
        return encryptedBallot;
    }

#pragma endregion

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
        throw "encryptSelection failed validity check";
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

        throw "encryptContest failed validity check";
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
        auto nonceSeed = CiphertextBallot::nonceSeed(metadata.getDescriptionHash(),
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
                                                      metadata.getDescriptionHash(),
                                                      move(encryptedContests), move(nonceSeed));

        if (!encryptedBallot) {
            throw runtime_error("encryptedBallot:: Error constructing encrypted ballot");
        }

        if (!shouldVerifyProofs) {
            return encryptedBallot;
        }

        // verify the ballot.
        if (encryptedBallot->isValidEncryption(metadata.getDescriptionHash(),
                                               *context.getElGamalPublicKey(),
                                               *context.getCryptoExtendedBaseHash())) {
            return encryptedBallot;
        }

        throw "encryptedBallot failed validity check";
    }

} // namespace electionguard
