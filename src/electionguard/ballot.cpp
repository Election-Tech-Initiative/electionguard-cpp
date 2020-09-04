#include "electionguard/ballot.hpp"

#include "electionguard/hash.hpp"

#include <cstdlib>
#include <cstring>

using std::string;
namespace electionguard
{
#pragma region PlaintextBallotSelection

    // Public Members

    PlaintextBallotSelection::PlaintextBallotSelection(string objectId, const string vote) : data()
    {
        auto _osize = objectId.size() + 1;
        objectId.copy(data.object_id, _osize, 0UL);

        auto _vsize = vote.size() + 1;
        vote.copy(data.vote, _vsize, 0UL);
    }

    PlaintextBallotSelection::PlaintextBallotSelection(const char *object_id, const char *vote)
        : data()
    {
        size_t _osize = strlen(object_id) + 1;
        strncpy(data.object_id, object_id, _osize);

        size_t _vsize = strlen(vote) + 1;
        strncpy(data.vote, vote, _vsize);
    }

    PlaintextBallotSelection::~PlaintextBallotSelection() { data = {}; }

    int PlaintextBallotSelection::toInt() { return atoi(data.vote); }

    char *PlaintextBallotSelection::getObjectId() { return data.object_id; }

#pragma endregion

#pragma region CiphertextBallotSelection

    // Lifecycle Methods

    CiphertextBallotSelection::CiphertextBallotSelection(
      const string objectId, ElementModQ *descriptionHash, ElGamalCiphertext *ciphertext,
      bool isPlaceholder, ElementModQ *nonce, ElementModQ *cryptoHash,
      DisjunctiveChaumPedersenProof *proof, ElGamalCiphertext *extendedData)
        : data()
    {
        // copy the object id
        auto _osize = objectId.size() + 1;
        objectId.copy(data.object_id, _osize);

        data.descriptionHash = descriptionHash;
        data.ciphertext = ciphertext;
        data.cryptoHash = cryptoHash;
        data.isPlaceholderSelection = isPlaceholder;
        data.nonce = nonce;
        data.proof = proof;
        data.extendedData = extendedData;
    }

    CiphertextBallotSelection::CiphertextBallotSelection(
      const char *object_id, ElementModQ *descriptionHash, ElGamalCiphertext *ciphertext,
      bool isPlaceholder, ElementModQ *nonce, ElementModQ *cryptoHash,
      DisjunctiveChaumPedersenProof *proof, ElGamalCiphertext *extendedData)
        : data()
    {
        // copy the object id
        size_t _osize = strlen(object_id) + 1;
        strncpy(data.object_id, object_id, _osize);

        data.descriptionHash = descriptionHash;
        data.ciphertext = ciphertext;
        data.cryptoHash = cryptoHash;
        data.isPlaceholderSelection = isPlaceholder;
        data.nonce = nonce;
        data.proof = proof;
        data.extendedData = extendedData;
    }

    // property Getters

    CiphertextBallotSelection::~CiphertextBallotSelection() { data = {}; }

    char *CiphertextBallotSelection::getObjectId() { return data.object_id; }

    ElementModQ *CiphertextBallotSelection::getDescriptionHash() { return data.descriptionHash; }

    ElGamalCiphertext *CiphertextBallotSelection::getCiphertext() { return data.ciphertext; }

    DisjunctiveChaumPedersenProof *CiphertextBallotSelection::getProof() { return data.proof; }

    // public Members

    CiphertextBallotSelection *CiphertextBallotSelection::make(
      const string object_id, ElementModQ *descriptionHash, ElGamalCiphertext *ciphertext,
      ElementModP *elgamalPublicKey, ElementModQ *cryptoExtendedBaseHash, ElementModQ *proofSeed,
      uint64_t plaintext, bool isPlaceholder, ElementModQ *nonce, ElementModQ *cryptoHash,
      DisjunctiveChaumPedersenProof *proof, ElGamalCiphertext *extendedData)
    {
        // TODO: implement
        return nullptr;
    }

    ElementModQ *CiphertextBallotSelection::crypto_hash_with(ElementModQ *seedHash)
    {
        return makeCryptoHash(string(data.object_id), seedHash, data.ciphertext);
    }

    bool CiphertextBallotSelection::isValidEncryption(ElementModQ *seedHash,
                                                      ElementModP *elgamalPublicKey,
                                                      ElementModQ *cryptoExtendedBaseHash)
    {
        // TODO: implement
        return true;
    }

    // Protected Members

    ElementModQ *CiphertextBallotSelection::makeCryptoHash(string object_id, ElementModQ *seedHash,
                                                           ElGamalCiphertext *ciphertext)
    {
        return hash_elems({object_id, seedHash, ciphertext->crypto_hash()});
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