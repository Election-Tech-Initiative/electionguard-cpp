#include "electionguard/ballot.hpp"
#include "electionguard/election_object_base.hpp"
#include "electionguard/encrypt.hpp"
#include "electionguard/hash.hpp"
#include "electionguard/tracker.hpp"
#include "log.hpp"
#include "serialize.hpp"
#include "utils.hpp"

#include <cstdlib>
#include <ctime>
#include <string>
#include <unordered_map>

using std::invalid_argument;
using std::make_unique;
using std::map;
using std::ref;
using std::reference_wrapper;
using std::runtime_error;
using std::string;
using std::time;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

using CompactPlaintextBallotSerializer = electionguard::Serialize::CompactPlaintextBallot;
using CompactCiphertextBallotSerializer = electionguard::Serialize::CompactCiphertextBallot;

namespace electionguard
{
#pragma region CompactPlaintextBallot

    struct CompactPlaintextBallot::Impl : public ElectionObjectBase {
        string ballotStyle;
        vector<uint64_t> selections;
        map<uint64_t, unique_ptr<ExtendedData>> extendedData;

        Impl(const string &objectId, const string &ballotStyle, vector<uint64_t> selections,
             map<uint64_t, unique_ptr<ExtendedData>> extendedData)
            : selections(move(selections)), extendedData(move(extendedData))
        {
            this->object_id = objectId;
            this->ballotStyle = ballotStyle;
        }
    };

    // Lifecycle Methods

    CompactPlaintextBallot::CompactPlaintextBallot(
      const string &objectId, const string &ballotStyle, vector<uint64_t> selections,
      map<uint64_t, unique_ptr<ExtendedData>> extendedData)
        : pimpl(new Impl(objectId, ballotStyle, move(selections), move(extendedData)))
    {
    }
    CompactPlaintextBallot::~CompactPlaintextBallot() = default;

    CompactPlaintextBallot &CompactPlaintextBallot::operator=(CompactPlaintextBallot other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string CompactPlaintextBallot::getObjectId() const { return pimpl->object_id; }
    string CompactPlaintextBallot::getBallotStyle() const { return pimpl->ballotStyle; }

    vector<uint64_t> CompactPlaintextBallot::getSelections() const { return pimpl->selections; }
    map<uint64_t, reference_wrapper<ExtendedData>> CompactPlaintextBallot::getExtendedData() const
    {
        map<uint64_t, reference_wrapper<ExtendedData>> extendedData;
        for (const auto &data : pimpl->extendedData) {
            extendedData.emplace(data.first, ref(*data.second));
        }
        return extendedData;
    }

    // Public Static Methods

    unique_ptr<CompactPlaintextBallot>
    CompactPlaintextBallot::make(const PlaintextBallot &plaintext)
    {
        return compressPlaintextBallot(plaintext);
    }

    // Public Methods

    unique_ptr<ExtendedData> CompactPlaintextBallot::getExtendedDataFor(const uint64_t index) const
    {
        auto extendedData = pimpl->extendedData.find(index);
        if (extendedData != pimpl->extendedData.end()) {
            return extendedData->second->clone();
        }
        return nullptr;
    }

    vector<uint8_t> CompactPlaintextBallot::toBson() const
    {
        return CompactPlaintextBallotSerializer::toBson(*this);
    }

    string CompactPlaintextBallot::toJson() const
    {
        return CompactPlaintextBallotSerializer::toJson(*this);
    }

    vector<uint8_t> CompactPlaintextBallot::toMsgPack() const
    {
        return CompactPlaintextBallotSerializer::toMsgPack(*this);
    }

    unique_ptr<CompactPlaintextBallot> CompactPlaintextBallot::fromBson(vector<uint8_t> data)
    {
        return CompactPlaintextBallotSerializer::fromBson(move(data));
    }

    unique_ptr<CompactPlaintextBallot> CompactPlaintextBallot::fromJson(string data)
    {
        return CompactPlaintextBallotSerializer::fromJson(move(data));
    }

    unique_ptr<CompactPlaintextBallot> CompactPlaintextBallot::fromMsgPack(vector<uint8_t> data)
    {
        return CompactPlaintextBallotSerializer::fromMsgPack(move(data));
    }

#pragma endregion

#pragma region CompactCiphertextBallot

    struct CompactCiphertextBallot::Impl : public ElectionObjectBase {
        unique_ptr<CompactPlaintextBallot> plaintext;
        BallotBoxState ballotBoxState;
        unique_ptr<ElementModQ> previousTrackingHash;
        unique_ptr<ElementModQ> trackingHash;
        uint64_t timestamp;
        unique_ptr<ElementModQ> ballotNonce;

        Impl(unique_ptr<CompactPlaintextBallot> plaintext, BallotBoxState ballotBoxState,
             unique_ptr<ElementModQ> previousTrackingHash, unique_ptr<ElementModQ> trackingHash,
             const uint64_t timestamp, unique_ptr<ElementModQ> ballotNonce)
            : plaintext(move(plaintext)), previousTrackingHash(move(previousTrackingHash)),
              trackingHash(move(trackingHash)), ballotNonce(move(ballotNonce))
        {
            this->ballotBoxState = ballotBoxState;
            this->timestamp = timestamp;
        }
    };

    // Lifecycle Methods

    CompactCiphertextBallot::CompactCiphertextBallot(unique_ptr<CompactPlaintextBallot> plaintext,
                                                     BallotBoxState ballotBoxState,
                                                     unique_ptr<ElementModQ> previousTrackingHash,
                                                     unique_ptr<ElementModQ> trackingHash,
                                                     const uint64_t timestamp,
                                                     unique_ptr<ElementModQ> ballotNonce)
        : pimpl(new Impl(move(plaintext), ballotBoxState, move(previousTrackingHash),
                         move(trackingHash), timestamp, move(ballotNonce)))
    {
    }
    CompactCiphertextBallot::~CompactCiphertextBallot() = default;

    CompactCiphertextBallot &CompactCiphertextBallot::operator=(CompactCiphertextBallot other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    std::string CompactCiphertextBallot::getObjectId() const
    {
        return pimpl->plaintext->getObjectId();
    }

    CompactPlaintextBallot *CompactCiphertextBallot::getPlaintext() const
    {
        return pimpl->plaintext.get();
    }

    ElementModQ *CompactCiphertextBallot::getPreviousTrackingHash() const
    {
        return pimpl->previousTrackingHash.get();
    }

    ElementModQ *CompactCiphertextBallot::getTrackingHash() const
    {
        return pimpl->trackingHash.get();
    }

    ElementModQ *CompactCiphertextBallot::getNonce() const { return pimpl->ballotNonce.get(); }

    uint64_t CompactCiphertextBallot::getTimestamp() const { return pimpl->timestamp; }

    BallotBoxState CompactCiphertextBallot::getBallotBoxState() const
    {
        return pimpl->ballotBoxState;
    }

    // Public Static Methods

    std::unique_ptr<CompactCiphertextBallot>
    CompactCiphertextBallot::make(const PlaintextBallot &plaintext,
                                  const CiphertextBallot &ciphertext)
    {
        return compressCiphertextBallot(plaintext, ciphertext);
    }

    // Public Methods

    void CompactCiphertextBallot::setBallotBoxState(BallotBoxState state)
    {
        pimpl->ballotBoxState = state;
    }

    vector<uint8_t> CompactCiphertextBallot::toMsgPack() const
    {
        return CompactCiphertextBallotSerializer::toMsgPack(*this);
    }

    vector<uint8_t> CompactCiphertextBallot::toBson() const
    {
        return CompactCiphertextBallotSerializer::toBson(*this);
    }

    string CompactCiphertextBallot::toJson() const
    {
        return CompactCiphertextBallotSerializer::toJson(*this);
    }

    unique_ptr<CompactCiphertextBallot> CompactCiphertextBallot::fromJson(string data)
    {
        return CompactCiphertextBallotSerializer::fromJson(move(data));
    }

    unique_ptr<CompactCiphertextBallot> CompactCiphertextBallot::fromBson(vector<uint8_t> data)
    {
        return CompactCiphertextBallotSerializer::fromBson(move(data));
    }

    unique_ptr<CompactCiphertextBallot> CompactCiphertextBallot::fromMsgPack(vector<uint8_t> data)
    {
        return CompactCiphertextBallotSerializer::fromMsgPack(move(data));
    }

#pragma endregion

#pragma region Compress Functions

    unique_ptr<CompactPlaintextBallot> compressPlaintextBallot(const PlaintextBallot &plaintext)
    {
        vector<uint64_t> selections;
        map<uint64_t, unique_ptr<ExtendedData>> extendedData;
        uint32_t index;
        for (const auto &contest : plaintext.getContests()) {
            for (const auto &selection : contest.get().getSelections()) {
                selections.push_back(selection.get().getVote());
                if (selection.get().getExtendedData() != nullptr) {
                    extendedData.emplace(index, selection.get().getExtendedData()->clone());
                }
                index++;
            }
        }

        return make_unique<CompactPlaintextBallot>(plaintext.getObjectId(),
                                                   plaintext.getBallotStyle(), move(selections),
                                                   move(extendedData));
    }

    unique_ptr<CompactCiphertextBallot> compressCiphertextBallot(const PlaintextBallot &plaintext,
                                                                 const CiphertextBallot &ciphertext)
    {
        if (ciphertext.getPreviousTrackingHash() == nullptr) {
            throw invalid_argument("The previous tracking hash was null");
        }

        if (ciphertext.getTrackingHash() == nullptr) {
            throw invalid_argument("The tracking hash was null");
        }

        if (ciphertext.getNonce() == nullptr) {
            throw invalid_argument("The nonce was null");
        }

        auto compactPlaintext = CompactPlaintextBallot::make(plaintext);

        return make_unique<CompactCiphertextBallot>(
          move(compactPlaintext), BallotBoxState::unknown,
          ciphertext.getPreviousTrackingHash()->clone(), ciphertext.getTrackingHash()->clone(),
          ciphertext.getTimestamp(), ciphertext.getNonce()->clone());
    }

#pragma endregion

#pragma region Expand Functions

    unique_ptr<PlaintextBallot>
    expandCompactPlaintextBallot(const CompactPlaintextBallot &compactBallot,
                                 const InternalElectionDescription &metadata)
    {
        vector<unique_ptr<PlaintextBallotContest>> contests;
        uint64_t index = 0;

        // Get the ballot style and iterate through the contests for that style
        for (const auto &contest : metadata.getContestsFor(compactBallot.getBallotStyle())) {
            vector<unique_ptr<PlaintextBallotSelection>> selections;

            // Iterate through the selections on the contest and expand each selection
            for (const auto &selection : contest.get().getSelections()) {
                auto vote = compactBallot.getSelections().at(index);
                auto extendedData = compactBallot.getExtendedDataFor(index);

                selections.push_back(make_unique<PlaintextBallotSelection>(
                  selection.get().getObjectId(), vote, false,
                  extendedData == nullptr ? nullptr : move(extendedData)));

                index++;
            }

            contests.push_back(
              make_unique<PlaintextBallotContest>(contest.get().getObjectId(), move(selections)));
        }

        return make_unique<PlaintextBallot>(compactBallot.getObjectId(),
                                            compactBallot.getBallotStyle(), move(contests));
    }

    unique_ptr<CiphertextBallot>
    expandCompactCiphertextBallot(const CompactCiphertextBallot &compactCiphertext,
                                  const InternalElectionDescription &metadata,
                                  const CiphertextElectionContext &context)
    {
        if (compactCiphertext.getPlaintext() == nullptr) {
            throw runtime_error("the plaintext seelctions were not found");
        }
        if (compactCiphertext.getPreviousTrackingHash() == nullptr) {
            throw runtime_error("the previous tracking hash was not found");
        }

        if (compactCiphertext.getTrackingHash() == nullptr) {
            throw runtime_error("the tracking hash was not found");
        }

        if (compactCiphertext.getNonce() == nullptr) {
            throw runtime_error("the nonce was not found");
        }

        auto plaintext = expandCompactPlaintextBallot(*compactCiphertext.getPlaintext(), metadata);
        auto ciphertext =
          encryptBallot(*plaintext, metadata, context, *compactCiphertext.getPreviousTrackingHash(),
                        compactCiphertext.getNonce()->clone(), compactCiphertext.getTimestamp());

        if (*ciphertext->getTrackingHash() != *compactCiphertext.getTrackingHash()) {
            throw runtime_error("The regenerated tracking hash does not match");
        }

        return ciphertext;
    }

#pragma endregion

} // namespace electionguard
