#include "electionguard/election.hpp"

#include "electionguard/hash.hpp"
#include "log.hpp"

#include <cstring>

using std::string;

namespace electionguard
{

#pragma region SelectionDescription

    struct SelectionDescription::Impl : public ElectionObjectBase {
        string candidateId;
        uint64_t sequenceOrder;

        Impl(const string &objectId, const string &candidateId, uint64_t sequenceOrder)
            : candidateId(candidateId)
        {
            this->object_id = objectId;
            this->sequenceOrder = sequenceOrder;
        }
    };

    // Lifecycle Methods

    SelectionDescription::SelectionDescription(const string &objectId, const string &candidateId,
                                               uint64_t sequenceOrder)
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

    // Public Methods

    unique_ptr<ElementModQ> SelectionDescription::crypto_hash() const
    {
        return hash_elems({pimpl->object_id, pimpl->sequenceOrder, pimpl->candidateId});
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

    // Public Methods

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

    uint64_t CiphertextElectionContext::getNumberOfGuardians() const
    {
        return pimpl->numberOfGuardians;
    }
    uint64_t CiphertextElectionContext::getQuorum() const { return pimpl->quorum; }
    ElementModP *CiphertextElectionContext::getElGamalPublicKey()
    {
        return pimpl->elGamalPublicKey.get();
    }
    ElementModQ *CiphertextElectionContext::getDescriptionHash()
    {
        return pimpl->descriptionHash.get();
    }
    ElementModQ *CiphertextElectionContext::getCryptoBaseHash()
    {
        return pimpl->cryptoBaseHash.get();
    }
    ElementModQ *CiphertextElectionContext::getCryptoExtendedBaseHash()
    {
        return pimpl->cryptoExtendedBaseHash.get();
    }

#pragma endregion

} // namespace electionguard
