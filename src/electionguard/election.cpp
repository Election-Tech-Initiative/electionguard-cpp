#include "electionguard/election.hpp"

#include "electionguard/hash.hpp"
#include "log.hpp"
#include "serialize.hpp"

#include <cstring>

using std::string;

using DescriptionSerializer = electionguard::Serialize::InternalElectionDescription;
using ContextSerializer = electionguard::Serialize::CiphertextelectionContext;

namespace electionguard
{

#pragma region SelectionDescription

    struct SelectionDescription::Impl : public ElectionObjectBase {
        string candidateId;
        uint64_t sequenceOrder;

        Impl(const string &objectId, string candidateId, const uint64_t sequenceOrder)
            : candidateId(move(candidateId))
        {
            this->object_id = objectId;
            this->sequenceOrder = sequenceOrder;
        }

        unique_ptr<ElementModQ> crypto_hash()
        {
            return hash_elems({object_id, sequenceOrder, candidateId});
        }
    };

    // Lifecycle Methods

    SelectionDescription::SelectionDescription(const string &objectId, const string &candidateId,
                                               const uint64_t sequenceOrder)
        : pimpl(new Impl(objectId, candidateId, sequenceOrder))
    {
    }

    SelectionDescription::~SelectionDescription() = default;

    SelectionDescription &SelectionDescription::operator=(SelectionDescription other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string SelectionDescription::getObjectId() const { return pimpl->object_id; }
    string SelectionDescription::getCandidateId() const { return pimpl->candidateId; }
    uint64_t SelectionDescription::getSequenceOrder() const { return pimpl->sequenceOrder; }

    // Interface Overrides

    unique_ptr<ElementModQ> SelectionDescription::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> SelectionDescription::crypto_hash() const
    {
        return pimpl->crypto_hash();
    }

#pragma endregion

#pragma region ContestDescription

    struct ContestDescription::Impl : ElectionObjectBase {
        string electoralDistrictId;
        uint64_t sequenceOrder;
        string voteVariation;
        uint64_t numberElected;
        uint64_t votesAllowed;
        string name;
        string ballotTitle;
        string ballotSubtitle;
        vector<unique_ptr<SelectionDescription>> selections;
        Impl(const string &objectId, const string &electoralDistrictId,
             const uint64_t sequenceOrder, const string &voteVariation,
             const uint64_t numberElected, const uint64_t votesAllowed, const string &name,
             const string &ballotTitle, const string &ballotSubtitle,
             vector<unique_ptr<SelectionDescription>> selections)
            : selections(move(selections))
        {
            this->object_id = objectId;
            this->electoralDistrictId = electoralDistrictId;
            this->sequenceOrder = sequenceOrder;
            this->voteVariation = voteVariation;
            this->numberElected = numberElected;
            this->votesAllowed = votesAllowed;
            this->name = name;
            this->ballotTitle = ballotTitle;
            this->ballotSubtitle = ballotSubtitle;
        }

        unique_ptr<ElementModQ> crypto_hash() const
        {
            vector<reference_wrapper<CryptoHashable>> selectionRefs;
            selectionRefs.reserve(selections.size());
            for (const auto &selection : selections) {
                selectionRefs.push_back(ref<CryptoHashable>(*selection));
            }

            return hash_elems({object_id, sequenceOrder, electoralDistrictId, voteVariation,
                               ballotTitle, ballotSubtitle, name, numberElected, votesAllowed,
                               selectionRefs});
        }
    };

    // Lifecycle Methods

    ContestDescription::ContestDescription(
      const string &objectId, const string &electoralDistrictId, const uint64_t sequenceOrder,
      const string &voteVariation, const uint64_t numberElected, const uint64_t votesAllowed,
      const string &name, const string &ballotTitle, const string &ballotSubtitle,
      vector<unique_ptr<SelectionDescription>> selections)
        : pimpl(new Impl(objectId, electoralDistrictId, sequenceOrder, voteVariation, numberElected,
                         votesAllowed, name, ballotTitle, ballotSubtitle, move(selections)))
    {
    }
    ContestDescription::~ContestDescription() = default;

    ContestDescription &ContestDescription::operator=(ContestDescription other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    //Property Getters

    string ContestDescription::getObjectId() const { return pimpl->object_id; }
    string ContestDescription::getElectoralDistrictId() const { return pimpl->electoralDistrictId; }
    uint64_t ContestDescription::getSequenceOrder() const { return pimpl->sequenceOrder; }
    string ContestDescription::getVoteVariation() const { return pimpl->voteVariation; }
    uint64_t ContestDescription::getNumberElected() const { return pimpl->numberElected; }
    uint64_t ContestDescription::getVotesAllowed() const { return pimpl->votesAllowed; }
    string ContestDescription::getName() const { return pimpl->name; }

    string ContestDescription::getBallotTitle() const { return pimpl->ballotTitle; }
    string ContestDescription::getBallotSubtitle() const { return pimpl->ballotSubtitle; }

    vector<reference_wrapper<SelectionDescription>> ContestDescription::getSelections() const
    {
        vector<reference_wrapper<SelectionDescription>> selections;
        selections.reserve(pimpl->selections.size());
        for (const auto &selection : pimpl->selections) {
            selections.push_back(ref(*selection));
        }
        return selections;
    }

    // Interface Overrides

    unique_ptr<ElementModQ> ContestDescription::crypto_hash() { return pimpl->crypto_hash(); }

    unique_ptr<ElementModQ> ContestDescription::crypto_hash() const { return pimpl->crypto_hash(); }

#pragma endregion

#pragma region InternalElectionDescription

    struct InternalElectionDescription::Impl {
        unique_ptr<ElementModQ> descriptionHash;
        vector<unique_ptr<ContestDescription>> contests;
        Impl(unique_ptr<ElementModQ> descriptionHash,
             vector<unique_ptr<ContestDescription>> contests)
            : descriptionHash(move(descriptionHash)), contests(move(contests))
        {
        }
    };

    // Lifecycle Methods

    InternalElectionDescription::InternalElectionDescription(
      const ElementModQ &descriptionHash, vector<unique_ptr<ContestDescription>> contests)
        : pimpl(new Impl(make_unique<ElementModQ>(descriptionHash), move(contests)))
    {
    }

    InternalElectionDescription::~InternalElectionDescription() = default;

    InternalElectionDescription &
    InternalElectionDescription::operator=(InternalElectionDescription other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    const ElementModQ &InternalElectionDescription::getDescriptionHash() const
    {
        return *pimpl->descriptionHash;
    }

    vector<reference_wrapper<ContestDescription>> InternalElectionDescription::getContests() const
    {
        vector<reference_wrapper<ContestDescription>> contests;
        contests.reserve(pimpl->contests.size());
        for (auto &contest : pimpl->contests) {
            contests.push_back(ref(*contest));
        }
        return contests;
    }

    // Public Methods

    vector<uint8_t> InternalElectionDescription::toBson() const
    {
        return DescriptionSerializer::toBson(*this);
    }

    string InternalElectionDescription::toJson() { return DescriptionSerializer::toJson(*this); }

    string InternalElectionDescription::toJson() const
    {
        return DescriptionSerializer::toJson(*this);
    }

    unique_ptr<InternalElectionDescription> InternalElectionDescription::fromJson(string data)
    {
        return DescriptionSerializer::fromJson(move(data));
    }

    unique_ptr<InternalElectionDescription>
    InternalElectionDescription::fromBson(vector<uint8_t> data)
    {
        return DescriptionSerializer::fromBson(move(data));
    }

#pragma endregion

#pragma region CiphertextElectionContext

    struct CiphertextElectionContext::Impl {
        uint64_t numberOfGuardians;
        uint64_t quorum;
        unique_ptr<ElementModP> elGamalPublicKey;
        unique_ptr<ElementModQ> descriptionHash;
        unique_ptr<ElementModQ> cryptoBaseHash;
        unique_ptr<ElementModQ> cryptoExtendedBaseHash;

        Impl(uint64_t numberOfGuardians, uint64_t quorum, unique_ptr<ElementModP> elGamalPublicKey,
             unique_ptr<ElementModQ> descriptionHash, unique_ptr<ElementModQ> cryptoBaseHash,
             unique_ptr<ElementModQ> cryptoExtendedBaseHash)
            : elGamalPublicKey(move(elGamalPublicKey)), descriptionHash(move(descriptionHash)),
              cryptoBaseHash(move(cryptoBaseHash)),
              cryptoExtendedBaseHash(move(cryptoExtendedBaseHash))
        {
            this->numberOfGuardians = numberOfGuardians;
            this->quorum = quorum;
        }
    };

    // Lifecycle Methods

    CiphertextElectionContext::CiphertextElectionContext(
      uint64_t numberOfGuardians, uint64_t quorum, unique_ptr<ElementModP> elGamalPublicKey,
      unique_ptr<ElementModQ> descriptionHash, unique_ptr<ElementModQ> cryptoBaseHash,
      unique_ptr<ElementModQ> cryptoExtendedBaseHash)
        : pimpl(new Impl(numberOfGuardians, quorum, move(elGamalPublicKey), move(descriptionHash),
                         move(cryptoBaseHash), move(cryptoExtendedBaseHash)))
    {
    }
    CiphertextElectionContext::~CiphertextElectionContext() = default;

    CiphertextElectionContext &CiphertextElectionContext::operator=(CiphertextElectionContext other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    uint64_t CiphertextElectionContext::getNumberOfGuardians() const
    {
        return pimpl->numberOfGuardians;
    }
    uint64_t CiphertextElectionContext::getQuorum() const { return pimpl->quorum; }
    const ElementModP *CiphertextElectionContext::getElGamalPublicKey() const
    {
        return pimpl->elGamalPublicKey.get();
    }
    const ElementModQ *CiphertextElectionContext::getDescriptionHash() const
    {
        return pimpl->descriptionHash.get();
    }
    const ElementModQ *CiphertextElectionContext::getCryptoBaseHash() const
    {
        return pimpl->cryptoBaseHash.get();
    }
    const ElementModQ *CiphertextElectionContext::getCryptoExtendedBaseHash() const
    {
        return pimpl->cryptoExtendedBaseHash.get();
    }

    // Public Methods

    vector<uint8_t> CiphertextElectionContext::toBson() const
    {
        return ContextSerializer::toBson(*this);
    }

    string CiphertextElectionContext::toJson() const { return ContextSerializer::toJson(*this); }

    unique_ptr<CiphertextElectionContext> CiphertextElectionContext::fromJson(string data)
    {
        return ContextSerializer::fromJson(move(data));
    }

    unique_ptr<CiphertextElectionContext> CiphertextElectionContext::fromBson(vector<uint8_t> data)
    {
        return ContextSerializer::fromBson(move(data));
    }

    // Public Static Methods

    unique_ptr<CiphertextElectionContext>
    CiphertextElectionContext::make(uint64_t numberOfGuardians, uint64_t quorum,
                                    unique_ptr<ElementModP> elGamalPublicKey,
                                    unique_ptr<ElementModQ> descriptionHash)
    {
        auto cryptoBaseHash = hash_elems(
          {&const_cast<ElementModP &>(P()), &const_cast<ElementModQ &>(Q()),
           &const_cast<ElementModP &>(G()), numberOfGuardians, quorum, descriptionHash.get()});

        auto cryptoExtendedBaseHash = hash_elems({cryptoBaseHash.get(), elGamalPublicKey.get()});

        return make_unique<CiphertextElectionContext>(
          numberOfGuardians, quorum, move(elGamalPublicKey), move(descriptionHash),
          move(cryptoBaseHash), move(cryptoExtendedBaseHash));
    }

    unique_ptr<CiphertextElectionContext>
    CiphertextElectionContext::make(uint64_t numberOfGuardians, uint64_t quorum,
                                    const string &elGamalPublicKeyInHex,
                                    const string &descriptionHashInHex)
    {
        auto elGamalPublicKey = ElementModP::fromHex(elGamalPublicKeyInHex);
        auto descriptionHash = ElementModQ::fromHex(descriptionHashInHex);

        return make(numberOfGuardians, quorum, move(elGamalPublicKey), move(descriptionHash));
    }

#pragma endregion

} // namespace electionguard
