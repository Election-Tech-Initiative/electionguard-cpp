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

    // property Getters

    CiphertextBallotSelection::~CiphertextBallotSelection() = default;

    CiphertextBallotSelection &CiphertextBallotSelection::operator=(CiphertextBallotSelection other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

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
        auto *seed = &const_cast<ElementModQ &>(seedHash);
        auto ciphertextHash = ciphertext.crypto_hash();
        auto hash = hash_elems({objectId, seed, ciphertextHash.get()});
        return hash;
    }

#pragma endregion

#pragma region CiphertextBallotContest

    // Protected Members

    ElementModQ *CiphertextBallotContest::makeCryptoHash(
      string object_id, vector<CiphertextBallotSelection> selections, ElementModQ *seed_hash)
    {
        // TODO: implement ballot.py::_ciphertext_ballot_context_crypto_hash
        return nullptr;
    }

#pragma endregion

} // namespace electionguard
