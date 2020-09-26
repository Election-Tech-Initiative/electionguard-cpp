#include "electionguard/ballot.hpp"

#include "electionguard/election_object_base.hpp"
#include "electionguard/hash.hpp"
#include "log.hpp"

#include <cstdlib>
#include <cstring>

using std::string;
namespace electionguard
{
#pragma region PlaintextBallotSelection

    struct PlaintextBallotSelection::Impl : public ElectionObjectBase {
        string vote;
        bool isPlaceholderSelection;

        Impl(const string &objectId, const string &vote, bool isPlaceholderSelection /* = false */)
            : vote(vote), isPlaceholderSelection(isPlaceholderSelection)
        {
            this->object_id = objectId;
        }

        // TODO: secure erase the vote
    };

    // Public Members

    PlaintextBallotSelection::PlaintextBallotSelection(const string &objectId, const string &vote,
                                                       bool isPlaceholderSelection /* = false */)
        : pimpl(new Impl(objectId, vote, isPlaceholderSelection))
    {
    }

    PlaintextBallotSelection::~PlaintextBallotSelection() = default;

    PlaintextBallotSelection &PlaintextBallotSelection::operator=(PlaintextBallotSelection other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

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

        Impl(const string &objectId, unique_ptr<ElementModQ> descriptionHash,
             unique_ptr<ElGamalCiphertext> ciphertext, bool isPlaceholder,
             unique_ptr<ElementModQ> nonce, unique_ptr<ElementModQ> cryptoHash,
             unique_ptr<DisjunctiveChaumPedersenProof> proof,
             unique_ptr<ElGamalCiphertext> extendedData)
            : objectId(objectId), descriptionHash(move(descriptionHash)),
              ciphertext(move(ciphertext)), nonce(move(nonce)), cryptoHash(move(cryptoHash)),
              proof(move(proof)), extendedData(move(extendedData))
        {
            this->isPlaceholder = isPlaceholder;
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

    ElGamalCiphertext *CiphertextBallotSelection::getCiphertext() const
    {
        return pimpl->ciphertext.get();
    }

    DisjunctiveChaumPedersenProof *CiphertextBallotSelection::getProof() const
    {
        return pimpl->proof.get();
    }

    ElementModQ *CiphertextBallotSelection::getCryptoHash() const
    {
        return pimpl->cryptoHash.get();
    }

    ElementModQ *CiphertextBallotSelection::getNonce() { return pimpl->nonce.get(); }

    // public Members

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
        return makeCryptoHash(pimpl->objectId, seedHash, *pimpl->ciphertext);
    }

    bool CiphertextBallotSelection::isValidEncryption(const ElementModQ &seedHash,
                                                      const ElementModP &elgamalPublicKey,
                                                      const ElementModQ &cryptoExtendedBaseHash)
    {
        if ((const_cast<ElementModQ &>(seedHash) != *pimpl->descriptionHash)) {
            Log::debug(": mismatching selection hash: ");
            Log::debugHex(": expected: ", seedHash.toHex());
            Log::debugHex(": actual: ", pimpl->descriptionHash->toHex());
            return false;
        }

        auto recalculatedCryptoHash = crypto_hash_with(seedHash);
        if ((*pimpl->cryptoHash != *recalculatedCryptoHash)) {
            Log::debug(": mismatching crypto hash: ");
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

    string PlaintextBallotContest::getObjectId() const { return pimpl->object_id; }

    vector<reference_wrapper<PlaintextBallotSelection>>
    PlaintextBallotContest::getSelections() const
    {
        vector<reference_wrapper<PlaintextBallotSelection>> selections;
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
        for (const auto &selection : pimpl->selections) {
            selections.push_back(ref(*selection));
        }
        return selections;
    }

    ConstantChaumPedersenProof *CiphertextBallotContest::getProof() const
    {
        return pimpl->proof.get();
    }

    // Public Methods

    unique_ptr<CiphertextBallotContest> CiphertextBallotContest::make(
      const string &objectId, const ElementModQ &descriptionHash,
      vector<unique_ptr<CiphertextBallotSelection>> selections, const ElementModP &elgamalPublicKey,
      const ElementModQ &cryptoExtendedBaseHash, const ElementModQ &proofSeed,
      uint64_t numberElected, unique_ptr<ElementModQ> nonce /* = nullptr */,
      unique_ptr<ElementModQ> cryptoHash /* = nullptr */,
      unique_ptr<ConstantChaumPedersenProof> proof /* = nullptr */)
    {
        vector<reference_wrapper<CiphertextBallotSelection>> selectionReferences;
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
                                                    move(cryptoHash), move(nonce), move(proof));
    }

    unique_ptr<ElementModQ> CiphertextBallotContest::crypto_hash_with(const ElementModQ &seedHash)
    {
        return makeCryptoHash(pimpl->object_id, this->getSelections(), seedHash);
    }

    unique_ptr<ElementModQ> CiphertextBallotContest::aggregateNonce()
    {
        return aggregateNonce(this->getSelections());
    }

    unique_ptr<ElGamalCiphertext> CiphertextBallotContest::elgamalAccumulate()
    {
        return elgamalAccumulate(this->getSelections());
    }

    bool CiphertextBallotContest::isValidEncryption(const ElementModQ &seedHash,
                                                    const ElementModP &elgamalPublicKey,
                                                    const ElementModQ &cryptoExtendedBaseHash)
    {
        if ((const_cast<ElementModQ &>(seedHash) != *pimpl->descriptionHash)) {
            Log::debug(": mismatching constest hash: ");
            Log::debugHex(": expected: ", seedHash.toHex());
            Log::debugHex(": actual: ", pimpl->descriptionHash->toHex());
            return false;
        }

        auto recalculatedCryptoHash = crypto_hash_with(seedHash);
        if ((*pimpl->cryptoHash != *recalculatedCryptoHash)) {
            Log::debug(": mismatching crypto hash: ");
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
        for (const auto &selection : selections) {
            elements.push_back(ref(*selection.get().getNonce()));
        }
        return add_mod_q(elements);
    }

    unique_ptr<ElGamalCiphertext> CiphertextBallotContest::elgamalAccumulate(
      const vector<reference_wrapper<CiphertextBallotSelection>> &selections)
    {
        vector<reference_wrapper<ElGamalCiphertext>> ciphertexts;
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

        vector<reference_wrapper<ElementModQ>> selectionHashes;
        for (const auto &selection : selections) {
            selectionHashes.push_back(ref(*selection.get().getCryptoHash()));
        }
        return hash_elems({objectId, &const_cast<ElementModQ &>(seedHash), selectionHashes});
    }

#pragma endregion

} // namespace electionguard
