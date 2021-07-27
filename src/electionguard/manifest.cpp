#include "electionguard/manifest.hpp"

#include "electionguard/hash.hpp"
#include "log.hpp"
#include "serialize.hpp"
#include "utils.hpp"

#include <cstring>
#include <iostream>
#include <set>
#include <utility>

using std::make_unique;
using std::map;
using std::move;
using std::out_of_range;
using std::ref;
using std::reference_wrapper;
using std::runtime_error;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;
using std::chrono::system_clock;

using ManifestSerializer = electionguard::Serialize::Manifest;
using InternalManifestSerializer = electionguard::Serialize::InternalManifest;

namespace electionguard
{

#pragma region ElectionType

    template <typename> struct _election_type {
        static const map<ElectionType, const string> _map;
    };
    template <typename T>
    const map<ElectionType, const string> _election_type<T>::_map = {
      {ElectionType::unknown, "unknown"},
      {ElectionType::general, "general"},
      {ElectionType::partisanPrimaryClosed, "partisanPrimaryClosed"},
      {ElectionType::partisanPrimaryOpen, "partisanPrimaryOpen"},
      {ElectionType::primary, "primary"},
      {ElectionType::runoff, "runoff"},
      {ElectionType::special, "special"},
      {ElectionType::other, "other"},
    };

    string getElectionTypeString(const ElectionType &value)
    {
        return _election_type<ElectionType>::_map.find(value)->second;
    }

    ElectionType getElectionType(const string &value)
    {
        try {
            auto item = findByValue(_election_type<ElectionType>::_map, value);
            return item;
        } catch (const std::exception &e) {
            Log::error(": error {}", e);
            return ElectionType::unknown;
        }
    }

#pragma endregion

#pragma region ReportingUnitType

    template <typename> struct _reporting_unit_type {
        static const map<ReportingUnitType, const string> _map;
    };
    template <typename T>
    const map<ReportingUnitType, const string> _reporting_unit_type<T>::_map = {
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

    string getReportingUnitTypeString(const ReportingUnitType &value)
    {
        return _reporting_unit_type<ReportingUnitType>::_map.find(value)->second;
    }

    ReportingUnitType getReportingUnitType(const string &value)
    {
        try {
            auto item = findByValue(_reporting_unit_type<ReportingUnitType>::_map, value);
            return item;
        } catch (const std::exception &e) {
            Log::error(": error {}", e);
            return ReportingUnitType::unknown;
        }
    }

#pragma endregion

#pragma region VoteVariationType

    template <typename> struct _vote_variation_type {
        static const map<VoteVariationType, const string> _map;
    };
    template <typename T>
    const map<VoteVariationType, const string> _vote_variation_type<T>::_map = {
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

    string getVoteVariationTypeString(const VoteVariationType &value)
    {
        return _vote_variation_type<VoteVariationType>::_map.find(value)->second;
    }

    VoteVariationType getVoteVariationType(const string &value)
    {
        try {
            auto item = findByValue(_vote_variation_type<VoteVariationType>::_map, value);
            return item;
        } catch (const std::exception &e) {
            Log::error(": error {}", e);
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

        [[nodiscard]] unique_ptr<AnnotatedString::Impl> clone() const
        {
            // TODO: verify string copy
            return make_unique<AnnotatedString::Impl>(*this);
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
            auto hash = hash_elems({annotation, value});
            Log::trace("AnnotatedString", hash.get());
            return hash;
        }
    };

    // Lifecycle Methods

    AnnotatedString::AnnotatedString(const AnnotatedString &other) : pimpl(other.pimpl->clone()) {}

    AnnotatedString::AnnotatedString(string annotation, string value)
        : pimpl(new Impl(move(annotation), move(value)))
    {
    }

    AnnotatedString::~AnnotatedString() = default;

    AnnotatedString &AnnotatedString::operator=(const AnnotatedString &other)
    {
        if (this == &other) {
            return *this;
        }

        pimpl = other.pimpl->clone();
        return *this;
    }

    AnnotatedString &AnnotatedString::operator=(AnnotatedString &&other) noexcept
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

        [[nodiscard]] unique_ptr<Language::Impl> clone() const
        {
            // TODO: verify string copy
            return make_unique<Language::Impl>(*this);
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
            auto hash = hash_elems({value, language});
            Log::trace("Language", hash.get());
            return hash;
        }
    };

    // Lifecycle Methods

    Language::Language(const Language &other) : pimpl(other.pimpl->clone()) {}

    Language::Language(string value, string language) : pimpl(new Impl(move(value), move(language)))
    {
    }

    Language::~Language() = default;

    Language &Language::operator=(const Language &other)
    {
        if (this == &other) {
            return *this;
        }

        pimpl = other.pimpl->clone();
        return *this;
    }

    Language &Language::operator=(Language &&other) noexcept
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

        [[nodiscard]] unique_ptr<InternationalizedText::Impl> clone() const
        {
            vector<unique_ptr<Language>> _text;
            _text.reserve(text.size());
            for (const auto &element : text) {
                _text.push_back(make_unique<Language>(*element));
            }

            return make_unique<InternationalizedText::Impl>(move(_text));
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
            vector<reference_wrapper<CryptoHashable>> refs;
            refs.reserve(this->text.size());
            for (const auto &i : this->text) {
                refs.push_back(ref<CryptoHashable>(*i));
            }

            auto hash = hash_elems(refs);
            Log::trace("InternationalizedText", hash.get());
            return hash;
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

        [[nodiscard]] unique_ptr<ContactInformation::Impl> clone() const
        {
            vector<string> _addressLine;
            _addressLine.reserve(addressLine.size());
            for (const auto &element : addressLine) {
                _addressLine.push_back(element);
            }

            vector<unique_ptr<AnnotatedString>> _email;
            _email.reserve(email.size());
            for (const auto &element : email) {
                _email.push_back(make_unique<AnnotatedString>(*element));
            }

            vector<unique_ptr<AnnotatedString>> _phone;
            _phone.reserve(phone.size());
            for (const auto &element : phone) {
                _phone.push_back(make_unique<AnnotatedString>(*element));
            }

            return make_unique<ContactInformation::Impl>(move(_addressLine), move(_email),
                                                         move(_phone), name);
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
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
            auto hash = hash_elems({name, addressLine, emailRefs, phoneRefs});
            Log::trace("ContactInformation", hash.get());
            return hash;
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

        [[nodiscard]] unique_ptr<GeopoliticalUnit::Impl> clone() const
        {
            if (contactInformation != nullptr) {
                auto _contactInfo = make_unique<ContactInformation>(*contactInformation);
                return make_unique<GeopoliticalUnit::Impl>(object_id, name, type,
                                                           move(_contactInfo));
            }

            return make_unique<GeopoliticalUnit::Impl>(object_id, name, type);
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
            if (contactInformation != nullptr) {
                auto hash = hash_elems(
                  {object_id, name, getReportingUnitTypeString(type), contactInformation.get()});
                Log::trace("GeopoliticalUnit", hash.get());
                return hash;
            }

            auto hash = hash_elems({object_id, name, getReportingUnitTypeString(type), nullptr});
            Log::trace("GeopoliticalUnit", hash.get());
            return hash;
        }
    };

    // Lifecycle Methods

    GeopoliticalUnit::GeopoliticalUnit(const GeopoliticalUnit &other) : pimpl(other.pimpl->clone())
    {
    }

    GeopoliticalUnit::GeopoliticalUnit(const string &objectId, const string &name,
                                       const ReportingUnitType type)
        : pimpl(new Impl(objectId, name, type))
    {
    }

    GeopoliticalUnit::GeopoliticalUnit(const string &objectId, const string &name,
                                       const ReportingUnitType type,
                                       unique_ptr<ContactInformation> contactInformation)
        : pimpl(new Impl(objectId, name, type, move(contactInformation)))
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
            this->partyIds = {};
        }

        explicit Impl(const string &objectId, vector<string> geopoliticalUnitIds,
                      vector<string> partyIds, const string &imageUri)
            : geopoliticalUnitIds(move(geopoliticalUnitIds)), partyIds(move(partyIds)),
              imageUri(imageUri)
        {
            this->object_id = objectId;
        }

        [[nodiscard]] unique_ptr<BallotStyle::Impl> clone() const
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

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
            auto hash = hash_elems({object_id, geopoliticalUnitIds, partyIds, imageUri});
            Log::trace("BallotStyle", hash.get());
            return hash;
        }
    };

    // Lifecycle Methods

    BallotStyle::BallotStyle(const BallotStyle &other) : pimpl(other.pimpl->clone()) {}

    BallotStyle::BallotStyle(const string &objectId, vector<string> geopoliticalUnitIds)
        : pimpl(new Impl(objectId, move(geopoliticalUnitIds)))
    {
    }

    BallotStyle::BallotStyle(const string &objectId, vector<string> geopoliticalUnitIds,
                             vector<string> partyIds, const string &imageUri)
        : pimpl(new Impl(objectId, move(geopoliticalUnitIds), move(partyIds), imageUri))
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

        explicit Impl(const string &objectId, const string &abbreviation)
            : abbreviation(abbreviation)
        {
            this->object_id = objectId;
        }

        explicit Impl(const string &objectId, const string &abbreviation, const string &color,
                      const string &logoUri)
            : abbreviation(abbreviation), color(color), logoUri(logoUri)
        {
            this->object_id = objectId;
        }

        explicit Impl(const string &objectId, unique_ptr<InternationalizedText> name,
                      const string &abbreviation, const string &color, const string &logoUri)
            : name(move(name)), abbreviation(abbreviation), color(color), logoUri(logoUri)
        {
            this->object_id = objectId;
        }

        [[nodiscard]] unique_ptr<Party::Impl> clone() const
        {
            if (name != nullptr) {
                auto _name = make_unique<InternationalizedText>(*name);
                return make_unique<Party::Impl>(object_id, move(_name), abbreviation, color,
                                                logoUri);
            }
            return make_unique<Party::Impl>(object_id, abbreviation, color, logoUri);
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
            if (name != nullptr) {
                auto hash = hash_elems({object_id, name.get(), abbreviation, color, logoUri});
                Log::trace("Party", hash.get());
                return hash;
            }
            auto hash = hash_elems({object_id, nullptr, abbreviation, color, logoUri});
            Log::trace("Party", hash.get());
            return hash;
        }
    };

    // Lifecycle Methods

    Party::Party(const Party &other) : pimpl(other.pimpl->clone()) {}

    Party::Party(const string &objectId) : pimpl(new Impl(objectId)) {}

    Party::Party(const string &objectId, const string &abbreviation)
        : pimpl(new Impl(objectId, abbreviation))
    {
    }

    Party::Party(const string &objectId, unique_ptr<InternationalizedText> name,
                 const string &abbreviation, const string &color, const string &logoUri)
        : pimpl(new Impl(objectId, move(name), abbreviation, color, logoUri))
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

        explicit Impl(const string &objectId, const string &partyId, bool isWriteIn)
            : partyId(partyId)
        {
            this->object_id = objectId;
            this->isWriteIn = isWriteIn;
        }

        explicit Impl(const string &objectId, const string &partyId, const string &imageUri,
                      bool isWriteIn)
            : partyId(partyId), imageUri(imageUri)
        {
            this->object_id = objectId;
            this->isWriteIn = isWriteIn;
        }

        explicit Impl(const string &objectId, unique_ptr<InternationalizedText> name,
                      bool isWriteIn)
            : name(move(name))
        {
            this->object_id = objectId;
            this->isWriteIn = isWriteIn;
        }

        explicit Impl(const string &objectId, unique_ptr<InternationalizedText> name,
                      const string &partyId, bool isWriteIn)
            : name(move(name)), partyId(partyId)
        {
            this->object_id = objectId;
            this->isWriteIn = isWriteIn;
        }

        explicit Impl(const string &objectId, unique_ptr<InternationalizedText> name,
                      const string &partyId, const string &imageUri, bool isWriteIn)
            : name(move(name)), partyId(partyId), imageUri(imageUri)
        {
            this->object_id = objectId;
            this->isWriteIn = isWriteIn;
        }

        [[nodiscard]] unique_ptr<Candidate::Impl> clone() const
        {
            if (name != nullptr) {
                auto _name = make_unique<InternationalizedText>(*name);
                return make_unique<Candidate::Impl>(object_id, move(_name), partyId, imageUri,
                                                    isWriteIn);
            }

            return make_unique<Candidate::Impl>(object_id, partyId, imageUri, isWriteIn);
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
            if (name != nullptr) {
                auto hash = hash_elems({object_id, name.get(), partyId, imageUri});
                Log::trace("Candidate", hash.get());
                return hash;
            }
            auto hash = hash_elems({object_id, nullptr, partyId, imageUri});
            Log::trace("Candidate", hash.get());
            return hash;
        }
    };

    // Lifecycle Methods

    Candidate::Candidate(const Candidate &other) : pimpl(other.pimpl->clone()) {}

    Candidate::Candidate(const string &objectId, bool isWriteIn)
        : pimpl(new Impl(objectId, isWriteIn))
    {
    }

    Candidate::Candidate(const string &objectId, const string &partyId, bool isWriteIn)
        : pimpl(new Impl(objectId, partyId, isWriteIn))
    {
    }

    Candidate::Candidate(const string &objectId, unique_ptr<InternationalizedText> name,
                         bool isWriteIn)
        : pimpl(new Impl(objectId, move(name), isWriteIn))
    {
    }

    Candidate::Candidate(const string &objectId, unique_ptr<InternationalizedText> name,
                         const string &partyId, bool isWriteIn)
        : pimpl(new Impl(objectId, move(name), partyId, isWriteIn))
    {
    }

    Candidate::Candidate(const string &objectId, unique_ptr<InternationalizedText> name,
                         const string &partyId, const string &imageUri, bool isWriteIn)
        : pimpl(new Impl(objectId, move(name), partyId, imageUri, isWriteIn))
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
            : candidateId(candidateId)
        {
            this->object_id = objectId;
            this->sequenceOrder = sequenceOrder;
        }

        [[nodiscard]] unique_ptr<SelectionDescription::Impl> clone() const
        {
            return make_unique<SelectionDescription::Impl>(object_id, candidateId, sequenceOrder);
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
            auto hash = hash_elems({object_id, sequenceOrder, candidateId});
            Log::trace("SelectionDescription", hash.get());
            return hash;
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
        vector<string> primaryPartyIds;

        Impl(const string &objectId, const string &electoralDistrictId,
             const uint64_t sequenceOrder, const VoteVariationType voteVariation,
             const uint64_t numberElected, const string &name,
             vector<unique_ptr<SelectionDescription>> selections)
            : electoralDistrictId(electoralDistrictId), name(name), selections(move(selections))
        {
            this->object_id = objectId;
            this->sequenceOrder = sequenceOrder;
            this->voteVariation = voteVariation;
            this->numberElected = numberElected;
            this->votesAllowed = 0UL;
            this->primaryPartyIds = {};
        }

        Impl(const string &objectId, const string &electoralDistrictId,
             const uint64_t sequenceOrder, const VoteVariationType voteVariation,
             const uint64_t numberElected, const string &name,
             vector<unique_ptr<SelectionDescription>> selections, vector<string> primaryPartyIds)
            : electoralDistrictId(electoralDistrictId), name(name), selections(move(selections)),
              primaryPartyIds(move(primaryPartyIds))
        {
            this->object_id = objectId;
            this->sequenceOrder = sequenceOrder;
            this->voteVariation = voteVariation;
            this->numberElected = numberElected;
            this->votesAllowed = 0UL;
        }

        Impl(const string &objectId, const string &electoralDistrictId,
             const uint64_t sequenceOrder, const VoteVariationType voteVariation,
             const uint64_t numberElected, const uint64_t votesAllowed, const string &name,
             unique_ptr<InternationalizedText> ballotTitle,
             unique_ptr<InternationalizedText> ballotSubtitle,
             vector<unique_ptr<SelectionDescription>> selections)
            : electoralDistrictId(electoralDistrictId), name(name), ballotTitle(move(ballotTitle)),
              ballotSubtitle(move(ballotSubtitle)), selections(move(selections))
        {
            this->object_id = objectId;
            this->sequenceOrder = sequenceOrder;
            this->voteVariation = voteVariation;
            this->numberElected = numberElected;
            this->votesAllowed = votesAllowed;
            this->primaryPartyIds = {};
        }

        Impl(const string &objectId, const string &electoralDistrictId,
             const uint64_t sequenceOrder, const VoteVariationType voteVariation,
             const uint64_t numberElected, const uint64_t votesAllowed, const string &name,
             unique_ptr<InternationalizedText> ballotTitle,
             unique_ptr<InternationalizedText> ballotSubtitle,
             vector<unique_ptr<SelectionDescription>> selections, vector<string> primaryPartyIds)
            : electoralDistrictId(electoralDistrictId), name(name), ballotTitle(move(ballotTitle)),
              ballotSubtitle(move(ballotSubtitle)), selections(move(selections)),
              primaryPartyIds(move(primaryPartyIds))
        {
            this->object_id = objectId;
            this->sequenceOrder = sequenceOrder;
            this->voteVariation = voteVariation;
            this->numberElected = numberElected;
            this->votesAllowed = votesAllowed;
        }

        [[nodiscard]] unique_ptr<ContestDescription::Impl> clone() const
        {
            vector<unique_ptr<SelectionDescription>> _selections;
            _selections.reserve(selections.size());
            for (const auto &element : selections) {
                _selections.push_back(make_unique<SelectionDescription>(*element));
            }

            // TODO: handle optional values
            if (ballotTitle != nullptr) {
                auto _ballotTitle = make_unique<InternationalizedText>(*ballotTitle);
                auto _ballotSubtitle = make_unique<InternationalizedText>(*ballotSubtitle);
                return make_unique<ContestDescription::Impl>(
                  object_id, electoralDistrictId, sequenceOrder, voteVariation, numberElected,
                  votesAllowed, name, move(_ballotTitle), move(_ballotSubtitle), move(_selections));
            }
            return make_unique<ContestDescription::Impl>(object_id, electoralDistrictId,
                                                         sequenceOrder, voteVariation,
                                                         numberElected, name, move(_selections));
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
        {
            vector<reference_wrapper<CryptoHashable>> selectionRefs;
            selectionRefs.reserve(selections.size());
            for (const auto &selection : selections) {
                selectionRefs.push_back(ref<CryptoHashable>(*selection));
            }

            // TODO: handle optional values
            if (ballotTitle != nullptr) {
                auto hash = hash_elems({object_id, sequenceOrder, electoralDistrictId,
                                        getVoteVariationTypeString(voteVariation),
                                        ref<CryptoHashable>(*ballotTitle),
                                        ref<CryptoHashable>(*ballotSubtitle), name, numberElected,
                                        votesAllowed, selectionRefs});
                Log::trace("ContestDescription", hash.get());
                return hash;
            }
            auto hash = hash_elems({object_id, sequenceOrder, electoralDistrictId,
                                    getVoteVariationTypeString(voteVariation), nullptr, nullptr,
                                    name, numberElected, votesAllowed, selectionRefs});
            Log::trace("ContestDescription", hash.get());
            return hash;
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
                                           const uint64_t numberElected, const string &name,
                                           vector<unique_ptr<SelectionDescription>> selections)
        : pimpl(new Impl(objectId, electoralDistrictId, sequenceOrder, voteVariation, numberElected,
                         name, move(selections)))
    {
    }

    ContestDescription::ContestDescription(
      const string &objectId, const string &electoralDistrictId, const uint64_t sequenceOrder,
      const VoteVariationType voteVariation, const uint64_t numberElected, const string &name,
      vector<unique_ptr<SelectionDescription>> selections, vector<string> primaryPartyIds)
        : pimpl(new Impl(objectId, electoralDistrictId, sequenceOrder, voteVariation, numberElected,
                         name, move(selections), move(primaryPartyIds)))
    {
    }

    ContestDescription::ContestDescription(const string &objectId,
                                           const string &electoralDistrictId,
                                           const uint64_t sequenceOrder,
                                           const VoteVariationType voteVariation,
                                           const uint64_t numberElected,
                                           const uint64_t votesAllowed, const string &name,
                                           unique_ptr<InternationalizedText> ballotTitle,
                                           unique_ptr<InternationalizedText> ballotSubtitle,
                                           vector<unique_ptr<SelectionDescription>> selections)
        : pimpl(new Impl(objectId, electoralDistrictId, sequenceOrder, voteVariation, numberElected,
                         votesAllowed, name, move(ballotTitle), move(ballotSubtitle),
                         move(selections)))
    {
    }

    ContestDescription::ContestDescription(
      const string &objectId, const string &electoralDistrictId, const uint64_t sequenceOrder,
      const VoteVariationType voteVariation, const uint64_t numberElected,
      const uint64_t votesAllowed, const string &name,
      unique_ptr<InternationalizedText> ballotTitle,
      unique_ptr<InternationalizedText> ballotSubtitle,
      vector<unique_ptr<SelectionDescription>> selections, vector<string> primaryPartyIds)
        : pimpl(new Impl(objectId, electoralDistrictId, sequenceOrder, voteVariation, numberElected,
                         votesAllowed, name, move(ballotTitle), move(ballotSubtitle),
                         move(selections), move(primaryPartyIds)))
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

        sort(selections.begin(), selections.end(),
             [](const reference_wrapper<SelectionDescription> left,
                const reference_wrapper<SelectionDescription> right) {
                 return left.get().getSequenceOrder() < right.get().getSequenceOrder();
             });

        return selections;
    }

    // Interface Overrides

    unique_ptr<ElementModQ> ContestDescription::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> ContestDescription::crypto_hash() const { return pimpl->crypto_hash(); }

    bool ContestDescription::isValid() const
    {
        auto contest_has_valid_number_elected = pimpl->numberElected <= pimpl->selections.size();
        auto contest_has_valid_votes_allowed =
          pimpl->votesAllowed == 0 || pimpl->numberElected <= pimpl->votesAllowed;

        std::set<string> candidateIds;
        std::set<string> selectionIds;
        std::set<uint64_t> sequenceIds;

        uint64_t selectionCount = 0;
        auto expectedSelectionCount = pimpl->selections.size();

        for (const auto &selection : getSelections()) {
            selectionCount++;

            auto selectionId = selection.get().getObjectId();
            if (!selectionId.empty()) {
                selectionIds.insert(selectionId);
            }

            auto candidateId = selection.get().getCandidateId();
            if (!candidateId.empty()) {
                candidateIds.insert(candidateId);
            }

            auto sequenceOrder = selection.get().getSequenceOrder();
            sequenceIds.insert(sequenceOrder);
        }

        auto selections_have_valid_candidate_ids = candidateIds.size() == expectedSelectionCount;
        auto selections_have_valid_selection_ids = selectionIds.size() == expectedSelectionCount;
        auto selections_have_valid_sequence_ids = sequenceIds.size() == expectedSelectionCount;

        auto success = contest_has_valid_number_elected && contest_has_valid_votes_allowed &&
                       selections_have_valid_candidate_ids && selections_have_valid_selection_ids &&
                       selections_have_valid_sequence_ids;

        if (!success) {
            map<string, bool> printMap{
              {"contest_has_valid_number_elected", contest_has_valid_number_elected},
              {"contest_has_valid_votes_allowed", contest_has_valid_votes_allowed},
              {"selections_have_valid_candidate_ids", selections_have_valid_candidate_ids},
              {"selections_have_valid_selection_ids", selections_have_valid_selection_ids},
              {"selections_have_valid_sequence_ids", selections_have_valid_sequence_ids}};

            Log::info("Contest " + getObjectId() + " failed validation check", printMap);
        }

        return success;
    }

#pragma endregion

#pragma region ContestDescriptionWithPlaceholders

    struct ContestDescriptionWithPlaceholders::Impl {
        vector<unique_ptr<SelectionDescription>> placeholderSelections;

        explicit Impl(vector<unique_ptr<SelectionDescription>> placeholderSelections)
            : placeholderSelections(move(placeholderSelections))
        {
        }

        [[nodiscard]] unique_ptr<ContestDescriptionWithPlaceholders::Impl> clone() const
        {
            vector<unique_ptr<SelectionDescription>> _placeholderSelections;
            _placeholderSelections.reserve(placeholderSelections.size());
            for (const auto &element : placeholderSelections) {
                _placeholderSelections.push_back(make_unique<SelectionDescription>(*element));
            }
            return make_unique<ContestDescriptionWithPlaceholders::Impl>(
              move(_placeholderSelections));
        }
    };

    // Lifecycle Methods

    ContestDescriptionWithPlaceholders::ContestDescriptionWithPlaceholders(
      const ContestDescriptionWithPlaceholders &other)
        : ContestDescription(other), pimpl(other.pimpl->clone())
    {
    }

    ContestDescriptionWithPlaceholders::ContestDescriptionWithPlaceholders(
      const ContestDescription &other,
      vector<unique_ptr<SelectionDescription>> placeholderSelections)
        : ContestDescription(other), pimpl(new Impl(move(placeholderSelections)))
    {
    }

    ContestDescriptionWithPlaceholders::ContestDescriptionWithPlaceholders(
      const string &objectId, const string &electoralDistrictId, const uint64_t sequenceOrder,
      const VoteVariationType voteVariation, const uint64_t numberElected, const string &name,
      vector<unique_ptr<SelectionDescription>> selections,
      vector<unique_ptr<SelectionDescription>> placeholderSelections)
        : ContestDescription(objectId, electoralDistrictId, sequenceOrder, voteVariation,
                             numberElected, name, move(selections)),
          pimpl(new Impl(move(placeholderSelections)))
    {
    }

    ContestDescriptionWithPlaceholders::ContestDescriptionWithPlaceholders(
      const string &objectId, const string &electoralDistrictId, const uint64_t sequenceOrder,
      const VoteVariationType voteVariation, const uint64_t numberElected, const string &name,
      vector<unique_ptr<SelectionDescription>> selections, vector<string> primaryPartyIds,
      vector<unique_ptr<SelectionDescription>> placeholderSelections)
        : ContestDescription(objectId, electoralDistrictId, sequenceOrder, voteVariation,
                             numberElected, name, move(selections), move(primaryPartyIds)),
          pimpl(new Impl(move(placeholderSelections)))
    {
    }

    ContestDescriptionWithPlaceholders::ContestDescriptionWithPlaceholders(
      const string &objectId, const string &electoralDistrictId, const uint64_t sequenceOrder,
      const VoteVariationType voteVariation, const uint64_t numberElected,
      const uint64_t votesAllowed, const string &name,
      unique_ptr<InternationalizedText> ballotTitle,
      unique_ptr<InternationalizedText> ballotSubtitle,
      vector<unique_ptr<SelectionDescription>> selections,
      vector<unique_ptr<SelectionDescription>> placeholderSelections)
        : ContestDescription(objectId, electoralDistrictId, sequenceOrder, voteVariation,
                             numberElected, votesAllowed, name, move(ballotTitle),
                             move(ballotSubtitle), move(selections)),
          pimpl(new Impl(move(placeholderSelections)))
    {
    }

    ContestDescriptionWithPlaceholders::ContestDescriptionWithPlaceholders(
      const string &objectId, const string &electoralDistrictId, const uint64_t sequenceOrder,
      const VoteVariationType voteVariation, const uint64_t numberElected,
      const uint64_t votesAllowed, const string &name,
      unique_ptr<InternationalizedText> ballotTitle,
      unique_ptr<InternationalizedText> ballotSubtitle,
      vector<unique_ptr<SelectionDescription>> selections, vector<string> primaryPartyIds,
      vector<unique_ptr<SelectionDescription>> placeholderSelections)
        : ContestDescription(objectId, electoralDistrictId, sequenceOrder, voteVariation,
                             numberElected, votesAllowed, name, move(ballotTitle),
                             move(ballotSubtitle), move(selections), move(primaryPartyIds)),
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

        sort(placeholders.begin(), placeholders.end(),
             [](const reference_wrapper<SelectionDescription> left,
                const reference_wrapper<SelectionDescription> right) {
                 return left.get().getSequenceOrder() < right.get().getSequenceOrder();
             });

        return placeholders;
    }

    // Public Methods

    bool ContestDescriptionWithPlaceholders::IsPlaceholder(SelectionDescription &selection) const
    {
        for (const auto &placeholder : pimpl->placeholderSelections) {
            if (selection.getObjectId() == placeholder->getObjectId()) {
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

#pragma region Manifest

    struct Manifest::Impl {
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

        Impl(const string &electionScopeId, ElectionType type, system_clock::time_point startDate,
             system_clock::time_point endDate,
             vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
             vector<unique_ptr<Party>> parties, vector<unique_ptr<Candidate>> candidates,
             vector<unique_ptr<ContestDescription>> contests,
             vector<unique_ptr<BallotStyle>> ballotStyles)
            : electionScopeId(electionScopeId), geopoliticalUnits(move(geopoliticalUnits)),
              parties(move(parties)), candidates(move(candidates)), contests(move(contests)),
              ballotStyles(move(ballotStyles))
        {
            this->type = type;
            this->startDate = startDate;
            this->endDate = endDate;
        }

        Impl(const string &electionScopeId, ElectionType type, system_clock::time_point startDate,
             system_clock::time_point endDate,
             vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
             vector<unique_ptr<Party>> parties, vector<unique_ptr<Candidate>> candidates,
             vector<unique_ptr<ContestDescription>> contests,
             vector<unique_ptr<BallotStyle>> ballotStyles, unique_ptr<InternationalizedText> name,
             unique_ptr<ContactInformation> contactInformation)
            : electionScopeId(electionScopeId), geopoliticalUnits(move(geopoliticalUnits)),
              parties(move(parties)), candidates(move(candidates)), contests(move(contests)),
              ballotStyles(move(ballotStyles)), name(move(name)),
              contactInformation(move(contactInformation))
        {
            this->type = type;
            this->startDate = startDate;
            this->endDate = endDate;
        }

        [[nodiscard]] unique_ptr<ElementModQ> crypto_hash() const
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

            // TODO: handle other optional cases
            if (name == nullptr) {
                auto hash = hash_elems(
                  {electionScopeId, getElectionTypeString(type), timePointToIsoString(startDate),
                   timePointToIsoString(endDate), nullptr, geopoliticalUnitRefs, partyRefs,
                   contestRefs, ballotStyleRefs});
                Log::trace("Manifest:: NO NAME!", hash->toHex());
                return hash;
            }

            auto hash =
              hash_elems({electionScopeId, getElectionTypeString(type),
                          timePointToIsoString(startDate), timePointToIsoString(endDate),
                          ref<CryptoHashable>(*name), ref<CryptoHashable>(*contactInformation),
                          geopoliticalUnitRefs, partyRefs, contestRefs, ballotStyleRefs});

            Log::trace("Manifest", hash->toHex());
            return hash;
        }
    };

    // Lifecycle Methods

    Manifest::Manifest(const string &electionScopeId, ElectionType type,
                       system_clock::time_point startDate, system_clock::time_point endDate,
                       vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
                       vector<unique_ptr<Party>> parties, vector<unique_ptr<Candidate>> candidates,
                       vector<unique_ptr<ContestDescription>> contests,
                       vector<unique_ptr<BallotStyle>> ballotStyles)
        : pimpl(new Impl(electionScopeId, type, startDate, endDate, move(geopoliticalUnits),
                         move(parties), move(candidates), move(contests), move(ballotStyles)))
    {
    }

    Manifest::Manifest(const string &electionScopeId, ElectionType type,
                       system_clock::time_point startDate, system_clock::time_point endDate,
                       vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
                       vector<unique_ptr<Party>> parties, vector<unique_ptr<Candidate>> candidates,
                       vector<unique_ptr<ContestDescription>> contests,
                       vector<unique_ptr<BallotStyle>> ballotStyles,
                       unique_ptr<InternationalizedText> name,
                       unique_ptr<ContactInformation> contactInformation)
        : pimpl(new Impl(electionScopeId, type, startDate, endDate, move(geopoliticalUnits),
                         move(parties), move(candidates), move(contests), move(ballotStyles),
                         move(name), move(contactInformation)))
    {
    }

    Manifest::~Manifest() = default;

    Manifest &Manifest::operator=(Manifest other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    string Manifest::getElectionScopeId() const { return pimpl->electionScopeId; }
    ElectionType Manifest::getElectionType() const { return pimpl->type; }
    system_clock::time_point Manifest::getStartDate() const { return pimpl->startDate; }
    system_clock::time_point Manifest::getEndDate() const { return pimpl->endDate; }

    vector<reference_wrapper<GeopoliticalUnit>> Manifest::getGeopoliticalUnits() const
    {
        vector<reference_wrapper<GeopoliticalUnit>> references;
        references.reserve(pimpl->geopoliticalUnits.size());
        for (auto &reference : pimpl->geopoliticalUnits) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    vector<reference_wrapper<Party>> Manifest::getParties() const
    {
        vector<reference_wrapper<Party>> references;
        references.reserve(pimpl->parties.size());
        for (auto &reference : pimpl->parties) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    vector<reference_wrapper<Candidate>> Manifest::getCandidates() const
    {
        vector<reference_wrapper<Candidate>> references;
        references.reserve(pimpl->candidates.size());
        for (auto &reference : pimpl->candidates) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    vector<reference_wrapper<ContestDescription>> Manifest::getContests() const
    {
        vector<reference_wrapper<ContestDescription>> references;
        references.reserve(pimpl->contests.size());
        for (auto &reference : pimpl->contests) {
            references.push_back(ref(*reference));
        }

        sort(references.begin(), references.end(),
             [](const reference_wrapper<ContestDescription> left,
                const reference_wrapper<ContestDescription> right) {
                 return left.get().getSequenceOrder() < right.get().getSequenceOrder();
             });

        return references;
    }

    vector<reference_wrapper<BallotStyle>> Manifest::getBallotStyles() const
    {
        vector<reference_wrapper<BallotStyle>> references;
        references.reserve(pimpl->ballotStyles.size());
        for (auto &reference : pimpl->ballotStyles) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    InternationalizedText *Manifest::getName() const { return pimpl->name.get(); }
    ContactInformation *Manifest::getContactInformation() const
    {
        return pimpl->contactInformation.get();
    }

    // Public Methods

    bool Manifest::isValid() const
    {

        std::set<string> gpUnitIds;
        std::set<string> ballotStyleIds;
        std::set<string> partyIds;
        std::set<string> candidateIds;
        std::set<string> contestIds;

        // Validate GP Units
        for (const auto &item : getGeopoliticalUnits()) {
            auto id = item.get().getObjectId();
            if (!id.empty()) {
                gpUnitIds.insert(id);
            }
        }

        auto geopolitical_units_valid = gpUnitIds.size() == pimpl->geopoliticalUnits.size();

        // Validate Ballot Styles
        auto ballot_styles_have_valid_gp_unit_ids = true;

        for (const auto &item : getBallotStyles()) {
            auto id = item.get().getObjectId();
            if (!id.empty()) {
                ballotStyleIds.insert(id);
            }

            if (item.get().getGeopoliticalUnitIds().size() == 0) {
                ballot_styles_have_valid_gp_unit_ids = false;
                break;
            }
            for (const auto &gpUnitId : item.get().getGeopoliticalUnitIds()) {
                ballot_styles_have_valid_gp_unit_ids = ballot_styles_have_valid_gp_unit_ids &&
                                                       gpUnitIds.find(gpUnitId) != gpUnitIds.end();
            }
        }

        auto ballot_styles_have_valid_length = ballotStyleIds.size() == pimpl->ballotStyles.size();
        auto ballot_styles_valid =
          ballot_styles_have_valid_length && ballot_styles_have_valid_gp_unit_ids;

        // Validate Parties
        for (const auto &item : getParties()) {
            auto id = item.get().getObjectId();
            if (!id.empty()) {
                partyIds.insert(id);
            }
        }

        auto parties_valid = partyIds.size() == pimpl->parties.size();

        // Validate Candidates
        auto candidates_have_valid_party_ids = true;

        for (const auto &item : getCandidates()) {
            auto id = item.get().getObjectId();
            if (!id.empty()) {
                candidateIds.insert(id);
            }

            candidates_have_valid_party_ids =
              candidates_have_valid_party_ids &&
              (item.get().getPartyId().empty() ||
               partyIds.find(item.get().getPartyId()) != partyIds.end());
        }

        auto candidates_have_valid_length = candidateIds.size() == pimpl->candidates.size();
        auto candidates_valid = candidates_have_valid_length && candidates_have_valid_party_ids;

        // Validate Contests
        auto contests_validate_their_properties = true;
        auto contests_have_valid_electoral_district_id = true;
        auto candidate_contests_have_valid_party_ids = true;

        std::set<uint64_t> contestSequenceIds;

        for (const auto &item : getContests()) {
            contests_validate_their_properties =
              contests_validate_their_properties && item.get().isValid();

            auto id = item.get().getObjectId();
            if (!id.empty()) {
                contestIds.insert(id);
            }

            // Validate the sequence order
            auto sequenceOrder = item.get().getSequenceOrder();
            contestSequenceIds.insert(sequenceOrder);

            // Validate the associated gp unit id
            auto electoralDistrictId = item.get().getElectoralDistrictId();
            contests_have_valid_electoral_district_id =
              contests_have_valid_electoral_district_id && !electoralDistrictId.empty() &&
              gpUnitIds.find(electoralDistrictId) != gpUnitIds.end();

            // Validate the party's if they exist
            // TODO: #174: validate party id's
        }

        auto contests_have_valid_object_ids = contestIds.size() == pimpl->contests.size();
        auto contests_have_valid_sequence_ids = contestSequenceIds.size() == pimpl->contests.size();

        auto contests_valid = contests_have_valid_object_ids && contests_have_valid_sequence_ids &&
                              contests_validate_their_properties &&
                              contests_have_valid_electoral_district_id &&
                              candidate_contests_have_valid_party_ids;

        auto success = geopolitical_units_valid && ballot_styles_valid && parties_valid &&
                       candidates_valid && contests_valid;

        if (!success) {
            map<string, bool> printMap{
              {"geopolitical_units_valid", geopolitical_units_valid},
              {"ballot_styles_valid", ballot_styles_valid},
              {"ballot_styles_have_valid_length", ballot_styles_have_valid_length},
              {"ballot_styles_have_valid_gp_unit_ids", ballot_styles_have_valid_gp_unit_ids},
              {"parties_valid", parties_valid},
              {"candidates_valid", candidates_valid},
              {"candidates_have_valid_length", candidates_have_valid_length},
              {"candidates_have_valid_party_ids", candidates_have_valid_party_ids},
              {"contests_valid", contests_valid},
              {"contests_have_valid_object_ids", contests_have_valid_object_ids},
              {"contests_have_valid_sequence_ids", contests_have_valid_sequence_ids},
              {"contests_validate_their_properties", contests_validate_their_properties},
              {"contests_have_valid_electoral_district_id",
               contests_have_valid_electoral_district_id},
              {"candidate_contests_have_valid_party_ids", candidate_contests_have_valid_party_ids},
            };

            Log::info("Election failed Validation check", printMap);
        }

        return success;
    }

    vector<uint8_t> Manifest::toBson() const { return ManifestSerializer::toBson(*this); }

    string Manifest::toJson() { return ManifestSerializer::toJson(*this); }

    string Manifest::toJson() const { return ManifestSerializer::toJson(*this); }

    unique_ptr<Manifest> Manifest::fromJson(string data)
    {
        return ManifestSerializer::fromJson(move(data));
    }

    unique_ptr<Manifest> Manifest::fromBson(vector<uint8_t> data)
    {
        return ManifestSerializer::fromBson(move(data));
    }

    unique_ptr<ElementModQ> Manifest::crypto_hash() { return pimpl->crypto_hash(); }
    unique_ptr<ElementModQ> Manifest::crypto_hash() const { return pimpl->crypto_hash(); }

#pragma endregion

#pragma region InternalManifest

    struct InternalManifest::Impl {
        vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits;
        vector<unique_ptr<ContestDescriptionWithPlaceholders>> contests;
        vector<unique_ptr<BallotStyle>> ballotStyles;
        unique_ptr<ElementModQ> manifestHash;

        Impl(vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
             vector<unique_ptr<ContestDescriptionWithPlaceholders>> contests,
             vector<unique_ptr<BallotStyle>> ballotStyles, unique_ptr<ElementModQ> manifestHash)
            : geopoliticalUnits(move(geopoliticalUnits)), contests(move(contests)),
              ballotStyles(move(ballotStyles)), manifestHash(move(manifestHash))
        {
        }
    };

    // Lifecycle Methods

    InternalManifest::InternalManifest(
      vector<unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
      vector<unique_ptr<ContestDescriptionWithPlaceholders>> contests,
      vector<unique_ptr<BallotStyle>> ballotStyles, const ElementModQ &manifestHash)
        : pimpl(new Impl(move(geopoliticalUnits), move(contests), move(ballotStyles),
                         make_unique<ElementModQ>(manifestHash)))
    {
    }

    InternalManifest::InternalManifest(const Manifest &description)
        : pimpl(new Impl(copyGeopoliticalUnits(description),
                         generateContestsWithPlaceholders(description),
                         copyBallotStyles(description), description.crypto_hash()))
    {
    }

    InternalManifest::~InternalManifest() = default;

    InternalManifest &InternalManifest::operator=(InternalManifest other)
    {
        swap(pimpl, other.pimpl);
        return *this;
    }

    // Property Getters

    const ElementModQ *InternalManifest::getManifestHash() const
    {
        return pimpl->manifestHash.get();
    }

    vector<reference_wrapper<GeopoliticalUnit>> InternalManifest::getGeopoliticalUnits() const
    {
        vector<reference_wrapper<GeopoliticalUnit>> references;
        references.reserve(pimpl->geopoliticalUnits.size());
        for (auto &reference : pimpl->geopoliticalUnits) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    vector<reference_wrapper<ContestDescriptionWithPlaceholders>>
    InternalManifest::getContests() const
    {
        vector<reference_wrapper<ContestDescriptionWithPlaceholders>> references;
        references.reserve(pimpl->contests.size());
        for (auto &reference : pimpl->contests) {
            references.push_back(ref(*reference));
        }
        sort(references.begin(), references.end(),
             [](const reference_wrapper<ContestDescriptionWithPlaceholders> left,
                const reference_wrapper<ContestDescriptionWithPlaceholders> right) {
                 return left.get().getSequenceOrder() < right.get().getSequenceOrder();
             });
        return references;
    }

    vector<reference_wrapper<BallotStyle>> InternalManifest::getBallotStyles() const
    {
        vector<reference_wrapper<BallotStyle>> references;
        references.reserve(pimpl->ballotStyles.size());
        for (auto &reference : pimpl->ballotStyles) {
            references.push_back(ref(*reference));
        }
        return references;
    }

    // Public Methods

    BallotStyle *InternalManifest::getBallotStyle(const std::string &ballotStyleId) const
    {
        for (auto &style : pimpl->ballotStyles) {
            if (style->getObjectId() == ballotStyleId) {
                return style.get();
            }
        }
        return nullptr;
    }

    vector<reference_wrapper<ContestDescriptionWithPlaceholders>>
    InternalManifest::getContestsFor(const string &ballotStyleId) const
    {
        auto *style = getBallotStyle(ballotStyleId);
        if (style == nullptr || style->getGeopoliticalUnitIds().empty()) {
            throw runtime_error("Could not resolve a valid geopolitical unit");
        }

        vector<reference_wrapper<ContestDescriptionWithPlaceholders>> contests;
        for (const auto &reference : pimpl->contests) {
            for (const auto &gpUnitId : style->getGeopoliticalUnitIds()) {
                if (reference->getElectoralDistrictId() == gpUnitId) {
                    contests.push_back(ref(*reference));
                }
            }
        }

        sort(contests.begin(), contests.end(),
             [](const reference_wrapper<ContestDescriptionWithPlaceholders> left,
                const reference_wrapper<ContestDescriptionWithPlaceholders> right) {
                 return left.get().getSequenceOrder() < right.get().getSequenceOrder();
             });

        return contests;
    }

    vector<uint8_t> InternalManifest::toBson() const
    {
        return InternalManifestSerializer::toBson(*this);
    }

    string InternalManifest::toJson() { return InternalManifestSerializer::toJson(*this); }

    string InternalManifest::toJson() const { return InternalManifestSerializer::toJson(*this); }

    unique_ptr<InternalManifest> InternalManifest::fromJson(string data)
    {
        return InternalManifestSerializer::fromJson(move(data));
    }

    unique_ptr<InternalManifest> InternalManifest::fromBson(vector<uint8_t> data)
    {
        return InternalManifestSerializer::fromBson(move(data));
    }

    // Protected Methods

    vector<unique_ptr<ContestDescriptionWithPlaceholders>>
    InternalManifest::generateContestsWithPlaceholders(const Manifest &description)
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
    InternalManifest::copyGeopoliticalUnits(const Manifest &description)
    {
        vector<unique_ptr<GeopoliticalUnit>> collection;
        auto source = description.getGeopoliticalUnits();
        collection.reserve(source.size());
        for (auto element : source) {
            collection.push_back(make_unique<GeopoliticalUnit>(element.get()));
        }
        return collection;
    }

    vector<unique_ptr<BallotStyle>> InternalManifest::copyBallotStyles(const Manifest &description)
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

    unique_ptr<SelectionDescription>
    generatePlaceholderSelectionFrom(const ContestDescription &contest, uint64_t useSequenceId)
    {
        auto placeholderObjectId = contest.getObjectId() + "-" + to_string(useSequenceId);
        return make_unique<SelectionDescription>(placeholderObjectId + "-placeholder",
                                                 placeholderObjectId + "-candidate", useSequenceId);
    }

} // namespace electionguard
