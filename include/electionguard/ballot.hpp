#ifndef __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__

#include "chaum_pedersen.hpp"
#include "crypto_hashable.hpp"
#include "elgamal.hpp"
#include "export.h"
#include "group.hpp"

#include <string>
#include <vector>

extern "C" {
#include "election_object_base.h"
}

#include <string>

namespace electionguard
{
    struct PlaintextBallotSelectionData : public ElectionObjectBase {
        char vote[256];
        bool isPlaceholderSelection;
    };

    /// <summary>
    /// A BallotSelection represents an individual selection on a ballot.
    ///
    /// This class accepts a `vote` string field which has no constraints
    /// in the ElectionGuard Data Specification, but is constrained logically
    /// in the application to resolve to `True` or `False`.  This implies that the
    /// data specification supports passing any string that can be represented as
    /// an integer, however only 0 and 1 is supported for now.
    ///
    /// This class can also be designated as `is_placeholder_selection` which has no
    /// context to the data specification but is useful for running validity checks internally
    ///
    /// an `extended_data` field exists to support any arbitrary data to be associated
    /// with the selection.  In practice, this field is the cleartext representation
    /// of a write-in candidate value.  In the current implementation these values are
    /// discarded when encrypting.
    /// </summary>
    class EG_API PlaintextBallotSelection
    {
      public:
        PlaintextBallotSelection(const string objectId, string vote);
        PlaintextBallotSelection(const char *object_id, const char *vote);
        ~PlaintextBallotSelection();

        int toInt();

        char *getObjectId();

      private:
        PlaintextBallotSelectionData data;
    };

    struct CiphertextBallotSelectionData : public ElectionObjectBase {
        ElementModQ *descriptionHash;
        ElGamalCiphertext *ciphertext;
        ElementModQ *cryptoHash;
        bool isPlaceholderSelection;
        ElementModQ *nonce;
        DisjunctiveChaumPedersenProof *proof;
        ElGamalCiphertext *extendedData;
    };

    /// <summary>
    /// A CiphertextBallotSelection represents an individual encrypted selection on a ballot.
    ///
    ///This class accepts a `description_hash` and a `ciphertext` as required parameters
    ///in its constructor.
    ///
    ///When a selection is encrypted, the `description_hash` and `ciphertext` required fields must
    ///be populated at construction however the `nonce` is also usually provided by convention.
    ///
    ///After construction, the `crypto_hash` field is populated automatically in the `__post_init__` cycle
    ///
    ///A consumer of this object has the option to discard the `nonce` and/or discard the `proof`,
    ///or keep both values.
    ///
    ///By discarding the `nonce`, the encrypted representation and `proof`
    /// can only be regenerated if the nonce was derived from the ballot's master nonce.  If the nonce
    ///used for this selection is truly random, and it is discarded, then the proofs cannot be regenerated.
    ///
    ///By keeping the `nonce`, or deriving the selection nonce from the ballot nonce, an external system can
    ///regenerate the proofs on demand.  This is useful for storage or memory constrained systems.
    ///
    ///By keeping the `proof` the nonce is not required fotor verify the encrypted selection.
    /// </summary>
    class EG_API CiphertextBallotSelection : public CryptoHashCheckable
    {
      public:
        CiphertextBallotSelection(const string objectId, ElementModQ *descriptionHash,
                                  ElGamalCiphertext *ciphertext, bool isPlaceholder,
                                  ElementModQ *nonce, ElementModQ *cryptoHash,
                                  DisjunctiveChaumPedersenProof *proof,
                                  ElGamalCiphertext *extendedData);
        CiphertextBallotSelection(const char *object_id, ElementModQ *descriptionHash,
                                  ElGamalCiphertext *ciphertext, bool isPlaceholder,
                                  ElementModQ *nonce, ElementModQ *cryptoHash,
                                  DisjunctiveChaumPedersenProof *proof,
                                  ElGamalCiphertext *extendedData);
        ~CiphertextBallotSelection();

        char *getObjectId();

        /// <summary>
        /// The SelectionDescription hash
        /// </summary>
        ElementModQ *getDescriptionHash();

        /// The encrypted representation of the vote field
        ElGamalCiphertext *getCiphertext();

        /// The proof that demonstrates the selection is an encryption of 0 or 1, and was encrypted using the `nonce`
        DisjunctiveChaumPedersenProof *getProof();

        /// <summary>
        /// Constructs a `CipherTextBallotSelection` object. Most of the parameters here match up to fields
        /// in the class, but this helper function will optionally compute a Chaum-Pedersen proof if the
        /// given nonce isn't `None`. Likewise, if a crypto_hash is not provided, it will be derived from
        /// the other fields.
        ///</summary>
        static CiphertextBallotSelection *
        make(const string objectId, ElementModQ *descriptionHash, ElGamalCiphertext *ciphertext,
             ElementModP *elgamalPublicKey, ElementModQ *cryptoExtendedBaseHash,
             ElementModQ *proofSeed, uint64_t plaintext, bool isPlaceholder = false,
             ElementModQ *nonce = nullptr, ElementModQ *cryptoHash = nullptr,
             DisjunctiveChaumPedersenProof *proof = nullptr,
             ElGamalCiphertext *extendedData = nullptr);

        /// <summary>
        /// Given an encrypted BallotSelection, generates a hash, suitable for rolling up
        /// into a hash / tracking code for an entire ballot. Of note, this particular hash examines
        /// the `seed_hash` and `message`, but not the proof.
        /// This is deliberate, allowing for the possibility of ElectionGuard variants running on
        /// much more limited hardware, wherein the Disjunctive Chaum-Pedersen proofs might be computed
        /// later on.
        ///
        /// In most cases the seed_hash should match the `description_hash`
        /// </summary>
        virtual ElementModQ *crypto_hash_with(ElementModQ *seedHash);

        bool isValidEncryption(ElementModQ *seedHash, ElementModP *elgamalPublicKey,
                               ElementModQ *cryptoExtendedBaseHash);

      protected:
        // TODO: determine if these shouild be public or protected static functions
        // since the python code exposes these, but marks them with an underscore
        // to indicate they are internal library functions.
        static ElementModQ *makeCryptoHash(string object_id, ElementModQ *seed_hash,
                                           ElGamalCiphertext *ciphertext);

      private:
        CiphertextBallotSelectionData data;

        /// set the hash of the encrypted values.
        /// used when reconstrcuting objects from cache
        // TODO: void setCryptoHash(ElementModQ *cryptoHash);

        /// The nonce used to generate the encryption.
        /// Sensitive & should be treated as a secret
        //TODO: void setNonce(ElementModQ *nonce);
    };

    class EG_API CiphertextBallotContest : public CryptoHashCheckable
    {
      public:
        // TODO: ISSUE #36 make this member protected or private
        // once the public api surface supports encrypt_contest
        static ElementModQ *makeCryptoHash(string object_id,
                                           vector<CiphertextBallotSelection> selections,
                                           ElementModQ *seed_hash);
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__ */