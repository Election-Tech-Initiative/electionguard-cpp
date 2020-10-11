#include "electionguard/ballot.hpp"

#include "electionguard/election_object_base.hpp"
#include "electionguard/hash.hpp"
#include "electionguard/tracker.hpp"
#include "log.hpp"
#include "serialize.hpp"

#include <cstdlib>
#include <ctime>
#include <string>
#include <unordered_map>

using std::string;
using std::time;

using PlaintextBallotSerializer = electionguard::Serialize::PlaintextBallot;
using CiphertextBallotSerializer = electionguard::Serialize::CiphertextBallot;

namespace electionguard
{
#pragma region PlaintextBallotSelection

    struct PlaintextBallotSelection::Impl : public ElectionObjectBase {
        string vote;
        bool isPlaceholderSelection;

        Impl(string objectId, string vote, bool isPlaceholderSelection /* = false */)
            : vote(move(vote)), isPlaceholderSelection(isPlaceholderSelection)
        {
            this->object_id = move(objectId);
        }

        // TODO: secure erase the vote
    };

    // Lifecycle Methods

    PlaintextBallotSelection::PlaintextBallotSelection(string objectId, string vote,
                                                       bool isPlaceholderSelection /* = false */)
        : pimpl(new Impl(move(objectId), move(vote), isPlaceholderSelection))
    {
    }

    PlaintextBallotSelection::~PlaintextBallotSelection() = default;

    PlaintextBallotSelection &PlaintextBallotSelection::operator=(PlaintextBallotSelection other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    uint64_t PlaintextBallotSelection::toInt() const { return stoul(pimpl->vote); }
    string PlaintextBallotSelection::getObjectId() const { return pimpl->object_id; }

#pragma endregion

#pragma region CiphertextBallotSelection

    struct CiphertextBallotSelection::Impl {
        string objectId;
        unique_ptr<ElementModQ> descriptionHash;
        unique_ptr<ElGamalCiphertext> ciphertext;
        bool isPlaceholder;
        unique_ptr<ElementModQ> nonce;
        unique_ptr<ElementModQ> cryptoHash;
        unique_ptr<DisjunctiveChaumPedersenProof> proof;
        unique_ptr<ElGamalCiphertext> extendedData;

        Impl(string objectId, unique_ptr<ElementModQ> descriptionHash,
             unique_ptr<ElGamalCiphertext> ciphertext, bool isPlaceholder,
             unique_ptr<ElementModQ> nonce, unique_ptr<ElementModQ> cryptoHash,
             unique_ptr<DisjunctiveChaumPedersenProof> proof,
             unique_ptr<ElGamalCiphertext> extendedData)
            : objectId(move(objectId)), descriptionHash(move(descriptionHash)),
              ciphertext(move(ciphertext)), nonce(move(nonce)), cryptoHash(move(cryptoHash)),
              proof(move(proof)), extendedData(move(extendedData))
        {
            this->isPlaceholder = isPlaceholder;
        }

        unique_ptr<ElementModQ> crypto_hash_with(const ElementModQ &seedHash) const
        {
            return makeCryptoHash(objectId, seedHash, *ciphertext);
        }
    };

    // Lifecycle Methods

    CiphertextBallotSelection::CiphertextBallotSelection(
      const string &objectId, const ElementModQ &descriptionHash,
      unique_ptr<ElGamalCiphertext> ciphertext, bool isPlaceholder, unique_ptr<ElementModQ> nonce,
      unique_ptr<ElementModQ> cryptoHash, unique_ptr<DisjunctiveChaumPedersenProof> proof,
      unique_ptr<ElGamalCiphertext> extendedData)
        : pimpl(new Impl(objectId, make_unique<ElementModQ>(descriptionHash), move(ciphertext),
                         isPlaceholder, move(nonce), move(cryptoHash), move(proof),
                         move(extendedData)))

    {
    }

    CiphertextBallotSelection::~CiphertextBallotSelection() = default;

    CiphertextBallotSelection &CiphertextBallotSelection::operator=(CiphertextBallotSelection other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // property Getters

    string CiphertextBallotSelection::getObjectId() const { return pimpl->objectId; }

    ElementModQ *CiphertextBallotSelection::getDescriptionHash() const
    {
        return pimpl->descriptionHash.get();
    }

    bool CiphertextBallotSelection::getIsPlaceholder() const { return pimpl->isPlaceholder; }

    ElGamalCiphertext *CiphertextBallotSelection::getCiphertext() const
    {
        return pimpl->ciphertext.get();
    }

    ElementModQ *CiphertextBallotSelection::getCryptoHash() const
    {
        return pimpl->cryptoHash.get();
    }

    ElementModQ *CiphertextBallotSelection::getNonce() { return pimpl->nonce.get(); }

    DisjunctiveChaumPedersenProof *CiphertextBallotSelection::getProof() const
    {
        return pimpl->proof.get();
    }

    // Interface Overrides

    unique_ptr<ElementModQ>
    CiphertextBallotSelection::crypto_hash_with(const ElementModQ &seedHash) const
    {
        return pimpl->crypto_hash_with(seedHash);
    }

    // public Static Members

    unique_ptr<CiphertextBallotSelection> CiphertextBallotSelection::make(
      const string &objectId, const ElementModQ &descriptionHash,
      unique_ptr<ElGamalCiphertext> ciphertext, const ElementModP &elgamalPublicKey,
      const ElementModQ &cryptoExtendedBaseHash, const ElementModQ &proofSeed, uint64_t plaintext,
      bool isPlaceholder /* = false */, unique_ptr<ElementModQ> nonce /* = nullptr */,
      unique_ptr<ElementModQ> cryptoHash /* = nullptr */,
      unique_ptr<DisjunctiveChaumPedersenProof> proof /* = nullptr */,
      unique_ptr<ElGamalCiphertext> extendedData /* = nullptr */)
    {
        if (cryptoHash == nullptr) {
            auto crypto_hash = makeCryptoHash(objectId, descriptionHash, *ciphertext);
            cryptoHash = move(crypto_hash);
        }
        if (proof == nullptr) {
            proof = DisjunctiveChaumPedersenProof::make(
              *ciphertext, *nonce, elgamalPublicKey, cryptoExtendedBaseHash, proofSeed, plaintext);
        }
        return make_unique<CiphertextBallotSelection>(objectId, descriptionHash, move(ciphertext),
                                                      isPlaceholder, move(nonce), move(cryptoHash),
                                                      move(proof), move(extendedData));
    }

    unique_ptr<ElementModQ> CiphertextBallotSelection::crypto_hash_with(const ElementModQ &seedHash)
    {
        return pimpl->crypto_hash_with(seedHash);
    }

    // Public Methods

    bool CiphertextBallotSelection::isValidEncryption(const ElementModQ &seedHash,
                                                      const ElementModP &elgamalPublicKey,
                                                      const ElementModQ &cryptoExtendedBaseHash)
    {
        if ((const_cast<ElementModQ &>(seedHash) != *pimpl->descriptionHash)) {
            Log::debug(": CiphertextBallotSelection mismatching selection hash: ");
            Log::debugHex(": expected: ", seedHash.toHex());
            Log::debugHex(": actual: ", pimpl->descriptionHash->toHex());
            return false;
        }

        auto recalculatedCryptoHash = crypto_hash_with(seedHash);
        if ((*pimpl->cryptoHash != *recalculatedCryptoHash)) {
            Log::debug(": CiphertextBallotSelection mismatching crypto hash: ");
            Log::debugHex(": expected: ", recalculatedCryptoHash->toHex());
            Log::debugHex(": actual: ", pimpl->cryptoHash->toHex());
            return false;
        }
        return pimpl->proof->isValid(*pimpl->ciphertext, elgamalPublicKey, cryptoExtendedBaseHash);
    }

    // Protected Members

    unique_ptr<ElementModQ>
    CiphertextBallotSelection::makeCryptoHash(const string &objectId, const ElementModQ &seedHash,
                                              const ElGamalCiphertext &ciphertext)
    {
        return hash_elems(
          {objectId, &const_cast<ElementModQ &>(seedHash), ciphertext.crypto_hash().get()});
    }

#pragma endregion

#pragma region PlaintextBallotContest

    struct PlaintextBallotContest::Impl : public ElectionObjectBase {
        vector<unique_ptr<PlaintextBallotSelection>> selections;
        Impl(const string &objectId, vector<unique_ptr<PlaintextBallotSelection>> selections)
            : selections(move(selections))
        {
            this->object_id = objectId;
        }
    };

    // Lifecycle Methods

    PlaintextBallotContest::PlaintextBallotContest(
      const string &objectId, vector<unique_ptr<PlaintextBallotSelection>> selections)
        : pimpl(new Impl(objectId, move(selections)))
    {
    }
    PlaintextBallotContest::~PlaintextBallotContest() = default;

    PlaintextBallotContest &PlaintextBallotContest::operator=(PlaintextBallotContest other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string PlaintextBallotContest::getObjectId() const { return pimpl->object_id; }

    vector<reference_wrapper<PlaintextBallotSelection>>
    PlaintextBallotContest::getSelections() const
    {
        vector<reference_wrapper<PlaintextBallotSelection>> selections;
        selections.reserve(pimpl->selections.size());
        for (auto &selection : pimpl->selections) {
            selections.push_back(ref(*selection));
        }
        return selections;
    }

#pragma endregion

#pragma region CiphertextBallotContest

    struct CiphertextBallotContest::Impl : ElectionObjectBase {
        unique_ptr<ElementModQ> descriptionHash;
        vector<unique_ptr<CiphertextBallotSelection>> selections;
        unique_ptr<ElementModQ> nonce;
        unique_ptr<ElementModQ> cryptoHash;
        unique_ptr<ConstantChaumPedersenProof> proof;

        Impl(const string &objectId, unique_ptr<ElementModQ> descriptionHash,
             vector<unique_ptr<CiphertextBallotSelection>> selections,
             unique_ptr<ElementModQ> nonce, unique_ptr<ElementModQ> cryptoHash,
             unique_ptr<ConstantChaumPedersenProof> proof)
            : descriptionHash(move(descriptionHash)), selections(move(selections)),
              nonce(move(nonce)), cryptoHash(move(cryptoHash)), proof(move(proof))
        {
            this->object_id = objectId;
        }
    };

    // Lifecycle Methods

    CiphertextBallotContest::CiphertextBallotContest(
      const string &objectId, const ElementModQ &descriptionHash,
      vector<unique_ptr<CiphertextBallotSelection>> selections, unique_ptr<ElementModQ> nonce,
      unique_ptr<ElementModQ> cryptoHash, unique_ptr<ConstantChaumPedersenProof> proof)
        : pimpl(new Impl(objectId, make_unique<ElementModQ>(descriptionHash), move(selections),
                         move(nonce), move(cryptoHash), move(proof)))
    {
    }
    CiphertextBallotContest::~CiphertextBallotContest() = default;

    CiphertextBallotContest &CiphertextBallotContest::operator=(CiphertextBallotContest other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string CiphertextBallotContest::getObjectId() const { return pimpl->object_id; }

    ElementModQ *CiphertextBallotContest::getDescriptionHash() const
    {
        return pimpl->descriptionHash.get();
    }

    vector<reference_wrapper<CiphertextBallotSelection>>
    CiphertextBallotContest::getSelections() const
    {
        vector<reference_wrapper<CiphertextBallotSelection>> selections;
        selections.reserve(pimpl->selections.size());
        for (const auto &selection : pimpl->selections) {
            selections.push_back(ref(*selection));
        }
        return selections;
    }

    ElementModQ *CiphertextBallotContest::getNonce() const { return pimpl->nonce.get(); }

    ElementModQ *CiphertextBallotContest::getCryptoHash() const { return pimpl->cryptoHash.get(); }

    ConstantChaumPedersenProof *CiphertextBallotContest::getProof() const
    {
        return pimpl->proof.get();
    }

    // Interface Overrides

    unique_ptr<ElementModQ>
    CiphertextBallotContest::crypto_hash_with(const ElementModQ &seedHash) const
    {
        return makeCryptoHash(pimpl->object_id, this->getSelections(), seedHash);
    }

    // Public Static Methods

    unique_ptr<CiphertextBallotContest> CiphertextBallotContest::make(
      const string &objectId, const ElementModQ &descriptionHash,
      vector<unique_ptr<CiphertextBallotSelection>> selections, const ElementModP &elgamalPublicKey,
      const ElementModQ &cryptoExtendedBaseHash, const ElementModQ &proofSeed,
      uint64_t numberElected, unique_ptr<ElementModQ> nonce /* = nullptr */,
      unique_ptr<ElementModQ> cryptoHash /* = nullptr */,
      unique_ptr<ConstantChaumPedersenProof> proof /* = nullptr */)
    {
        vector<reference_wrapper<CiphertextBallotSelection>> selectionReferences;
        selectionReferences.reserve(selections.size());
        for (const auto &selection : selections) {
            selectionReferences.push_back(ref(*selection));
        }

        if (cryptoHash == nullptr) {
            auto crypto_hash = makeCryptoHash(objectId, selectionReferences, descriptionHash);
            cryptoHash = move(crypto_hash);
        }
        if (proof == nullptr) {
            auto accumulated = elgamalAccumulate(selectionReferences);
            auto aggregate = aggregateNonce(selectionReferences);
            auto owned_proof =
              ConstantChaumPedersenProof::make(*accumulated, *aggregate, elgamalPublicKey,
                                               proofSeed, cryptoExtendedBaseHash, numberElected);
            proof = move(owned_proof);
        }
        return make_unique<CiphertextBallotContest>(objectId, descriptionHash, move(selections),
                                                    move(nonce), move(cryptoHash), move(proof));
    }

    // Public Methods

    unique_ptr<ElementModQ> CiphertextBallotContest::aggregateNonce() const
    {
        return aggregateNonce(this->getSelections());
    }

    unique_ptr<ElGamalCiphertext> CiphertextBallotContest::elgamalAccumulate() const
    {
        return elgamalAccumulate(this->getSelections());
    }

    bool CiphertextBallotContest::isValidEncryption(const ElementModQ &seedHash,
                                                    const ElementModP &elgamalPublicKey,
                                                    const ElementModQ &cryptoExtendedBaseHash)
    {
        if ((const_cast<ElementModQ &>(seedHash) != *pimpl->descriptionHash)) {
            Log::debug(": CiphertextBallotContest mismatching constest hash: ");
            Log::debugHex(": expected: ", seedHash.toHex());
            Log::debugHex(": actual: ", pimpl->descriptionHash->toHex());
            return false;
        }

        auto recalculatedCryptoHash = crypto_hash_with(seedHash);
        if ((*pimpl->cryptoHash != *recalculatedCryptoHash)) {
            Log::debug(": CiphertextBallotContest mismatching crypto hash: ");
            Log::debugHex(": expected: ", recalculatedCryptoHash->toHex());
            Log::debugHex(": actual: ", pimpl->cryptoHash->toHex());
            return false;
        }

        if (!pimpl->proof) {
            Log::debug(": no proof exists for: " + pimpl->object_id);
            return false;
        }

        auto accumulation = this->elgamalAccumulate();
        return pimpl->proof->isValid(*accumulation, elgamalPublicKey, cryptoExtendedBaseHash);
    }

    // Protected Members

    unique_ptr<ElementModQ> CiphertextBallotContest::aggregateNonce(
      const vector<reference_wrapper<CiphertextBallotSelection>> &selections)
    {
        vector<reference_wrapper<ElementModQ>> elements;
        elements.reserve(selections.size());
        for (const auto &selection : selections) {
            elements.push_back(ref(*selection.get().getNonce()));
        }
        return add_mod_q(elements);
    }

    unique_ptr<ElGamalCiphertext> CiphertextBallotContest::elgamalAccumulate(
      const vector<reference_wrapper<CiphertextBallotSelection>> &selections)
    {
        vector<reference_wrapper<ElGamalCiphertext>> ciphertexts;
        ciphertexts.reserve(selections.size());
        for (const auto &selection : selections) {
            ciphertexts.push_back(ref(*selection.get().getCiphertext()));
        }
        return elgamalAdd(ciphertexts);
    }

    unique_ptr<ElementModQ> CiphertextBallotContest::makeCryptoHash(
      string objectId, const vector<reference_wrapper<CiphertextBallotSelection>> &selections,
      const ElementModQ &seedHash)
    {
        if (selections.empty()) {
            throw invalid_argument("mismatching selection state for " + objectId +
                                   "expected(some) actual(none)");
        }
        vector<CryptoHashableType> elems = {objectId, &const_cast<ElementModQ &>(seedHash)};
        for (const auto &selection : selections) {
            elems.push_back(ref(*selection.get().getCryptoHash()));
        }
        return hash_elems(elems);
    }

#pragma endregion

#pragma region PlaintextBallot

    struct PlaintextBallot::Impl : public ElectionObjectBase {
        string ballotStyle;
        vector<unique_ptr<PlaintextBallotContest>> contests;
        Impl(const string &objectId, const string &ballotStyle,
             vector<unique_ptr<PlaintextBallotContest>> contests)
            : contests(move(contests))
        {
            this->object_id = objectId;
            this->ballotStyle = ballotStyle;
        }
    };

    // Lifecycle Methods

    PlaintextBallot::PlaintextBallot(const string &objectId, const string &ballotStyle,
                                     vector<unique_ptr<PlaintextBallotContest>> contests)
        : pimpl(new Impl(objectId, ballotStyle, move(contests)))
    {
    }
    PlaintextBallot::~PlaintextBallot() = default;

    PlaintextBallot &PlaintextBallot::operator=(PlaintextBallot other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string PlaintextBallot::getObjectId() const { return pimpl->object_id; }
    string PlaintextBallot::getBallotStyle() const { return pimpl->ballotStyle; }

    vector<reference_wrapper<PlaintextBallotContest>> PlaintextBallot::getContests() const
    {
        vector<reference_wrapper<PlaintextBallotContest>> contests;
        contests.reserve(pimpl->contests.size());
        for (const auto &contest : pimpl->contests) {
            contests.push_back(ref(*contest));
        }
        return contests;
    }

    // Public Methods

    vector<uint8_t> PlaintextBallot::toBson() const
    {
        return PlaintextBallotSerializer::toBson(*this);
    }

    string PlaintextBallot::toJson() const { return PlaintextBallotSerializer::toJson(*this); }

    unique_ptr<PlaintextBallot> PlaintextBallot::fromJson(string data)
    {
        return PlaintextBallotSerializer::fromJson(move(data));
    }

    unique_ptr<PlaintextBallot> PlaintextBallot::fromBson(vector<uint8_t> data)
    {
        return PlaintextBallotSerializer::fromBson(move(data));
    }

#pragma endregion

#pragma region CiphertextBallot

    struct CiphertextBallot::Impl : public ElectionObjectBase {
        string ballotStyle;
        unique_ptr<ElementModQ> descriptionHash;
        unique_ptr<ElementModQ> previousTrackingHash;
        vector<unique_ptr<CiphertextBallotContest>> contests;
        unique_ptr<ElementModQ> trackingHash;
        uint64_t timestamp;
        unique_ptr<ElementModQ> nonce;
        unique_ptr<ElementModQ> cryptoHash;

        Impl(const string &objectId, const string &ballotStyle,
             unique_ptr<ElementModQ> descriptionHash, unique_ptr<ElementModQ> previousTrackingHash,
             vector<unique_ptr<CiphertextBallotContest>> contests,
             unique_ptr<ElementModQ> trackingHash, const uint64_t timestamp,
             unique_ptr<ElementModQ> nonce, unique_ptr<ElementModQ> cryptoHash)
            : descriptionHash(move(descriptionHash)),
              previousTrackingHash(move(previousTrackingHash)), contests(move(contests)),
              trackingHash(move(trackingHash)), nonce(move(nonce)), cryptoHash(move(cryptoHash))
        {
            this->object_id = objectId;
            this->ballotStyle = ballotStyle;
            this->timestamp = timestamp;
        }
    };

    // Lifecycle Methods

    CiphertextBallot::CiphertextBallot(const string &objectId, const string &ballotStyle,
                                       const ElementModQ &descriptionHash,
                                       unique_ptr<ElementModQ> previousTrackingHash,
                                       vector<unique_ptr<CiphertextBallotContest>> contests,
                                       unique_ptr<ElementModQ> trackingHash,
                                       const uint64_t timestamp, unique_ptr<ElementModQ> nonce,
                                       unique_ptr<ElementModQ> cryptoHash)
        : pimpl(new Impl(objectId, ballotStyle, make_unique<ElementModQ>(descriptionHash),
                         move(previousTrackingHash), move(contests), move(trackingHash), timestamp,
                         move(nonce), move(cryptoHash)))
    {
    }
    CiphertextBallot::~CiphertextBallot() = default;

    CiphertextBallot &CiphertextBallot::operator=(CiphertextBallot other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string CiphertextBallot::getObjectId() const { return pimpl->object_id; }
    string CiphertextBallot::getBallotStyle() const { return pimpl->ballotStyle; }
    ElementModQ *CiphertextBallot::getDescriptionHash() const
    {
        return pimpl->descriptionHash.get();
    }
    ElementModQ *CiphertextBallot::getPreviousTrackingHash() const
    {
        return pimpl->previousTrackingHash.get();
    }

    vector<reference_wrapper<CiphertextBallotContest>> CiphertextBallot::getContests() const
    {
        // TODO: templatize this pattern
        vector<reference_wrapper<CiphertextBallotContest>> contests;
        contests.reserve(pimpl->contests.size());
        for (const auto &contest : pimpl->contests) {
            contests.push_back(ref(*contest));
        }
        return contests;
    }

    ElementModQ *CiphertextBallot::getTrackingHash() const { return pimpl->trackingHash.get(); }

    string CiphertextBallot::getTrackingCode() const
    {
        return Tracker::hashToWords(*pimpl->trackingHash);
    }

    uint64_t CiphertextBallot::getTimestamp() const { return pimpl->timestamp; }

    ElementModQ *CiphertextBallot::getNonce() const { return pimpl->nonce.get(); }

    ElementModQ *CiphertextBallot::getCryptoHash() const { return pimpl->cryptoHash.get(); }

    // Interface Overrides

    unique_ptr<ElementModQ> CiphertextBallot::crypto_hash_with(const ElementModQ &seedHash) const
    {
        return makeCryptoHash(pimpl->object_id, this->getContests(), seedHash);
    }

    // Public Static Methods

    unique_ptr<CiphertextBallot> CiphertextBallot::make(
      const string &objectId, const string &ballotStyle, const ElementModQ &descriptionHash,
      vector<unique_ptr<CiphertextBallotContest>> contests,
      unique_ptr<ElementModQ> nonce /* = nullptr */, const uint64_t timestamp /* = 0 */,
      unique_ptr<ElementModQ> previousTrackingHash /* = nullptr */,
      unique_ptr<ElementModQ> trackingHash /* = nullptr */)
    {
        if (contests.empty()) {
            Log::debug(":ballot must have at least some contests");
            throw invalid_argument("ballot must have at least some contests");
        }

        vector<reference_wrapper<CiphertextBallotContest>> contestsRefs;
        contestsRefs.reserve(contests.size());
        for (const auto &contest : contests) {
            contestsRefs.push_back(ref(*contest));
        }

        auto cryptoHash = makeCryptoHash(objectId, contestsRefs, descriptionHash);
        auto ballotTimestamp = timestamp == 0 ? time(nullptr) : timestamp;
        if (!previousTrackingHash) {
            auto previous = make_unique<ElementModQ>(descriptionHash);
            previousTrackingHash = move(previous);
        }

        if (!trackingHash) {
            auto tracking =
              Tracker::getRotatingTrackerHash(*previousTrackingHash, ballotTimestamp, *cryptoHash);
            trackingHash.swap(tracking);
        }

        return make_unique<CiphertextBallot>(
          objectId, ballotStyle, descriptionHash, move(previousTrackingHash), move(contests),
          move(trackingHash), ballotTimestamp, move(nonce), move(cryptoHash));
    }

    unique_ptr<ElementModQ> CiphertextBallot::nonceSeed(const ElementModQ &descriptionHash,
                                                        const string &objectId,
                                                        const ElementModQ &nonce)
    {
        return hash_elems({ref(descriptionHash), objectId, ref(nonce)});
    }

    // Public Methods

    bool CiphertextBallot::isValidEncryption(const ElementModQ &seedHash,
                                             const ElementModP &elgamalPublicKey,
                                             const ElementModQ &cryptoExtendedBaseHash)
    {
        if ((const_cast<ElementModQ &>(seedHash) != *pimpl->descriptionHash)) {
            Log::debug(": CiphertextBallot mismatching ballot hash: ");
            Log::debugHex(": expected: ", seedHash.toHex());
            Log::debugHex(": actual: ", pimpl->descriptionHash->toHex());
            return false;
        }

        auto recalculatedCryptoHash = crypto_hash_with(seedHash);
        if ((*pimpl->cryptoHash != *recalculatedCryptoHash)) {
            Log::debug(": CiphertextBallot mismatching crypto hash: ");
            Log::debugHex(": expected: ", recalculatedCryptoHash->toHex());
            Log::debugHex(": actual: ", pimpl->cryptoHash->toHex());
            return false;
        }

        // Check the proofs on the ballot
        unordered_map<string, bool> validProofs;
        for (const auto &contest : this->getContests()) {
            for (const auto &selection : contest.get().getSelections()) {
                string key = contest.get().getObjectId() + '-' + selection.get().getObjectId();
                validProofs[key] = selection.get().isValidEncryption(
                  *selection.get().getDescriptionHash(), elgamalPublicKey, cryptoExtendedBaseHash);
            }
            string key = contest.get().getObjectId();
            validProofs[key] = contest.get().isValidEncryption(
              *contest.get().getDescriptionHash(), elgamalPublicKey, cryptoExtendedBaseHash);
        }

        bool isValid = true;
        for (const auto &[key, value] : validProofs) {
            if (!value) {
                Log::debug(": CiphertextBallot found invalid proof for: " + key);
                isValid = false;
            }
        }

        return isValid;
    }

    unique_ptr<ElementModQ> CiphertextBallot::nonceSeed()
    {
        return nonceSeed(*pimpl->descriptionHash, pimpl->object_id, *pimpl->nonce);
    }

    vector<uint8_t> CiphertextBallot::toBson() const
    {
        return CiphertextBallotSerializer::toBson(*this);
    }

    string CiphertextBallot::toJson() const { return CiphertextBallotSerializer::toJson(*this); }

    unique_ptr<CiphertextBallot> CiphertextBallot::fromJson(string data)
    {
        return CiphertextBallotSerializer::fromJson(move(data));
    }

    unique_ptr<CiphertextBallot> CiphertextBallot::fromBson(vector<uint8_t> data)
    {
        return CiphertextBallotSerializer::fromBson(move(data));
    }

    // Protected Methods

    unique_ptr<ElementModQ> CiphertextBallot::makeCryptoHash(
      string objectId, const vector<reference_wrapper<CiphertextBallotContest>> &contests,
      const ElementModQ &seedHash)
    {
        if (contests.empty()) {
            throw invalid_argument("mismatching contests state for " + objectId +
                                   "expected(some) actual(none)");
        }

        vector<CryptoHashableType> elems = {objectId, &const_cast<ElementModQ &>(seedHash)};
        for (const auto &contest : contests) {
            elems.push_back(ref(*contest.get().getCryptoHash()));
        }
        return hash_elems(elems);
    }

#pragma endregion

} // namespace electionguard
