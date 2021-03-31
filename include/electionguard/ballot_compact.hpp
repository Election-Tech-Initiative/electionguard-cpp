#ifndef __ELECTIONGUARD_CPP_BALLOT_COMPACT_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_BALLOT_COMPACT_HPP_INCLUDED__

#include "ballot.hpp"
#include "chaum_pedersen.hpp"
#include "crypto_hashable.hpp"
#include "election.hpp"
#include "elgamal.hpp"
#include "export.h"
#include "group.hpp"

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
                               std::vector<uint64_t> selections,
                               std::map<uint64_t, std::unique_ptr<ExtendedData>> extendedData);
        ~CompactPlaintextBallot();

        CompactPlaintextBallot &operator=(CompactPlaintextBallot other);
        CompactPlaintextBallot &operator=(CompactPlaintextBallot &&other);

        std::string getObjectId() const;
        std::string getStyleId() const;

        std::vector<uint64_t> getSelections() const;
        std::map<uint64_t, std::reference_wrapper<ExtendedData>> getExtendedData() const;

        static std::unique_ptr<CompactPlaintextBallot> make(const PlaintextBallot &plaintext);

        std::unique_ptr<ExtendedData> getExtendedDataFor(const uint64_t index) const;

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

        std::string getObjectId() const;
        CompactPlaintextBallot *getPlaintext() const;
        ElementModQ *getBallotCodeSeed() const;
        ElementModQ *getBallotCode() const;
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

    EG_API std::unique_ptr<CompactPlaintextBallot>
    compressPlaintextBallot(const PlaintextBallot &plaintext);

    EG_API std::unique_ptr<CompactCiphertextBallot>
    compressCiphertextBallot(const PlaintextBallot &plaintext, const CiphertextBallot &ciphertext);

    EG_API std::unique_ptr<PlaintextBallot>
    expandCompactPlaintextBallot(const CompactPlaintextBallot &compactBallot,
                                 const InternalElectionDescription &metadata);

    EG_API std::unique_ptr<CiphertextBallot>
    expandCompactCiphertextBallot(const CompactCiphertextBallot &compactCiphertext,
                                  const InternalElectionDescription &metadata,
                                  const CiphertextElectionContext &context);

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_BALLOT_COMPACT_HPP_INCLUDED__ */