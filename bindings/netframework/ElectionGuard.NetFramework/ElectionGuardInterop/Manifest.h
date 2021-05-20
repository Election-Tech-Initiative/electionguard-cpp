#pragma once

#include "Group.h"
#include "ManagedInstance.h"
#include "Utilities.h"
#include "manifest.hpp"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ElectionGuardInterop
{
  public
    enum class ElectionType {
        unknown = 0,
        general = 1,
        partisanPrimaryClosed = 2,
        partisanPrimaryOpen = 3,
        primary = 4,
        runoff = 5,
        special = 6,
        other = 7
    };

  public
    enum class ReportingUnitType {
        unknown = 0,
        ballotBatch = 1,
        ballotStyleArea = 2,
        borough = 3,
        city = 4,
        cityCouncil = 5,
        combinedPrecinct = 6,
        congressional = 7,
        country = 8,
        county = 9,
        countyCouncil = 10,
        dropBox = 11,
        judicial = 12,
        municipality = 13,
        polling_place = 14,
        precinct = 15,
        school = 16,
        special = 17,
        splitPrecinct = 18,
        state = 19,
        stateHouse = 20,
        stateSenate = 21,
        township = 22,
        utility = 23,
        village = 24,
        voteCenter = 25,
        ward = 26,
        water = 27,
        other = 28,
    };

  public
    enum class VoteVariationType {
        unknown = 0,
        one_of_m = 1,
        approval = 2,
        borda = 3,
        cumulative = 4,
        majority = 5,
        n_of_m = 6,
        plurality = 7,
        proportional = 8,
        range = 9,
        rcv = 10,
        super_majority = 11,
        other = 12
    };

  public
    ref class AnnotatedString : ManagedInstance<electionguard::AnnotatedString>
    {
      public:
        AnnotatedString(String ^ annotation, String ^ value) : ManagedInstance()
        {
            std::string _annotation;
            std::string _value;

            Utilities::MarshalString(annotation, _annotation);
            Utilities::MarshalString(value, _value);

            this->_instance = new electionguard::AnnotatedString(_annotation, _value);
        }
        internal : AnnotatedString(std::unique_ptr<electionguard::AnnotatedString> other)
        {
            this->_instance = other.release();
        }

        AnnotatedString(electionguard::AnnotatedString *unowned) : ManagedInstance(unowned, false)
        {
        }
        AnnotatedString(const electionguard::AnnotatedString *unowned) : ManagedInstance(unowned) {}

      public:
        property String ^
          Annotation {
              String ^ get() {
                  auto unmanaged = this->_instance->getAnnotation();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property String ^
          Value {
              String ^ get() {
                  auto unmanaged = this->_instance->getValue();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property ElementModQ ^
          CryptoHash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->crypto_hash();
                  return gcnew ElementModQ(std::move(unmanaged));
              }
          }
    };

  public
    ref class Language : ManagedInstance<electionguard::Language>
    {
      public:
        Language(String ^ value, String ^ language) : ManagedInstance()
        {
            std::string _value;
            std::string _language;

            Utilities::MarshalString(value, _value);
            Utilities::MarshalString(language, _language);

            this->_instance = new electionguard::Language(_value, _language);
        }
        internal : Language(std::unique_ptr<electionguard::Language> other)
        {
            this->_instance = other.release();
        }

        Language(electionguard::Language *unowned) : ManagedInstance(unowned, false) {}
        Language(const electionguard::Language *unowned) : ManagedInstance(unowned) {}

      public:
        property String ^
          Value {
              String ^ get() {
                  auto unmanaged = this->_instance->getValue();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property String ^
          GetLanguage {
              String ^ get() {
                  auto unmanaged = this->_instance->getLanguage();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property ElementModQ ^
          CryptoHash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->crypto_hash();
                  return gcnew ElementModQ(std::move(unmanaged));
              }
          }
    };

  public
    ref class InternationalizedText : ManagedInstance<electionguard::InternationalizedText>
    {
      public:
        InternationalizedText(array<Language ^> ^ language) : ManagedInstance()
        {
            std::vector<std::unique_ptr<electionguard::Language>> languages;

            for each (auto item in language) {
                languages.push_back(std::make_unique<electionguard::Language>(*item->_instance));
            }

            this->_instance = new electionguard::InternationalizedText(std::move(languages));
        }
        internal
            : InternationalizedText(std::unique_ptr<electionguard::InternationalizedText> other)
        {
            this->_instance = other.release();
        }

        InternationalizedText(electionguard::InternationalizedText *unowned)
            : ManagedInstance(unowned, false)
        {
        }
        InternationalizedText(const electionguard::InternationalizedText *unowned)
            : ManagedInstance(unowned)
        {
        }

      public:
        property array<Language ^> ^
          Text {
              array<Language ^> ^ get() {
                  auto unmanaged = this->_instance->getText();
                  auto elements = gcnew array<Language ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew Language(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property ElementModQ ^
          CryptoHash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->crypto_hash();
                  return gcnew ElementModQ(std::move(unmanaged));
              }
          }
    };

  public
    ref class ContactInformation : ManagedInstance<electionguard::ContactInformation>
    {
      public:
        ContactInformation(String ^ name) : ManagedInstance()
        {
            std::string _name;
            Utilities::MarshalString(name, _name);

            this->_instance = new electionguard::ContactInformation(_name);
        }
        ContactInformation(array<String ^> ^ addressLine, array<AnnotatedString ^> ^ email,
                           array<AnnotatedString ^> ^ phone, String ^ name)
            : ManagedInstance()
        {
            std::vector<std::string> addressLines;

            for each (auto item in addressLine) {
                std::string address;
                Utilities::MarshalString(item, address);
                addressLines.push_back(address);
            }

            std::vector<std::unique_ptr<electionguard::AnnotatedString>> emails;

            for each (auto item in email) {
                emails.push_back(
                  std::make_unique<electionguard::AnnotatedString>(*item->_instance));
            }

            std::vector<std::unique_ptr<electionguard::AnnotatedString>> phones;

            for each (auto item in phone) {
                phones.push_back(
                  std::make_unique<electionguard::AnnotatedString>(*item->_instance));
            }

            std::string _name;
            Utilities::MarshalString(name, _name);

            this->_instance = new electionguard::ContactInformation(
              std::move(addressLines), std::move(emails), std::move(phones), _name);
        }

        internal : ContactInformation(std::unique_ptr<electionguard::ContactInformation> other)
        {
            this->_instance = other.release();
        }

        ContactInformation(electionguard::ContactInformation *unowned)
            : ManagedInstance(unowned, false)
        {
        }
        ContactInformation(const electionguard::ContactInformation *unowned)
            : ManagedInstance(unowned)
        {
        }

      public:
        property array<String ^> ^
          AddressLine {
              array<String ^> ^ get() {
                  auto unmanaged = this->_instance->getAddressLine();

                  auto elements = gcnew array<String ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew String(item.c_str());
                      index++;
                  }

                  return elements;
              }
          }

          property array<AnnotatedString ^> ^
          Email {
              array<AnnotatedString ^> ^ get() {
                  auto unmanaged = this->_instance->getEmail();
                  auto elements = gcnew array<AnnotatedString ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew AnnotatedString(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property array<AnnotatedString ^> ^
          Phone {
              array<AnnotatedString ^> ^ get() {
                  auto unmanaged = this->_instance->getPhone();
                  auto elements = gcnew array<AnnotatedString ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew AnnotatedString(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property String ^
          Name {
              String ^ get() {
                  auto unmanaged = this->_instance->getName();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property ElementModQ ^
          CryptoHash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->crypto_hash();
                  return gcnew ElementModQ(std::move(unmanaged));
              }
          }
    };

  public
    ref class GeopoliticalUnit : ManagedInstance<electionguard::GeopoliticalUnit>
    {
      public:
        GeopoliticalUnit(String ^ objectId, String ^ name, ReportingUnitType type)
            : ManagedInstance()
        {
            std::string _objectId;
            std::string _name;

            electionguard::ReportingUnitType _type =
              static_cast<electionguard::ReportingUnitType>(type);

            Utilities::MarshalString(objectId, _objectId);
            Utilities::MarshalString(name, _name);

            this->_instance = new electionguard::GeopoliticalUnit(_objectId, _name, _type);
        }
        // TODO: Constructor with Contact Info
        internal : GeopoliticalUnit(std::unique_ptr<electionguard::GeopoliticalUnit> other)
        {
            this->_instance = other.release();
        }

        GeopoliticalUnit(electionguard::GeopoliticalUnit *unowned) : ManagedInstance(unowned, false)
        {
        }
        GeopoliticalUnit(const electionguard::GeopoliticalUnit *unowned) : ManagedInstance(unowned)
        {
        }

      public:
        property String ^
          ObjectId {
              String ^ get() {
                  auto unmanaged = this->_instance->getObjectId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property String ^
          Name {
              String ^ get() {
                  auto unmanaged = this->_instance->getName();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property ReportingUnitType ^
          ReportingType {
              ReportingUnitType ^ get() {
                  auto unmanaged = this->_instance->getType();
                  return static_cast<ReportingUnitType>(unmanaged);
              }
          }

          // TODO: contact information

          property ElementModQ ^
          CryptoHash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->crypto_hash();
                  return gcnew ElementModQ(std::move(unmanaged));
              }
          }
    };

  public
    ref class BallotStyle : ManagedInstance<electionguard::BallotStyle>
    {
      public:
        BallotStyle(String ^ objectId, array<String ^> ^ geopoliticalUnitIds) : ManagedInstance()
        {
            std::string _objectId;
            Utilities::MarshalString(objectId, _objectId);

            std::vector<std::string> _geopoliticalUnitIds;
            for each (auto item in geopoliticalUnitIds) {
                std::string gpUnit;
                Utilities::MarshalString(item, gpUnit);
                _geopoliticalUnitIds.push_back(gpUnit);
            }

            this->_instance =
              new electionguard::BallotStyle(_objectId, std::move(_geopoliticalUnitIds));
        }

        BallotStyle(String ^ objectId, array<String ^> ^ geopoliticalUnitIds,
                    array<String ^> ^ partyIds, String ^ imageUri)
            : ManagedInstance()
        {
            std::string _objectId;
            Utilities::MarshalString(objectId, _objectId);

            std::vector<std::string> _geopoliticalUnitIds;
            for each (auto item in geopoliticalUnitIds) {
                std::string gpUnit;
                Utilities::MarshalString(item, gpUnit);
                _geopoliticalUnitIds.push_back(gpUnit);
            }

            std::vector<std::string> _partyIds;
            for each (auto item in partyIds) {
                std::string party;
                Utilities::MarshalString(item, party);
                _partyIds.push_back(party);
            }

            std::string _imageUri;
            Utilities::MarshalString(imageUri, _imageUri);

            this->_instance = new electionguard::BallotStyle(
              _objectId, std::move(_geopoliticalUnitIds), std::move(_partyIds), _imageUri);
        }

        internal : BallotStyle(std::unique_ptr<electionguard::BallotStyle> other)
        {
            this->_instance = other.release();
        }

        BallotStyle(electionguard::BallotStyle *unowned) : ManagedInstance(unowned, false) {}
        BallotStyle(const electionguard::BallotStyle *unowned) : ManagedInstance(unowned) {}

      public:
        property String ^
          ObjectId {
              String ^ get() {
                  auto unmanaged = this->_instance->getObjectId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property array<String ^> ^
          GeopoliticalUnits {
              array<String ^> ^ get() {
                  auto unmanaged = this->_instance->getGeopoliticalUnitIds();
                  auto elements = gcnew array<String ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew String(item.c_str());
                      index++;
                  }

                  return elements;
              }
          }

          property array<String ^> ^
          PartyIds {
              array<String ^> ^ get() {
                  auto unmanaged = this->_instance->getPartyIds();
                  auto elements = gcnew array<String ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew String(item.c_str());
                      index++;
                  }

                  return elements;
              }
          }

          property String ^
          ImageUri {
              String ^ get() {
                  auto unmanaged = this->_instance->getImageUri();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property ElementModQ ^
          CryptoHash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->crypto_hash();
                  return gcnew ElementModQ(std::move(unmanaged));
              }
          }
    };

  public
    ref class Party : ManagedInstance<electionguard::Party>
    {
      public:
        Party(String ^ objectId) : ManagedInstance()
        {
            std::string _objectId;

            Utilities::MarshalString(objectId, _objectId);

            this->_instance = new electionguard::Party(_objectId);
        }
        Party(String ^ objectId, String ^ abbreviation) : ManagedInstance()
        {
            std::string _objectId;
            std::string _abbreviation;

            Utilities::MarshalString(objectId, _objectId);
            Utilities::MarshalString(abbreviation, _abbreviation);

            this->_instance = new electionguard::Party(_objectId, _abbreviation);
        }
        Party(String ^ objectId, InternationalizedText ^ name, String ^ abbreviation,
              String ^ color, String ^ logoUri)
            : ManagedInstance()
        {
            std::string _objectId;
            std::string _abbreviation;
            std::string _color;
            std::string _logoUri;

            Utilities::MarshalString(objectId, _objectId);
            Utilities::MarshalString(abbreviation, _abbreviation);
            Utilities::MarshalString(color, _color);
            Utilities::MarshalString(logoUri, _logoUri);

            this->_instance = new electionguard::Party(
              _objectId, std::make_unique<electionguard::InternationalizedText>(*name->_instance),
              _abbreviation, _color, _logoUri);
        }

        internal : Party(std::unique_ptr<electionguard::Party> other)
        {
            this->_instance = other.release();
        }

        Party(electionguard::Party *unowned) : ManagedInstance(unowned, false) {}
        Party(const electionguard::Party *unowned) : ManagedInstance(unowned) {}

      public:
        property String ^
          ObjectId {
              String ^ get() {
                  auto unmanaged = this->_instance->getObjectId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property InternationalizedText ^
          Name {
              InternationalizedText ^ get() {
                  auto unmanaged = this->_instance->getName();
                  return gcnew InternationalizedText(unmanaged);
              }
          }

          property String ^
          Abbreviation {
              String ^ get() {
                  auto unmanaged = this->_instance->getAbbreviation();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property String ^
          Color {
              String ^ get() {
                  auto unmanaged = this->_instance->getColor();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property String ^
          LogoUri {
              String ^ get() {
                  auto unmanaged = this->_instance->getLogoUri();
                  return gcnew String(unmanaged.c_str());
              }
          }

          // TODO: contact information

          property ElementModQ ^
          CryptoHash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->crypto_hash();
                  return gcnew ElementModQ(std::move(unmanaged));
              }
          }
    };

  public
    ref class Candidate : ManagedInstance<electionguard::Candidate>
    {
      public:
        Candidate(String ^ objectId, bool isWriteIn) : ManagedInstance()
        {
            std::string _objectId;

            Utilities::MarshalString(objectId, _objectId);

            this->_instance = new electionguard::Candidate(_objectId, isWriteIn);
        }
        Candidate(String ^ objectId, String ^ partyId, bool isWriteIn) : ManagedInstance()
        {
            std::string _objectId;
            std::string _partyId;

            Utilities::MarshalString(objectId, _objectId);
            Utilities::MarshalString(partyId, _partyId);

            this->_instance = new electionguard::Candidate(_objectId, _partyId, isWriteIn);
        }
        Candidate(String ^ objectId, InternationalizedText ^ name, bool isWriteIn)
            : ManagedInstance()
        {
            std::string _objectId;

            Utilities::MarshalString(objectId, _objectId);

            this->_instance = new electionguard::Candidate(
              _objectId, std::make_unique<electionguard::InternationalizedText>(*name->_instance),
              isWriteIn);
        }
        Candidate(String ^ objectId, InternationalizedText ^ name, String ^ partyId, bool isWriteIn)
            : ManagedInstance()
        {
            std::string _objectId;
            std::string _partyId;

            Utilities::MarshalString(objectId, _objectId);
            Utilities::MarshalString(partyId, _partyId);

            this->_instance = new electionguard::Candidate(
              _objectId, std::make_unique<electionguard::InternationalizedText>(*name->_instance),
              _partyId, isWriteIn);
        }
        Candidate(String ^ objectId, InternationalizedText ^ name, String ^ partyId,
                  String ^ imageUri, bool isWriteIn)
            : ManagedInstance()
        {
            std::string _objectId;
            std::string _partyId;
            std::string _iamgeUri;

            Utilities::MarshalString(objectId, _objectId);
            Utilities::MarshalString(partyId, _partyId);
            Utilities::MarshalString(imageUri, _iamgeUri);

            this->_instance = new electionguard::Candidate(
              _objectId, std::make_unique<electionguard::InternationalizedText>(*name->_instance),
              _partyId, _iamgeUri, isWriteIn);
        }

        internal : Candidate(std::unique_ptr<electionguard::Candidate> other)
        {
            this->_instance = other.release();
        }

        Candidate(electionguard::Candidate *unowned) : ManagedInstance(unowned, false) {}
        Candidate(const electionguard::Candidate *unowned) : ManagedInstance(unowned) {}

      public:
        property String ^
          ObjectId {
              String ^ get() {
                  auto unmanaged = this->_instance->getObjectId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property String ^
          CandidateId {
              String ^ get() {
                  auto unmanaged = this->_instance->getCandidateId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property InternationalizedText ^
          Name {
              InternationalizedText ^ get() {
                  auto unmanaged = this->_instance->getName();
                  return gcnew InternationalizedText(unmanaged);
              }
          }

          property String ^
          PartyId {
              String ^ get() {
                  auto unmanaged = this->_instance->getPartyId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property String ^
          ImageUri {
              String ^ get() {
                  auto unmanaged = this->_instance->getImageUri();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property bool IsWriteIn
        {
            bool get() { return this->_instance->isWriteIn(); }
        }

        property ElementModQ ^ CryptoHash {
            ElementModQ ^ get() {
                auto unmanaged = this->_instance->crypto_hash();
                return gcnew ElementModQ(std::move(unmanaged));
            }
        }
    };

  public
    ref class SelectionDescription : ManagedInstance<electionguard::SelectionDescription>
    {
      public:
        SelectionDescription(String ^ objectId, String ^ candidateId, uint64_t sequenceOrder)
            : ManagedInstance()
        {
            std::string _objectId;
            std::string _candidateId;
            std::string _iamgeUri;

            Utilities::MarshalString(objectId, _objectId);
            Utilities::MarshalString(candidateId, _candidateId);

            this->_instance =
              new electionguard::SelectionDescription(_objectId, _candidateId, sequenceOrder);
        }

        internal : SelectionDescription(std::unique_ptr<electionguard::SelectionDescription> other)
        {
            this->_instance = other.release();
        }

        SelectionDescription(electionguard::SelectionDescription *unowned)
            : ManagedInstance(unowned, false)
        {
        }
        SelectionDescription(const electionguard::SelectionDescription *unowned)
            : ManagedInstance(unowned)
        {
        }

      public:
        property String ^
          ObjectId {
              String ^ get() {
                  auto unmanaged = this->_instance->getObjectId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property String ^
          CandidateId {
              String ^ get() {
                  auto unmanaged = this->_instance->getCandidateId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property uint64_t SequenceOrder
        {
            uint64_t get() { return this->_instance->getSequenceOrder(); }
        }

        property ElementModQ ^ CryptoHash {
            ElementModQ ^ get() {
                auto unmanaged = this->_instance->crypto_hash();
                return gcnew ElementModQ(std::move(unmanaged));
            }
        }
    };

  public
    ref class ContestDescription : ManagedInstance<electionguard::ContestDescription>
    {
      public:
        ContestDescription(String ^ objectId, String ^ electoralDistrictId, uint64_t sequenceOrder,
                           VoteVariationType voteVariation, uint64_t numberElected, String ^ name,
                           array<SelectionDescription ^> ^ selections)
            : ManagedInstance()
        {
            std::string _objectId;
            std::string _electoralDistrictId;
            std::string _name;

            Utilities::MarshalString(objectId, _objectId);
            Utilities::MarshalString(electoralDistrictId, _electoralDistrictId);
            Utilities::MarshalString(name, _name);

            std::vector<std::unique_ptr<electionguard::SelectionDescription>> _selections;
            for each (auto item in selections) {
                _selections.push_back(
                  std::make_unique<electionguard::SelectionDescription>(*item->_instance));
            }

            this->_instance = new electionguard::ContestDescription(
              _objectId, _electoralDistrictId, sequenceOrder,
              static_cast<electionguard::VoteVariationType>(voteVariation), numberElected, _name,
              std::move(_selections));
        }
        ContestDescription(String ^ objectId, String ^ electoralDistrictId, uint64_t sequenceOrder,
                           VoteVariationType voteVariation, uint64_t numberElected,
                           uint64_t votesAllowed, String ^ name,
                           InternationalizedText ^ ballotTitle,
                           InternationalizedText ^ ballotSubtitle,
                           array<SelectionDescription ^> ^ selections)
            : ManagedInstance()
        {
            std::string _objectId;
            std::string _electoralDistrictId;
            std::string _name;

            Utilities::MarshalString(objectId, _objectId);
            Utilities::MarshalString(electoralDistrictId, _electoralDistrictId);
            Utilities::MarshalString(name, _name);

            std::vector<std::unique_ptr<electionguard::SelectionDescription>> _selections;
            for each (auto item in selections) {
                _selections.push_back(
                  std::make_unique<electionguard::SelectionDescription>(*item->_instance));
            }

            this->_instance = new electionguard::ContestDescription(
              _objectId, _electoralDistrictId, sequenceOrder,
              static_cast<electionguard::VoteVariationType>(voteVariation), numberElected,
              votesAllowed, _name,
              std::make_unique<electionguard::InternationalizedText>(*ballotTitle->_instance),
              std::make_unique<electionguard::InternationalizedText>(*ballotSubtitle->_instance),
              std::move(_selections));
        }

        internal : ContestDescription(std::unique_ptr<electionguard::ContestDescription> other)
        {
            this->_instance = other.release();
        }

        ContestDescription(electionguard::ContestDescription *unowned)
            : ManagedInstance(unowned, false)
        {
        }
        ContestDescription(const electionguard::ContestDescription *unowned)
            : ManagedInstance(unowned)
        {
        }

      public:
        property String ^
          ObjectId {
              String ^ get() {
                  auto unmanaged = this->_instance->getObjectId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property String ^
          ElectoralDistrictId {
              String ^ get() {
                  auto unmanaged = this->_instance->getElectoralDistrictId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property uint64_t SequenceOrder
        {
            uint64_t get() { return this->_instance->getSequenceOrder(); }
        }

        property VoteVariationType VoteVariation
        {
            VoteVariationType get()
            {
                return static_cast<VoteVariationType>(this->_instance->getVoteVariation());
            }
        }

        property uint64_t NumberElected
        {
            uint64_t get() { return this->_instance->getNumberElected(); }
        }

        property uint64_t VotesAllowed
        {
            uint64_t get() { return this->_instance->getVotesAllowed(); }
        }

        property String ^
          Name {
              String ^ get() {
                  auto unmanaged = this->_instance->getName();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property InternationalizedText ^
          BallotTitle {
              InternationalizedText ^ get() {
                  auto unmanaged = this->_instance->getBallotTitle();
                  return gcnew InternationalizedText(unmanaged);
              }
          }

          property InternationalizedText ^
          BallotSubtitle {
              InternationalizedText ^ get() {
                  auto unmanaged = this->_instance->getBallotSubtitle();
                  return gcnew InternationalizedText(unmanaged);
              }
          }

          property array<SelectionDescription ^> ^
          Selections {
              array<SelectionDescription ^> ^ get() {
                  auto unmanaged = this->_instance->getSelections();
                  auto elements = gcnew array<SelectionDescription ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew SelectionDescription(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property ElementModQ ^
          CryptoHash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->crypto_hash();
                  return gcnew ElementModQ(std::move(unmanaged));
              }
          }

          bool
          IsValid()
        {
            return this->_instance->isValid();
        }
    };

  public
    ref class ContestDescriptionWithPlaceholders
        : ManagedInstance<electionguard::ContestDescriptionWithPlaceholders>
    {
      public:
        ContestDescriptionWithPlaceholders(String ^ objectId, String ^ electoralDistrictId,
                                           uint64_t sequenceOrder, VoteVariationType voteVariation,
                                           uint64_t numberElected, uint64_t votesAllowed,
                                           String ^ name, InternationalizedText ^ ballotTitle,
                                           InternationalizedText ^ ballotSubtitle,
                                           array<SelectionDescription ^> ^ selections,
                                           array<SelectionDescription ^> ^ placeholderSelections)
            : ManagedInstance()
        {
            std::string _objectId;
            std::string _electoralDistrictId;
            std::string _name;

            Utilities::MarshalString(objectId, _objectId);
            Utilities::MarshalString(electoralDistrictId, _electoralDistrictId);
            Utilities::MarshalString(name, _name);

            std::vector<std::unique_ptr<electionguard::SelectionDescription>> _selections;
            for each (auto item in selections) {
                _selections.push_back(
                  std::make_unique<electionguard::SelectionDescription>(*item->_instance));
            }

            std::vector<std::unique_ptr<electionguard::SelectionDescription>> _placeholders;
            for each (auto item in placeholderSelections) {
                _placeholders.push_back(
                  std::make_unique<electionguard::SelectionDescription>(*item->_instance));
            }

            this->_instance = new electionguard::ContestDescriptionWithPlaceholders(
              _objectId, _electoralDistrictId, sequenceOrder,
              static_cast<electionguard::VoteVariationType>(voteVariation), numberElected,
              votesAllowed, _name,
              std::make_unique<electionguard::InternationalizedText>(*ballotTitle->_instance),
              std::make_unique<electionguard::InternationalizedText>(*ballotSubtitle->_instance),
              std::move(_selections), std::move(_placeholders));
        }

        internal : ContestDescriptionWithPlaceholders(
                     std::unique_ptr<electionguard::ContestDescriptionWithPlaceholders> other)
        {
            this->_instance = other.release();
        }

        ContestDescriptionWithPlaceholders(
          electionguard::ContestDescriptionWithPlaceholders *unowned)
            : ManagedInstance(unowned, false)
        {
        }

        ContestDescriptionWithPlaceholders(
          const electionguard::ContestDescriptionWithPlaceholders *unowned)
            : ManagedInstance(unowned)
        {
        }

      public:
        property String ^
          ObjectId {
              String ^ get() {
                  auto unmanaged = this->_instance->getObjectId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property String ^
          ElectoralDistrictId {
              String ^ get() {
                  auto unmanaged = this->_instance->getElectoralDistrictId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property uint64_t SequenceOrder
        {
            uint64_t get() { return this->_instance->getSequenceOrder(); }
        }

        property VoteVariationType VoteVariation
        {
            VoteVariationType get()
            {
                return static_cast<VoteVariationType>(this->_instance->getVoteVariation());
            }
        }

        property uint64_t NumberElected
        {
            uint64_t get() { return this->_instance->getNumberElected(); }
        }

        property uint64_t VotesAllowed
        {
            uint64_t get() { return this->_instance->getVotesAllowed(); }
        }

        property String ^
          Name {
              String ^ get() {
                  auto unmanaged = this->_instance->getName();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property InternationalizedText ^
          BallotTitle {
              InternationalizedText ^ get() {
                  auto unmanaged = this->_instance->getBallotTitle();
                  return gcnew InternationalizedText(unmanaged);
              }
          }

          property InternationalizedText ^
          BallotSubtitle {
              InternationalizedText ^ get() {
                  auto unmanaged = this->_instance->getBallotSubtitle();
                  return gcnew InternationalizedText(unmanaged);
              }
          }

          property array<SelectionDescription ^> ^
          Selections {
              array<SelectionDescription ^> ^ get() {
                  auto unmanaged = this->_instance->getSelections();
                  auto elements = gcnew array<SelectionDescription ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew SelectionDescription(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property array<SelectionDescription ^> ^
          Placeholders {
              array<SelectionDescription ^> ^ get() {
                  auto unmanaged = this->_instance->getPlaceholders();
                  auto elements = gcnew array<SelectionDescription ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew SelectionDescription(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property ElementModQ ^
          CryptoHash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->crypto_hash();
                  return gcnew ElementModQ(std::move(unmanaged));
              }
          }
    };

  public
    ref class Manifest : ManagedInstance<electionguard::Manifest>
    {
      public:
        Manifest(String ^ electionScopeId, ElectionType type, DateTime startDate, DateTime endDate,
                 array<GeopoliticalUnit ^> ^ geopoliticalUnits, array<Party ^> ^ parties,
                 array<Candidate ^> ^ candidates, array<ContestDescription ^> ^ contests,
                 array<BallotStyle ^> ^ ballotStyles)
            : ManagedInstance()
        {
            std::string _electionScopeId;
            Utilities::MarshalString(electionScopeId, _electionScopeId);

            auto nativeStartDate = Utilities::ToNativeTime(startDate);
            auto nativeEndDate = Utilities::ToNativeTime(startDate);

            std::vector<std::unique_ptr<electionguard::GeopoliticalUnit>> _gpUnits;
            for each (auto item in geopoliticalUnits) {
                _gpUnits.push_back(
                  std::make_unique<electionguard::GeopoliticalUnit>(*item->_instance));
            }

            std::vector<std::unique_ptr<electionguard::Party>> _parties;
            for each (auto item in parties) {
                _parties.push_back(std::make_unique<electionguard::Party>(*item->_instance));
            }

            std::vector<std::unique_ptr<electionguard::Candidate>> _candidates;
            for each (auto item in candidates) {
                _candidates.push_back(std::make_unique<electionguard::Candidate>(*item->_instance));
            }

            std::vector<std::unique_ptr<electionguard::ContestDescription>> _contests;
            for each (auto item in contests) {
                _contests.push_back(
                  std::make_unique<electionguard::ContestDescription>(*item->_instance));
            }

            std::vector<std::unique_ptr<electionguard::BallotStyle>> _ballotStyles;
            for each (auto item in ballotStyles) {
                _ballotStyles.push_back(
                  std::make_unique<electionguard::BallotStyle>(*item->_instance));
            }

            this->_instance = new electionguard::Manifest(
              _electionScopeId, static_cast<electionguard::ElectionType>(type), nativeStartDate,
              nativeEndDate,
              std::move(_gpUnits), std::move(_parties), std::move(_candidates),
              std::move(_contests), std::move(_ballotStyles));
        }
        Manifest(String ^ json) : ManagedInstance()
        {
            std::string _json;
            Utilities::MarshalString(json, _json);
            auto unmanaged = electionguard::Manifest::fromJson(_json);
            this->_instance = unmanaged.release();
        }
        internal : Manifest(std::unique_ptr<electionguard::Manifest> other)
        {
            this->_instance = other.release();
        }
        Manifest(electionguard::Manifest *unowned) : ManagedInstance(unowned, false) {}

        Manifest(const electionguard::Manifest *unowned) : ManagedInstance(unowned) {}

      public:
        property String ^
          ElectionScopeId {
              String ^ get() {
                  auto unmanaged = this->_instance->getElectionScopeId();
                  return gcnew String(unmanaged.c_str());
              }
          }

          property ElectionType VoteVariation
        {
            ElectionType get()
            {
                return static_cast<ElectionType>(this->_instance->getElectionType());
            }
        }

        // TODO: datetime fields

        property array<GeopoliticalUnit ^> ^
          GeopoliticalUnits {
              array<GeopoliticalUnit ^> ^ get() {
                  auto unmanaged = this->_instance->getGeopoliticalUnits();
                  auto elements = gcnew array<GeopoliticalUnit ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew GeopoliticalUnit(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property array<Party ^> ^
          Parties {
              array<Party ^> ^ get() {
                  auto unmanaged = this->_instance->getParties();
                  auto elements = gcnew array<Party ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew Party(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property array<Candidate ^> ^
          Candidates {
              array<Candidate ^> ^ get() {
                  auto unmanaged = this->_instance->getCandidates();
                  auto elements = gcnew array<Candidate ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew Candidate(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property array<ContestDescription ^> ^
          Contests {
              array<ContestDescription ^> ^ get() {
                  auto unmanaged = this->_instance->getContests();
                  auto elements = gcnew array<ContestDescription ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew ContestDescription(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property array<BallotStyle ^> ^
          BallotStyles {
              array<BallotStyle ^> ^ get() {
                  auto unmanaged = this->_instance->getBallotStyles();
                  auto elements = gcnew array<BallotStyle ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew BallotStyle(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          // TODO: name and contact info

          bool
          IsValid()
        {
            return this->_instance->isValid();
        }

          property array<Byte> ^
          Bson {
              array<Byte> ^ get() {
                  auto unmanaged = this->_instance->toBson();

                  array<Byte> ^ byteArray = gcnew array<Byte>(unmanaged.size());
                  Marshal::Copy((IntPtr)unmanaged.data(), byteArray, 0, unmanaged.size());
                  return byteArray;
              }
          }

          property String ^
          Json {
              String ^ get() {
                  auto unmanaged = this->_instance->toJson();
                  return gcnew String(unmanaged.c_str());
              }
          }

          static Manifest ^
          FromJson(String ^ json) {
              std::string _json;
              Utilities::MarshalString(json, _json);
              auto unamanaged = electionguard::Manifest::fromJson(_json);
              return gcnew Manifest(move(unamanaged));
          }

          property ElementModQ
          ^ CryptoHash {
                ElementModQ ^ get() {
                    auto unmanaged = this->_instance->crypto_hash();
                    return gcnew ElementModQ(std::move(unmanaged));
                }
            }
    };

  public
    ref class InternalManifest : ManagedInstance<electionguard::InternalManifest>
    {
      public:
        InternalManifest(array<GeopoliticalUnit ^> ^ geopoliticalUnits,
                         array<ContestDescriptionWithPlaceholders ^> ^ contests,
                         array<BallotStyle ^> ^ ballotStyles, ElementModQ ^ manifestHash)
            : ManagedInstance()
        {
            std::vector<std::unique_ptr<electionguard::GeopoliticalUnit>> _gpUnits;
            for each (auto item in geopoliticalUnits) {
                _gpUnits.push_back(
                  std::make_unique<electionguard::GeopoliticalUnit>(*item->_instance));
            }

            std::vector<std::unique_ptr<electionguard::ContestDescriptionWithPlaceholders>>
              _contests;
            for each (auto item in contests) {
                _contests.push_back(
                  std::make_unique<electionguard::ContestDescriptionWithPlaceholders>(
                    *item->_instance));
            }

            std::vector<std::unique_ptr<electionguard::BallotStyle>> _ballotStyles;
            for each (auto item in ballotStyles) {
                _ballotStyles.push_back(
                  std::make_unique<electionguard::BallotStyle>(*item->_instance));
            }

            this->_instance = new electionguard::InternalManifest(
              std::move(_gpUnits), std::move(_contests), std::move(_ballotStyles),
              *manifestHash->_instance);
        }
        InternalManifest(Manifest ^ manifest)
            : ManagedInstance(new electionguard::InternalManifest(*manifest->_instance))
        {
        }

        internal : InternalManifest(std::unique_ptr<electionguard::InternalManifest> other)
        {
            this->_instance = other.release();
        }
        InternalManifest(electionguard::InternalManifest *unowned) : ManagedInstance(unowned, false)
        {
        }

        InternalManifest(const electionguard::InternalManifest *unowned) : ManagedInstance(unowned)
        {
        }

      public:
        property ElementModQ ^
          ManifestHash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->getManifestHash();
                  return gcnew ElementModQ(unmanaged);
              }
          }

          property array<GeopoliticalUnit ^> ^
          GeopoliticalUnits {
              array<GeopoliticalUnit ^> ^ get() {
                  auto unmanaged = this->_instance->getGeopoliticalUnits();
                  auto elements = gcnew array<GeopoliticalUnit ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew GeopoliticalUnit(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property array<ContestDescriptionWithPlaceholders ^> ^
          Contests {
              array<ContestDescriptionWithPlaceholders ^> ^ get() {
                  auto unmanaged = this->_instance->getContests();
                  auto elements =
                    gcnew array<ContestDescriptionWithPlaceholders ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew ContestDescriptionWithPlaceholders(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property array<BallotStyle ^> ^
          BallotStyles {
              array<BallotStyle ^> ^ get() {
                  auto unmanaged = this->_instance->getBallotStyles();
                  auto elements = gcnew array<BallotStyle ^>(unmanaged.size());

                  int index = 0;
                  for (const auto &item : unmanaged) {
                      elements[index] = gcnew BallotStyle(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          BallotStyle ^
          GetBallotStyle(String ^ ballotStyleId) {
              std::string unmanagedParam;
              Utilities::MarshalString(ballotStyleId, unmanagedParam);
              auto unmanaged = this->_instance->getBallotStyle(unmanagedParam);
              return gcnew BallotStyle(unmanaged);
          }

          array<ContestDescriptionWithPlaceholders ^> ^
          GetContestsFor(String ^ ballotStyleId) {
              std::string unmanagedParam;
              Utilities::MarshalString(ballotStyleId, unmanagedParam);
              auto unmanaged = this->_instance->getContestsFor(unmanagedParam);
              auto elements = gcnew array<ContestDescriptionWithPlaceholders ^>(unmanaged.size());

              int index = 0;
              for (const auto &item : unmanaged) {
                  elements[index] = gcnew ContestDescriptionWithPlaceholders(&item.get());
                  index++;
              }

              return elements;
          }

          property array<Byte> ^
          Bson {
              array<Byte> ^ get() {
                  auto unmanaged = this->_instance->toBson();

                  array<Byte> ^ byteArray = gcnew array<Byte>(unmanaged.size());
                  Marshal::Copy((IntPtr)unmanaged.data(), byteArray, 0, unmanaged.size());
                  return byteArray;
              }
          }

          property String
          ^
          Json {
              String ^ get() {
                  auto unmanaged = this->_instance->toJson();
                  return gcnew String(unmanaged.c_str());
              }
          }

          static InternalManifest
          ^ FromJson(String ^ json) {
                std::string _json;
                Utilities::MarshalString(json, _json);
                auto unmanaged = electionguard::InternalManifest::fromJson(_json);
                return gcnew InternalManifest(move(unmanaged));
            }
    };

} // namespace ElectionGuardInterop
