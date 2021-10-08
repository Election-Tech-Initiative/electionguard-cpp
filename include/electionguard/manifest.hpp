#ifndef __ELECTIONGUARD_CPP_MANIFEST_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_MANIFEST_HPP_INCLUDED__

#include "crypto_hashable.hpp"
#include "election_object_base.hpp"
#include "export.h"
#include "group.hpp"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace electionguard
{

    /// <Summary>
    /// Enumeration for the type of ElectionType
    /// see: https://developers.google.com/elections-data/reference/election-type
    /// </Summary>
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

    /// <Summary>
    /// Get a string representation of a ElectionType enum
    /// </Summary>
    EG_API std::string getElectionTypeString(const ElectionType &value);

    /// <Summary>
    /// Get a ElectionType enum from a string.
    /// <returns>ElectionType.unknown if the value cannot be resolved</returns>
    /// </Summary>
    EG_API ElectionType getElectionType(const std::string &value);

    /// <Summary>
    /// Enumeration for the type of geopolitical unit
    /// see: https://developers.google.com/elections-data/reference/reporting-unit-type
    /// </Summary>
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

    /// <Summary>
    /// Get a string representation of a ReportingUnitType enum
    /// </Summary>
    EG_API std::string getReportingUnitTypeString(const ReportingUnitType &value);

    /// <Summary>
    /// Get a ReportingUnitType enum from a string.
    /// <returns>ReportingUnitType.unknown if the value cannot be resolved</returns>
    /// </Summary>
    EG_API ReportingUnitType getReportingUnitType(const std::string &value);

    /// <Summary>
    /// Enumeration for the type of VoteVariationType
    /// see: https://developers.google.com/elections-data/reference/vote-variation
    /// </Summary>
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

    /// <Summary>
    /// Get a string representation of a VoteVariationType enum
    /// </Summary>
    EG_API std::string getVoteVariationTypeString(const VoteVariationType &value);

    /// <Summary>
    /// Get a VoteVariationType enum from a string.
    /// <returns>VoteVariationType.unknown if the value cannot be resolved</returns>
    /// </Summary>
    EG_API VoteVariationType getVoteVariationType(const std::string &value);

    /// <Summary>
    /// Use this as a type for character strings.
    /// See: https://developers.google.com/elections-data/reference/annotated-string
    /// </Summary>
    class EG_API AnnotatedString : public CryptoHashable
    {
      public:
        AnnotatedString(const AnnotatedString &other);
        AnnotatedString(AnnotatedString &&other);
        explicit AnnotatedString(std::string annotation, std::string value);
        ~AnnotatedString();

        AnnotatedString &operator=(const AnnotatedString &other);
        AnnotatedString &operator=(AnnotatedString &&other) noexcept;

        /// <Summary>
        /// An annotation of up to 16 characters that's associated with a character string.
        /// </Summary>
        std::string getAnnotation() const;

        /// <Summary>
        /// The character string
        /// </Summary>
        std::string getValue() const;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() override;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// The ISO-639 language
    /// see: https://en.wikipedia.org/wiki/ISO_639
    /// see: https://developers.google.com/civics-data/reference/internationalized-text#language-string
    /// </Summary>
    class EG_API Language : public CryptoHashable
    {
      public:
        Language(const Language &other);
        Language(Language &&other);
        explicit Language(std::string value, std::string language);
        ~Language();

        Language &operator=(const Language &other);
        Language &operator=(Language &&other) noexcept;

        /// <Summary>
        /// The value
        /// </Summary>
        std::string getValue() const;

        /// <Summary>
        /// Identifies the language
        /// </Summary>
        std::string getLanguage() const;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() override;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// Data entity used to represent multi-national text. Use when text on a ballot contains multi-national text.
    /// See: https://developers.google.com/elections-data/reference/internationalized-text
    /// </Summary>
    class EG_API InternationalizedText : public CryptoHashable
    {
      public:
        InternationalizedText(const InternationalizedText &other);
        InternationalizedText(InternationalizedText &&other);
        explicit InternationalizedText(std::vector<std::unique_ptr<Language>> text);
        ~InternationalizedText();

        InternationalizedText &operator=(InternationalizedText other);
        InternationalizedText &operator=(InternationalizedText &&other);

        /// <Summary>
        /// A string of possibly non-English text.
        /// </Summary>
        std::vector<std::reference_wrapper<Language>> getText() const;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() override;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// For defining contact information about objects such as persons, boards of authorities, and organizations.
    ///
    /// Contact Information values are not used internally by ElectionGuard when encrypting ballots
    /// but are included for checking the validity of a supplied manifest.
    ///
    /// See: https://developers.google.com/elections-data/reference/contact-information
    /// </Summary>
    class EG_API ContactInformation : public CryptoHashable
    {
      public:
        ContactInformation(const ContactInformation &other);
        ContactInformation(ContactInformation &&other);
        explicit ContactInformation(std::string name);
        explicit ContactInformation(std::vector<std::string> addressLine,
                                    std::string name = nullptr);
        explicit ContactInformation(std::vector<std::string> addressLine,
                                    std::vector<std::unique_ptr<AnnotatedString>> phone,
                                    std::string name = nullptr);
        explicit ContactInformation(std::vector<std::string> addressLine,
                                    std::vector<std::unique_ptr<AnnotatedString>> email,
                                    std::vector<std::unique_ptr<AnnotatedString>> phone,
                                    std::string name = nullptr);
        ~ContactInformation();

        ContactInformation &operator=(ContactInformation other);
        ContactInformation &operator=(ContactInformation &&other);

        /// <Summary>
        /// Associates an address with the contact.
        /// AddressLine needs to contain the lines that someone would
        /// enter into a web mapping service to find the address on a map.
        /// That is, the value of the field needs to geocode the contact location.
        /// </Summary>
        std::vector<std::string> getAddressLine() const;

        /// <Summary>
        /// Associates an email address with the contact.
        /// </Summary>
        std::vector<std::reference_wrapper<AnnotatedString>> getEmail() const;

        /// <Summary>
        /// Associates a phone number with the contact.
        /// </Summary>
        std::vector<std::reference_wrapper<AnnotatedString>> getPhone() const;
        std::string getName() const;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() override;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// A geopolitical unit describes any physical or
    /// virtual unit of representation or vote/seat aggregation.
    /// Use this entity for defining geopolitical units such as
    /// cities, districts, jurisdictions, or precincts,
    /// for the purpose of associating contests, offices, vote counts,
    /// or other information with the geographies.
    ///
    /// Geopolitical Units are not used when encrypting ballots but are required by
    /// ElectionGuard to determine the validity of ballot styles.
    ///
    /// See: https://developers.google.com/elections-data/reference/gp-unit
    /// </Summary>
    class EG_API GeopoliticalUnit : public CryptoHashable
    {
      public:
        GeopoliticalUnit(const GeopoliticalUnit &other);
        GeopoliticalUnit(GeopoliticalUnit &&other);
        explicit GeopoliticalUnit(const std::string &objectId, const std::string &name,
                                  const ReportingUnitType type);
        explicit GeopoliticalUnit(const std::string &objectId, const std::string &name,
                                  const ReportingUnitType type,
                                  std::unique_ptr<ContactInformation> contactInformation);
        ~GeopoliticalUnit();

        GeopoliticalUnit &operator=(GeopoliticalUnit other);
        GeopoliticalUnit &operator=(GeopoliticalUnit &&other);

        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        std::string getObjectId() const;

        /// <Summary>
        /// Name of the geopolitical unit.
        /// </Summary>
        std::string getName() const;

        /// <Summary>
        /// The type of reporting unit
        /// </Summary>
        ReportingUnitType getType() const;

        ContactInformation *getContactInformation() const;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() override;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// A BallotStyle works as a key to uniquely specify a set of contests. See also `ContestDescription`.
    ///
    /// For ElectionGuard, each contest is associated with a specific geopolitical unit,
    /// and each ballot style is associated with at least one geopolitical unit.
    ///
    /// It is up to the consuming application to determine how to interpreit the relationships
    /// between these entity types.
    /// </Summary>
    class EG_API BallotStyle : public CryptoHashable
    {
      public:
        BallotStyle(const BallotStyle &other);
        BallotStyle(BallotStyle &&other);
        explicit BallotStyle(const std::string &objectId,
                             std::vector<std::string> geopoliticalUnitIds);
        explicit BallotStyle(const std::string &objectId,
                             std::vector<std::string> geopoliticalUnitIds,
                             std::vector<std::string> partyIds, const std::string &imageUri);
        ~BallotStyle();

        BallotStyle &operator=(BallotStyle other);
        BallotStyle &operator=(BallotStyle &&other);

        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        std::string getObjectId() const;

        /// <Summary>
        /// the Geopolitical Unit Id or id's that correlate to this ballot style
        /// </Summary>
        std::vector<std::string> getGeopoliticalUnitIds() const;

        /// <Summary>
        /// the Party Id or Id's (if any) for this ballot style
        /// </Summary>
        std::vector<std::string> getPartyIds() const;

        /// <Summary>
        /// The image uri
        /// </Summary>
        std::string getImageUri() const;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() override;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// Use this entity to describe a political party that can then be referenced from other entities.
    ///
    /// It is not required to define a party for Electionguard.
    ///
    /// See: https://developers.google.com/elections-data/reference/party
    /// </Summary>
    class EG_API Party : public CryptoHashable
    {
      public:
        Party(const Party &other);
        Party(Party &&other);
        explicit Party(const std::string &objectId);
        explicit Party(const std::string &objectId, const std::string &abbreviation);
        explicit Party(const std::string &objectId, std::unique_ptr<InternationalizedText> name,
                       const std::string &abbreviation, const std::string &color,
                       const std::string &logoUri);
        ~Party();

        Party &operator=(Party other);
        Party &operator=(Party &&other);

        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        std::string getObjectId() const;

        /// <Summary>
        /// Name of the party
        /// </Summary>
        InternationalizedText *getName() const;

        /// <Summary>
        /// Abbreviation of the party
        /// </Summary>
        std::string getAbbreviation() const;

        /// <Summary>
        /// An optional color in hex
        /// </Summary>
        std::string getColor() const;

        /// <Summary>
        /// An optional logo uri
        /// </Summary>
        std::string getLogoUri() const;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() override;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// Entity describing information about a candidate in a contest.
    /// See: https://developers.google.com/elections-data/reference/candidate
    ///
    /// Note: The ElectionGuard Data Spec deviates from the NIST model in that
    /// selections for any contest type are considered a "candidate".
    /// for instance, on a yes-no referendum contest, two `candidate` objects
    /// would be included in the model to represent the `affirmative` and `negative`
    /// selections for the contest.  See the wiki, readme's, and tests in this repo for more info.
    /// </Summary>
    class EG_API Candidate : public CryptoHashable
    {
      public:
        Candidate(const Candidate &other);
        Candidate(Candidate &&other);
        explicit Candidate(const std::string &objectId, bool isWriteIn = false);
        explicit Candidate(const std::string &objectId, const std::string &partyId, bool isWriteIn);
        explicit Candidate(const std::string &objectId, std::unique_ptr<InternationalizedText> name,
                           bool isWriteIn);
        explicit Candidate(const std::string &objectId, std::unique_ptr<InternationalizedText> name,
                           const std::string &partyId, bool isWriteIn);
        explicit Candidate(const std::string &objectId, std::unique_ptr<InternationalizedText> name,
                           const std::string &partyId, const std::string &imageUri, bool isWriteIn);
        ~Candidate();

        Candidate &operator=(Candidate other);
        Candidate &operator=(Candidate &&other);

        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        std::string getObjectId() const;

        /// <Summary>
        /// A convenience accessor for getObjectId
        /// </Summary>
        std::string getCandidateId() const;

        /// <Summary>
        /// Name of the candidate
        /// </Summary>
        InternationalizedText *getName() const;

        /// <Summary>
        /// Optional party id of the candidate
        /// </Summary>
        std::string getPartyId() const;

        /// <Summary>
        /// Optional image uri for the candidate
        /// </Summary>
        std::string getImageUri() const;

        /// <Summary>
        /// Does the candidate support write-ins?
        /// </Summary>
        bool isWriteIn() const;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() override;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// Data entity for the ballot selections in a contest,
    /// for example linking candidates and parties to their vote counts.
    /// See: https://developers.google.com/elections-data/reference/ballot-selection
    ///
    /// Note: The ElectionGuard Data Spec deviates from the NIST model in that
    /// there is no difference for different types of selections.
    ///
    /// The ElectionGuard Data Spec deviates from the NIST model in that
    /// `sequence_order` is a required field since it is used for ordering selections
    /// in a contest to ensure various encryption primitives are deterministic.
    /// For a given election, the sequence of selections displayed to a user may be different
    /// however that information is not captured by default when encrypting a specific ballot.
    /// </summary>
    class EG_API SelectionDescription : public CryptoHashable
    {
      public:
        SelectionDescription(const SelectionDescription &other);
        SelectionDescription(SelectionDescription &&other);
        explicit SelectionDescription(const std::string &objectId, const std::string &candidateId,
                                      const uint64_t sequenceOrder);
        ~SelectionDescription();

        SelectionDescription &operator=(SelectionDescription other);
        SelectionDescription &operator=(SelectionDescription &&other);

        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        std::string getObjectId() const;

        /// <Summary>
        /// the object id of the candidate
        /// </Summary>
        std::string getCandidateId() const;

        /// <Summary>
        /// The sequence order defining this selections place in the contest selection collection.
        /// Note: this is specifically for programs to interpret and does not necessarily represent
        /// the order in which selections are presented to a user.
        /// </Summary>
        uint64_t getSequenceOrder() const;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() override;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// Use this data entity for describing a contest and linking the contest
    /// to the associated candidates and parties.
    /// See: https://developers.google.com/elections-data/reference/contest
    /// Note: The ElectionGuard Data Spec deviates from the NIST model in that
    /// `sequence_order` is a required field since it is used for ordering selections
    /// in a contest to ensure various encryption primitives are deterministic.
    /// For a given election, the sequence of contests displayed to a user may be different
    /// however that information is not captured by default when encrypting a specific ballot.
    /// </summary>
    class EG_API ContestDescription : public CryptoHashable
    {
      public:
        ContestDescription(const ContestDescription &other);
        ContestDescription(ContestDescription &&other);
        explicit ContestDescription(const std::string &objectId,
                                    const std::string &electoralDistrictId,
                                    const uint64_t sequenceOrder,
                                    const VoteVariationType voteVariation,
                                    const uint64_t numberElected, const std::string &name,
                                    std::vector<std::unique_ptr<SelectionDescription>> selections);
        explicit ContestDescription(const std::string &objectId,
                                    const std::string &electoralDistrictId,
                                    const uint64_t sequenceOrder,
                                    const VoteVariationType voteVariation,
                                    const uint64_t numberElected, const std::string &name,
                                    std::vector<std::unique_ptr<SelectionDescription>> selections,
                                    std::vector<std::string> primaryPartyIds);
        explicit ContestDescription(const std::string &objectId,
                                    const std::string &electoralDistrictId,
                                    const uint64_t sequenceOrder,
                                    const VoteVariationType voteVariation,
                                    const uint64_t numberElected, const uint64_t votesAllowed,
                                    const std::string &name,
                                    std::unique_ptr<InternationalizedText> ballotTitle,
                                    std::unique_ptr<InternationalizedText> ballotSubtitle,
                                    std::vector<std::unique_ptr<SelectionDescription>> selections);
        explicit ContestDescription(const std::string &objectId,
                                    const std::string &electoralDistrictId,
                                    const uint64_t sequenceOrder,
                                    const VoteVariationType voteVariation,
                                    const uint64_t numberElected, const uint64_t votesAllowed,
                                    const std::string &name,
                                    std::unique_ptr<InternationalizedText> ballotTitle,
                                    std::unique_ptr<InternationalizedText> ballotSubtitle,
                                    std::vector<std::unique_ptr<SelectionDescription>> selections,
                                    std::vector<std::string> primaryPartyIds);
        ~ContestDescription();

        ContestDescription &operator=(ContestDescription other);
        ContestDescription &operator=(ContestDescription &&other);

        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        std::string getObjectId() const;

        /// <Summary>
        /// The object id of the geopolitical unit associated with this contest.
        /// Note: in concordance with the NIST standard, the name `ElectoralDistrictId` is kept
        /// </Summary>
        std::string getElectoralDistrictId() const;

        /// <Summary>
        /// The sequence order defining this contest's place in the contest collection of the ballot style.
        /// Note: this is specifically for programs to interpret and does not necessarily represent
        /// the order in which contests are presented to a user.
        /// </Summary>
        uint64_t getSequenceOrder() const;

        /// <Summary>
        /// The vote variation type.  Currently ElectionGuard supports one_of_m and n_of_m
        /// </Summary>
        VoteVariationType getVoteVariation() const;

        /// <Summary>
        /// The number of candidates that are elected in the contest, which is the n of an n-of-m contest
        /// </Summary>
        uint64_t getNumberElected() const;

        /// <Summary>
        /// The maximum number of votes or write-ins allowed per voter in this contest.
        /// </Summary>
        uint64_t getVotesAllowed() const;

        /// <Summary>
        /// Name of the contest as it's listed on the results report,
        /// not necessarily as it appears on the ballot.
        /// </Summary>
        std::string getName() const;

        /// <Summary>
        /// Title of the contest, which must match how it appears on the voters' ballots.
        /// </Summary>
        InternationalizedText *getBallotTitle() const;

        /// <Summary>
        /// Subtitle of the contest, which must match how it appears on the voters' ballots.
        /// </Summary>
        InternationalizedText *getBallotSubtitle() const;

        /// <Summary>
        /// The collection of selections in this contest.
        /// </Summary>
        std::vector<std::reference_wrapper<SelectionDescription>> getSelections() const;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() override;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

        bool isValid() const;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// ContestDescriptionWithPlaceholders is a `ContestDescription` with ElectionGuard `placeholder_selections`.
    /// (The ElectionGuard spec requires for n-of-m elections that there be *exactly* n counters that are one
    /// with the rest zero, so if a voter deliberately undervotes, one or more of the placeholder counters will
    /// become one. This allows the `ConstantChaumPedersenProof` to verify correctly for undervoted contests.)
    /// </summary>
    class EG_API ContestDescriptionWithPlaceholders : public ContestDescription
    {
      public:
        ContestDescriptionWithPlaceholders(const ContestDescriptionWithPlaceholders &other);
        ContestDescriptionWithPlaceholders(ContestDescriptionWithPlaceholders &&other);
        explicit ContestDescriptionWithPlaceholders(
          const ContestDescription &other,
          std::vector<std::unique_ptr<SelectionDescription>> placeholderSelections);
        explicit ContestDescriptionWithPlaceholders(
          const std::string &objectId, const std::string &electoralDistrictId,
          const uint64_t sequenceOrder, const VoteVariationType voteVariation,
          const uint64_t numberElected, const std::string &name,
          std::vector<std::unique_ptr<SelectionDescription>> selections,
          std::vector<std::unique_ptr<SelectionDescription>> placeholderSelections);
        explicit ContestDescriptionWithPlaceholders(
          const std::string &objectId, const std::string &electoralDistrictId,
          const uint64_t sequenceOrder, const VoteVariationType voteVariation,
          const uint64_t numberElected, const std::string &name,
          std::vector<std::unique_ptr<SelectionDescription>> selections,
          std::vector<std::string> primaryPartyIds,
          std::vector<std::unique_ptr<SelectionDescription>> placeholderSelections);
        explicit ContestDescriptionWithPlaceholders(
          const std::string &objectId, const std::string &electoralDistrictId,
          const uint64_t sequenceOrder, const VoteVariationType voteVariation,
          const uint64_t numberElected, const uint64_t votesAllowed, const std::string &name,
          std::unique_ptr<InternationalizedText> ballotTitle,
          std::unique_ptr<InternationalizedText> ballotSubtitle,
          std::vector<std::unique_ptr<SelectionDescription>> selections,
          std::vector<std::unique_ptr<SelectionDescription>> placeholderSelections);
        explicit ContestDescriptionWithPlaceholders(
          const std::string &objectId, const std::string &electoralDistrictId,
          const uint64_t sequenceOrder, const VoteVariationType voteVariation,
          const uint64_t numberElected, const uint64_t votesAllowed, const std::string &name,
          std::unique_ptr<InternationalizedText> ballotTitle,
          std::unique_ptr<InternationalizedText> ballotSubtitle,
          std::vector<std::unique_ptr<SelectionDescription>> selections,
          std::vector<std::string> primaryPartyIds,
          std::vector<std::unique_ptr<SelectionDescription>> placeholderSelections);
        ~ContestDescriptionWithPlaceholders();

        ContestDescriptionWithPlaceholders &operator=(ContestDescriptionWithPlaceholders other);
        ContestDescriptionWithPlaceholders &operator=(ContestDescriptionWithPlaceholders &&other);

        /// <Summary>
        /// The collection of placeholder selections in this contest.  Order is not guaranteed.
        /// </Summary>
        std::vector<std::reference_wrapper<SelectionDescription>> getPlaceholders() const;

        /// <Summary>
        /// Check if a specific selection is a placeholder.
        /// </Summary>
        bool IsPlaceholder(SelectionDescription &selection) const;

        // TODO: bool isValid() const;

        /// <Summary>
        /// Get a selection for a specific selection id.
        /// </Summary>
        std::reference_wrapper<SelectionDescription> selectionFor(std::string &selectionId);

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// Use this entity for defining the structure of the election and associated
    /// information such as candidates, contests, and vote counts.  This class is
    /// based on the NIST Election Common Standard Data Specification.  Some deviations
    /// from the standard exist.
    ///
    /// This structure is considered an immutable input object and should not be changed
    /// through the course of an election, as it's hash representation is the basis for all
    /// other hash representations within an ElectionGuard election context.
    ///
    /// See: https://developers.google.com/elections-data/reference/election
    /// </summary>
    class EG_API Manifest : public CryptoHashable
    {
      public:
        Manifest(const Manifest &other);
        Manifest(Manifest &&other);
        explicit Manifest(const std::string &electionScopeId, ElectionType type,
                          std::chrono::system_clock::time_point startDate,
                          std::chrono::system_clock::time_point endDate,
                          std::vector<std::unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
                          std::vector<std::unique_ptr<Party>> parties,
                          std::vector<std::unique_ptr<Candidate>> candidates,
                          std::vector<std::unique_ptr<ContestDescription>> contests,
                          std::vector<std::unique_ptr<BallotStyle>> ballotStyles);
        explicit Manifest(const std::string &electionScopeId, ElectionType type,
                          std::chrono::system_clock::time_point startDate,
                          std::chrono::system_clock::time_point endDate,
                          std::vector<std::unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
                          std::vector<std::unique_ptr<Party>> parties,
                          std::vector<std::unique_ptr<Candidate>> candidates,
                          std::vector<std::unique_ptr<ContestDescription>> contests,
                          std::vector<std::unique_ptr<BallotStyle>> ballotStyles,
                          std::unique_ptr<InternationalizedText> name,
                          std::unique_ptr<ContactInformation> contactInformation);
        ~Manifest();

        Manifest &operator=(Manifest other);
        Manifest &operator=(Manifest &&other);

        /// <Summary>
        /// Unique identifier for a GpUnit element. Associates the election with
        /// a reporting unit that represents the geographical scope of the election,
        /// such as a state or city.
        /// </Summary>
        std::string getElectionScopeId() const;

        /// <Summary>
        /// Enumerated type of election, such as partisan-primary or open-primary.
        /// </Summary>
        ElectionType getElectionType() const;

        /// <Summary>
        /// The start date/time of the election.
        /// </Summary>
        std::chrono::system_clock::time_point getStartDate() const;

        /// <Summary>
        /// The end date/time of the election.
        /// </Summary>
        std::chrono::system_clock::time_point getEndDate() const;

        /// <Summary>
        /// Collection of geopolitical units for this election.
        /// </Summary>
        std::vector<std::reference_wrapper<GeopoliticalUnit>> getGeopoliticalUnits() const;

        /// <Summary>
        /// Collection of parties for this election.
        /// </Summary>
        std::vector<std::reference_wrapper<Party>> getParties() const;

        /// <Summary>
        /// Collection of candidates for this election.
        /// </Summary>
        std::vector<std::reference_wrapper<Candidate>> getCandidates() const;

        /// <Summary>
        /// Collection of contests for this election.
        /// </Summary>
        std::vector<std::reference_wrapper<ContestDescription>> getContests() const;

        /// <Summary>
        /// Collection of ballot styles for this election.
        /// </Summary>
        std::vector<std::reference_wrapper<BallotStyle>> getBallotStyles() const;

        /// <Summary>
        /// The friendly name of the election
        /// </Summary>
        InternationalizedText *getName() const;
        ContactInformation *getContactInformation() const;

        bool isValid() const;

        /// <Summary>
        /// Export the representation as BSON
        /// </Summary>
        std::vector<uint8_t> toBson() const;

        /// <Summary>
        /// Export the representation as MsgPack
        /// </Summary>
        std::vector<uint8_t> toMsgPack() const;

        /// <Summary>
        /// Export the representation as JSON
        /// </Summary>
        std::string toJson();

        /// <Summary>
        /// Export the representation as JSON
        /// </Summary>
        std::string toJson() const;

        /// <Summary>
        /// Import the representation from JSON
        /// </Summary>
        static std::unique_ptr<Manifest> fromJson(std::string data);

        /// <Summary>
        /// Import the representation from BSON
        /// </Summary>
        static std::unique_ptr<Manifest> fromBson(std::vector<uint8_t> data);

        /// <Summary>
        /// Import the representation from MsgPack
        /// </Summary>
        static std::unique_ptr<Manifest> fromMsgPack(std::vector<uint8_t> data);

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() override;

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// `InternalManifest` is a subset of the `Manifest` structure that specifies
    /// the components that ElectionGuard uses for conducting an election.  The key component is the
    /// `contests` collection, which applies placeholder selections to the `Manifest` contests
    /// </summary>
    class EG_API InternalManifest
    {
      public:
        InternalManifest(const InternalManifest &other);
        InternalManifest(InternalManifest &&other);
        explicit InternalManifest(
          std::vector<std::unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
          std::vector<std::unique_ptr<ContestDescriptionWithPlaceholders>> contests,
          std::vector<std::unique_ptr<BallotStyle>> ballotStyles, const ElementModQ &manifestHash);
        InternalManifest(const Manifest &description);
        ~InternalManifest();

        InternalManifest &operator=(InternalManifest other);
        InternalManifest &operator=(InternalManifest &&other);

        /// <summary>
        /// The hash of the election metadata
        /// </summary>
        const ElementModQ *getManifestHash() const;

        /// <Summary>
        /// Collection of geopolitical units for this election.
        /// </Summary>
        std::vector<std::reference_wrapper<GeopoliticalUnit>> getGeopoliticalUnits() const;

        /// <Summary>
        /// Collection of contests for this election.
        /// </Summary>
        std::vector<std::reference_wrapper<ContestDescriptionWithPlaceholders>> getContests() const;

        /// <Summary>
        /// Collection of ballot styles for this election.
        /// </Summary>
        std::vector<std::reference_wrapper<BallotStyle>> getBallotStyles() const;

        /// <Summary>
        /// Get a ballot style for a given ballot style id
        /// </Summary>
        BallotStyle *getBallotStyle(const std::string &ballotStyleId) const;

        /// <Summary>
        /// Collection of contests for a given ballot style id
        /// </Summary>
        std::vector<std::reference_wrapper<ContestDescriptionWithPlaceholders>>
        getContestsFor(const std::string &ballotStyleId) const;

        /// <Summary>
        /// Export the ballot representation as BSON
        /// </Summary>
        std::vector<uint8_t> toBson() const;

        /// <Summary>
        /// Export the representation as MsgPack
        /// </Summary>
        std::vector<uint8_t> toMsgPack() const;

        /// <Summary>
        /// Export the ballot representation as JSON
        /// </Summary>
        std::string toJson();

        /// <Summary>
        /// Export the ballot representation as JSON
        /// </Summary>
        std::string toJson() const;

        /// <Summary>
        /// Import the ballot representation from JSON
        /// </Summary>
        static std::unique_ptr<InternalManifest> fromJson(std::string data);

        /// <Summary>
        /// Import the ballot representation from BSON
        /// </Summary>
        static std::unique_ptr<InternalManifest> fromBson(std::vector<uint8_t> data);

        /// <Summary>
        /// Import the representation from MsgPack
        /// </Summary>
        static std::unique_ptr<InternalManifest> fromMsgPack(std::vector<uint8_t> data);

      protected:
        static std::vector<std::unique_ptr<ContestDescriptionWithPlaceholders>>
        generateContestsWithPlaceholders(const Manifest &description);

        static std::vector<std::unique_ptr<GeopoliticalUnit>>
        copyGeopoliticalUnits(const Manifest &description);

        static std::vector<std::unique_ptr<BallotStyle>>
        copyBallotStyles(const Manifest &description);

      private:
        class Impl;
#pragma warning(suppress : 4251)
        std::unique_ptr<Impl> pimpl;
    };

    std::unique_ptr<SelectionDescription>
    generatePlaceholderSelectionFrom(const ContestDescription &contest, uint64_t useSequenceId);

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_MANIFEST_HPP_INCLUDED__ */