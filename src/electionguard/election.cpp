#include "electionguard/election.hpp"

#include "electionguard/hash.hpp"
#include "log.hpp"
#include "serialize.hpp"

#include <cstring>
#include <iostream>
#include <utility>

using std::get;
using std::make_unique;
using std::map;
using std::move;
using std::out_of_range;
using std::ref;
using std::reference_wrapper;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::variant;
using std::vector;
using std::chrono::system_clock;

using DescriptionSerializer = electionguard::Serialize::ElectionDescription;
using InternalDescriptionSerializer = electionguard::Serialize::InternalElectionDescription;
using ContextSerializer = electionguard::Serialize::CiphertextelectionContext;

namespace electionguard
{

    template <typename K, typename V> K findByValue(const map<K, V> &collection, const V &value)
    {
        for (auto &element : collection) {
            if (element.second == value) {
                return element.first;
            }
        }
        throw out_of_range("value not found");
    }

#pragma region ElectionType

    template <typename> struct _election_type {
        static const map<ElectionType, const char *> _map;
    };
    template <typename T>
    const map<ElectionType, const char *> _election_type<T>::_map = {
      {ElectionType::unknown, "unknown"},
      {ElectionType::general, "general"},
      {ElectionType::partisanPrimaryClosed, "partisanPrimaryClosed"},
      {ElectionType::partisanPrimaryOpen, "partisanPrimaryOpen"},
      {ElectionType::primary, "primary"},
      {ElectionType::runoff, "runoff"},
      {ElectionType::special, "special"},
      {ElectionType::other, "other"},
    };

    EG_API const char *getElectionTypeString(const ElectionType &value)
    {
        return _election_type<ElectionType>::_map.find(value)->second;
    }

    EG_API ElectionType getElectionType(const string &value)
    {
        try {
            auto item = findByValue(_election_type<ElectionType>::_map, value.c_str());
            return item;
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            return ElectionType::unknown;
        }
    }

#pragma endregion

#pragma region ReportingUnitType

    template <typename> struct _reporting_unit_type {
        static const map<ReportingUnitType, const char *> _map;
    };
    template <typename T>
    const map<ReportingUnitType, const char *> _reporting_unit_type<T>::_map = {
      {ReportingUnitType::unknown, "unknown"},
      {ReportingUnitType::ballotBatch, "ballotBatch"},
      {ReportingUnitType::ballotStyleArea, "ballotStyleArea"},
      {ReportingUnitType::borough, "borough"},
      {ReportingUnitType::city, "city"},
      {ReportingUnitType::cityCouncil, "cityCouncil"},
      {ReportingUnitType::combinedPrecinct, "combinedPrecinct"},
      {ReportingUnitType::congressional, "congressional"},
      {ReportingUnitType::country, "country"},
      {ReportingUnitType::county, "county"},
      {ReportingUnitType::countyCouncil, "countyCouncil"},
      {ReportingUnitType::dropBox, "dropBox"},
      {ReportingUnitType::judicial, "judicial"},
      {ReportingUnitType::municipality, "municipality"},
      {ReportingUnitType::polling_place, "polling_place"},
      {ReportingUnitType::precinct, "precinct"},
      {ReportingUnitType::school, "school"},
      {ReportingUnitType::special, "special"},
      {ReportingUnitType::splitPrecinct, "splitPrecinct"},
      {ReportingUnitType::state, "state"},
      {ReportingUnitType::stateHouse, "stateHouse"},
      {ReportingUnitType::stateSenate, "stateSenate"},
      {ReportingUnitType::township, "township"},
      {ReportingUnitType::utility, "utility"},
      {ReportingUnitType::village, "village"},
      {ReportingUnitType::voteCenter, "voteCenter"},
      {ReportingUnitType::ward, "ward"},
      {ReportingUnitType::water, "water"},
      {ReportingUnitType::other, "other"},
    };

    EG_API const char *getReportingUnitTypeString(const ReportingUnitType &value)
    {
        return _reporting_unit_type<ReportingUnitType>::_map.find(value)->second;
    }

    EG_API ReportingUnitType getReportingUnitType(const string &value)
    {
        try {
            auto item = findByValue(_reporting_unit_type<ReportingUnitType>::_map, value.c_str());
            return item;
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            return ReportingUnitType::unknown;
        }
    }

#pragma endregion

#pragma region VoteVariationType

    template <typename> struct _vote_variation_type {
        static const map<VoteVariationType, const char *> _map;
    };
    template <typename T>
    const map<VoteVariationType, const char *> _vote_variation_type<T>::_map = {
      {VoteVariationType::unknown, "unknown"},
      {VoteVariationType::one_of_m, "one_of_m"},
      {VoteVariationType::borda, "borda"},
      {VoteVariationType::cumulative, "cumulative"},
      {VoteVariationType::majority, "majority"},
      {VoteVariationType::n_of_m, "n_of_m"},
      {VoteVariationType::plurality, "plurality"},
      {VoteVariationType::proportional, "proportional"},
      {VoteVariationType::range, "range"},
      {VoteVariationType::rcv, "rcv"},
      {VoteVariationType::super_majority, "super_majority"},
      {VoteVariationType::other, "other"},
    };

    EG_API const char *getVoteVariationTypeString(const VoteVariationType &value)
    {
        return _vote_variation_type<VoteVariationType>::_map.find(value)->second;
    }

    EG_API VoteVariationType getVoteVariationType(const string &value)
    {
        try {
            auto item = findByValue(_vote_variation_type<VoteVariationType>::_map, value.c_str());
            return item;
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            return VoteVariationType::unknown;
        }
    }

#pragma endregion

#pragma region AnnotatedString

    struct AnnotatedString::Impl {
        string annotation;
        string value;

        explicit Impl(string annotation, string value)
            : annotation(move(annotation)), value(move(value))
        {
        }

        unique_ptr<AnnotatedString::Impl> clone() const
        {
            // TODO: verify string copy
            return make_unique<AnnotatedString::Impl>(*this);
        }

        unique_ptr<ElementModQ> crypto_hash() const { return hash_elems({annotation, value}); }
    };

    // Lifecycle Methods

    AnnotatedString::AnnotatedString(const AnnotatedString &other) : pimpl(other.pimpl->clone()) {}

    AnnotatedString::AnnotatedString(string annotation, string value)
        : pimpl(new Impl(annotation, value))
    {
    }

    AnnotatedString::~AnnotatedString() = default;

    AnnotatedString &AnnotatedString::operator=(const AnnotatedString &other)
    {
        pimpl = other.pimpl->clone();
        return *this;
    }

    AnnotatedString &AnnotatedString::operator=(AnnotatedString &&other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string AnnotatedString::getAnnotation() const { return pimpl->annotation; }
    string AnnotatedString::getValue() const { return pimpl->value; }

    // Interface Overrides

    unique_ptr<ElementModQ> AnnotatedString::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> AnnotatedString::crypto_hash() const { return pimpl->crypto_hash(); }

#pragma endregion

#pragma region Language

    struct Language::Impl {
        string value;
        string language;

        explicit Impl(string value, string language) : value(move(value)), language(move(language))
        {
        }

        unique_ptr<Language::Impl> clone() const
        {
            // TODO: verify string copy
            return make_unique<Language::Impl>(*this);
        }

        unique_ptr<ElementModQ> crypto_hash() const { return hash_elems({value, language}); }
    };

    // Lifecycle Methods

    Language::Language(const Language &other) : pimpl(other.pimpl->clone()) {}

    Language::Language(string value, string language) : pimpl(new Impl(value, language)) {}

    Language::~Language() = default;

    Language &Language::operator=(const Language &other)
    {
        pimpl = other.pimpl->clone();
        return *this;
    }

    Language &Language::operator=(Language &&other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string Language::getValue() const { return pimpl->value; }
    string Language::getLanguage() const { return pimpl->language; }

    // Interface Overrides

    unique_ptr<ElementModQ> Language::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> Language::crypto_hash() const { return pimpl->crypto_hash(); }

#pragma endregion

#pragma region InternationalizedText

    struct InternationalizedText::Impl {
        vector<unique_ptr<Language>> text;

        explicit Impl(vector<unique_ptr<Language>> text) : text(move(text)) {}

        unique_ptr<InternationalizedText::Impl> clone() const
        {
            vector<unique_ptr<Language>> _text;
            _text.reserve(text.size());
            for (const auto &element : text) {
                _text.push_back(make_unique<Language>(*element.get()));
            }

            return make_unique<InternationalizedText::Impl>(move(_text));
        }

        unique_ptr<ElementModQ> crypto_hash() const
        {
            vector<reference_wrapper<CryptoHashable>> refs;
            refs.reserve(this->text.size());
            for (const auto &i : this->text) {
                refs.push_back(ref<CryptoHashable>(*i));
            }

            return hash_elems(refs);
        }
    };

    // Lifecycle Methods

    InternationalizedText::InternationalizedText(const InternationalizedText &other)
        : pimpl(other.pimpl->clone())
    {
    }

    InternationalizedText::InternationalizedText(vector<unique_ptr<Language>> text)
        : pimpl(new Impl(move(text)))
    {
    }

    InternationalizedText::~InternationalizedText() = default;

    InternationalizedText &InternationalizedText::operator=(InternationalizedText other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    vector<reference_wrapper<Language>> InternationalizedText::getText() const
    {
        vector<reference_wrapper<Language>> refs;
        refs.reserve(pimpl->text.size());
        for (const auto &i : pimpl->text) {
            refs.push_back(ref(*i));
        }
        return refs;
    }

    // Interface Overrides

    unique_ptr<ElementModQ> InternationalizedText::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> InternationalizedText::crypto_hash() const
    {
        return pimpl->crypto_hash();
    }

#pragma endregion

#pragma region ContactInformation

    struct ContactInformation::Impl {
        vector<string> addressLine;
        vector<unique_ptr<AnnotatedString>> email;
        vector<unique_ptr<AnnotatedString>> phone;
        string name;

        explicit Impl(string name) : name(move(name)) {}

        explicit Impl(vector<string> addressLine, string name)
            : addressLine(move(addressLine)), name(move(name))
        {
        }

        explicit Impl(vector<string> addressLine, vector<unique_ptr<AnnotatedString>> phone,
                      string name)
            : addressLine(move(addressLine)), phone(move(phone)), name(move(name))
        {
        }

        explicit Impl(vector<string> addressLine, vector<unique_ptr<AnnotatedString>> email,
                      vector<unique_ptr<AnnotatedString>> phone, string name)
            : addressLine(move(addressLine)), email(move(email)), phone(move(phone)),
              name(move(name))
        {
        }

        unique_ptr<ContactInformation::Impl> clone() const
        {
            vector<string> _addressLine;
            _addressLine.reserve(addressLine.size());
            for (const auto &element : addressLine) {
                _addressLine.push_back(element);
            }

            vector<unique_ptr<AnnotatedString>> _email;
            _email.reserve(email.size());
            for (const auto &element : email) {
                _email.push_back(make_unique<AnnotatedString>(*element.get()));
            }

            vector<unique_ptr<AnnotatedString>> _phone;
            _phone.reserve(phone.size());
            for (const auto &element : phone) {
                _phone.push_back(make_unique<AnnotatedString>(*element.get()));
            }

            return make_unique<ContactInformation::Impl>(move(_addressLine), move(_email),
                                                         move(_phone), name);
        }

        unique_ptr<ElementModQ> crypto_hash() const
        {
            vector<reference_wrapper<CryptoHashable>> emailRefs;
            emailRefs.reserve(this->email.size());
            for (const auto &i : this->email) {
                emailRefs.push_back(ref<CryptoHashable>(*i));
            }

            vector<reference_wrapper<CryptoHashable>> phoneRefs;
            phoneRefs.reserve(this->phone.size());
            for (const auto &i : this->phone) {
                phoneRefs.push_back(ref<CryptoHashable>(*i));
            }
            return hash_elems({name, addressLine, emailRefs, phoneRefs});
        }
    };

    // Lifecycle Methods

    ContactInformation::ContactInformation(const ContactInformation &other)
        : pimpl(other.pimpl->clone())
    {
    }

    ContactInformation::ContactInformation(string name) : pimpl(new Impl(move(name))) {}

    ContactInformation::ContactInformation(vector<string> addressLine, string name)
        : pimpl(new Impl(move(addressLine), move(name)))
    {
    }

    ContactInformation::ContactInformation(vector<string> addressLine,
                                           vector<unique_ptr<AnnotatedString>> phone, string name)
        : pimpl(new Impl(move(addressLine), move(phone), move(name)))
    {
    }

    ContactInformation::ContactInformation(vector<string> addressLine,
                                           vector<unique_ptr<AnnotatedString>> email,
                                           vector<unique_ptr<AnnotatedString>> phone, string name)
        : pimpl(new Impl(move(addressLine), move(email), move(phone), move(name)))
    {
    }

    ContactInformation::~ContactInformation() = default;

    ContactInformation &ContactInformation::operator=(ContactInformation other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    vector<string> ContactInformation::getAddressLine() const { return pimpl->addressLine; }

    vector<reference_wrapper<AnnotatedString>> ContactInformation::getEmail() const
    {
        vector<reference_wrapper<AnnotatedString>> refs;
        refs.reserve(pimpl->email.size());
        for (const auto &i : pimpl->email) {
            refs.push_back(ref(*i));
        }
        return refs;
    }

    vector<reference_wrapper<AnnotatedString>> ContactInformation::getPhone() const
    {
        vector<reference_wrapper<AnnotatedString>> refs;
        refs.reserve(pimpl->phone.size());
        for (const auto &i : pimpl->phone) {
            refs.push_back(ref(*i));
        }
        return refs;
    }

    // Property Getters

    string ContactInformation::getName() const { return pimpl->name; }

    // Interface Overrides

    unique_ptr<ElementModQ> ContactInformation::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> ContactInformation::crypto_hash() const { return pimpl->crypto_hash(); }

#pragma endregion

#pragma region GeopoliticalUnit

    struct GeopoliticalUnit::Impl : public ElectionObjectBase {
        string name;
        ReportingUnitType type;
        unique_ptr<ContactInformation> contactInformation;

        explicit Impl(const string &objectId, string name, const ReportingUnitType type,
                      unique_ptr<ContactInformation> contactInformation)
            : name(move(name)), contactInformation(move(contactInformation))
        {
            this->object_id = objectId;
            this->type = type;
        }

        explicit Impl(const string &objectId, string name, const ReportingUnitType type)
            : name(move(name))
        {
            this->object_id = objectId;
            this->type = type;
        }

        unique_ptr<GeopoliticalUnit::Impl> clone() const
        {
            if (contactInformation != nullptr) {
                auto _contactInfo = make_unique<ContactInformation>(*contactInformation.get());
                return make_unique<GeopoliticalUnit::Impl>(object_id, name, type,
                                                           move(_contactInfo));
            }

            return make_unique<GeopoliticalUnit::Impl>(object_id, name, type);
        }

        unique_ptr<ElementModQ> crypto_hash() const
        {
            if (contactInformation != nullptr) {
                return hash_elems(
                  {object_id, name, getReportingUnitTypeString(type), contactInformation.get()});
            }

            return hash_elems({object_id, name, getReportingUnitTypeString(type)});
        }
    };

    // Lifecycle Methods

    GeopoliticalUnit::GeopoliticalUnit(const GeopoliticalUnit &other) : pimpl(other.pimpl->clone())
    {
    }

    GeopoliticalUnit::GeopoliticalUnit(const string &objectId, const string name,
                                       const ReportingUnitType type)
        : pimpl(new Impl(objectId, move(name), type))
    {
    }

    GeopoliticalUnit::GeopoliticalUnit(const string &objectId, const string name,
                                       const ReportingUnitType type,
                                       unique_ptr<ContactInformation> contactInformation)
        : pimpl(new Impl(objectId, move(name), type, move(contactInformation)))
    {
    }

    GeopoliticalUnit::~GeopoliticalUnit() = default;

    GeopoliticalUnit &GeopoliticalUnit::operator=(GeopoliticalUnit other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string GeopoliticalUnit::getObjectId() const { return pimpl->object_id; }
    string GeopoliticalUnit::getName() const { return pimpl->name; }
    ReportingUnitType GeopoliticalUnit::getType() const { return pimpl->type; }
    ContactInformation *GeopoliticalUnit::getContactInformation() const
    {
        return pimpl->contactInformation.get();
    }

    // Interface Overrides

    unique_ptr<ElementModQ> GeopoliticalUnit::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> GeopoliticalUnit::crypto_hash() const { return pimpl->crypto_hash(); }

#pragma endregion

#pragma region BallotStyle

    struct BallotStyle::Impl : public ElectionObjectBase {
        vector<string> geopoliticalUnitIds;
        vector<string> partyIds;
        string imageUri;

        explicit Impl(const string &objectId, vector<string> geopoliticalUnitIds)
            : geopoliticalUnitIds(move(geopoliticalUnitIds))
        {
            this->object_id = objectId;
        }

        explicit Impl(const string &objectId, vector<string> geopoliticalUnitIds,
                      vector<string> partyIds, const string imageUri)
            : geopoliticalUnitIds(move(geopoliticalUnitIds)), partyIds(move(partyIds)),
              imageUri(move(imageUri))
        {
            this->object_id = objectId;
        }

        unique_ptr<BallotStyle::Impl> clone() const
        {
            vector<string> _gpUnitIds;
            _gpUnitIds.reserve(geopoliticalUnitIds.size());
            for (const auto &element : geopoliticalUnitIds) {
                _gpUnitIds.push_back(element);
            }

            vector<string> _partyIds;
            _partyIds.reserve(partyIds.size());
            for (const auto &element : partyIds) {
                _partyIds.push_back(element);
            }

            return make_unique<BallotStyle::Impl>(object_id, _gpUnitIds, _partyIds, imageUri);
        }

        unique_ptr<ElementModQ> crypto_hash() const
        {
            return hash_elems({object_id, geopoliticalUnitIds, geopoliticalUnitIds, imageUri});
        }
    };

    // Lifecycle Methods

    BallotStyle::BallotStyle(const BallotStyle &other) : pimpl(other.pimpl->clone()) {}

    BallotStyle::BallotStyle(const string &objectId, vector<string> geopoliticalUnitIds)
        : pimpl(new Impl(objectId, move(geopoliticalUnitIds)))
    {
    }

    BallotStyle::BallotStyle(const string &objectId, vector<string> geopoliticalUnitIds,
                             vector<string> partyIds, const string imageUri)
        : pimpl(new Impl(objectId, move(geopoliticalUnitIds), move(partyIds), move(imageUri)))
    {
    }

    BallotStyle::~BallotStyle() = default;

    BallotStyle &BallotStyle::operator=(BallotStyle other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string BallotStyle::getObjectId() const { return pimpl->object_id; }
    vector<string> BallotStyle::getGeopoliticalUnitIds() const
    {
        return pimpl->geopoliticalUnitIds;
    }
    vector<string> BallotStyle::getPartyIds() const { return pimpl->partyIds; }
    string BallotStyle::getImageUri() const { return pimpl->imageUri; }

    // Interface Overrides

    unique_ptr<ElementModQ> BallotStyle::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> BallotStyle::crypto_hash() const { return pimpl->crypto_hash(); }

#pragma endregion

#pragma region Party

    struct Party::Impl : public ElectionObjectBase {
        unique_ptr<InternationalizedText> name;
        string abbreviation;
        string color;
        string logoUri;

        explicit Impl(const string &objectId) { this->object_id = objectId; }

        explicit Impl(const string &objectId, const string abbreviation, const string color,
                      const string logoUri)
            : abbreviation(move(abbreviation)), color(move(color)), logoUri(move(logoUri))
        {
            this->object_id = objectId;
        }

        explicit Impl(const string &objectId, unique_ptr<InternationalizedText> name,
                      const string abbreviation, const string color, const string logoUri)
            : name(move(name)), abbreviation(move(abbreviation)), color(move(color)),
              logoUri(move(logoUri))
        {
            this->object_id = objectId;
        }

        unique_ptr<Party::Impl> clone() const
        {
            if (name != nullptr) {
                auto _name = make_unique<InternationalizedText>(*name.get());
                return make_unique<Party::Impl>(object_id, move(_name), abbreviation, color,
                                                logoUri);
            }
            return make_unique<Party::Impl>(object_id, abbreviation, color, logoUri);
        }

        unique_ptr<ElementModQ> crypto_hash() const
        {
            if (name != nullptr) {
                return hash_elems({object_id, name.get(), abbreviation, color, logoUri});
            }
            return hash_elems({object_id, abbreviation, color, logoUri});
        }
    };

    // Lifecycle Methods

    Party::Party(const Party &other) : pimpl(other.pimpl->clone()) {}

    Party::Party(const string &objectId) : pimpl(new Impl(objectId)) {}

    Party::Party(const string &objectId, unique_ptr<InternationalizedText> name,
                 const string abbreviation, const string color, const string logoUri)
        : pimpl(new Impl(objectId, move(name), move(abbreviation), move(color), move(logoUri)))
    {
    }

    Party::~Party() = default;

    Party &Party::operator=(Party other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string Party::getObjectId() const { return pimpl->object_id; }
    InternationalizedText *Party::getName() const { return pimpl->name.get(); }
    string Party::getAbbreviation() const { return pimpl->abbreviation; }
    string Party::getColor() const { return pimpl->color; }
    string Party::getLogoUri() const { return pimpl->logoUri; }

    // Interface Overrides

    unique_ptr<ElementModQ> Party::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> Party::crypto_hash() const { return pimpl->crypto_hash(); }

#pragma endregion

#pragma region Candidate

    struct Candidate::Impl : public ElectionObjectBase {
        unique_ptr<InternationalizedText> name;
        string partyId;
        string imageUri;
        bool isWriteIn;

        explicit Impl(const string &objectId, bool isWriteIn)
        {
            this->object_id = objectId;
            this->isWriteIn = isWriteIn;
        }

        explicit Impl(const string &objectId, const string &partyId, const string imageUri,
                      bool isWriteIn)
            : imageUri(move(imageUri))
        {
            this->object_id = objectId;
            this->partyId = partyId;
            this->isWriteIn = isWriteIn;
        }

        explicit Impl(const string &objectId, unique_ptr<InternationalizedText> name,
                      const string &partyId, const string imageUri, bool isWriteIn)
            : name(move(name)), imageUri(move(imageUri))
        {
            this->object_id = objectId;
            this->partyId = partyId;
            this->isWriteIn = isWriteIn;
        }

        unique_ptr<Candidate::Impl> clone() const
        {
            if (name != nullptr) {
                auto _name = make_unique<InternationalizedText>(*name.get());
                return make_unique<Candidate::Impl>(object_id, move(_name), partyId, imageUri,
                                                    isWriteIn);
            }

            return make_unique<Candidate::Impl>(object_id, partyId, imageUri, isWriteIn);
        }

        unique_ptr<ElementModQ> crypto_hash() const
        {
            if (name != nullptr) {
                return hash_elems({object_id, name.get(), partyId, imageUri});
            }
            return hash_elems({object_id, name.get(), partyId, imageUri});
        }
    };

    // Lifecycle Methods

    Candidate::Candidate(const Candidate &other) : pimpl(other.pimpl->clone()) {}

    Candidate::Candidate(const string &objectId, bool isWriteIn)
        : pimpl(new Impl(objectId, isWriteIn))
    {
    }

    Candidate::Candidate(const string &objectId, unique_ptr<InternationalizedText> name,
                         const string &partyId, const string imageUri, bool isWriteIn)
        : pimpl(new Impl(objectId, move(name), partyId, move(imageUri), isWriteIn))
    {
    }

    Candidate::~Candidate() = default;

    Candidate &Candidate::operator=(Candidate other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string Candidate::getObjectId() const { return pimpl->object_id; }
    string Candidate::getCandidateId() const { return pimpl->object_id; }
    InternationalizedText *Candidate::getName() const { return pimpl->name.get(); }
    string Candidate::getPartyId() const { return pimpl->partyId; }
    string Candidate::getImageUri() const { return pimpl->imageUri; }
    bool Candidate::isWriteIn() const { return pimpl->isWriteIn; }

    // Interface Overrides

    unique_ptr<ElementModQ> Candidate::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> Candidate::crypto_hash() const { return pimpl->crypto_hash(); }

#pragma endregion

#pragma region SelectionDescription

    struct SelectionDescription::Impl : public ElectionObjectBase {
        string candidateId;
        uint64_t sequenceOrder;

        Impl(const string &objectId, const string &candidateId, const uint64_t sequenceOrder)
        {
            this->object_id = objectId;
            this->candidateId = candidateId;
            this->sequenceOrder = sequenceOrder;
        }

        unique_ptr<SelectionDescription::Impl> clone() const
        {
            return make_unique<SelectionDescription::Impl>(object_id, candidateId, sequenceOrder);
        }

        unique_ptr<ElementModQ> crypto_hash() const
        {
            return hash_elems({object_id, sequenceOrder, candidateId});
        }
    };

    // Lifecycle Methods

    SelectionDescription::SelectionDescription(const SelectionDescription &other)
        : pimpl(other.pimpl->clone())
    {
    }

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
        VoteVariationType voteVariation;
        uint64_t numberElected;
        uint64_t votesAllowed;
        string name;
        unique_ptr<InternationalizedText> ballotTitle;
        unique_ptr<InternationalizedText> ballotSubtitle;
        vector<unique_ptr<SelectionDescription>> selections;

        Impl(const string &objectId, const string &electoralDistrictId,
             const uint64_t sequenceOrder, const VoteVariationType voteVariation,
             const uint64_t numberElected, const string name,
             vector<unique_ptr<SelectionDescription>> selections)
            : name(move(name)), selections(move(selections))
        {
            this->object_id = objectId;
            this->electoralDistrictId = electoralDistrictId;
            this->sequenceOrder = sequenceOrder;
            this->voteVariation = voteVariation;
            this->numberElected = numberElected;
        }

        Impl(const string &objectId, const string &electoralDistrictId,
             const uint64_t sequenceOrder, const VoteVariationType voteVariation,
             const uint64_t numberElected, const uint64_t votesAllowed, const string name,
             unique_ptr<InternationalizedText> ballotTitle,
             unique_ptr<InternationalizedText> ballotSubtitle,
             vector<unique_ptr<SelectionDescription>> selections)
            : name(move(name)), ballotTitle(move(ballotTitle)),
              ballotSubtitle(move(ballotSubtitle)), selections(move(selections))
        {
            this->object_id = objectId;
            this->electoralDistrictId = electoralDistrictId;
            this->sequenceOrder = sequenceOrder;
            this->voteVariation = voteVariation;
            this->numberElected = numberElected;
            this->votesAllowed = votesAllowed;
        }

        unique_ptr<ContestDescription::Impl> clone() const
        {
            vector<unique_ptr<SelectionDescription>> _selections;
            _selections.reserve(selections.size());
            for (const auto &element : selections) {
                _selections.push_back(make_unique<SelectionDescription>(*element.get()));
            }

            // TODO: handle optional values
            if (ballotTitle != nullptr) {
                auto _ballotTitle = make_unique<InternationalizedText>(*ballotTitle.get());
                auto _ballotSubtitle = make_unique<InternationalizedText>(*ballotSubtitle.get());
                return make_unique<ContestDescription::Impl>(
                  object_id, electoralDistrictId, sequenceOrder, voteVariation, numberElected,
                  votesAllowed, name, move(_ballotTitle), move(_ballotSubtitle), move(_selections));
            }
            return make_unique<ContestDescription::Impl>(object_id, electoralDistrictId,
                                                         sequenceOrder, voteVariation,
                                                         numberElected, name, move(_selections));
        }

        unique_ptr<ElementModQ> crypto_hash() const
        {
            vector<reference_wrapper<CryptoHashable>> selectionRefs;
            selectionRefs.reserve(selections.size());
            for (const auto &selection : selections) {
                selectionRefs.push_back(ref<CryptoHashable>(*selection));
            }

            // TODO: handle optional values
            if (ballotTitle != nullptr) {
                return hash_elems({object_id, sequenceOrder, electoralDistrictId,
                                   getVoteVariationTypeString(voteVariation),
                                   ref<CryptoHashable>(*ballotTitle),
                                   ref<CryptoHashable>(*ballotSubtitle), name, numberElected,
                                   votesAllowed, selectionRefs});
            }
            return hash_elems({object_id, sequenceOrder, electoralDistrictId,
                               getVoteVariationTypeString(voteVariation), name, numberElected,
                               votesAllowed, selectionRefs});
        }
    };

    // Lifecycle Methods

    ContestDescription::ContestDescription(const ContestDescription &other)
        : pimpl(other.pimpl->clone())
    {
    }

    ContestDescription::ContestDescription(const string &objectId,
                                           const string &electoralDistrictId,
                                           const uint64_t sequenceOrder,
                                           const VoteVariationType voteVariation,
                                           const uint64_t numberElected, const string name,
                                           vector<unique_ptr<SelectionDescription>> selections)
        : pimpl(new Impl(objectId, electoralDistrictId, sequenceOrder, voteVariation, numberElected,
                         move(name), move(selections)))
    {
    }

    ContestDescription::ContestDescription(
      const string &objectId, const string &electoralDistrictId, const uint64_t sequenceOrder,
      const VoteVariationType voteVariation, const uint64_t numberElected,
      const uint64_t votesAllowed, const string name, unique_ptr<InternationalizedText> ballotTitle,
      unique_ptr<InternationalizedText> ballotSubtitle,
      vector<unique_ptr<SelectionDescription>> selections)
        : pimpl(new Impl(objectId, electoralDistrictId, sequenceOrder, voteVariation, numberElected,
                         votesAllowed, move(name), move(ballotTitle), move(ballotSubtitle),
                         move(selections)))
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
    VoteVariationType ContestDescription::getVoteVariation() const { return pimpl->voteVariation; }
    uint64_t ContestDescription::getNumberElected() const { return pimpl->numberElected; }
    uint64_t ContestDescription::getVotesAllowed() const { return pimpl->votesAllowed; }
    string ContestDescription::getName() const { return pimpl->name; }

    InternationalizedText *ContestDescription::getBallotTitle() const
    {
        return pimpl->ballotTitle.get();
    }
    InternationalizedText *ContestDescription::getBallotSubtitle() const
    {
        return pimpl->ballotSubtitle.get();
    }

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

#pragma region ContestDescriptionWithPlaceholders

    struct ContestDescriptionWithPlaceholders::Impl : ElectionObjectBase {
        vector<unique_ptr<SelectionDescription>> placeholderSelections;

        Impl(vector<unique_ptr<SelectionDescription>> placeholderSelections)
            : placeholderSelections(move(placeholderSelections))
        {
        }
    };

    // Lifecycle Methods

    ContestDescriptionWithPlaceholders::ContestDescriptionWithPlaceholders(
      const ContestDescription &other,
      vector<unique_ptr<SelectionDescription>> placeholderSelections)
        : ContestDescription(other), pimpl(new Impl(move(placeholderSelections)))
    {
    }

    ContestDescriptionWithPlaceholders::ContestDescriptionWithPlaceholders(
      const string &objectId, const string &electoralDistrictId, const uint64_t sequenceOrder,
      const VoteVariationType voteVariation, const uint64_t numberElected, const string name,
      vector<unique_ptr<SelectionDescription>> selections,
      vector<unique_ptr<SelectionDescription>> placeholderSelections)
        : ContestDescription(objectId, electoralDistrictId, sequenceOrder, voteVariation,
                             numberElected, move(name), move(selections)),
          pimpl(new Impl(move(placeholderSelections)))
    {
    }

    ContestDescriptionWithPlaceholders::ContestDescriptionWithPlaceholders(
      const string &objectId, const string &electoralDistrictId, const uint64_t sequenceOrder,
      const VoteVariationType voteVariation, const uint64_t numberElected,
      const uint64_t votesAllowed, const string name, unique_ptr<InternationalizedText> ballotTitle,
      unique_ptr<InternationalizedText> ballotSubtitle,
      vector<unique_ptr<SelectionDescription>> selections,
      vector<unique_ptr<SelectionDescription>> placeholderSelections)
        : ContestDescription(objectId, electoralDistrictId, sequenceOrder, voteVariation,
                             numberElected, votesAllowed, move(name), move(ballotTitle),
                             move(ballotSubtitle), move(selections)),
          pimpl(new Impl(move(placeholderSelections)))
    {
    }

    ContestDescriptionWithPlaceholders::~ContestDescriptionWithPlaceholders() = default;

    ContestDescriptionWithPlaceholders &
    ContestDescriptionWithPlaceholders::operator=(ContestDescriptionWithPlaceholders other)
    {
        // TODO: swap the base as well via cast?
        //(ContestDescription) this = (ContestDescription)other;
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    vector<reference_wrapper<SelectionDescription>>
    ContestDescriptionWithPlaceholders::getPlaceholders() const
    {
        vector<reference_wrapper<SelectionDescription>> placeholders;
        placeholders.reserve(pimpl->placeholderSelections.size());
        for (const auto &selection : pimpl->placeholderSelections) {
            placeholders.push_back(ref(*selection));
        }
        return placeholders;
    }

    // Public Methods

    bool ContestDescriptionWithPlaceholders::IsPlaceholder(SelectionDescription &selection) const
    {
        for (const auto &placeholder : pimpl->placeholderSelections) {
            if (selection.getObjectId() == placeholder.get()->getObjectId()) {
                return true;
            }
        }
        return false;
    }

    reference_wrapper<SelectionDescription>
    ContestDescriptionWithPlaceholders::selectionFor(string &selectionId)
    {
        for (const auto &selection : this->getSelections()) {
            if (selectionId == selection.get().getObjectId()) {
                return selection.get();
            }
        }

        for (const auto &placeholder : this->getPlaceholders()) {
            if (selectionId == placeholder.get().getObjectId()) {
                return placeholder.get();
            }
        }

        throw out_of_range("could not find the item");
    }

#pragma endregion

#pragma region ElectionDescription

    struct ElectionDescription::Impl {
        string electionScopeId;
        ElectionType type;
        system_clock::time_point startDate;
        system_clock::time_point endDate;
        vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits;
        vector<unique_ptr<Party>> parties;
        vector<unique_ptr<Candidate>> candidates;
        vector<unique_ptr<ContestDescription>> contests;
        vector<unique_ptr<BallotStyle>> ballotStyles;
        unique_ptr<InternationalizedText> name;
        unique_ptr<ContactInformation> contactInformation;

        Impl(string electionScopeId, ElectionType type, system_clock::time_point startDate,
             system_clock::time_point endDate,
             vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
             vector<unique_ptr<Party>> parties, vector<unique_ptr<Candidate>> candidates,
             vector<unique_ptr<ContestDescription>> contests,
             vector<unique_ptr<BallotStyle>> ballotStyles)
            : electionScopeId(move(electionScopeId)), geopoliticalUnits(move(geopoliticalUnits)),
              parties(move(parties)), candidates(move(candidates)), contests(move(contests)),
              ballotStyles(move(ballotStyles))
        {
        }

        Impl(string electionScopeId, ElectionType type, system_clock::time_point startDate,
             system_clock::time_point endDate,
             vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
             vector<unique_ptr<Party>> parties, vector<unique_ptr<Candidate>> candidates,
             vector<unique_ptr<ContestDescription>> contests,
             vector<unique_ptr<BallotStyle>> ballotStyles, unique_ptr<InternationalizedText> name,
             unique_ptr<ContactInformation> contactInformation)
            : electionScopeId(move(electionScopeId)), geopoliticalUnits(move(geopoliticalUnits)),
              parties(move(parties)), candidates(move(candidates)), contests(move(contests)),
              ballotStyles(move(ballotStyles)), name(move(name)),
              contactInformation(move(contactInformation))
        {
        }

        unique_ptr<ElementModQ> crypto_hash() const
        {
            vector<reference_wrapper<CryptoHashable>> geopoliticalUnitRefs;
            geopoliticalUnitRefs.reserve(geopoliticalUnits.size());
            for (const auto &element : geopoliticalUnits) {
                geopoliticalUnitRefs.push_back(ref<CryptoHashable>(*element));
            }

            vector<reference_wrapper<CryptoHashable>> partyRefs;
            partyRefs.reserve(parties.size());
            for (const auto &element : parties) {
                partyRefs.push_back(ref<CryptoHashable>(*element));
            }

            vector<reference_wrapper<CryptoHashable>> contestRefs;
            contestRefs.reserve(contests.size());
            for (const auto &element : contests) {
                contestRefs.push_back(ref<CryptoHashable>(*element));
            }

            vector<reference_wrapper<CryptoHashable>> ballotStyleRefs;
            ballotStyleRefs.reserve(ballotStyles.size());
            for (const auto &element : ballotStyles) {
                ballotStyleRefs.push_back(ref<CryptoHashable>(*element));
            }

            // TODO: handle optional cases
            if (name == nullptr) {
                return hash_elems({electionScopeId, getElectionTypeString(type),
                                   timePointToIsoString(startDate, "%FT%TZ"),
                                   timePointToIsoString(endDate, "%FT%TZ"), geopoliticalUnitRefs,
                                   partyRefs, contestRefs, ballotStyleRefs});
            }

            // TODO: verify hash time format is iso string
            return hash_elems({electionScopeId, getElectionTypeString(type),
                               timePointToIsoString(startDate, "%FT%TZ"),
                               timePointToIsoString(endDate, "%FT%TZ"), ref<CryptoHashable>(*name),
                               ref<CryptoHashable>(*contactInformation), geopoliticalUnitRefs,
                               partyRefs, contestRefs, ballotStyleRefs});
        }
    };

    // Lifecycle Methods

    ElectionDescription::ElectionDescription(
      string electionScopeId, ElectionType type, system_clock::time_point startDate,
      system_clock::time_point endDate, vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
      vector<unique_ptr<Party>> parties, vector<unique_ptr<Candidate>> candidates,
      vector<unique_ptr<ContestDescription>> contests, vector<unique_ptr<BallotStyle>> ballotStyles)
        : pimpl(new Impl(move(electionScopeId), type, startDate, endDate, move(geopoliticalUnits),
                         move(parties), move(candidates), move(contests), move(ballotStyles)))
    {
    }

    ElectionDescription::ElectionDescription(
      string electionScopeId, ElectionType type, system_clock::time_point startDate,
      system_clock::time_point endDate, vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
      vector<unique_ptr<Party>> parties, vector<unique_ptr<Candidate>> candidates,
      vector<unique_ptr<ContestDescription>> contests, vector<unique_ptr<BallotStyle>> ballotStyles,
      unique_ptr<InternationalizedText> name, unique_ptr<ContactInformation> contactInformation)
        : pimpl(new Impl(move(electionScopeId), type, startDate, endDate, move(geopoliticalUnits),
                         move(parties), move(candidates), move(contests), move(ballotStyles),
                         move(name), move(contactInformation)))
    {
    }

    ElectionDescription::~ElectionDescription() = default;

    ElectionDescription &ElectionDescription::operator=(ElectionDescription other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string ElectionDescription::getElectionScopeId() const { return pimpl->electionScopeId; }
    ElectionType ElectionDescription::getElectionType() const { return pimpl->type; }
    system_clock::time_point ElectionDescription::getStartDate() const { return pimpl->startDate; }
    system_clock::time_point ElectionDescription::getEndDate() const { return pimpl->endDate; }

    vector<reference_wrapper<GeopoliticalUnit>> ElectionDescription::getGeopoliticalUnits() const
    {
        vector<reference_wrapper<GeopoliticalUnit>> references;
        references.reserve(pimpl->geopoliticalUnits.size());
        for (auto &reference : pimpl->geopoliticalUnits) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    vector<reference_wrapper<Party>> ElectionDescription::getParties() const
    {
        vector<reference_wrapper<Party>> references;
        references.reserve(pimpl->parties.size());
        for (auto &reference : pimpl->parties) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    vector<reference_wrapper<Candidate>> ElectionDescription::getCandidates() const
    {
        vector<reference_wrapper<Candidate>> references;
        references.reserve(pimpl->candidates.size());
        for (auto &reference : pimpl->candidates) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    vector<reference_wrapper<ContestDescription>> ElectionDescription::getContests() const
    {
        vector<reference_wrapper<ContestDescription>> references;
        references.reserve(pimpl->contests.size());
        for (auto &reference : pimpl->contests) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    vector<reference_wrapper<BallotStyle>> ElectionDescription::getBallotStyles() const
    {
        vector<reference_wrapper<BallotStyle>> references;
        references.reserve(pimpl->ballotStyles.size());
        for (auto &reference : pimpl->ballotStyles) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    InternationalizedText *ElectionDescription::getName() const { return pimpl->name.get(); }
    ContactInformation *ElectionDescription::getContactInformation() const
    {
        return pimpl->contactInformation.get();
    }

    // Public Methods

    vector<uint8_t> ElectionDescription::toBson() const
    {
        return DescriptionSerializer::toBson(*this);
    }

    string ElectionDescription::toJson() { return DescriptionSerializer::toJson(*this); }

    string ElectionDescription::toJson() const { return DescriptionSerializer::toJson(*this); }

    unique_ptr<ElectionDescription> ElectionDescription::fromJson(string data)
    {
        return DescriptionSerializer::fromJson(move(data));
    }

    unique_ptr<ElectionDescription> ElectionDescription::fromBson(vector<uint8_t> data)
    {
        return DescriptionSerializer::fromBson(move(data));
    }

    unique_ptr<ElementModQ> ElectionDescription::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> ElectionDescription::crypto_hash() const
    {
        return pimpl->crypto_hash();
    }

#pragma endregion

#pragma region InternalElectionDescription

    struct InternalElectionDescription::Impl {
        vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits;
        vector<unique_ptr<ContestDescriptionWithPlaceholders>> contests;
        vector<unique_ptr<BallotStyle>> ballotStyles;
        unique_ptr<ElementModQ> descriptionHash;

        Impl(vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
             vector<unique_ptr<ContestDescriptionWithPlaceholders>> contests,
             vector<unique_ptr<BallotStyle>> ballotStyles, unique_ptr<ElementModQ> descriptionHash)
            : geopoliticalUnits(move(geopoliticalUnits)), contests(move(contests)),
              ballotStyles(move(ballotStyles)), descriptionHash(move(descriptionHash))
        {
        }
    };

    // Lifecycle Methods

    InternalElectionDescription::InternalElectionDescription(
      vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
      vector<unique_ptr<ContestDescriptionWithPlaceholders>> contests,
      vector<unique_ptr<BallotStyle>> ballotStyles, const ElementModQ &descriptionHash)
        : pimpl(new Impl(move(geopoliticalUnits), move(contests), move(ballotStyles),
                         make_unique<ElementModQ>(descriptionHash)))
    {
    }

    InternalElectionDescription::InternalElectionDescription(const ElectionDescription &description)
        : pimpl(new Impl(copyGeopoliticalUnits(description),
                         generateContestsWithPlaceholders(description),
                         copyBallotStyles(description), description.crypto_hash()))
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

    vector<reference_wrapper<GeopoliticalUnit>>
    InternalElectionDescription::getGeopoliticalUnits() const
    {
        vector<reference_wrapper<GeopoliticalUnit>> references;
        references.reserve(pimpl->geopoliticalUnits.size());
        for (auto &reference : pimpl->geopoliticalUnits) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    vector<reference_wrapper<ContestDescriptionWithPlaceholders>>
    InternalElectionDescription::getContests() const
    {
        vector<reference_wrapper<ContestDescriptionWithPlaceholders>> references;
        references.reserve(pimpl->contests.size());
        for (auto &reference : pimpl->contests) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    vector<reference_wrapper<BallotStyle>> InternalElectionDescription::getBallotStyles() const
    {
        vector<reference_wrapper<BallotStyle>> references;
        references.reserve(pimpl->ballotStyles.size());
        for (auto &reference : pimpl->ballotStyles) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    // Public Methods

    vector<uint8_t> InternalElectionDescription::toBson() const
    {
        return InternalDescriptionSerializer::toBson(*this);
    }

    string InternalElectionDescription::toJson()
    {
        return InternalDescriptionSerializer::toJson(*this);
    }

    string InternalElectionDescription::toJson() const
    {
        return InternalDescriptionSerializer::toJson(*this);
    }

    unique_ptr<InternalElectionDescription> InternalElectionDescription::fromJson(string data)
    {
        return InternalDescriptionSerializer::fromJson(move(data));
    }

    unique_ptr<InternalElectionDescription>
    InternalElectionDescription::fromBson(vector<uint8_t> data)
    {
        return InternalDescriptionSerializer::fromBson(move(data));
    }

    // Protected Methods

    vector<unique_ptr<ContestDescriptionWithPlaceholders>>
    InternalElectionDescription::generateContestsWithPlaceholders(
      const ElectionDescription &description)
    {
        vector<unique_ptr<ContestDescriptionWithPlaceholders>> contests;
        for (const auto &contest : description.getContests()) {

            vector<unique_ptr<SelectionDescription>> placeholders;
            placeholders.reserve(contest.get().getNumberElected());
            uint32_t maxSequenceOrder = 0;
            for (const auto &selection : contest.get().getSelections()) {
                auto sequenceOrder = selection.get().getSequenceOrder();
                if (sequenceOrder > maxSequenceOrder) {
                    maxSequenceOrder = sequenceOrder;
                }
            }

            maxSequenceOrder += 1;

            for (uint64_t i = 0; i < contest.get().getNumberElected(); i++) {
                auto placeholder =
                  generatePlaceholderSelectionFrom(contest.get(), maxSequenceOrder + i);
                placeholders.push_back(move(placeholder));
            }
            contests.push_back(
              make_unique<ContestDescriptionWithPlaceholders>(contest.get(), move(placeholders)));
        }
        return contests;
    }

    vector<unique_ptr<GeopoliticalUnit>>
    InternalElectionDescription::copyGeopoliticalUnits(const ElectionDescription &description)
    {
        vector<unique_ptr<GeopoliticalUnit>> collection;
        auto source = description.getGeopoliticalUnits();
        collection.reserve(source.size());
        for (auto element : source) {
            collection.push_back(make_unique<GeopoliticalUnit>(element.get()));
        }
        return collection;
    }

    vector<unique_ptr<BallotStyle>>
    InternalElectionDescription::copyBallotStyles(const ElectionDescription &description)
    {
        vector<unique_ptr<BallotStyle>> collection;
        auto source = description.getBallotStyles();
        collection.reserve(source.size());
        for (auto &element : source) {
            collection.push_back(make_unique<BallotStyle>(element.get()));
        }
        return collection;
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

    unique_ptr<SelectionDescription>
    generatePlaceholderSelectionFrom(const ContestDescription &contest, uint64_t useSequenceId)
    {
        auto placeholderObjectId = contest.getObjectId() + "-" + to_string(useSequenceId);
        return make_unique<SelectionDescription>(placeholderObjectId + "-placeholder",
                                                 placeholderObjectId + "-candidate", useSequenceId);
    }

} // namespace electionguard
