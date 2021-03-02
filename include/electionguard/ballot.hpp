#ifndef __ELECTIONGUARD_CPP_BALLOT_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_BALLOT_HPP_INCLUDED__

#include "chaum_pedersen.hpp"
#include "crypto_hashable.hpp"
#include "elgamal.hpp"
#include "export.h"
#include "group.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace electionguard
{
    enum class BallotBoxState { cast = 1, spoiled = 2, unknown = 999 };

    EG_API const std::string getBallotBoxStateString(const BallotBoxState &value);
    EG_API BallotBoxState getBallotBoxState(const std::string &value);

    /// <summary>
    /// ExtendedData represents any arbitrary data expressible as a string with a length.
    ///
    ///  This class is used primarily as a field on a selection to indicate a write-in candidate text value
    /// </summary>
    struct ExtendedData {
      public:
        std::string value;
        uint64_t length;

        ExtendedData(std::string value, uint64_t length) : value(value), length(length) {}

        std::unique_ptr<ExtendedData> clone() const
        {
            return std::make_unique<ExtendedData>(this->value, this->length);
        }
    };

    /// <summary>
    /// A BallotSelection represents an individual selection on a ballot.
    ///
    /// This class accepts a `vote` integer field which has no constraints
    /// in the ElectionGuard Data Specification, but is constrained logically
    /// in the application to resolve to `True` or `False`aka only 0 and 1 is
    /// supported for now.
    ///
    /// This class can also be designated as `isPlaceholderSelection` which has no
    /// context to the data specification but is useful for running validity checks internally
    ///
    /// an `extendedData` field exists to support any arbitrary data to be associated
    /// with the selection.  In practice, this field is the cleartext representation
    /// of a write-in candidate value.  In the current implementation these values are
    /// discarded when encrypting.
    /// </summary>
    class EG_API PlaintextBallotSelection
    {
      public:
        PlaintextBallotSelection(const PlaintextBallotSelection &other);
        PlaintextBallotSelection(const PlaintextBallotSelection &&other);
        PlaintextBallotSelection(std::string objectId, uint64_t vote,
                                 bool isPlaceholderSelection = false,
                                 std::unique_ptr<ExtendedData> extendedData = nullptr);
        ~PlaintextBallotSelection();

        PlaintextBallotSelection &operator=(PlaintextBallotSelection other);
        PlaintextBallotSelection &operator=(PlaintextBallotSelection &&other);

        std::string getObjectId() const;
        uint64_t getVote() const;

        /// <summary>
        /// Determines if this is a placeholder selection
        /// </summary>
        bool getIsPlaceholder() const;

        /// <summary>
        /// an optional field of arbitrary data, such as the value of a write-in candidate
        /// </summary>
        ExtendedData *getExtendedData() const;

        /// <summary>
        /// Given a PlaintextBallotSelection validates that the object matches an expected object
        /// and that the plaintext value can resolve to a valid representation
        /// </summary>
        bool isValid(const std::string &expectedObjectId) const;

        std::unique_ptr<PlaintextBallotSelection> clone() const;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// A CiphertextBallotSelection represents an individual encrypted selection on a ballot.
    ///
    /// This class accepts a `description_hash` and a `ciphertext` as required parameters
    /// in its constructor.
    ///
    /// When a selection is encrypted, the `description_hash` and `ciphertext` required fields must
    /// be populated at construction however the `nonce` is also usually provided by convention.
    ///
    /// After construction, the `crypto_hash` field is populated automatically in the `__post_init__` cycle
    ///
    /// A consumer of this object has the option to discard the `nonce` and/or discard the `proof`,
    /// or keep both values.
    ///
    /// By discarding the `nonce`, the encrypted representation and `proof`
    /// can only be regenerated if the nonce was derived from the ballot's master nonce.  If the nonce
    /// used for this selection is truly random, and it is discarded, then the proofs cannot be regenerated.
    ///
    /// By keeping the `nonce`, or deriving the selection nonce from the ballot nonce, an external system can
    /// regenerate the proofs on demand.  This is useful for storage or memory constrained systems.
    ///
    /// By keeping the `proof` the nonce is not required fotor verify the encrypted selection.
    /// </summary>
    class EG_API CiphertextBallotSelection : public CryptoHashCheckable
    {
      public:
        CiphertextBallotSelection(const CiphertextBallotSelection &other);
        CiphertextBallotSelection(const CiphertextBallotSelection &&other);
        CiphertextBallotSelection(const std::string &objectId, const ElementModQ &descriptionHash,
                                  std::unique_ptr<ElGamalCiphertext> ciphertext, bool isPlaceholder,
                                  std::unique_ptr<ElementModQ> nonce,
                                  std::unique_ptr<ElementModQ> cryptoHash,
                                  std::unique_ptr<DisjunctiveChaumPedersenProof> proof,
                                  std::unique_ptr<ElGamalCiphertext> extendedData = nullptr);
        ~CiphertextBallotSelection();

        CiphertextBallotSelection &operator=(CiphertextBallotSelection other);
        CiphertextBallotSelection &operator=(CiphertextBallotSelection &&other);

        std::string getObjectId() const;

        /// <summary>
        /// The SelectionDescription hash
        /// </summary>
        ElementModQ *getDescriptionHash() const;

        /// <summary>
        /// Determines if this is a placeholder selection
        /// </summary>
        bool getIsPlaceholder() const;

        /// <summary>
        /// The encrypted representation of the vote field
        /// </summary>
        ElGamalCiphertext *getCiphertext() const;

        /// <summary>
        /// The hash of the encrypted values
        /// </summary>
        ElementModQ *getCryptoHash() const;

        /// <summary>
        /// The nonce used to generate the encryption. Sensitive &amp; should be treated as a secret
        /// </summary>
        ElementModQ *getNonce();

        /// <summary>
        /// The proof that demonstrates the selection is an encryption of 0 or 1, and was encrypted using the `nonce`
        /// </summary>
        DisjunctiveChaumPedersenProof *getProof() const;

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
        virtual std::unique_ptr<ElementModQ> crypto_hash_with(const ElementModQ &seedHash) override;
        virtual std::unique_ptr<ElementModQ>
        crypto_hash_with(const ElementModQ &seedHash) const override;

        /// <summary>
        /// Constructs a `CipherTextBallotSelection` object. Most of the parameters here match up to fields
        /// in the class, but this helper function will optionally compute a Chaum-Pedersen proof if the
        /// given nonce isn't `None`. Likewise, if a crypto_hash is not provided, it will be derived from
        /// the other fields.
        ///</summary>
        static std::unique_ptr<CiphertextBallotSelection>
        make(const std::string &objectId, const ElementModQ &descriptionHash,
             std::unique_ptr<ElGamalCiphertext> ciphertext, const ElementModP &elgamalPublicKey,
             const ElementModQ &cryptoExtendedBaseHash, const ElementModQ &proofSeed,
             uint64_t plaintext, bool isPlaceholder = false,
             std::unique_ptr<ElementModQ> nonce = nullptr,
             std::unique_ptr<ElementModQ> cryptoHash = nullptr,
             std::unique_ptr<DisjunctiveChaumPedersenProof> proof = nullptr,
             std::unique_ptr<ElGamalCiphertext> extendedData = nullptr);

        /// <sumary>
        /// Given an encrypted BallotSelection, validates the encryption state against a specific seed hash and public key.
        /// Calling this function expects that the object is in a well-formed encrypted state
        /// with the elgamal encrypted `message` field populated along with
        /// the DisjunctiveChaumPedersenProof`proof` populated.
        /// the ElementModQ `description_hash` and the ElementModQ `crypto_hash` are also checked.
        ///
        /// <param name="seed_hash">The hash of the SelectionDescription, or
        ///                         whatever `ElementModQ` was used to populate the `description_hash` field. </param>
        /// <param name="elgamalPublicKey">The election public key</param>
        /// <param name="cryptoExtendedBaseHash">The extended base hash of the election</param>
        /// </summary>
        bool isValidEncryption(const ElementModQ &seedHash, const ElementModP &elgamalPublicKey,
                               const ElementModQ &cryptoExtendedBaseHash);

      protected:
        static std::unique_ptr<ElementModQ> makeCryptoHash(const std::string &objectId,
                                                           const ElementModQ &seedHash,
                                                           const ElGamalCiphertext &ciphertext);

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;

        /// set the hash of the encrypted values.
        /// used when reconstrcuting objects from cache
        // TODO: void setCryptoHash(ElementModQ *cryptoHash);

        /// The nonce used to generate the encryption.
        /// Sensitive & should be treated as a secret
        //TODO: void setNonce(ElementModQ *nonce);
    };

    class EG_API PlaintextBallotContest
    {
      public:
        PlaintextBallotContest(const PlaintextBallotContest &other);
        PlaintextBallotContest(const PlaintextBallotContest &&other);
        PlaintextBallotContest(const std::string &objectId,
                               std::vector<std::unique_ptr<PlaintextBallotSelection>> selections);
        ~PlaintextBallotContest();

        PlaintextBallotContest &operator=(PlaintextBallotContest other);
        PlaintextBallotContest &operator=(PlaintextBallotContest &&other);

        std::string getObjectId() const;
        std::vector<std::reference_wrapper<PlaintextBallotSelection>> getSelections() const;

        bool isValid(const std::string &expectedObjectId, uint64_t expectedNumberSelections,
                     uint64_t expectedNumberElected, uint64_t votesAllowd = 0) const;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// A CiphertextBallotContest represents the selections made by a voter for a specific ContestDescription
    ///
    /// CiphertextBallotContest can only be a complete representation of a contest dataset.  While
    /// PlaintextBallotContest supports a partial representation, a CiphertextBallotContest includes all data
    /// necessary for a verifier to verify the contest.  Specifically, it includes both explicit affirmative
    /// and negative selections of the contest, as well as the placeholder selections that satisfy
    /// the ConstantChaumPedersen proof.
    ///
    /// Similar to `CiphertextBallotSelection` the consuming application can choose to discard or keep both
    /// the `nonce` and the `proof` in some circumstances.  For deterministic nonce's derived from the
    /// master nonce, both values can be regenerated.  If the `nonce` for this contest is completely random,
    /// then it is required in order to regenerate the proof.
    /// </summary>
    class EG_API CiphertextBallotContest : public CryptoHashCheckable
    {
      public:
        CiphertextBallotContest(const CiphertextBallotContest &other);
        CiphertextBallotContest(const CiphertextBallotContest &&other);
        CiphertextBallotContest(const std::string &objectId, const ElementModQ &descriptionHash,
                                std::vector<std::unique_ptr<CiphertextBallotSelection>> selections,
                                std::unique_ptr<ElementModQ> nonce,
                                std::unique_ptr<ElGamalCiphertext> ciphertextAccumulation,
                                std::unique_ptr<ElementModQ> cryptoHash,
                                std::unique_ptr<ConstantChaumPedersenProof> proof);
        ~CiphertextBallotContest();

        CiphertextBallotContest &operator=(CiphertextBallotContest other);
        CiphertextBallotContest &operator=(CiphertextBallotContest &&other);

        std::string getObjectId() const;

        /// <summary>
        /// Hash from contestDescription
        /// </summary>
        ElementModQ *getDescriptionHash() const;

        /// <summary>
        /// Collection of ballot selections
        /// </summary>
        std::vector<std::reference_wrapper<CiphertextBallotSelection>> getSelections() const;

        /// <summary>
        /// The nonce used to generate the encryption. Sensitive & should be treated as a secret
        /// </summary>
        ElementModQ *getNonce() const;

        /// <summary>
        /// The encrypted representation of all of the vote fields (the contest total)
        /// </summary>
        ElGamalCiphertext *getCiphertextAccumulation() const;

        /// <summary>
        /// Hash of the encrypted values
        /// </summary>
        ElementModQ *getCryptoHash() const;

        /// <summary>
        /// The proof demonstrates the sum of the selections does not exceed the maximum
        /// available selections for the contest, and that the proof was generated with the nonce
        /// </summary>
        ConstantChaumPedersenProof *getProof() const;

        /// <summary>
        /// Given an encrypted BallotContest, generates a hash, suitable for rolling up
        /// into a hash / tracking code for an entire ballot. Of note, this particular hash examines
        /// the `seed_hash` and `ballot_selections`, but not the proof.
        /// This is deliberate, allowing for the possibility of ElectionGuard variants running on
        /// much more limited hardware, wherein the Disjunctive Chaum-Pedersen proofs might be computed
        /// later on.
        ///
        /// In most cases, the seed_hash is the description_hash
        /// </summary>
        virtual std::unique_ptr<ElementModQ>
        crypto_hash_with(const ElementModQ &seedHash) const override;

        /// <summary>
        /// Constructs a `CipherTextBallotContest` object. Most of the parameters here match up to fields
        /// in the class, but this helper function will optionally compute a Chaum-Pedersen proof if the
        /// ballot selections include their encryption nonces. Likewise, if a crypto_hash is not provided,
        /// it will be derived from the other fields.
        /// </summary>
        static std::unique_ptr<CiphertextBallotContest>
        make(const std::string &objectId, const ElementModQ &descriptionHash,
             std::vector<std::unique_ptr<CiphertextBallotSelection>> selections,
             const ElementModP &elgamalPublicKey, const ElementModQ &cryptoExtendedBaseHash,
             const ElementModQ &proofSeed, const uint64_t numberElected,
             std::unique_ptr<ElementModQ> nonce = nullptr,
             std::unique_ptr<ElementModQ> cryptoHash = nullptr,
             std::unique_ptr<ConstantChaumPedersenProof> proof = nullptr);

        /// <summary>
        /// an aggregate nonce for the contest composed of the nonces of the selections
        /// </summary>
        std::unique_ptr<ElementModQ> aggregateNonce() const;

        /// <summary>
        /// Add the individual ballot_selections `message` fields together, suitable for use
        /// in a Chaum-Pedersen proof.
        /// </summary>
        std::unique_ptr<ElGamalCiphertext> elgamalAccumulate() const;

        /// <summary>
        /// Given an encrypted BallotContest, validates the encryption state against a specific seed hash and public key
        /// by verifying the accumulated sum of selections match the proof.
        /// Calling this function expects that the object is in a well-formed encrypted state
        /// with the `ballot_selections` populated with valid encrypted ballot selections,
        /// the ElementModQ `description_hash`, the ElementModQ `crypto_hash`,
        /// and the ConstantChaumPedersenProof all populated.
        /// Specifically, the seed hash in this context is the hash of the ContestDescription,
        /// or whatever `ElementModQ` was used to populate the `description_hash` field.
        /// </summary>
        bool isValidEncryption(const ElementModQ &seedHash, const ElementModP &elgamalPublicKey,
                               const ElementModQ &cryptoExtendedBaseHash);

      protected:
        static std::unique_ptr<ElementModQ> makeCryptoHash(
          std::string objectId,
          const std::vector<std::reference_wrapper<CiphertextBallotSelection>> &selections,
          const ElementModQ &seedHash);
        static std::unique_ptr<ElementModQ> aggregateNonce(
          const std::vector<std::reference_wrapper<CiphertextBallotSelection>> &selections);
        static std::unique_ptr<ElGamalCiphertext> elgamalAccumulate(
          const std::vector<std::reference_wrapper<CiphertextBallotSelection>> &selections);

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// A PlaintextBallot represents a voters selections for a given ballot and ballot style
    ///
    /// <param name="object_id"> A unique Ballot ID that is relevant to the external system </param>
    /// </summary>
    class EG_API PlaintextBallot
    {
      public:
        PlaintextBallot(const PlaintextBallot &other);
        PlaintextBallot(const PlaintextBallot &&other);
        PlaintextBallot(const std::string &objectId, const std::string &ballotStyle,
                        std::vector<std::unique_ptr<PlaintextBallotContest>> contests);
        ~PlaintextBallot();

        PlaintextBallot &operator=(PlaintextBallot other);
        PlaintextBallot &operator=(PlaintextBallot &&other);

        std::string getObjectId() const;
        std::string getBallotStyle() const;

        std::vector<std::reference_wrapper<PlaintextBallotContest>> getContests() const;

        std::vector<uint8_t> toBson() const;
        std::string toJson() const;
        std::vector<uint8_t> toMsgPack() const;
        static std::unique_ptr<PlaintextBallot> fromBson(std::vector<uint8_t> data);
        static std::unique_ptr<PlaintextBallot> fromJson(std::string data);
        static std::unique_ptr<PlaintextBallot> fromMsgPack(std::vector<uint8_t> data);

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// A PlaintextBallot represents a voters selections for a given ballot and ballot style
    ///
    /// <param name="object_id"> A unique Ballot ID that is relevant to the external system </param>
    /// </summary>
    class EG_API CompactPlaintextBallot
    {
      public:
        CompactPlaintextBallot(const CompactPlaintextBallot &other);
        CompactPlaintextBallot(const CompactPlaintextBallot &&other);
        CompactPlaintextBallot(const std::string &objectId, const std::string &ballotStyle,
                               std::vector<uint64_t> selections,
                               std::map<std::string, std::unique_ptr<ExtendedData>> extendedData);
        ~CompactPlaintextBallot();

        CompactPlaintextBallot &operator=(CompactPlaintextBallot other);
        CompactPlaintextBallot &operator=(CompactPlaintextBallot &&other);

        std::string getObjectId() const;
        std::string getBallotStyle() const;

        std::vector<uint64_t> getSelections() const;
        std::map<std::string, std::reference_wrapper<ExtendedData>> getExtendedData() const;

        static std::unique_ptr<CompactPlaintextBallot> make(const PlaintextBallot &plaintext);

        std::unique_ptr<ExtendedData>
        getExtendedDataFor(const std::string &selectionObjectId) const;

        std::vector<uint8_t> toBson() const;
        std::string toJson() const;
        std::vector<uint8_t> toMsgPack() const;
        static std::unique_ptr<CompactPlaintextBallot> fromJson(std::string data);
        static std::unique_ptr<CompactPlaintextBallot> fromBson(std::vector<uint8_t> data);
        static std::unique_ptr<CompactPlaintextBallot> fromMsgPack(std::vector<uint8_t> data);

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// A CiphertextBallot represents a voters encrypted selections for a given ballot and ballot style.
    ///
    /// When a ballot is in it's complete, encrypted state, the `nonce` is the master nonce
    /// from which all other nonces can be derived to encrypt the ballot.  Allong with the `nonce`
    /// fields on `Ballotcontest` and `BallotSelection`, this value is sensitive.
    ///
    /// Don't make this directly. Use `make_ciphertext_ballot` instead.
    /// <param name="object_id"> A unique Ballot ID that is relevant to the external system </param>
    /// </summary>
    class EG_API CiphertextBallot : public CryptoHashCheckable
    {
      public:
        CiphertextBallot(const CiphertextBallot &other);
        CiphertextBallot(const CiphertextBallot &&other);
        CiphertextBallot(const std::string &objectId, const std::string &ballotStyle,
                         const ElementModQ &descriptionHash,
                         std::unique_ptr<ElementModQ> previousTrackingHash,
                         std::vector<std::unique_ptr<CiphertextBallotContest>> contests,
                         std::unique_ptr<ElementModQ> trackingHash, const uint64_t timestamp,
                         std::unique_ptr<ElementModQ> nonce,
                         std::unique_ptr<ElementModQ> cryptoHash);
        ~CiphertextBallot();

        CiphertextBallot &operator=(CiphertextBallot other);
        CiphertextBallot &operator=(CiphertextBallot &&other);

        std::string getObjectId() const;
        std::string getBallotStyle() const;
        ElementModQ *getDescriptionHash() const;
        ElementModQ *getPreviousTrackingHash() const;

        std::vector<std::reference_wrapper<CiphertextBallotContest>> getContests() const;

        ElementModQ *getTrackingHash() const;
        std::string getTrackingCode() const;
        uint64_t getTimestamp() const;
        ElementModQ *getNonce() const;
        ElementModQ *getCryptoHash() const;

        virtual std::unique_ptr<ElementModQ>
        crypto_hash_with(const ElementModQ &seedHash) const override;

        static std::unique_ptr<CiphertextBallot>
        make(const std::string &objectId, const std::string &ballotStyle,
             const ElementModQ &descriptionHash,
             std::vector<std::unique_ptr<CiphertextBallotContest>> contests,
             std::unique_ptr<ElementModQ> nonce = nullptr, const uint64_t timestamp = 0,
             std::unique_ptr<ElementModQ> previousTrackingHash = nullptr,
             std::unique_ptr<ElementModQ> trackingHash = nullptr);

        static std::unique_ptr<ElementModQ> nonceSeed(const ElementModQ &descriptionHash,
                                                      const std::string &objectId,
                                                      const ElementModQ &nonce);

        bool isValidEncryption(const ElementModQ &seedHash, const ElementModP &elgamalPublicKey,
                               const ElementModQ &cryptoExtendedBaseHash);

        std::unique_ptr<ElementModQ> nonceSeed();

        std::vector<uint8_t> toBson(bool withNonces = false) const;
        std::string toJson(bool withNonces = false) const;
        std::vector<uint8_t> toMsgPack(bool withNonces = false) const;
        static std::unique_ptr<CiphertextBallot> fromJson(std::string data);
        static std::unique_ptr<CiphertextBallot> fromBson(std::vector<uint8_t> data);
        static std::unique_ptr<CiphertextBallot> fromMsgPack(std::vector<uint8_t> data);

      protected:
        static std::unique_ptr<ElementModQ>
        makeCryptoHash(std::string objectId,
                       const std::vector<std::reference_wrapper<CiphertextBallotContest>> &contests,
                       const ElementModQ &seedHash);

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    class EG_API CompactCiphertextBallot
    {
      public:
        CompactCiphertextBallot(const CompactCiphertextBallot &other);
        CompactCiphertextBallot(const CompactCiphertextBallot &&other);
        CompactCiphertextBallot(std::unique_ptr<CompactPlaintextBallot> plaintext,
                                BallotBoxState ballotBoxState,
                                std::unique_ptr<ElementModQ> previousTrackingHash,
                                std::unique_ptr<ElementModQ> trackingHash, const uint64_t timestamp,
                                std::unique_ptr<ElementModQ> ballotNonce);
        ~CompactCiphertextBallot();

        CompactCiphertextBallot &operator=(CompactCiphertextBallot other);
        CompactCiphertextBallot &operator=(CompactCiphertextBallot &&other);

        std::string getObjectId() const;
        CompactPlaintextBallot *getPlaintext() const;
        ElementModQ *getPreviousTrackingHash() const;
        ElementModQ *getTrackingHash() const;
        ElementModQ *getNonce() const;
        uint64_t getTimestamp() const;
        BallotBoxState getBallotBoxState() const;

        static std::unique_ptr<CompactCiphertextBallot> make(const PlaintextBallot &plaintext,
                                                             const CiphertextBallot &ciphertext);

        void setBallotBoxState(BallotBoxState state);

        std::vector<uint8_t> toBson() const;
        std::string toJson() const;
        std::vector<uint8_t> toMsgPack() const;
        static std::unique_ptr<CompactCiphertextBallot> fromJson(std::string data);
        static std::unique_ptr<CompactCiphertextBallot> fromBson(std::vector<uint8_t> data);
        static std::unique_ptr<CompactCiphertextBallot> fromMsgPack(std::vector<uint8_t> data);

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_BALLOT_HPP_INCLUDED__ */