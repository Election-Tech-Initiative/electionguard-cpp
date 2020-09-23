#ifndef __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__

#include "chaum_pedersen.hpp"
#include "crypto_hashable.hpp"
#include "elgamal.hpp"
#include "export.h"
#include "group.hpp"

#include <memory>
#include <string>
#include <vector>

namespace electionguard
{
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
        PlaintextBallotSelection(const PlaintextBallotSelection &other);
        PlaintextBallotSelection(const PlaintextBallotSelection &&other);
        PlaintextBallotSelection(const string &objectId, const string &vote,
                                 bool isPlaceholderSelection = false);
        ~PlaintextBallotSelection();

        PlaintextBallotSelection &operator=(PlaintextBallotSelection other);
        PlaintextBallotSelection &operator=(PlaintextBallotSelection &&other);

        uint64_t toInt() const;

        string getObjectId() const;

      private:
        class Impl;
        unique_ptr<Impl> pimpl;
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
        CiphertextBallotSelection(const CiphertextBallotSelection &other);
        CiphertextBallotSelection(const CiphertextBallotSelection &&other);
        CiphertextBallotSelection(const string &objectId, const ElementModQ &descriptionHash,
                                  unique_ptr<ElGamalCiphertext> ciphertext, bool isPlaceholder,
                                  unique_ptr<ElementModQ> nonce, unique_ptr<ElementModQ> cryptoHash,
                                  unique_ptr<DisjunctiveChaumPedersenProof> proof,
                                  unique_ptr<ElGamalCiphertext> extendedData = nullptr);
        ~CiphertextBallotSelection();

        CiphertextBallotSelection &operator=(CiphertextBallotSelection other);
        CiphertextBallotSelection &operator=(CiphertextBallotSelection &&other);

        string getObjectId() const;

        /// <summary>
        /// The SelectionDescription hash
        /// </summary>
        ElementModQ *getDescriptionHash() const;

        /// The encrypted representation of the vote field
        ElGamalCiphertext *getCiphertext() const;

        /// The proof that demonstrates the selection is an encryption of 0 or 1, and was encrypted using the `nonce`
        DisjunctiveChaumPedersenProof *getProof() const;

        /// <summary>
        /// Constructs a `CipherTextBallotSelection` object. Most of the parameters here match up to fields
        /// in the class, but this helper function will optionally compute a Chaum-Pedersen proof if the
        /// given nonce isn't `None`. Likewise, if a crypto_hash is not provided, it will be derived from
        /// the other fields.
        ///</summary>
        static unique_ptr<CiphertextBallotSelection>
        make(const string &objectId, const ElementModQ &descriptionHash,
             unique_ptr<ElGamalCiphertext> ciphertext, const ElementModP &elgamalPublicKey,
             const ElementModQ &cryptoExtendedBaseHash, const ElementModQ &proofSeed,
             uint64_t plaintext, bool isPlaceholder = false,
             unique_ptr<ElementModQ> nonce = nullptr, unique_ptr<ElementModQ> cryptoHash = nullptr,
             unique_ptr<DisjunctiveChaumPedersenProof> proof = nullptr,
             unique_ptr<ElGamalCiphertext> extendedData = nullptr);

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
        virtual unique_ptr<ElementModQ> crypto_hash_with(const ElementModQ &seedHash);

        bool isValidEncryption(const ElementModQ &seedHash, const ElementModP &elgamalPublicKey,
                               const ElementModQ &cryptoExtendedBaseHash);

      protected:
        // TODO: determine if these shouild be public or protected static functions
        // since the python code exposes these, but marks them with an underscore
        // to indicate they are internal library functions.
        static unique_ptr<ElementModQ> makeCryptoHash(const string &objectId,
                                                      const ElementModQ &seedHash,
                                                      const ElGamalCiphertext &ciphertext);

      private:
        class Impl;
        unique_ptr<Impl> pimpl;

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