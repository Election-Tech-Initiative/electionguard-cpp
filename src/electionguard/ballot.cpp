#include "electionguard/ballot.hpp"

#include "electionguard/ballot_code.hpp"
#include "electionguard/election_object_base.hpp"
#include "electionguard/hash.hpp"
#include "electionguard/precompute_buffers.hpp"
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
using std::string;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

using PlaintextBallotSerializer = electionguard::Serialize::PlaintextBallot;
using CiphertextBallotSerializer = electionguard::Serialize::CiphertextBallot;
using SubmittedBallotSerializer = electionguard::Serialize::SubmittedBallot;

namespace electionguard
{

#pragma region BallotBoxState

    template <typename> struct _ballot_box_state {
        static const map<BallotBoxState, const string> _map;
    };
    template <typename T>
    const map<BallotBoxState, const string> _ballot_box_state<T>::_map = {
      {BallotBoxState::cast, "cast"},
      {BallotBoxState::spoiled, "spoiled"},
      {BallotBoxState::unknown, "unknown"},
    };

    string getBallotBoxStateString(const BallotBoxState &value)
    {
        return _ballot_box_state<BallotBoxState>::_map.find(value)->second;
    }

    BallotBoxState getBallotBoxState(const string &value)
    {
        try {
            auto item = findByValue(_ballot_box_state<BallotBoxState>::_map, value);
            return item;
        } catch (const std::exception &e) {
            Log::error(": error", e);
            return BallotBoxState::unknown;
        }
    }

#pragma endregion

#pragma region PlaintextBallotSelection

    struct PlaintextBallotSelection::Impl : public ElectionObjectBase {
        uint64_t vote;
        bool isPlaceholderSelection;
        string writeIn;

        Impl(string objectId, uint64_t vote, bool isPlaceholderSelection /* = false */,
             string writeIn)
            : vote(vote), isPlaceholderSelection(isPlaceholderSelection),
              writeIn(move(writeIn))
        {
            this->object_id = move(objectId);
        }

        [[nodiscard]] unique_ptr<PlaintextBallotSelection::Impl> clone() const
        {
            return make_unique<PlaintextBallotSelection::Impl>(
              this->object_id, this->vote, this->isPlaceholderSelection,
              this->writeIn);
        }
    };

    // Lifecycle Methods

    PlaintextBallotSelection::PlaintextBallotSelection(const PlaintextBallotSelection &other)
        : pimpl(other.pimpl->clone())
    {
    }

    PlaintextBallotSelection::PlaintextBallotSelection(PlaintextBallotSelection &&other)
        : pimpl(move(other.pimpl))
    {
    }

    PlaintextBallotSelection::PlaintextBallotSelection(
      string objectId, uint64_t vote, bool isPlaceholderSelection /* = false */,
      string writeIn)
        : pimpl(new Impl(move(objectId), vote, isPlaceholderSelection, writeIn))
    {
    }

    PlaintextBallotSelection::~PlaintextBallotSelection() = default;

    PlaintextBallotSelection &PlaintextBallotSelection::operator=(PlaintextBallotSelection other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string PlaintextBallotSelection::getObjectId() const { return pimpl->object_id; }
    uint64_t PlaintextBallotSelection::getVote() const { return pimpl->vote; }
    bool PlaintextBallotSelection::getIsPlaceholder() const
    {
        return pimpl->isPlaceholderSelection;
    }
    string PlaintextBallotSelection::getWriteIn() const { return pimpl->writeIn; }

    bool PlaintextBallotSelection::isValid(const std::string &expectedObjectId) const
    {
        if (pimpl->object_id != expectedObjectId) {
            Log::info("invalid object_id: expected: " + expectedObjectId +
                      " actual: " + pimpl->object_id);
            return false;
        }
        if (pimpl->vote > 1) {
            Log::warn("Currently only supporting choices of 0 or 1");
            return false;
        }
        return true;
    }

    std::unique_ptr<PlaintextBallotSelection> PlaintextBallotSelection::clone() const
    {
        return make_unique<PlaintextBallotSelection>(
          pimpl->object_id, pimpl->vote, pimpl->isPlaceholderSelection,
          pimpl->writeIn);
    }

#pragma endregion

#pragma region CiphertextBallotSelection

    struct CiphertextBallotSelection::Impl {
        string objectId;
        uint64_t sequenceOrder;
        unique_ptr<ElementModQ> descriptionHash;
        unique_ptr<ElGamalCiphertext> ciphertext;
        bool isPlaceholder;
        unique_ptr<ElementModQ> nonce;
        unique_ptr<ElementModQ> cryptoHash;
        unique_ptr<DisjunctiveChaumPedersenProof> proof;
        unique_ptr<ElGamalCiphertext> extendedData;

        Impl(string objectId, uint64_t sequenceOrder, unique_ptr<ElementModQ> descriptionHash,
             unique_ptr<ElGamalCiphertext> ciphertext, bool isPlaceholder,
             unique_ptr<ElementModQ> nonce, unique_ptr<ElementModQ> cryptoHash,
             unique_ptr<DisjunctiveChaumPedersenProof> proof,
             unique_ptr<ElGamalCiphertext> extendedData)
            : objectId(move(objectId)), sequenceOrder(sequenceOrder),
              descriptionHash(move(descriptionHash)), ciphertext(move(ciphertext)),
              nonce(move(nonce)), cryptoHash(move(cryptoHash)), proof(move(proof)),
              extendedData(move(extendedData))
        {
            this->isPlaceholder = isPlaceholder;
        }

        [[nodiscard]] unique_ptr<CiphertextBallotSelection::Impl> clone() const
        {
            auto _descriptionHash = make_unique<ElementModQ>(*descriptionHash);
            auto _ciphertext = make_unique<ElGamalCiphertext>(*ciphertext);
            auto _nonce = make_unique<ElementModQ>(*nonce);
            auto _cryptoHash = make_unique<ElementModQ>(*cryptoHash);
            auto _proof = make_unique<DisjunctiveChaumPedersenProof>(*proof);
            auto _extendedData =
              extendedData != nullptr ? make_unique<ElGamalCiphertext>(*extendedData) : nullptr;

            return make_unique<CiphertextBallotSelection::Impl>(
              objectId, sequenceOrder, move(_descriptionHash), move(_ciphertext), isPlaceholder,
              move(_nonce), move(_cryptoHash), move(_proof), move(_extendedData));
        }

        [[nodiscard]] unique_ptr<ElementModQ>
        crypto_hash_with(const ElementModQ &encryptionSeed) const
        {
            return makeCryptoHash(objectId, encryptionSeed, *ciphertext);
        }
    };

    // Lifecycle Methods

    CiphertextBallotSelection::CiphertextBallotSelection(const CiphertextBallotSelection &other)
        : pimpl(other.pimpl->clone())
    {
    }

    CiphertextBallotSelection::CiphertextBallotSelection(
      const string &objectId, uint64_t sequenceOrder, const ElementModQ &descriptionHash,
      unique_ptr<ElGamalCiphertext> ciphertext, bool isPlaceholder, unique_ptr<ElementModQ> nonce,
      unique_ptr<ElementModQ> cryptoHash, unique_ptr<DisjunctiveChaumPedersenProof> proof,
      unique_ptr<ElGamalCiphertext> extendedData)
        : pimpl(new Impl(objectId, sequenceOrder, make_unique<ElementModQ>(descriptionHash),
                         move(ciphertext), isPlaceholder, move(nonce), move(cryptoHash),
                         move(proof), move(extendedData)))

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

    uint64_t CiphertextBallotSelection::getSequenceOrder() const { return pimpl->sequenceOrder; }

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
    CiphertextBallotSelection::crypto_hash_with(const ElementModQ &encryptionSeed) const
    {
        return pimpl->crypto_hash_with(encryptionSeed);
    }

    // public Static Members

    unique_ptr<CiphertextBallotSelection> CiphertextBallotSelection::make(
      const string &objectId, uint64_t sequenceOrder, const ElementModQ &descriptionHash,
      unique_ptr<ElGamalCiphertext> ciphertext, const ElementModP &elgamalPublicKey,
      const ElementModQ &cryptoExtendedBaseHash, uint64_t plaintext,
      bool isPlaceholder /* = false */, bool computeProof /* = true */,
      unique_ptr<ElementModQ> nonce /* = nullptr */,
      unique_ptr<ElementModQ> cryptoHash /* = nullptr */,
      unique_ptr<ElGamalCiphertext> extendedData /* = nullptr */)
    {
        if (cryptoHash == nullptr) {
            auto crypto_hash = makeCryptoHash(objectId, descriptionHash, *ciphertext);
            cryptoHash = move(crypto_hash);
        }

        unique_ptr<DisjunctiveChaumPedersenProof> proof = nullptr;
        if (computeProof) {
            // always make a proof using the faster, non-deterministic method
            proof = DisjunctiveChaumPedersenProof::make(*ciphertext, *nonce, elgamalPublicKey,
                                                        cryptoExtendedBaseHash, plaintext);
        }

        return make_unique<CiphertextBallotSelection>(
          objectId, sequenceOrder, descriptionHash, move(ciphertext), isPlaceholder, move(nonce),
          move(cryptoHash), move(proof), move(extendedData));
    }

    unique_ptr<CiphertextBallotSelection> CiphertextBallotSelection::make(
      const string &objectId, uint64_t sequenceOrder, const ElementModQ &descriptionHash,
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
        // if no proof is provided and there is a nonce
        // then make a proof using the deterministic method
        if (proof == nullptr && nonce != nullptr) {
            proof = DisjunctiveChaumPedersenProof::make(
              *ciphertext, *nonce, elgamalPublicKey, cryptoExtendedBaseHash, proofSeed, plaintext);
        }
        return make_unique<CiphertextBallotSelection>(
          objectId, sequenceOrder, descriptionHash, move(ciphertext), isPlaceholder, move(nonce),
          move(cryptoHash), move(proof), move(extendedData));
    }

    unique_ptr<CiphertextBallotSelection> CiphertextBallotSelection::make_with_precomputed(
      const std::string &objectId, uint64_t sequenceOrder, const ElementModQ &descriptionHash,
      unique_ptr<ElGamalCiphertext> ciphertext,
      const ElementModQ &cryptoExtendedBaseHash, uint64_t plaintext,
      unique_ptr<TwoTriplesAndAQuadruple> precomputedTwoTriplesAndAQuad,
      bool isPlaceholder /* = false */, bool computeProof /* = true */,
      unique_ptr<ElementModQ> cryptoHash /* = nullptr */,
      unique_ptr<ElGamalCiphertext> extendedData /* = nullptr */)
    {
        unique_ptr<CiphertextBallotSelection> result = NULL;

        if (cryptoHash == nullptr) {
            auto crypto_hash = makeCryptoHash(objectId, descriptionHash, *ciphertext);
            cryptoHash = move(crypto_hash);
        }

        // need to make sure we use the nonce used in precomputed values
        auto nonce = precomputedTwoTriplesAndAQuad->get_triple1()->get_exp();

        unique_ptr<DisjunctiveChaumPedersenProof> proof = nullptr;
        if (computeProof) {
            // always make a proof using the faster, non-deterministic method
            proof = DisjunctiveChaumPedersenProof::make_with_precomputed(*ciphertext,
                                                    move(precomputedTwoTriplesAndAQuad),
                                                    cryptoExtendedBaseHash, plaintext);
        }

        return make_unique<CiphertextBallotSelection>(
          objectId, sequenceOrder, descriptionHash, move(ciphertext), isPlaceholder,
          move(nonce),
          move(cryptoHash), move(proof), move(extendedData));

        return result;
    }

    unique_ptr<ElementModQ>
    CiphertextBallotSelection::crypto_hash_with(const ElementModQ &encryptionSeed)
    {
        return pimpl->crypto_hash_with(encryptionSeed);
    }

    // Public Methods

    bool CiphertextBallotSelection::isValidEncryption(const ElementModQ &encryptionSeed,
                                                      const ElementModP &elgamalPublicKey,
                                                      const ElementModQ &cryptoExtendedBaseHash)
    {
        if ((const_cast<ElementModQ &>(encryptionSeed) != *pimpl->descriptionHash)) {
            Log::info(": CiphertextBallotSelection mismatching selection hash: ");
            Log::info(": expected: ", encryptionSeed.toHex());
            Log::info(": actual: ", pimpl->descriptionHash->toHex());
            return false;
        }

        auto recalculatedCryptoHash = crypto_hash_with(encryptionSeed);
        if ((*pimpl->cryptoHash != *recalculatedCryptoHash)) {
            Log::info(": CiphertextBallotSelection mismatching crypto hash: ");
            Log::info(": expected: ", recalculatedCryptoHash->toHex());
            Log::info(": actual: ", pimpl->cryptoHash->toHex());
            return false;
        }

        if (pimpl->proof == nullptr) {
            Log::info(": No proof exists for: " + pimpl->objectId);
            return false;
        }
        return pimpl->proof->isValid(*pimpl->ciphertext, elgamalPublicKey, cryptoExtendedBaseHash);
    }

    // Protected Members

    unique_ptr<ElementModQ>
    CiphertextBallotSelection::makeCryptoHash(const string &objectId,
                                              const ElementModQ &encryptionSeed,
                                              const ElGamalCiphertext &ciphertext)
    {
        return hash_elems(
          {objectId, &const_cast<ElementModQ &>(encryptionSeed), ciphertext.crypto_hash().get()});
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

        [[nodiscard]] unique_ptr<PlaintextBallotContest::Impl> clone() const
        {
            vector<unique_ptr<PlaintextBallotSelection>> _selections;
            _selections.reserve(selections.size());
            for (const auto &element : selections) {
                _selections.push_back(make_unique<PlaintextBallotSelection>(*element));
            }
            return make_unique<PlaintextBallotContest::Impl>(this->object_id, move(_selections));
        }
    };

    // Lifecycle Methods

    PlaintextBallotContest::PlaintextBallotContest(const PlaintextBallotContest &other)
        : pimpl(other.pimpl->clone())
    {
    }

    PlaintextBallotContest::PlaintextBallotContest(PlaintextBallotContest &&other)
        : pimpl(move(other.pimpl))
    {
    }

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
        // TODO: template
        vector<reference_wrapper<PlaintextBallotSelection>> references;
        references.reserve(pimpl->selections.size());
        for (auto &selection : pimpl->selections) {
            references.push_back(ref(*selection));
        }

        // TODO: when sequence order added
        // sort(references.begin(), references.end(),
        //      [](const reference_wrapper<PlaintextBallotSelection> left,
        //         const reference_wrapper<PlaintextBallotSelection> right) {
        //          return left.get().getSequenceOrder() < right.get().getSequenceOrder();
        //      });
        return references;
    }

    // Public Functions

    valid_contest_return PlaintextBallotContest::isValid(const string &expectedObjectId,
                                         uint64_t expectedNumberSelections,
                                         uint64_t expectedNumberElected,
                                         uint64_t votesAllowd, /* = 0 */
                                         bool supportOvervotes /* = true */) const
    {
        if (pimpl->object_id != expectedObjectId) {
            Log::info(": invalid objectId");
            return INVALID_OBJECT_ID_ERROR;
        }

        if (pimpl->selections.size() > expectedNumberSelections) {
            Log::info(": too many selections");
            return TOO_MANY_SELECTIONS_ERROR;
        }

        uint64_t numberElected = 0;
        uint64_t votes = 0;

        for (const auto &selection : pimpl->selections) {
            votes += selection->getVote();
            if (selection->getVote() >= 1) {
                numberElected += 1;
            }
        }

        if (numberElected > expectedNumberElected) {
            Log::info(": too many elections");
            if (supportOvervotes) {
                return OVERVOTE;
            } else {
                return OVERVOTE_ERROR;
            }
        }

        if (votesAllowd == 0) {
            votesAllowd = expectedNumberElected;
        }

        if (votes > votesAllowd) {
            Log::info(": too many votes");
            if (supportOvervotes) {
                return OVERVOTE;
            } else {
                return OVERVOTE_ERROR;
            }
        }

        return SUCCESS;
    }

#pragma endregion

#pragma region CiphertextBallotContest

    struct CiphertextBallotContest::Impl : ElectionObjectBase {
        uint64_t sequenceOrder;
        unique_ptr<ElementModQ> descriptionHash;
        vector<unique_ptr<CiphertextBallotSelection>> selections;
        unique_ptr<ElementModQ> nonce;
        unique_ptr<ElGamalCiphertext> ciphertextAccumulation;
        unique_ptr<ElementModQ> cryptoHash;
        unique_ptr<ConstantChaumPedersenProof> proof;
        unique_ptr<HashedElGamalCiphertext> hashedElGamal;

        Impl(const string &objectId, uint64_t sequenceOrder,
             unique_ptr<ElementModQ> descriptionHash,
             vector<unique_ptr<CiphertextBallotSelection>> selections,
             unique_ptr<ElementModQ> nonce, unique_ptr<ElGamalCiphertext> ciphertextAccumulation,
             unique_ptr<ElementModQ> cryptoHash, unique_ptr<ConstantChaumPedersenProof> proof,
             unique_ptr<HashedElGamalCiphertext> hashedElGamal)
            : sequenceOrder(sequenceOrder), descriptionHash(move(descriptionHash)),
              selections(move(selections)), nonce(move(nonce)),
              ciphertextAccumulation(move(ciphertextAccumulation)), cryptoHash(move(cryptoHash)),
              proof(move(proof)), hashedElGamal(move(hashedElGamal))
        {
            this->object_id = objectId;
        }

        [[nodiscard]] unique_ptr<CiphertextBallotContest::Impl> clone() const
        {
            vector<unique_ptr<CiphertextBallotSelection>> _selections;
            _selections.reserve(selections.size());
            for (const auto &element : selections) {
                _selections.push_back(make_unique<CiphertextBallotSelection>(*element));
            }

            auto _descriptionHash = make_unique<ElementModQ>(*descriptionHash);
            auto _nonce = make_unique<ElementModQ>(*nonce);
            auto _accumulation = make_unique<ElGamalCiphertext>(*ciphertextAccumulation);
            auto _cryptoHash = make_unique<ElementModQ>(*cryptoHash);
            auto _proof = make_unique<ConstantChaumPedersenProof>(*proof);
            auto _hashedElGamal = make_unique<HashedElGamalCiphertext>(*hashedElGamal);

            return make_unique<CiphertextBallotContest::Impl>(
              object_id, sequenceOrder, move(_descriptionHash), move(_selections), move(_nonce),
              move(_accumulation), move(_cryptoHash), move(_proof), move(_hashedElGamal));
        }
    };

    // Lifecycle Methods

    CiphertextBallotContest::CiphertextBallotContest(const CiphertextBallotContest &other)
        : pimpl(other.pimpl->clone())
    {
    }

    CiphertextBallotContest::CiphertextBallotContest(
      const string &objectId, uint64_t sequenceOrder, const ElementModQ &descriptionHash,
      vector<unique_ptr<CiphertextBallotSelection>> selections, unique_ptr<ElementModQ> nonce,
      unique_ptr<ElGamalCiphertext> ciphertextAccumulation, unique_ptr<ElementModQ> cryptoHash,
      unique_ptr<ConstantChaumPedersenProof> proof,
      unique_ptr<HashedElGamalCiphertext> hashedElGamal)
        : pimpl(new Impl(objectId, sequenceOrder, make_unique<ElementModQ>(descriptionHash),
                         move(selections), move(nonce), move(ciphertextAccumulation),
                         move(cryptoHash), move(proof), move(hashedElGamal)))
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

    uint64_t CiphertextBallotContest::getSequenceOrder() const { return pimpl->sequenceOrder; }

    ElementModQ *CiphertextBallotContest::getDescriptionHash() const
    {
        return pimpl->descriptionHash.get();
    }

    vector<reference_wrapper<CiphertextBallotSelection>>
    CiphertextBallotContest::getSelections() const
    {
        vector<reference_wrapper<CiphertextBallotSelection>> references;
        references.reserve(pimpl->selections.size());
        for (const auto &selection : pimpl->selections) {
            references.push_back(ref(*selection));
        }

        sort(references.begin(), references.end(),
             [](const reference_wrapper<CiphertextBallotSelection> left,
                const reference_wrapper<CiphertextBallotSelection> right) {
                 return left.get().getSequenceOrder() < right.get().getSequenceOrder();
             });
        return references;
    }

    ElementModQ *CiphertextBallotContest::getNonce() const { return pimpl->nonce.get(); }

    ElGamalCiphertext *CiphertextBallotContest::getCiphertextAccumulation() const
    {
        return pimpl->ciphertextAccumulation.get();
    }

    ElementModQ *CiphertextBallotContest::getCryptoHash() const { return pimpl->cryptoHash.get(); }

    ConstantChaumPedersenProof *CiphertextBallotContest::getProof() const
    {
        return pimpl->proof.get();
    }

    unique_ptr<HashedElGamalCiphertext> CiphertextBallotContest::getHashedElGamalCiphertext() const
    {
        return pimpl->hashedElGamal.get()->clone();
    }

    // Interface Overrides

    unique_ptr<ElementModQ>
    CiphertextBallotContest::crypto_hash_with(const ElementModQ &encryptionSeed) const
    {
        return makeCryptoHash(pimpl->object_id, this->getSelections(), encryptionSeed);
    }

    // Public Static Methods

    unique_ptr<CiphertextBallotContest> CiphertextBallotContest::make(
      const string &objectId, uint64_t sequenceOrder, const ElementModQ &descriptionHash,
      vector<unique_ptr<CiphertextBallotSelection>> selections, const ElementModP &elgamalPublicKey,
      const ElementModQ &cryptoExtendedBaseHash, const ElementModQ &proofSeed,
      uint64_t numberElected, unique_ptr<ElementModQ> nonce /* = nullptr */,
      unique_ptr<ElementModQ> cryptoHash /* = nullptr */,
      unique_ptr<ConstantChaumPedersenProof> proof /* = nullptr */,
      unique_ptr<HashedElGamalCiphertext> hashedElGamal /*nullptr */)
    {
        vector<reference_wrapper<CiphertextBallotSelection>> selectionReferences;
        selectionReferences.reserve(selections.size());
        for (const auto &selection : selections) {
            selectionReferences.push_back(ref(*selection));
        }

        sort(selectionReferences.begin(), selectionReferences.end(),
             [](const reference_wrapper<CiphertextBallotSelection> left,
                const reference_wrapper<CiphertextBallotSelection> right) {
                 return left.get().getSequenceOrder() < right.get().getSequenceOrder();
             });

        if (cryptoHash == nullptr) {
            auto crypto_hash = makeCryptoHash(objectId, selectionReferences, descriptionHash);
            cryptoHash = move(crypto_hash);
        }

        auto accumulation = elgamalAccumulate(selectionReferences);
        if (proof == nullptr) {
            auto aggregate = aggregateNonce(selectionReferences);
            auto owned_proof =
              ConstantChaumPedersenProof::make(*accumulation, *aggregate, elgamalPublicKey,
                                               proofSeed, cryptoExtendedBaseHash, numberElected);
            proof = move(owned_proof);
        }

        return make_unique<CiphertextBallotContest>(
          objectId, sequenceOrder, descriptionHash, move(selections), move(nonce),
          move(accumulation), move(cryptoHash), move(proof), move(hashedElGamal));
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

    bool CiphertextBallotContest::isValidEncryption(const ElementModQ &encryptionSeed,
                                                    const ElementModP &elgamalPublicKey,
                                                    const ElementModQ &cryptoExtendedBaseHash)
    {
        bool consistent_encryption_seed = true;
        if ((const_cast<ElementModQ &>(encryptionSeed) != *pimpl->descriptionHash)) {
            Log::debug("CiphertextBallotContest mismatching constest hash");
            Log::debug("expected", encryptionSeed.toHex());
            Log::debug("actual", pimpl->descriptionHash->toHex());
            consistent_encryption_seed = false;
        }

        bool consistent_crypto_hash = true;
        auto recalculatedCryptoHash = crypto_hash_with(encryptionSeed);
        if ((*pimpl->cryptoHash != *recalculatedCryptoHash)) {
            Log::debug("CiphertextBallotContest mismatching crypto hash");
            Log::debug("expected", recalculatedCryptoHash->toHex());
            Log::debug("actual", pimpl->cryptoHash->toHex());
            consistent_crypto_hash = false;
        }

        // NOTE: this check does not verify the proofs of the individual selections by design.

        bool proof_exists = true;
        if (!pimpl->proof) {
            Log::debug("no proof exists for" + pimpl->object_id);
            proof_exists = false;
        }

        bool consistent_accumulation = true;
        auto computedAccumulation = this->elgamalAccumulate();

        // Verify that the contest ciphertext matches the elgamal accumulation of all selections
        if (*pimpl->ciphertextAccumulation != *computedAccumulation) {
            Log::debug("ciphertext does not equal elgamal accumulation for" + pimpl->object_id);
            Log::debug("expected (pad)", computedAccumulation->getPad()->toHex());
            Log::debug("expected (data)", computedAccumulation->getData()->toHex());
            Log::debug("actual (pad)", pimpl->ciphertextAccumulation->getPad()->toHex());
            Log::debug("actual (data)", pimpl->ciphertextAccumulation->getData()->toHex());
            consistent_accumulation = false;
        }

        bool valid_proof =
          pimpl->proof->isValid(*computedAccumulation, elgamalPublicKey, cryptoExtendedBaseHash);

        bool success = consistent_encryption_seed && consistent_crypto_hash && proof_exists &&
                       consistent_accumulation && valid_proof;

        if (!success) {
            map<string, bool> printMap{{"consistent_encryption_seed", consistent_encryption_seed},
                                       {"consistent_crypto_hash", consistent_crypto_hash},
                                       {"proof_exists", proof_exists},
                                       {"consistent_accumulation", consistent_accumulation},
                                       {"valid_proof", valid_proof}};

            Log::info("CiphertextBallotContest::isValidEncryption failed!", printMap);
        }
        return success;
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
      const ElementModQ &encryptionSeed)
    {
        if (selections.empty()) {
            throw invalid_argument("mismatching selection state for " + objectId +
                                   "expected(some) actual(none)");
        }
        vector<CryptoHashableType> elems = {objectId, &const_cast<ElementModQ &>(encryptionSeed)};
        for (const auto &selection : selections) {
            elems.emplace_back(ref(*selection.get().getCryptoHash()));
        }
        return hash_elems(elems);
    }

#pragma endregion

#pragma region PlaintextBallot

    struct PlaintextBallot::Impl : public ElectionObjectBase {
        string styleId;
        vector<unique_ptr<PlaintextBallotContest>> contests;
        Impl(const string &objectId, const string &styleId,
             vector<unique_ptr<PlaintextBallotContest>> contests)
            : contests(move(contests))
        {
            this->object_id = objectId;
            this->styleId = styleId;
        }

        [[nodiscard]] unique_ptr<PlaintextBallot::Impl> clone() const
        {
            vector<unique_ptr<PlaintextBallotContest>> _contests;
            _contests.reserve(contests.size());
            for (const auto &element : contests) {
                _contests.push_back(make_unique<PlaintextBallotContest>(*element));
            }
            return make_unique<PlaintextBallot::Impl>(this->object_id, this->styleId,
                                                      move(_contests));
        }
    };

    // Lifecycle Methods

    PlaintextBallot::PlaintextBallot(const PlaintextBallot &other) : pimpl(other.pimpl->clone()) {}

    PlaintextBallot::PlaintextBallot(const string &objectId, const string &styleId,
                                     vector<unique_ptr<PlaintextBallotContest>> contests)
        : pimpl(new Impl(objectId, styleId, move(contests)))
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
    string PlaintextBallot::getStyleId() const { return pimpl->styleId; }

    vector<reference_wrapper<PlaintextBallotContest>> PlaintextBallot::getContests() const
    {
        vector<reference_wrapper<PlaintextBallotContest>> references;
        references.reserve(pimpl->contests.size());
        for (const auto &contest : pimpl->contests) {
            references.push_back(ref(*contest));
        }

        // TODO: add sequence order to object
        // sort(references.begin(), references.end(),
        //      [](const reference_wrapper<PlaintextBallotContest> left,
        //         const reference_wrapper<PlaintextBallotContest> right) {
        //          return left.get().getSequenceOrder() < right.get().getSequenceOrder();
        //      });
        return references;
    }

    // Public Methods

    vector<uint8_t> PlaintextBallot::toBson() const
    {
        return PlaintextBallotSerializer::toBson(*this);
    }

    string PlaintextBallot::toJson() const { return PlaintextBallotSerializer::toJson(*this); }

    vector<uint8_t> PlaintextBallot::toMsgPack() const
    {
        return PlaintextBallotSerializer::toMsgPack(*this);
    }

    unique_ptr<PlaintextBallot> PlaintextBallot::fromJson(string data)
    {
        return PlaintextBallotSerializer::fromJson(move(data));
    }

    unique_ptr<PlaintextBallot> PlaintextBallot::fromBson(vector<uint8_t> data)
    {
        return PlaintextBallotSerializer::fromBson(move(data));
    }

    unique_ptr<PlaintextBallot> PlaintextBallot::fromMsgPack(vector<uint8_t> data)
    {
        return PlaintextBallotSerializer::fromMsgPack(move(data));
    }

#pragma endregion

#pragma region CiphertextBallot

    struct CiphertextBallot::Impl : public ElectionObjectBase {
        string styleId;
        unique_ptr<ElementModQ> manifestHash;
        unique_ptr<ElementModQ> ballotCodeSeed;
        vector<unique_ptr<CiphertextBallotContest>> contests;
        unique_ptr<ElementModQ> ballotCode;
        uint64_t timestamp;
        unique_ptr<ElementModQ> nonce;
        unique_ptr<ElementModQ> cryptoHash;

        Impl(const string &objectId, const string &styleId, unique_ptr<ElementModQ> manifestHash,
             unique_ptr<ElementModQ> ballotCodeSeed,
             vector<unique_ptr<CiphertextBallotContest>> contests,
             unique_ptr<ElementModQ> ballotCode, const uint64_t timestamp,
             unique_ptr<ElementModQ> nonce, unique_ptr<ElementModQ> cryptoHash)
            : manifestHash(move(manifestHash)), ballotCodeSeed(move(ballotCodeSeed)),
              contests(move(contests)), ballotCode(move(ballotCode)), nonce(move(nonce)),
              cryptoHash(move(cryptoHash))
        {
            this->object_id = objectId;
            this->styleId = styleId;
            this->timestamp = timestamp;
        }

        [[nodiscard]] unique_ptr<CiphertextBallot::Impl> clone() const
        {
            vector<unique_ptr<CiphertextBallotContest>> _contests;
            _contests.reserve(contests.size());
            for (const auto &element : contests) {
                _contests.push_back(make_unique<CiphertextBallotContest>(*element));
            }

            auto _manifestHash = make_unique<ElementModQ>(*manifestHash);
            auto _ballotCodeSeed = make_unique<ElementModQ>(*ballotCodeSeed);
            auto _ballotCode = make_unique<ElementModQ>(*ballotCode);
            auto _nonce = make_unique<ElementModQ>(*nonce);
            auto _cryptoHash = make_unique<ElementModQ>(*cryptoHash);
            return make_unique<CiphertextBallot::Impl>(
              object_id, styleId, move(_manifestHash), move(_ballotCodeSeed), move(_contests),
              move(_ballotCode), timestamp, move(_nonce), move(_cryptoHash));
        }
    };

    // Lifecycle Methods

    CiphertextBallot::CiphertextBallot(const CiphertextBallot &other) : pimpl(other.pimpl->clone())
    {
    }

    CiphertextBallot::CiphertextBallot(const string &objectId, const string &styleId,
                                       const ElementModQ &manifestHash,
                                       unique_ptr<ElementModQ> ballotCodeSeed,
                                       vector<unique_ptr<CiphertextBallotContest>> contests,
                                       unique_ptr<ElementModQ> ballotCode, const uint64_t timestamp,
                                       unique_ptr<ElementModQ> nonce,
                                       unique_ptr<ElementModQ> cryptoHash)
        : pimpl(new Impl(objectId, styleId, make_unique<ElementModQ>(manifestHash),
                         move(ballotCodeSeed), move(contests), move(ballotCode), timestamp,
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
    string CiphertextBallot::getStyleId() const { return pimpl->styleId; }
    ElementModQ *CiphertextBallot::getManifestHash() const { return pimpl->manifestHash.get(); }
    ElementModQ *CiphertextBallot::getBallotCodeSeed() const { return pimpl->ballotCodeSeed.get(); }

    vector<reference_wrapper<CiphertextBallotContest>> CiphertextBallot::getContests() const
    {
        // TODO: templatize this pattern
        vector<reference_wrapper<CiphertextBallotContest>> references;
        references.reserve(pimpl->contests.size());
        for (const auto &contest : pimpl->contests) {
            references.push_back(ref(*contest));
        }

        sort(references.begin(), references.end(),
             [](const reference_wrapper<CiphertextBallotContest> left,
                const reference_wrapper<CiphertextBallotContest> right) {
                 return left.get().getSequenceOrder() < right.get().getSequenceOrder();
             });
        return references;
    }

    ElementModQ *CiphertextBallot::getBallotCode() const { return pimpl->ballotCode.get(); }

    uint64_t CiphertextBallot::getTimestamp() const { return pimpl->timestamp; }

    ElementModQ *CiphertextBallot::getNonce() const { return pimpl->nonce.get(); }

    ElementModQ *CiphertextBallot::getCryptoHash() const { return pimpl->cryptoHash.get(); }

    // Interface Overrides

    unique_ptr<ElementModQ>
    CiphertextBallot::crypto_hash_with(const ElementModQ &manifestHash) const
    {
        return makeCryptoHash(pimpl->object_id, this->getContests(), manifestHash);
    }

    // Public Static Methods

    unique_ptr<CiphertextBallot> CiphertextBallot::make(
      const string &objectId, const string &styleId, const ElementModQ &manifestHash,
      vector<unique_ptr<CiphertextBallotContest>> contests,
      unique_ptr<ElementModQ> nonce /* = nullptr */, const uint64_t timestamp /* = 0 */,
      unique_ptr<ElementModQ> ballotCodeSeed /* = nullptr */,
      unique_ptr<ElementModQ> ballotCode /* = nullptr */)
    {
        if (contests.empty()) {
            Log::error(":ballot must have at least some contests");
            throw invalid_argument("ballot must have at least some contests");
        }

        vector<reference_wrapper<CiphertextBallotContest>> contestsRefs;
        contestsRefs.reserve(contests.size());
        for (const auto &contest : contests) {
            contestsRefs.push_back(ref(*contest));
        }

        auto cryptoHash = makeCryptoHash(objectId, contestsRefs, manifestHash);
        auto ballotTimestamp = timestamp == 0 ? getSystemTimestamp() : timestamp;
        if (!ballotCodeSeed) {
            // if there is no ballotCodeSeed, default to using the manifest
            auto previous = make_unique<ElementModQ>(manifestHash);
            ballotCodeSeed = move(previous);
        }

        if (!ballotCode) {
            auto _ballotCode =
              BallotCode::getBallotCode(*ballotCodeSeed, ballotTimestamp, *cryptoHash);
            ballotCode.swap(_ballotCode);
        }

        return make_unique<CiphertextBallot>(objectId, styleId, manifestHash, move(ballotCodeSeed),
                                             move(contests), move(ballotCode), ballotTimestamp,
                                             move(nonce), move(cryptoHash));
    }

    unique_ptr<ElementModQ> CiphertextBallot::nonceSeed(const ElementModQ &manifestHash,
                                                        const string &objectId,
                                                        const ElementModQ &nonce)
    {
        return hash_elems({ref(manifestHash), objectId, ref(nonce)});
    }

    // Public Methods

    bool CiphertextBallot::isValidEncryption(const ElementModQ &manifestHash,
                                             const ElementModP &elgamalPublicKey,
                                             const ElementModQ &cryptoExtendedBaseHash)
    {
        if ((const_cast<ElementModQ &>(manifestHash) != *pimpl->manifestHash)) {
            Log::info(": CiphertextBallot mismatching manifestHash: ");
            Log::info(": expected: ", manifestHash.toHex());
            Log::info(": actual: ", pimpl->manifestHash->toHex());
            return false;
        }

        auto recalculatedCryptoHash = crypto_hash_with(manifestHash);
        if ((*pimpl->cryptoHash != *recalculatedCryptoHash)) {
            Log::info(": CiphertextBallot mismatching crypto hash: ");
            Log::info(": expected: ", recalculatedCryptoHash->toHex());
            Log::info(": actual: ", pimpl->cryptoHash->toHex());
            return false;
        }

        Log::debug("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");

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
                Log::info(": CiphertextBallot found invalid proof for: " + key);
                isValid = false;
            }
        }

        return isValid;
    }

    unique_ptr<ElementModQ> CiphertextBallot::nonceSeed()
    {
        return nonceSeed(*pimpl->manifestHash, pimpl->object_id, *pimpl->nonce);
    }

    vector<uint8_t> CiphertextBallot::toBson(bool withNonces /* = false */) const
    {
        return CiphertextBallotSerializer::toBson(*this, withNonces);
    }

    string CiphertextBallot::toJson(bool withNonces /* = false */) const
    {
        return CiphertextBallotSerializer::toJson(*this, withNonces);
    }

    vector<uint8_t> CiphertextBallot::toMsgPack(bool withNonces /* = false */) const
    {
        return CiphertextBallotSerializer::toMsgPack(*this, withNonces);
    }

    unique_ptr<CiphertextBallot> CiphertextBallot::fromJson(string data)
    {
        return CiphertextBallotSerializer::fromJson(move(data));
    }

    unique_ptr<CiphertextBallot> CiphertextBallot::fromBson(vector<uint8_t> data)
    {
        return CiphertextBallotSerializer::fromBson(move(data));
    }

    unique_ptr<CiphertextBallot> CiphertextBallot::fromMsgPack(vector<uint8_t> data)
    {
        return CiphertextBallotSerializer::fromMsgPack(move(data));
    }

    // Protected Methods

    unique_ptr<ElementModQ> CiphertextBallot::makeCryptoHash(
      string objectId, const vector<reference_wrapper<CiphertextBallotContest>> &contests,
      const ElementModQ &manifestHash)
    {
        if (contests.empty()) {
            throw invalid_argument("mismatching contests state for " + objectId +
                                   "expected(some) actual(none)");
        }

        vector<CryptoHashableType> elems = {objectId, &const_cast<ElementModQ &>(manifestHash)};
        for (const auto &contest : contests) {
            elems.emplace_back(ref(*contest.get().getCryptoHash()));
        }
        return hash_elems(elems);
    }

#pragma endregion

#pragma region SubmittedBallot

    struct SubmittedBallot::Impl {
        BallotBoxState state;

        Impl(BallotBoxState state) { this->state = state; }
    };

    // Lifecycle Methods

    SubmittedBallot::SubmittedBallot(const CiphertextBallot &other, BallotBoxState state)
        : CiphertextBallot(other), pimpl(new Impl(state))
    {
    }

    SubmittedBallot::SubmittedBallot(const string &objectId, const string &styleId,
                                     const ElementModQ &manifestHash,
                                     unique_ptr<ElementModQ> ballotCodeSeed,
                                     vector<unique_ptr<CiphertextBallotContest>> contests,
                                     unique_ptr<ElementModQ> ballotCode, const uint64_t timestamp,
                                     unique_ptr<ElementModQ> nonce,
                                     unique_ptr<ElementModQ> cryptoHash, BallotBoxState state)
        : CiphertextBallot(objectId, styleId, manifestHash, move(ballotCodeSeed), move(contests),
                           move(ballotCode), timestamp, move(nonce), move(cryptoHash)),
          pimpl(new Impl(state))
    {
    }
    SubmittedBallot::~SubmittedBallot() = default;

    SubmittedBallot &SubmittedBallot::operator=(SubmittedBallot other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    BallotBoxState SubmittedBallot::getState() const { return pimpl->state; }

    // Public Static Methods

    unique_ptr<SubmittedBallot>
    SubmittedBallot::from(const CiphertextBallot &ballot,
                          BallotBoxState state /* = BallotBoxState::unknown */)
    {
        return make_unique<SubmittedBallot>(ballot, state);
    }

    unique_ptr<SubmittedBallot> SubmittedBallot::make(
      const string &objectId, const string &styleId, const ElementModQ &manifestHash,
      vector<unique_ptr<CiphertextBallotContest>> contests,
      unique_ptr<ElementModQ> nonce /* = nullptr */, const uint64_t timestamp /* = 0 */,
      unique_ptr<ElementModQ> ballotCodeSeed /* = nullptr */,
      unique_ptr<ElementModQ> ballotCode /* = nullptr */,
      BallotBoxState state /* = BallotBoxState::unknown */)
    {
        if (contests.empty()) {
            Log::error(":ballot must have at least some contests");
            throw invalid_argument("ballot must have at least some contests");
        }

        auto ciphertextBallot =
          CiphertextBallot::make(objectId, styleId, manifestHash, move(contests), move(nonce),
                                 timestamp, move(ballotCodeSeed), move(ballotCode));
        return SubmittedBallot::from(*ciphertextBallot, state);
    }

    // Public Methods

    vector<uint8_t> SubmittedBallot::toBson() const
    {
        return SubmittedBallotSerializer::toBson(*this);
    }

    string SubmittedBallot::toJson() const { return SubmittedBallotSerializer::toJson(*this); }

    vector<uint8_t> SubmittedBallot::toMsgPack() const
    {
        return SubmittedBallotSerializer::toMsgPack(*this);
    }

    unique_ptr<SubmittedBallot> SubmittedBallot::fromJson(string data)
    {
        return SubmittedBallotSerializer::fromJson(data);
    }

    unique_ptr<SubmittedBallot> SubmittedBallot::fromBson(vector<uint8_t> data)
    {
        return SubmittedBallotSerializer::fromBson(move(data));
    }

    unique_ptr<SubmittedBallot> SubmittedBallot::fromMsgPack(vector<uint8_t> data)
    {
        return SubmittedBallotSerializer::fromMsgPack(move(data));
    }

#pragma endregion

} // namespace electionguard
