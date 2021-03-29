#ifndef __ELECTIONGUARD_CPP_ELECTION_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_ELECTION_HPP_INCLUDED__

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

    EG_API std::string getElectionTypeString(const ElectionType &value);
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

    EG_API std::string getReportingUnitTypeString(const ReportingUnitType &value);
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

    EG_API std::string getVoteVariationTypeString(const VoteVariationType &value);
    EG_API VoteVariationType getVoteVariationType(const std::string &value);

    /// <Summary>
    /// Use this as a type for character strings.
    /// See: https://developers.google.com/elections-data/reference/annotated-string
    /// </Summary>
    class EG_API AnnotatedString : public CryptoHashable
    {
      public:
        AnnotatedString(const AnnotatedString &other);
        AnnotatedString(const AnnotatedString &&other);
        explicit AnnotatedString(std::string annotation, std::string value);
        ~AnnotatedString();

        AnnotatedString &operator=(const AnnotatedString &other);
        AnnotatedString &operator=(AnnotatedString &&other) noexcept;

        std::string getAnnotation() const;
        std::string getValue() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// The ISO-639 language
    /// see: https://en.wikipedia.org/wiki/ISO_639
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

        std::string getValue() const;
        std::string getLanguage() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
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
        InternationalizedText(const InternationalizedText &&other);
        explicit InternationalizedText(std::vector<std::unique_ptr<Language>> text);
        ~InternationalizedText();

        InternationalizedText &operator=(InternationalizedText other);
        InternationalizedText &operator=(InternationalizedText &&other);

        std::vector<std::reference_wrapper<Language>> getText() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// For defining contact information about objects such as persons, boards of authorities, and organizations.
    /// See: https://developers.google.com/elections-data/reference/contact-information
    /// </Summary>
    class EG_API ContactInformation : public CryptoHashable
    {
      public:
        ContactInformation(const ContactInformation &other);
        ContactInformation(const ContactInformation &&other);
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

        std::vector<std::string> getAddressLine() const;
        std::vector<std::reference_wrapper<AnnotatedString>> getEmail() const;
        std::vector<std::reference_wrapper<AnnotatedString>> getPhone() const;
        std::string getName() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// Use this entity for defining geopolitical units such as cities, districts, jurisdictions, or precincts,
    /// for the purpose of associating contests, offices, vote counts, or other information with the geographies.
    /// See: https://developers.google.com/elections-data/reference/gp-unit
    /// </Summary>
    class EG_API GeopoliticalUnit : public CryptoHashable
    {
      public:
        GeopoliticalUnit(const GeopoliticalUnit &other);
        GeopoliticalUnit(const GeopoliticalUnit &&other);
        explicit GeopoliticalUnit(const std::string &objectId, const std::string &name,
                                  const ReportingUnitType type);
        explicit GeopoliticalUnit(const std::string &objectId, const std::string &name,
                                  const ReportingUnitType type,
                                  std::unique_ptr<ContactInformation> contactInformation);
        ~GeopoliticalUnit();

        GeopoliticalUnit &operator=(GeopoliticalUnit other);
        GeopoliticalUnit &operator=(GeopoliticalUnit &&other);

        std::string getObjectId() const;
        std::string getName() const;
        ReportingUnitType getType() const;
        ContactInformation *getContactInformation() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// A BallotStyle works as a key to uniquely specify a set of contests. See also `ContestDescription`.
    /// </Summary>
    class EG_API BallotStyle : public CryptoHashable
    {
      public:
        BallotStyle(const BallotStyle &other);
        BallotStyle(const BallotStyle &&other);
        explicit BallotStyle(const std::string &objectId,
                             std::vector<std::string> geopoliticalUnitIds);
        explicit BallotStyle(const std::string &objectId,
                             std::vector<std::string> geopoliticalUnitIds,
                             std::vector<std::string> partyIds, const std::string &imageUri);
        ~BallotStyle();

        BallotStyle &operator=(BallotStyle other);
        BallotStyle &operator=(BallotStyle &&other);

        std::string getObjectId() const;
        std::vector<std::string> getGeopoliticalUnitIds() const;
        std::vector<std::string> getPartyIds() const;
        std::string getImageUri() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// Use this entity to describe a political party that can then be referenced from other entities.
    /// See: https://developers.google.com/elections-data/reference/party
    /// </Summary>
    class EG_API Party : public CryptoHashable
    {
      public:
        Party(const Party &other);
        Party(const Party &&other);
        explicit Party(const std::string &objectId);
        explicit Party(const std::string &objectId, std::unique_ptr<InternationalizedText> name,
                       const std::string &abbreviation, const std::string &color,
                       const std::string &logoUri);
        ~Party();

        Party &operator=(Party other);
        Party &operator=(Party &&other);

        std::string getObjectId() const;
        InternationalizedText *getName() const;
        std::string getAbbreviation() const;
        std::string getColor() const;
        std::string getLogoUri() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <Summary>
    /// Entity describing information about a candidate in a contest.
    /// See: https://developers.google.com/elections-data/reference/candidate
    /// Note: The ElectionGuard Data Spec deviates from the NIST model in that
    /// selections for any contest type are considered a "candidate".
    /// for instance, on a yes-no referendum contest, two `candidate` objects
    /// would be included in the model to represent the `affirmative` and `negative`
    /// selections for the contest.  See the wiki, readme's, and tests in this repo for more info
    /// </Summary>
    class EG_API Candidate : public CryptoHashable
    {
      public:
        Candidate(const Candidate &other);
        Candidate(const Candidate &&other);
        explicit Candidate(const std::string &objectId, bool isWriteIn = false);
        explicit Candidate(const std::string &objectId, std::unique_ptr<InternationalizedText> name,
                           const std::string &partyId, const std::string &imageUri, bool isWriteIn);
        ~Candidate();

        Candidate &operator=(Candidate other);
        Candidate &operator=(Candidate &&other);

        std::string getObjectId() const;
        std::string getCandidateId() const;
        InternationalizedText *getName() const;
        std::string getPartyId() const;
        std::string getImageUri() const;
        bool isWriteIn() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// Data entity for the ballot selections in a contest,
    /// for example linking candidates and parties to their vote counts.
    /// See: https://developers.google.com/elections-data/reference/ballot-selection
    /// Note: The ElectionGuard Data Spec deviates from the NIST model in that
    /// there is no difference for different types of selections.
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
        SelectionDescription(const SelectionDescription &&other);
        explicit SelectionDescription(const std::string &objectId, const std::string &candidateId,
                                      const uint64_t sequenceOrder);
        ~SelectionDescription();

        SelectionDescription &operator=(SelectionDescription other);
        SelectionDescription &operator=(SelectionDescription &&other);

        std::string getObjectId() const;
        std::string getCandidateId() const;
        uint64_t getSequenceOrder() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
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
        ContestDescription(const ContestDescription &&other);
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
                                    const uint64_t numberElected, const uint64_t votesAllowed,
                                    const std::string &name,
                                    std::unique_ptr<InternationalizedText> ballotTitle,
                                    std::unique_ptr<InternationalizedText> ballotSubtitle,
                                    std::vector<std::unique_ptr<SelectionDescription>> selections);
        ~ContestDescription();

        ContestDescription &operator=(ContestDescription other);
        ContestDescription &operator=(ContestDescription &&other);

        std::string getObjectId() const;
        std::string getElectoralDistrictId() const;
        uint64_t getSequenceOrder() const;
        VoteVariationType getVoteVariation() const;
        uint64_t getNumberElected() const;
        uint64_t getVotesAllowed() const;
        std::string getName() const;

        InternationalizedText *getBallotTitle() const;
        InternationalizedText *getBallotSubtitle() const;

        std::vector<std::reference_wrapper<SelectionDescription>> getSelections() const;

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

        // TODO: ISSUE #119: isValid() const;

      private:
        class Impl;
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
        ContestDescriptionWithPlaceholders(const ContestDescriptionWithPlaceholders &&other);
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
          const uint64_t numberElected, const uint64_t votesAllowed, const std::string &name,
          std::unique_ptr<InternationalizedText> ballotTitle,
          std::unique_ptr<InternationalizedText> ballotSubtitle,
          std::vector<std::unique_ptr<SelectionDescription>> selections,
          std::vector<std::unique_ptr<SelectionDescription>> placeholderSelections);
        ~ContestDescriptionWithPlaceholders();

        ContestDescriptionWithPlaceholders &operator=(ContestDescriptionWithPlaceholders other);
        ContestDescriptionWithPlaceholders &operator=(ContestDescriptionWithPlaceholders &&other);

        std::vector<std::reference_wrapper<SelectionDescription>> getPlaceholders() const;
        bool IsPlaceholder(SelectionDescription &selection) const;

        // TODO: ISSUE #119: isValid() const;

        std::reference_wrapper<SelectionDescription> selectionFor(std::string &selectionId);

      private:
        class Impl;
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
    class EG_API ElectionDescription : public CryptoHashable
    {
      public:
        ElectionDescription(const ElectionDescription &other);
        ElectionDescription(const ElectionDescription &&other);
        explicit ElectionDescription(
          const std::string &electionScopeId, ElectionType type,
          std::chrono::system_clock::time_point startDate,
          std::chrono::system_clock::time_point endDate,
          std::vector<std::unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
          std::vector<std::unique_ptr<Party>> parties,
          std::vector<std::unique_ptr<Candidate>> candidates,
          std::vector<std::unique_ptr<ContestDescription>> contests,
          std::vector<std::unique_ptr<BallotStyle>> ballotStyles);
        explicit ElectionDescription(
          const std::string &electionScopeId, ElectionType type,
          std::chrono::system_clock::time_point startDate,
          std::chrono::system_clock::time_point endDate,
          std::vector<std::unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
          std::vector<std::unique_ptr<Party>> parties,
          std::vector<std::unique_ptr<Candidate>> candidates,
          std::vector<std::unique_ptr<ContestDescription>> contests,
          std::vector<std::unique_ptr<BallotStyle>> ballotStyles,
          std::unique_ptr<InternationalizedText> name,
          std::unique_ptr<ContactInformation> contactInformation);
        ~ElectionDescription();

        ElectionDescription &operator=(ElectionDescription other);
        ElectionDescription &operator=(ElectionDescription &&other);

        std::string getElectionScopeId() const;
        ElectionType getElectionType() const;
        std::chrono::system_clock::time_point getStartDate() const;
        std::chrono::system_clock::time_point getEndDate() const;
        std::vector<std::reference_wrapper<GeopoliticalUnit>> getGeopoliticalUnits() const;
        std::vector<std::reference_wrapper<Party>> getParties() const;
        std::vector<std::reference_wrapper<Candidate>> getCandidates() const;
        std::vector<std::reference_wrapper<ContestDescription>> getContests() const;
        std::vector<std::reference_wrapper<BallotStyle>> getBallotStyles() const;

        InternationalizedText *getName() const;
        ContactInformation *getContactInformation() const;

        // TODO: ISSUE #119: isValid() const;

        std::vector<uint8_t> toBson() const;
        std::string toJson();
        std::string toJson() const;

        static std::unique_ptr<ElectionDescription> fromJson(std::string data);
        static std::unique_ptr<ElectionDescription> fromBson(std::vector<uint8_t> data);

        virtual std::unique_ptr<ElementModQ> crypto_hash() override;
        virtual std::unique_ptr<ElementModQ> crypto_hash() const override;

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// `InternalElectionDescription` is a subset of the `ElectionDescription` structure that specifies
    /// the components that ElectionGuard uses for conducting an election.  The key component is the
    /// `contests` collection, which applies placeholder selections to the `ElectionDescription` contests
    /// </summary>
    class EG_API InternalElectionDescription
    {
      public:
        InternalElectionDescription(const InternalElectionDescription &other);
        InternalElectionDescription(const InternalElectionDescription &&other);
        explicit InternalElectionDescription(
          std::vector<std::unique_ptr<GeopoliticalUnit>> geopoliticalUnits,
          std::vector<std::unique_ptr<ContestDescriptionWithPlaceholders>> contests,
          std::vector<std::unique_ptr<BallotStyle>> ballotStyles,
          const ElementModQ &descriptionHash);
        InternalElectionDescription(const ElectionDescription &description);
        ~InternalElectionDescription();

        InternalElectionDescription &operator=(InternalElectionDescription other);
        InternalElectionDescription &operator=(InternalElectionDescription &&other);

        /// <summary>
        /// The hash of the election metadata
        /// </summary>
        const ElementModQ *getDescriptionHash() const;

        std::vector<std::reference_wrapper<GeopoliticalUnit>> getGeopoliticalUnits() const;
        std::vector<std::reference_wrapper<ContestDescriptionWithPlaceholders>> getContests() const;
        std::vector<std::reference_wrapper<BallotStyle>> getBallotStyles() const;

        BallotStyle *getBallotStyle(const std::string &ballotStyleId) const;

        std::vector<std::reference_wrapper<ContestDescriptionWithPlaceholders>>
        getContestsFor(const std::string &ballotStyleId) const;

        std::vector<uint8_t> toBson() const;
        std::string toJson();
        std::string toJson() const;

        static std::unique_ptr<InternalElectionDescription> fromJson(std::string data);
        static std::unique_ptr<InternalElectionDescription> fromBson(std::vector<uint8_t> data);

      protected:
        static std::vector<std::unique_ptr<ContestDescriptionWithPlaceholders>>
        generateContestsWithPlaceholders(const ElectionDescription &description);

        static std::vector<std::unique_ptr<GeopoliticalUnit>>
        copyGeopoliticalUnits(const ElectionDescription &description);

        static std::vector<std::unique_ptr<BallotStyle>>
        copyBallotStyles(const ElectionDescription &description);

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    /// <summary>
    /// `CiphertextElectionContext` is the ElectionGuard representation of a specific election
    /// Note: The ElectionGuard Data Spec deviates from the NIST model in that
    /// this object includes fields that are populated in the course of encrypting an election
    /// Specifically, `crypto_base_hash`, `crypto_extended_base_hash` and `elgamal_public_key`
    /// are populated with election-specific information necessary for encrypting the election.
    /// Refer to the [Electionguard Specification](https://github.com/microsoft/electionguard) for more information.
    ///
    /// To make an instance of this class, don't construct it directly. Use
    /// `make_ciphertext_election_context` instead.
    /// </summary>
    class EG_API CiphertextElectionContext
    {
      public:
        CiphertextElectionContext(const CiphertextElectionContext &other);
        CiphertextElectionContext(const CiphertextElectionContext &&other);
        explicit CiphertextElectionContext(const uint64_t numberOfGuardians, const uint64_t quorum,
                                           std::unique_ptr<ElementModP> elGamalPublicKey,
                                           std::unique_ptr<ElementModQ> commitmentHash,
                                           std::unique_ptr<ElementModQ> descriptionHash,
                                           std::unique_ptr<ElementModQ> cryptoBaseHash,
                                           std::unique_ptr<ElementModQ> cryptoExtendedBaseHash);
        ~CiphertextElectionContext();

        CiphertextElectionContext &operator=(CiphertextElectionContext other);
        CiphertextElectionContext &operator=(CiphertextElectionContext &&other);

        /// <summary>
        /// The number of guardians necessary to generate the public key
        /// </summary>
        uint64_t getNumberOfGuardians() const;

        /// <summary>
        /// The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians`
        /// </summary>
        uint64_t getQuorum() const;

        /// <summary>
        /// the `joint public key (K)` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        const ElementModP *getElGamalPublicKey() const;

        /// <summary>
        /// the `commitment hash H(K 1,0 , K 2,0 ... , K n,0 )` of the public commitments
        /// guardians make to each other in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        const ElementModQ *getCommitmentHash() const;

        /// <summary>
        /// The hash of the election metadata
        /// </summary>
        const ElementModQ *getDescriptionHash() const;

        /// <summary>
        /// the `base hash code (𝑄)` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        const ElementModQ *getCryptoBaseHash() const;

        /// <summary>
        /// the `extended base hash code (𝑄')` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        const ElementModQ *getCryptoExtendedBaseHash() const;

        /// <summary>
        ///  Makes a CiphertextElectionContext object.
        ///
        /// <param name="number_of_guardians"> The number of guardians necessary to generate the public key </param>
        /// <param name="quorum"> The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians` </param>
        /// <param name="elGamalPublicKey"> the public key of the election </param>
        /// <param name="commitmentHash"> the hash of the commitments the guardians make to each other </param>
        /// <param name="descriptionHash"> the hash of the election metadata </param>
        /// </summary>
        static std::unique_ptr<CiphertextElectionContext>
        make(const uint64_t numberOfGuardians, const uint64_t quorum,
             std::unique_ptr<ElementModP> elGamalPublicKey,
             std::unique_ptr<ElementModQ> commitmentHash,
             std::unique_ptr<ElementModQ> descriptionHash);

        /// <summary>
        ///  Makes a CiphertextElectionContext object from hex string representations.
        ///
        /// <param name="number_of_guardians"> The number of guardians necessary to generate the public key </param>
        /// <param name="quorum"> The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians` </param>
        /// <param name="elGamalPublicKeyInHex"> the public key of the election </param>
        /// <param name="commitmentHashInHex"> the hash of the commitments the guardians make to each other </param>
        /// <param name="descriptionHashInHex"> the hash of the election metadata </param>
        /// </summary>
        static std::unique_ptr<CiphertextElectionContext>
        make(const uint64_t numberOfGuardians, const uint64_t quorum,
             const std::string &elGamalPublicKeyInHex, const std::string &commitmentHashInHex,
             const std::string &descriptionHashInHex);

        std::vector<uint8_t> toBson() const;
        std::string toJson() const;
        static std::unique_ptr<CiphertextElectionContext> fromJson(std::string data);
        static std::unique_ptr<CiphertextElectionContext> fromBson(std::vector<uint8_t> data);

      private:
        class Impl;
        std::unique_ptr<Impl> pimpl;
    };

    std::unique_ptr<SelectionDescription>
    generatePlaceholderSelectionFrom(const ContestDescription &contest, uint64_t useSequenceId);

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_ELECTION_HPP_INCLUDED__ */
