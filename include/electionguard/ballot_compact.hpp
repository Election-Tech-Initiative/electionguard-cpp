#ifndef __ELECTIONGUARD_CPP_BALLOT_COMPACT_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_BALLOT_COMPACT_HPP_INCLUDED__

#include "ballot.hpp"
#include "chaum_pedersen.hpp"
#include "crypto_hashable.hpp"
#include "election.hpp"
#include "elgamal.hpp"
#include "export.h"
#include "group.hpp"
#include "manifest.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace electionguard
{

    /// <summary>
    /// A CompactPlaintextBallot is a representation of a PlaintextBallot
    /// that includes only the minimal data necessary to interact with electionguard
    ///
    /// This class is space optimized to serve specific use cases on systems where storage
    /// is a concern.  This object expects an array of slection value votes that are ordered
    /// correctly according to the sequence order of all of the contests and selections on a ballot
    /// The CompactPlaintextBallot can be expanded into a PlaintextBallot by traversing the
    /// selections array and rebuilding the selections from the vote values.
    ///
    /// Extended data is included as a map where the key value of the map is the selection id
    /// corresponding to the selection that includes any extended data.
    ///
    /// Don't make this directly. Use `make` instead.
    /// </summary>
    class EG_API CompactPlaintextBallot
    {
      public:
        CompactPlaintextBallot(const CompactPlaintextBallot &other);
        CompactPlaintextBallot(const CompactPlaintextBallot &&other);
        CompactPlaintextBallot(const std::string &objectId, const std::string &styleId,
                               std::vector<uint64_t> selections, std::vector<std::string> writeins);
        ~CompactPlaintextBallot();

        CompactPlaintextBallot &operator=(CompactPlaintextBallot other);
        CompactPlaintextBallot &operator=(CompactPlaintextBallot &&other);

        /// <Summary>
        /// A unique Ballot ID that is relevant to the external system and must be unique
        /// within the dataset of the election.
        /// </Summary>
        std::string getObjectId() const;

        /// <Summary>
        /// The Object Id of the ballot style in the election manifest.  This value is used
        /// to determine which contests to expect on the ballot, to fill in missing values,
        /// and to validate that the ballot is well-formed
        /// </Summary>
        std::string getStyleId() const;

        /// <Summary>
        /// The collection of selections on the ballot ordered by the contest sequence order
        /// and the selection sequence order.  It is up to the consumer to guarantee the order of elements
        /// </Summary>
        std::vector<uint64_t> getSelections() const;

        /// <Summary>
        /// The collection of writeins on the ballot ordered by the contest sequence order
        /// and the selection sequence order.  It is up to the consumer to guarantee the order of elements
        /// </Summary>
        std::vector<std::string> getWriteIns() const;

        /// <Summary>
        /// Make a compact representation of a plaintext ballot
        /// </Summary>
        static std::unique_ptr<CompactPlaintextBallot> make(const PlaintextBallot &plaintext);

        /// <Summary>
        /// Export the ballot representation as BSON
        /// </Summary>
        std::vector<uint8_t> toBson() const;

        /// <Summary>
        /// Export the ballot representation as JSON
        /// </Summary>
        std::string toJson() const;

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        std::vector<uint8_t> toMsgPack() const;

        /// <Summary>
        /// Import the ballot representation from JSON
        /// </Summary>
        static std::unique_ptr<CompactPlaintextBallot> fromJson(std::string data);

        /// <Summary>
        /// Import the ballot representation from BSON
        /// </Summary>
        static std::unique_ptr<CompactPlaintextBallot> fromBson(std::vector<uint8_t> data);

        /// <Summary>
        /// Import the ballot representation from MsgPack
        /// </Summary>
        static std::unique_ptr<CompactPlaintextBallot> fromMsgPack(std::vector<uint8_t> data);

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// A CompactCiphertextBallot is a CompactPlaintextBallot that includes the encryption parameters
    /// to properly re-encrypt the same ballot.
    ///
    /// This class is space optimized to serve specific use cases where an encrypted ballot is used
    /// to verify that plaintext selections have not been tampered with.
    ///
    /// Don't make this directly. Use `make` instead.
    /// </summary>
    class EG_API CompactCiphertextBallot
    {
      public:
        CompactCiphertextBallot(const CompactCiphertextBallot &other);
        CompactCiphertextBallot(const CompactCiphertextBallot &&other);
        CompactCiphertextBallot(std::unique_ptr<CompactPlaintextBallot> plaintext,
                                BallotBoxState ballotBoxState,
                                std::unique_ptr<ElementModQ> ballotCodeSeed,
                                std::unique_ptr<ElementModQ> ballotCode, const uint64_t timestamp,
                                std::unique_ptr<ElementModQ> ballotNonce);
        ~CompactCiphertextBallot();

        CompactCiphertextBallot &operator=(CompactCiphertextBallot other);
        CompactCiphertextBallot &operator=(CompactCiphertextBallot &&other);

        /// <Summary>
        /// A unique Ballot ID that is relevant to the external system and must be unique
        /// within the dataset of the election.
        /// </Summary>
        std::string getObjectId() const;

        /// <Summary>
        /// Get a pointer to the plaintext representation
        /// </Summary>
        CompactPlaintextBallot *getPlaintext() const;

        /// <summary>
        /// The seed hash for the ballot.  It may be the encryption device hash,
        /// the hash of a previous ballot or the hash of some other value
        /// that is meaningful to the consuming application.
        /// </summary>
        ElementModQ *getBallotCodeSeed() const;

        /// <summary>
        /// The unique ballot code for this ballot that is derived from
        /// the ballot seed, the timestamp, and the hash of the encrypted values
        /// </summary>
        ElementModQ *getBallotCode() const;

        /// <summary>
        /// The nonce value used to encrypt all values in the ballot
        /// </summary>
        ElementModQ *getNonce() const;

        /// <summary>
        /// The timestamp indicating when the ballot was encrypted
        /// as measured by the encryption device.  This value does not
        /// provide units as it is up to the consuming system to indicate the scale.
        /// Typically a consumer may use seconds since epoch or ticks since epoch
        /// </summary>
        uint64_t getTimestamp() const;

        /// <summary>
        /// Get the ballot box state for this ballot
        /// </summary>
        BallotBoxState getBallotBoxState() const;

        /// <summary>
        /// Make a Compact Ciphertext Ballot from plaintext and ciphertext representations
        /// </summary>
        static std::unique_ptr<CompactCiphertextBallot> make(const PlaintextBallot &plaintext,
                                                             const CiphertextBallot &ciphertext);

        /// <summary>
        /// Set the ballot box state of the ballot
        /// </summary>
        void setBallotBoxState(BallotBoxState state);

        /// <Summary>
        /// Export the ballot representation as BSON
        /// </Summary>
        std::vector<uint8_t> toBson() const;

        /// <Summary>
        /// Export the ballot representation as JSON
        /// </Summary>
        std::string toJson() const;

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        std::vector<uint8_t> toMsgPack() const;

        /// <Summary>
        /// Import the ballot representation from JSON
        /// </Summary>
        static std::unique_ptr<CompactCiphertextBallot> fromJson(std::string data);

        /// <Summary>
        /// Import the ballot representation from BSON
        /// </Summary>
        static std::unique_ptr<CompactCiphertextBallot> fromBson(std::vector<uint8_t> data);

        /// <Summary>
        /// Import the ballot representation from MsgPack
        /// </Summary>
        static std::unique_ptr<CompactCiphertextBallot> fromMsgPack(std::vector<uint8_t> data);

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// Make a compact representation of a plaintext ballot
    /// </Summary>
    EG_API std::unique_ptr<CompactPlaintextBallot>
    compressPlaintextBallot(const PlaintextBallot &plaintext);

    /// <Summary>
    /// Make a compact representation of a ciphertext ballot
    /// </Summary>
    EG_API std::unique_ptr<CompactCiphertextBallot>
    compressCiphertextBallot(const PlaintextBallot &plaintext, const CiphertextBallot &ciphertext);

    /// <Summary>
    /// Expand a compact plaintext ballot into a plaintext ballot
    /// </Summary>
    EG_API std::unique_ptr<PlaintextBallot>
    expandCompactPlaintextBallot(const CompactPlaintextBallot &compactBallot,
                                 const InternalManifest &manifest);

    /// <Summary>
    /// Expand a compact ciphertext ballot into a ciphertext ballot
    /// </Summary>
    EG_API std::unique_ptr<CiphertextBallot>
    expandCompactCiphertextBallot(const CompactCiphertextBallot &compactCiphertext,
                                  const InternalManifest &manifest,
                                  const CiphertextElectionContext &context);

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_BALLOT_COMPACT_HPP_INCLUDED__ */