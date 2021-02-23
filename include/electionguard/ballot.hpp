#ifndef __ELECTIONGUARD_CPP_BALLOT_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_BALLOT_HPP_INCLUDED__

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
    /// ExtendedData represents any arbitrary data expressible as a string with a length.
    ///
    ///  This class is used primarily as a field on a selection to indicate a write-in candidate text value
    /// </summary>
    struct ExtendedData {
      public:
        std::string value;
        uint64_t length;

        ExtendedData(std::string value, uint64_t length) : value(value), length(length) {}
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
        // TODO: determine if these shouild be public or protected static functions
        // since the python code exposes these, but marks them with an underscore
        // to indicate they are internal library functions.
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

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    class EG_API CiphertextBallotContest : public CryptoHashCheckable
    {
      public:
        CiphertextBallotContest(const CiphertextBallotContest &other);
        CiphertextBallotContest(const CiphertextBallotContest &&other);
        CiphertextBallotContest(const std::string &objectId, const ElementModQ &descriptionHash,
                                std::vector<std::unique_ptr<CiphertextBallotSelection>> selections,
                                std::unique_ptr<ElementModQ> nonce,
                                std::unique_ptr<ElementModQ> cryptoHash,
                                std::unique_ptr<ConstantChaumPedersenProof> proof);
        ~CiphertextBallotContest();

        CiphertextBallotContest &operator=(CiphertextBallotContest other);
        CiphertextBallotContest &operator=(CiphertextBallotContest &&other);

        std::string getObjectId() const;
        ElementModQ *getDescriptionHash() const;
        std::vector<std::reference_wrapper<CiphertextBallotSelection>> getSelections() const;
        ElementModQ *getNonce() const;
        ElementModQ *getCryptoHash() const;
        ConstantChaumPedersenProof *getProof() const;

        virtual std::unique_ptr<ElementModQ>
        crypto_hash_with(const ElementModQ &seedHash) const override;

        static std::unique_ptr<CiphertextBallotContest>
        make(const std::string &objectId, const ElementModQ &descriptionHash,
             std::vector<std::unique_ptr<CiphertextBallotSelection>> selections,
             const ElementModP &elgamalPublicKey, const ElementModQ &cryptoExtendedBaseHash,
             const ElementModQ &proofSeed, const uint64_t numberElected,
             std::unique_ptr<ElementModQ> nonce = nullptr,
             std::unique_ptr<ElementModQ> cryptoHash = nullptr,
             std::unique_ptr<ConstantChaumPedersenProof> proof = nullptr);

        std::unique_ptr<ElementModQ> aggregateNonce() const;
        std::unique_ptr<ElGamalCiphertext> elgamalAccumulate() const;

        bool isValidEncryption(const ElementModQ &seedHash, const ElementModP &elgamalPublicKey,
                               const ElementModQ &cryptoExtendedBaseHash);

        //protected:
        // TODO: ISSUE #36: make this member protected or private
        // once the public api surface supports encrypt_contest
        static std::unique_ptr<ElementModQ> makeCryptoHash(
          std::string objectId,
          const std::vector<std::reference_wrapper<CiphertextBallotSelection>> &selections,
          const ElementModQ &seedHash);

      protected:
        static std::unique_ptr<ElementModQ> aggregateNonce(
          const std::vector<std::reference_wrapper<CiphertextBallotSelection>> &selections);
        static std::unique_ptr<ElGamalCiphertext> elgamalAccumulate(
          const std::vector<std::reference_wrapper<CiphertextBallotSelection>> &selections);

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

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
        static std::unique_ptr<PlaintextBallot> fromJson(std::string data);
        static std::unique_ptr<PlaintextBallot> fromBson(std::vector<uint8_t> data);

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

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
        static std::unique_ptr<CiphertextBallot> fromJson(std::string data);
        static std::unique_ptr<CiphertextBallot> fromBson(std::vector<uint8_t> data);

        //protected:
        // TODO: ISSUE #36: make this member protected or private
        // once the public api surface supports encrypt_contest
        static std::unique_ptr<ElementModQ>
        makeCryptoHash(std::string objectId,
                       const std::vector<std::reference_wrapper<CiphertextBallotContest>> &contests,
                       const ElementModQ &seedHash);

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_BALLOT_HPP_INCLUDED__ */