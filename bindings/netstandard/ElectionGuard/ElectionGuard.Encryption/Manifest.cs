using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NativeAnnotatedString = NativeInterface.AnnotatedString.AnnotatedStringHandle;
    using NativeLanguage = NativeInterface.Language.LanguageHandle;
    using NativeInternationalizedText = NativeInterface.InternationalizedText.InternationalizedTextHandle;
    using NativeContactInformation = NativeInterface.ContactInformation.ContactInformationHandle;
    using NativeGeopoliticalUnit = NativeInterface.GeopoliticalUnit.GeopoliticalUnitHandle;
    using NativeBallotStyle = NativeInterface.BallotStyle.BallotStyleHandle;
    using NativeParty = NativeInterface.Party.PartyHandle;
    using NativeCandidate = NativeInterface.Candidate.CandidateHandle;
    using NativeSelectionDescription = NativeInterface.SelectionDescription.SelectionDescriptionHandle;
    using NativeContestDescription = NativeInterface.ContestDescription.ContestDescriptionHandle;
    using NativeContestDescriptionWithPlaceholders = NativeInterface.ContestDescriptionWithPlaceholders.ContestDescriptionWithPlaceholdersHandle;
    using NativeManifest = NativeInterface.Manifest.ManifestHandle;
    using NativeInternalManifest = NativeInterface.InternalManifest.InternalManifestHandle;
    using NativeElementModP = NativeInterface.ElementModP.ElementModPHandle;
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQHandle;

    #region AnnotatedString

    /// <Summary>
    /// Use this as a type for character strings.
    /// See: https://developers.google.com/elections-data/reference/annotated-string
    /// </Summary>
    public class AnnotatedString : DisposableBase
    {
        /// <Summary>
        /// An annotation of up to 16 characters that's associated with a character string.
        /// </Summary>
        public unsafe string Annotation
        {
            get
            {
                var status = NativeInterface.AnnotatedString.GetAnnotation(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"AnnotatedString Error Annotation: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// The character string
        /// </Summary>
        public unsafe string Value
        {
            get
            {
                var status = NativeInterface.AnnotatedString.GetValue(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"AnnotatedString Error Value: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        internal unsafe NativeAnnotatedString Handle;

        unsafe internal AnnotatedString(
            NativeAnnotatedString handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Creating a new `AnnotatedString`
        /// </summary>
        /// <param name="annotation">annotation for new string</param>
        /// <param name="value">string value</param>
        public unsafe AnnotatedString(string annotation, string value)
        {
            var status = NativeInterface.AnnotatedString.New(annotation, value, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"AnnotatedString Error Status: {status}");
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.AnnotatedString.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CryptoHash Error Status: {status}");
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region Language

    /// <Summary>
    /// The ISO-639 language
    /// see: https://en.wikipedia.org/wiki/ISO_639
    /// see: https://developers.google.com/civics-data/reference/internationalized-text#language-string
    /// </Summary>
    public class Language : DisposableBase
    {
        /// <Summary>
        /// The value
        /// </Summary>
        public unsafe string Value
        {
            get
            {
                var status = NativeInterface.Language.GetValue(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Language Error Value: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Identifies the language
        /// </Summary>
        public unsafe string LanguageAbbreviation
        {
            get
            {
                var status = NativeInterface.Language.GetLanguage(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Language Error LanguageAbbreviation: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        internal unsafe NativeLanguage Handle;

        unsafe internal Language(
            NativeLanguage handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a new `Language` object
        /// </summary>
        /// <param name="value">value to represent language</param>
        /// <param name="language">string with language info</param>
        public unsafe Language(string value, string language)
        {
            var data = ReplaceAccents(value);
            var status = NativeInterface.Language.New(data, language, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Language Error Status: {status}");
            }
        }

        /// <summary>
        /// Temp function for handling accented latin characters for v1.0
        /// </summary>
        /// <param name="value">string to convert</param>
        /// <returns>string with replaced characters</returns>
        private string ReplaceAccents(string value)
        {
            return value.Replace("ú", "u")
                        .Replace("é", "e")
                        .Replace("ë", "e")
                        .Replace("ó", "o")
                        .Replace("ô", "o")
                        .Replace("ö", "o")
                        .Replace("î", "i");
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.Language.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CryptoHash Error Status: {status}");
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region InternationalizedText

    /// <Summary>
    /// Data entity used to represent multi-national text. Use when text on a ballot contains multi-national text.
    /// See: https://developers.google.com/elections-data/reference/internationalized-text
    /// </Summary>
    public class InternationalizedText : DisposableBase
    {
        /// <Summary>
        /// A string of possibly non-English text.
        /// </Summary>
        public unsafe ulong TextSize
        {
            get
            {
                var size = NativeInterface.InternationalizedText.GetTextSize(
                    Handle);
                return size;
            }
        }

        internal unsafe NativeInternationalizedText Handle;

        unsafe internal InternationalizedText(
            NativeInternationalizedText handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create an `InternationalizedText` object
        /// </summary>
        /// <param name="text">array of text for languages</param>
        public unsafe InternationalizedText(Language[] text)
        {
            IntPtr[] nativeText = new IntPtr[text.Length];
            for (var i = 0; i < text.Length; i++)
            {
                nativeText[i] = text[i].Handle.Ptr;
                text[i].Dispose();
            }

            var status = NativeInterface.InternationalizedText.New(nativeText, (ulong)nativeText.Length, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"InternationalizedText Error Status: {status}");
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <summary>
        /// Gets text from array of languages
        /// </summary>
        /// <param name="index">index to use to get `Language`</param>
        /// <returns>`Language` object</returns>
        public unsafe Language GetTextAt(ulong index)
        {
            var status = NativeInterface.InternationalizedText.GetTextAtIndex(
                Handle, index, out NativeLanguage value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"InternationalizedText Error GetTextAt: {status}");
            }
            return new Language(value);
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.InternationalizedText.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CryptoHash Error Status: {status}");
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region ContactInformation

    /// <Summary>
    /// For defining contact information about objects such as persons, boards of authorities, and organizations.
    ///
    /// Contact Information values are not used internally by ElectionGuard when encrypting ballots
    /// but are included for checking the validity of a supplied manifest.
    ///
    /// See: https://developers.google.com/elections-data/reference/contact-information
    /// </Summary>
    public class ContactInformation : DisposableBase
    {
        /// <Summary>
        /// Get the size of the address collection
        /// </Summary>
        public unsafe ulong AddressLineSize
        {
            get
            {
                var size = NativeInterface.ContactInformation.GetAddressLineSize(
                    Handle);
                return size;
            }
        }

        /// <Summary>
        /// Get the size of the email collection
        /// </Summary>
        public unsafe ulong EmailLineSize
        {
            get
            {
                var size = NativeInterface.ContactInformation.GetEmailLineSize(
                    Handle);
                return size;
            }
        }

        /// <Summary>
        /// Get the size of the phone collection
        /// </Summary>
        public unsafe ulong PhoneLineSeize
        {
            get
            {
                var size = NativeInterface.ContactInformation.GetPhoneLineSize(
                    Handle);
                return size;
            }
        }

        internal unsafe NativeContactInformation Handle;

        unsafe internal ContactInformation(
            NativeContactInformation handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a `ContactInformation` object
        /// </summary>
        /// <param name="name">name of the contact</param>
        public unsafe ContactInformation(string name)
        {
            var status = NativeInterface.ContactInformation.New(name, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContactInformation Error Status: {status}");
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// Associates an address with the contact.
        /// AddressLine needs to contain the lines that someone would
        /// enter into a web mapping service to find the address on a map.
        /// That is, the value of the field needs to geocode the contact location.
        /// </Summary>
        public unsafe string GetAddressLineAt(ulong index)
        {
            var status = NativeInterface.ContactInformation.GetAddressLineAtIndex(
                Handle, index, out IntPtr value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContactInformation Error GetAddressLineAt: {status}");
            }
            var data = Marshal.PtrToStringAnsi(value);
            NativeInterface.Memory.FreeIntPtr(value);
            return data;
        }

        /// <Summary>
        /// Associates an email address with the contact.
        /// </Summary>
        public unsafe InternationalizedText GetEmailLineAt(ulong index)
        {
            var status = NativeInterface.ContactInformation.GetEmailLineAtIndex(
                Handle, index, out NativeInternationalizedText value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContactInformation Error GetEmailLineAt: {status}");
            }
            return new InternationalizedText(value);
        }

        /// <Summary>
        /// Associates a phone number with the contact.
        /// </Summary>
        public unsafe InternationalizedText GetPhoneLineAt(ulong index)
        {
            var status = NativeInterface.ContactInformation.GetPhoneLineAtIndex(
                Handle, index, out NativeInternationalizedText value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContactInformation Error GetPhoneLineAt: {status}");
            }
            return new InternationalizedText(value);
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.ContactInformation.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CryptoHash Error Status: {status}");
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region GeopoliticalUnit

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
    public class GeopoliticalUnit : DisposableBase
    {
        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.GeopoliticalUnit.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"GeopoliticalUnit Error ObjectId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Name of the geopolitical unit.
        /// </Summary>
        public unsafe string Name
        {
            get
            {
                var status = NativeInterface.GeopoliticalUnit.GetName(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"GeopoliticalUnit Error Name: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// The type of reporting unit
        /// </Summary>
        public unsafe ReportingUnitType ReportingUnitType
        {
            get
            {
                var value = NativeInterface.GeopoliticalUnit.GetReportingUnitType(
                    Handle);

                return value;
            }
        }

        internal unsafe NativeGeopoliticalUnit Handle;

        unsafe internal GeopoliticalUnit(
            NativeGeopoliticalUnit handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a `GeopoliticalUnit` object
        /// </summary>
        /// <param name="objectId">string to identify the unit</param>
        /// <param name="name">name of the unit</param>
        /// <param name="reportingUnitType">type of geopolitical unit</param>
        public unsafe GeopoliticalUnit(
            string objectId, string name, ReportingUnitType reportingUnitType)
        {
            var status = NativeInterface.GeopoliticalUnit.New(
                objectId, name, reportingUnitType, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContactInformation Error Status: {status}");
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.GeopoliticalUnit.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CryptoHash Error Status: {status}");
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region BallotStyle

    /// <Summary>
    /// A BallotStyle works as a key to uniquely specify a set of contests. See also `ContestDescription`.
    ///
    /// For ElectionGuard, each contest is associated with a specific geopolitical unit,
    /// and each ballot style is associated with at least one geopolitical unit.
    ///
    /// It is up to the consuming application to determine how to interpreit the relationships
    /// between these entity types.
    /// </Summary>
    public class BallotStyle : DisposableBase
    {
        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.BallotStyle.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"BallotStyle Error ObjectId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// the size of the geopolitical unit id collection
        /// </Summary>
        public unsafe ulong GeopoliticalUnitIdsSize
        {
            get
            {
                var size = NativeInterface.BallotStyle.GetGeopoliticalUnitSize(
                    Handle);
                return size;
            }
        }

        /// <Summary>
        /// the size of the party id collection
        /// </Summary>
        public unsafe ulong PartyIdsSize
        {
            get
            {
                var size = NativeInterface.BallotStyle.GetPartyIdsSize(
                    Handle);
                return size;
            }
        }

        internal unsafe NativeBallotStyle Handle;

        unsafe internal BallotStyle(
            NativeBallotStyle handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a `BallotStyle` object
        /// </summary>
        /// <param name="objectId">string to identify the `BallotStyle`</param>
        /// <param name="gpUnitIds">array of objectIds for the `GeopoliticalUnit` for this `BallotStyle`</param>
        public unsafe BallotStyle(string objectId, string[] gpUnitIds)
        {
            var status = NativeInterface.BallotStyle.New(objectId, gpUnitIds, (ulong)gpUnitIds.Length, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"BallotStyle Error Status: {status}");
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// the Geopolitical Unit Id or id's that correlate to this ballot style
        /// </Summary>
        public unsafe String GetGeopoliticalUnitIdAt(ulong index)
        {
            var status = NativeInterface.BallotStyle.GetGeopoliticalInitIdAtIndex(
                Handle, index, out IntPtr value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"BallotStyle Error GetGeopoliticalUnitIdAt: {status}");
            }
            var data = Marshal.PtrToStringAnsi(value);
            NativeInterface.Memory.FreeIntPtr(value);
            return data;
        }

        /// <Summary>
        /// the Party Id or Id's (if any) for this ballot style
        /// </Summary>
        public unsafe String GetPartyIdAt(ulong index)
        {
            var status = NativeInterface.BallotStyle.GetPartyIdAtIndex(
                Handle, index, out IntPtr value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"BallotStyle Error GetPartyIdAt: {status}");
            }
            var data = Marshal.PtrToStringAnsi(value);
            NativeInterface.Memory.FreeIntPtr(value);
            return data;
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.BallotStyle.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CryptoHash Error Status: {status}");
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region Party

    /// <Summary>
    /// Use this entity to describe a political party that can then be referenced from other entities.
    ///
    /// It is not required to define a party for Electionguard.
    ///
    /// See: https://developers.google.com/elections-data/reference/party
    /// </Summary>
    public class Party : DisposableBase
    {
        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.Party.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Party Error ObjectId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Abbreviation of the party
        /// </Summary>
        public unsafe string Abbreviation
        {
            get
            {
                var status = NativeInterface.Party.GetAbbreviation(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Party Error Abbreviation: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Name of the party
        /// </Summary>
        public unsafe InternationalizedText Name
        {
            get
            {
                var status = NativeInterface.Party.GetName(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Party Error Name: {status}");
                }
                return new InternationalizedText(value);
            }
        }

        /// <Summary>
        /// An optional color in hex
        /// </Summary>
        public unsafe string Color
        {
            get
            {
                var status = NativeInterface.Party.GetColor(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Party Error Color: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// An optional logo uri
        /// </Summary>
        public unsafe string LogoUri
        {
            get
            {
                var status = NativeInterface.Party.GetLogoUri(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Party Error LogoUri: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        internal unsafe NativeParty Handle;

        unsafe internal Party(
            NativeParty handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a `Party` object
        /// </summary>
        /// <param name="objectId">string to identify the object</param>
        public unsafe Party(string objectId)
        {
            var status = NativeInterface.Party.New(objectId, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Party Error Status: {status}");
            }
        }

        /// <summary>
        /// Create a `Party` object
        /// </summary>
        /// <param name="objectId">string to identify the object</param>
        /// <param name="name">name of the party</param>
        /// <param name="abbreviation">abbreviation for the object</param>
        /// <param name="color">string for the name of the color used</param>
        /// <param name="logoUri">string for the uri for the logo</param>
        public unsafe Party(
            string objectId, InternationalizedText name,
            string abbreviation, string color, string logoUri)
        {
            var status = NativeInterface.Party.New(
                objectId, name.Handle, abbreviation, color ?? string.Empty, logoUri ?? string.Empty, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Party Error Status: {status}");
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.Party.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CryptoHash Error Status: {status}");
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region Candidate

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
    public class Candidate : DisposableBase
    {
        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.Candidate.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Candidate Error ObjectId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// A convenience accessor for getObjectId
        /// </Summary>
        public unsafe string CandidateId
        {
            get
            {
                var status = NativeInterface.Candidate.GetCandidateId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Candidate Error CandidateId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Name of the candidate
        /// </Summary>
        public unsafe string Name
        {
            get
            {
                var status = NativeInterface.Candidate.GetName(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Candidate Error Name: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Optional party id of the candidate
        /// </Summary>
        public unsafe string PartyId
        {
            get
            {
                var status = NativeInterface.Candidate.GetPartyId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Candidate Error PartyId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Optional image uri for the candidate
        /// </Summary>
        public unsafe string ImageUri
        {
            get
            {
                var status = NativeInterface.Candidate.GetImageUri(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Candidate Error ImageUri: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        internal unsafe NativeCandidate Handle;

        unsafe internal Candidate(
            NativeCandidate handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a `Candidate` object
        /// </summary>
        /// <param name="objectId">string for the identity of the object</param>
        /// <param name="isWriteIn">is the candidate a write in</param>
        public unsafe Candidate(string objectId, bool isWriteIn)
        {
            var status = NativeInterface.Candidate.New(objectId, isWriteIn, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Candidate Error Status: {status}");
            }
        }

        /// <summary>
        /// Create a `Candidate` object
        /// </summary>
        /// <param name="objectId">string for the identity of the object</param>
        /// <param name="partyId">string identifying the party for the candidate</param>
        /// <param name="isWriteIn">is the candidate a write in</param>
        public unsafe Candidate(string objectId, string partyId, bool isWriteIn)
        {
            var status = NativeInterface.Candidate.New(
                objectId, partyId, isWriteIn, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Candidate Error Status: {status}");
            }
        }

        /// <summary>
        /// Create a `Candidate` object
        /// </summary>
        /// <param name="objectId">string for the identity of the object</param>
        /// <param name="name">name of the candidate</param>
        /// <param name="partyId">string identifying the party for the candidate</param>
        /// <param name="imageUri">string for uir for image of candidate</param>
        /// <param name="isWriteIn">is the candidate a write in</param>
        public unsafe Candidate(
            string objectId, InternationalizedText name,
            string partyId, string imageUri, bool isWriteIn)
        {
            var status = NativeInterface.Candidate.New(
                objectId, name.Handle, partyId, imageUri ?? string.Empty, isWriteIn, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Candidate Error Status: {status}");
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.Candidate.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CryptoHash Error Status: {status}");
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region SelectionDescription

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
    public class SelectionDescription : DisposableBase
    {
        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.SelectionDescription.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"SelectionDescription Error ObjectId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// the object id of the candidate
        /// </Summary>
        public unsafe string CandidateId
        {
            get
            {
                var status = NativeInterface.SelectionDescription.GetCandidateId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"SelectionDescription Error CandidateId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// The sequence order defining this selections place in the contest selection collection.
        /// Note: this is specifically for programs to interpret and does not necessarily represent
        /// the order in which selections are presented to a user.
        /// </Summary>
        public unsafe ulong SequenceOrder
        {
            get
            {
                var value = NativeInterface.SelectionDescription.GetSequenceOrder(Handle);
                return value;
            }
        }

        internal unsafe NativeSelectionDescription Handle;

        unsafe internal SelectionDescription(
            NativeSelectionDescription handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a `SelectionDescription` object
        /// </summary>
        /// <param name="objectId">string identifying the object</param>
        /// <param name="candidateId">string identifying the candidate</param>
        /// <param name="sequenceOrder">the number of the item for sequence order</param>
        public unsafe SelectionDescription(
            string objectId, string candidateId, ulong sequenceOrder)
        {
            var status = NativeInterface.SelectionDescription.New(
                objectId, candidateId, sequenceOrder, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"SelectionDescription Error Status: {status}");
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.SelectionDescription.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CryptoHash Error Status: {status}");
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region ContestDescription

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
    public class ContestDescription : DisposableBase
    {
        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.ContestDescription.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ContestDescription Error ObjectId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// The object id of the geopolitical unit associated with this contest.
        /// Note: in concordance with the NIST standard, the name `ElectoralDistrictId` is kept
        /// </Summary>
        public unsafe string ElectoralDistrictId
        {
            get
            {
                var status = NativeInterface.ContestDescription.GetElectoralDistrictId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ContestDescription Error ElectoralDistrictId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// The sequence order defining this contest's place in the contest collection of the ballot style.
        /// Note: this is specifically for programs to interpret and does not necessarily represent
        /// the order in which contests are presented to a user.
        /// </Summary>
        public unsafe ulong SequenceOrder
        {
            get
            {
                var value = NativeInterface.ContestDescription.GetSequenceOrder(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The vote variation type.  Currently ElectionGuard supports one_of_m and n_of_m
        /// </Summary>
        public unsafe VoteVariationType VoteVariationType
        {
            get
            {
                var value = NativeInterface.ContestDescription.GetVoteVariationType(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The number of candidates that are elected in the contest, which is the n of an n-of-m contest
        /// </Summary>
        public unsafe ulong NumberElected
        {
            get
            {
                var value = NativeInterface.ContestDescription.GetNumberElected(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The maximum number of votes or write-ins allowed per voter in this contest.
        /// </Summary>
        public unsafe ulong VotesAllowed
        {
            get
            {
                var value = NativeInterface.ContestDescription.GetVotesAllowed(Handle);
                return value;
            }
        }

        /// <Summary>
        /// Name of the contest as it's listed on the results report,
        /// not necessarily as it appears on the ballot.
        /// </Summary>
        public unsafe string Name
        {
            get
            {
                var status = NativeInterface.ContestDescription.GetName(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ContestDescription Error Name: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Title of the contest, which must match how it appears on the voters' ballots.
        /// </Summary>
        public unsafe InternationalizedText BallotTitle
        {
            get
            {
                var status = NativeInterface.ContestDescription.GetBallotTitle(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ContestDescription Error BallotTitle: {status}");
                }
                return new InternationalizedText(value);
            }
        }

        /// <Summary>
        /// Subtitle of the contest, which must match how it appears on the voters' ballots.
        /// </Summary>
        public unsafe InternationalizedText BallotSubTitle
        {
            get
            {
                var status = NativeInterface.ContestDescription.GetBallotSubTitle(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ContestDescription Error BallotSubTitle: {status}");
                }
                return new InternationalizedText(value);
            }
        }

        /// <Summary>
        /// The size of the selections collection
        /// </Summary>
        public unsafe ulong SelectionsSize
        {
            get
            {
                var value = NativeInterface.ContestDescription.GetSelectionsSize(Handle);
                return value;
            }
        }

        internal unsafe NativeContestDescription Handle;

        unsafe internal ContestDescription(
            NativeContestDescription handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a `ContestDescription` object
        /// </summary>
        /// <param name="objectId">string identifying object</param>
        /// <param name="electoralDistrictId">string identifying electoral district</param>
        /// <param name="sequenceOrder">the sequence order to show this in</param>
        /// <param name="voteVariation">vote variation type</param>
        /// <param name="numberElected">the number of elected</param>
        /// <param name="name">string for name of the contest</param>
        /// <param name="selections">array of `SelectionDescription`</param>
        public unsafe ContestDescription(
            string objectId, string electoralDistrictId, ulong sequenceOrder,
            VoteVariationType voteVariation, ulong numberElected, string name,
            SelectionDescription[] selections)
        {
            IntPtr[] selectionPointers = new IntPtr[selections.Length];
            for (var i = 0; i < selections.Length; i++)
            {
                selectionPointers[i] = selections[i].Handle.Ptr;
                selections[i].Dispose();
            }

            var status = NativeInterface.ContestDescription.New(
                objectId, electoralDistrictId, sequenceOrder,
                voteVariation, numberElected, name,
                selectionPointers, (ulong)selectionPointers.LongLength, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescription Error Status: {status}");
            }
        }

        /// <summary>
        /// Create a `ContestDescription` object
        /// </summary>
        /// <param name="objectId">string identifying object</param>
        /// <param name="electoralDistrictId">string identifying electoral district</param>
        /// <param name="sequenceOrder">the sequence order to show this in</param>
        /// <param name="voteVariation">vote variation type</param>
        /// <param name="numberElected">number of elected</param>
        /// <param name="votesAllowed">number of votes allowed</param>
        /// <param name="name">string for name of the contest</param>
        /// <param name="ballotTitle">international string for the ballot title</param>
        /// <param name="ballotSubtitle">international string for the ballot title</param>
        /// <param name="selections">array of `SelectionDescription`</param>
        public unsafe ContestDescription(
            string objectId, string electoralDistrictId, ulong sequenceOrder,
            VoteVariationType voteVariation, ulong numberElected, ulong votesAllowed,
            string name, InternationalizedText ballotTitle, InternationalizedText ballotSubtitle,
            SelectionDescription[] selections)
        {
            IntPtr[] selectionPointers = new IntPtr[selections.Length];
            for (var i = 0; i < selections.Length; i++)
            {
                selectionPointers[i] = selections[i].Handle.Ptr;
                selections[i].Dispose();
            }

            var status = NativeInterface.ContestDescription.New(
                objectId, electoralDistrictId, sequenceOrder,
                voteVariation, numberElected, votesAllowed,
                name, ballotTitle.Handle, ballotSubtitle.Handle,
                selectionPointers, (ulong)selectionPointers.LongLength, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescription Error Status: {status}");
            }
        }

        /// <summary>
        /// Create a `ContestDescription` object
        /// </summary>
        /// <param name="objectId">string identifying object</param>
        /// <param name="electoralDistrictId">string identifying electoral district</param>
        /// <param name="sequenceOrder">the sequence order to show this in</param>
        /// <param name="voteVariation">vote variation type</param>
        /// <param name="numberElected">number of elected</param>
        /// <param name="name">string for name of the contest</param>
        /// <param name="selections">array of `SelectionDescription`</param>
        /// <param name="primaryPartyIds">array of strings for `PartyIds`</param>
        public unsafe ContestDescription(
            string objectId, string electoralDistrictId, ulong sequenceOrder,
            VoteVariationType voteVariation, ulong numberElected, string name,
            SelectionDescription[] selections, string[] primaryPartyIds)
        {
            IntPtr[] selectionPointers = new IntPtr[selections.Length];
            for (var i = 0; i < selections.Length; i++)
            {
                selectionPointers[i] = selections[i].Handle.Ptr;
                selections[i].Dispose();
            }

            var status = NativeInterface.ContestDescription.New(
                objectId, electoralDistrictId, sequenceOrder,
                voteVariation, numberElected, name,
                selectionPointers, (ulong)selectionPointers.LongLength,
                primaryPartyIds, (ulong)primaryPartyIds.LongLength, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescription Error Status: {status}");
            }
        }

        /// <summary>
        /// Create a `ContestDescription` object
        /// </summary>
        /// <param name="objectId">string identifying object</param>
        /// <param name="electoralDistrictId">string identifying electoral district</param>
        /// <param name="sequenceOrder">the sequence order to show this in</param>
        /// <param name="voteVariation">vote variation type</param>
        /// <param name="numberElected">number of elected</param>
        /// <param name="votesAllowed">number of votes allowed</param>
        /// <param name="name">string for name of the contest</param>
        /// <param name="ballotTitle">international string for the ballot title</param>
        /// <param name="ballotSubtitle">international string for the ballot title</param>
        /// <param name="selections">array of `SelectionDescription`</param>
        /// <param name="primaryPartyIds">array of strings for `PartyIds`</param>
        public unsafe ContestDescription(
            string objectId, string electoralDistrictId, ulong sequenceOrder,
            VoteVariationType voteVariation, ulong numberElected, ulong votesAllowed,
            string name, InternationalizedText ballotTitle, InternationalizedText ballotSubtitle,
            SelectionDescription[] selections, string[] primaryPartyIds)
        {
            IntPtr[] selectionPointers = new IntPtr[selections.Length];
            for (var i = 0; i < selections.Length; i++)
            {
                selectionPointers[i] = selections[i].Handle.Ptr;
                selections[i].Dispose();
            }

            var status = NativeInterface.ContestDescription.New(
                objectId, electoralDistrictId, sequenceOrder,
                voteVariation, numberElected, votesAllowed,
                name, ballotTitle.Handle, ballotSubtitle.Handle,
                selectionPointers, (ulong)selectionPointers.LongLength,
                primaryPartyIds, (ulong)primaryPartyIds.LongLength, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescription Error Status: {status}");
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// The collection of selections in this contest.
        /// </Summary>
        public unsafe SelectionDescription GetSelectionAtIndex(ulong index)
        {
            var status = NativeInterface.ContestDescription.GetSelectionAtIndex(
                Handle, index, out NativeSelectionDescription value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescription Error GetSelectionAtIndex: {status}");
            }
            return new SelectionDescription(value);
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.ContestDescription.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CryptoHash Error Status: {status}");
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region ContestDescriptionWithPlaceholders

    /// <summary>
    /// ContestDescriptionWithPlaceholders is a `ContestDescription` with ElectionGuard `placeholder_selections`.
    /// (The ElectionGuard spec requires for n-of-m elections that there be *exactly* n counters that are one
    /// with the rest zero, so if a voter deliberately undervotes, one or more of the placeholder counters will
    /// become one. This allows the `ConstantChaumPedersenProof` to verify correctly for undervoted contests.)
    /// </summary>
    public class ContestDescriptionWithPlaceholders : DisposableBase
    {
        /// <Summary>
        /// Unique internal identifier that's used by other elements to reference this element.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.ContestDescriptionWithPlaceholders.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ContestDescriptionWithPlaceholders Error ObjectId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// The object id of the geopolitical unit associated with this contest.
        /// Note: in concordance with the NIST standard, the name `ElectoralDistrictId` is kept
        /// </Summary>
        public unsafe string ElectoralDistrictId
        {
            get
            {
                var status = NativeInterface.ContestDescriptionWithPlaceholders.GetElectoralDistrictId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ContestDescriptionWithPlaceholders Error ElectoralDistrictId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// The sequence order defining this contest's place in the contest collection of the ballot style.
        /// Note: this is specifically for programs to interpret and does not necessarily represent
        /// the order in which contests are presented to a user.
        /// </Summary>
        public unsafe ulong SequenceOrder
        {
            get
            {
                var value = NativeInterface.ContestDescriptionWithPlaceholders.GetSequenceOrder(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The vote variation type.  Currently ElectionGuard supports one_of_m and n_of_m
        /// </Summary>
        public unsafe VoteVariationType VoteVariationType
        {
            get
            {
                var value = NativeInterface.ContestDescriptionWithPlaceholders.GetVoteVariationType(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The number of candidates that are elected in the contest, which is the n of an n-of-m contest
        /// </Summary>
        public unsafe ulong NumberElected
        {
            get
            {
                var value = NativeInterface.ContestDescriptionWithPlaceholders.GetNumberElected(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The maximum number of votes or write-ins allowed per voter in this contest.
        /// </Summary>
        public unsafe ulong VotesAllowed
        {
            get
            {
                var value = NativeInterface.ContestDescriptionWithPlaceholders.GetVotesAllowed(Handle);
                return value;
            }
        }

        /// <Summary>
        /// Name of the contest as it's listed on the results report,
        /// not necessarily as it appears on the ballot.
        /// </Summary>
        public unsafe string Name
        {
            get
            {
                var status = NativeInterface.ContestDescriptionWithPlaceholders.GetName(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ContestDescriptionWithPlaceholders Error Name: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Title of the contest, which must match how it appears on the voters' ballots.
        /// </Summary>
        public unsafe InternationalizedText BallotTitle
        {
            get
            {
                var status = NativeInterface.ContestDescriptionWithPlaceholders.GetBallotTitle(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ContestDescriptionWithPlaceholders Error BallotTitle: {status}");
                }
                return new InternationalizedText(value);
            }
        }

        /// <Summary>
        /// Subtitle of the contest, which must match how it appears on the voters' ballots.
        /// </Summary>
        public unsafe InternationalizedText BallotSubTitle
        {
            get
            {
                var status = NativeInterface.ContestDescriptionWithPlaceholders.GetBallotSubTitle(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ContestDescriptionWithPlaceholders Error BallotSubTitle: {status}");
                }
                return new InternationalizedText(value);
            }
        }

        /// <Summary>
        /// The size of the selections collection
        /// </Summary>
        public unsafe ulong SelectionsSize
        {
            get
            {
                var value = NativeInterface.ContestDescriptionWithPlaceholders.GetSelectionsSize(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The size of the placeholder collection
        /// </Summary>
        public unsafe ulong PlaceholdersSize
        {
            get
            {
                var value = NativeInterface.ContestDescriptionWithPlaceholders.GetPlaceholdersSize(Handle);
                return value;
            }
        }

        internal unsafe NativeContestDescriptionWithPlaceholders Handle;

        unsafe internal ContestDescriptionWithPlaceholders(
            NativeContestDescriptionWithPlaceholders handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a `ContestDescriptionWithPlaceholders` object
        /// </summary>
        /// <param name="objectId">string identifying object</param>
        /// <param name="electoralDistrictId">string identifying electoral district</param>
        /// <param name="sequenceOrder">the sequence order to show this in</param>
        /// <param name="voteVariation">vote variation type</param>
        /// <param name="numberElected">the number of elected</param>
        /// <param name="name">string for name of the contest</param>
        /// <param name="selections">array of `SelectionDescription`</param>
        /// <param name="placeholders">array of `SelectionDescription` to use as placeholders</param>
        public unsafe ContestDescriptionWithPlaceholders(
            string objectId, string electoralDistrictId, ulong sequenceOrder,
            VoteVariationType voteVariation, ulong numberElected, string name,
            SelectionDescription[] selections, SelectionDescription[] placeholders)
        {
            IntPtr[] selectionPointers = new IntPtr[selections.Length];
            for (var i = 0; i < selections.Length; i++)
            {
                selectionPointers[i] = selections[i].Handle.Ptr;
                selections[i].Dispose();
            }

            IntPtr[] placeholderPointers = new IntPtr[placeholders.Length];
            for (var i = 0; i < placeholders.Length; i++)
            {
                placeholderPointers[i] = placeholders[i].Handle.Ptr;
                placeholders[i].Dispose();
            }

            var status = NativeInterface.ContestDescriptionWithPlaceholders.New(
                objectId, electoralDistrictId, sequenceOrder,
                voteVariation, numberElected, name,
                selectionPointers, (ulong)selectionPointers.LongLength,
                placeholderPointers, (ulong)placeholderPointers.LongLength,
                 out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescriptionWithPlaceholders Error Status: {status}");
            }
        }

        /// <summary>
        /// Create a `ContestDescriptionWithPlaceholders` object
        /// </summary>
        /// <param name="objectId">string identifying object</param>
        /// <param name="electoralDistrictId">string identifying electoral district</param>
        /// <param name="sequenceOrder">the sequence order to show this in</param>
        /// <param name="voteVariation">vote variation type</param>
        /// <param name="numberElected">the number of elected</param>
        /// <param name="votesAllowed">number of votes allowed</param>
        /// <param name="name">string for name of the contest</param>
        /// <param name="ballotTitle">international string for the ballot title</param>
        /// <param name="ballotSubtitle">international string for the ballot title</param>
        /// <param name="selections">array of `SelectionDescription`</param>
        /// <param name="placeholders">array of `SelectionDescription` to use as placeholders</param>
        public unsafe ContestDescriptionWithPlaceholders(
            string objectId, string electoralDistrictId, ulong sequenceOrder,
            VoteVariationType voteVariation, ulong numberElected, ulong votesAllowed,
            string name, InternationalizedText ballotTitle, InternationalizedText ballotSubtitle,
            SelectionDescription[] selections, SelectionDescription[] placeholders)
        {
            IntPtr[] selectionPointers = new IntPtr[selections.Length];
            for (var i = 0; i < selections.Length; i++)
            {
                selectionPointers[i] = selections[i].Handle.Ptr;
                selections[i].Dispose();
            }

            IntPtr[] placeholderPointers = new IntPtr[placeholders.Length];
            for (var i = 0; i < placeholders.Length; i++)
            {
                placeholderPointers[i] = placeholders[i].Handle.Ptr;
                placeholders[i].Dispose();
            }

            var status = NativeInterface.ContestDescriptionWithPlaceholders.New(
                objectId, electoralDistrictId, sequenceOrder,
                voteVariation, numberElected, votesAllowed,
                name, ballotTitle.Handle, ballotSubtitle.Handle,
                selectionPointers, (ulong)selectionPointers.LongLength,
                placeholderPointers, (ulong)placeholderPointers.LongLength,
                out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescriptionWithPlaceholders Error Status: {status}");
            }
        }

        /// <summary>
        /// Create a `ContestDescriptionWithPlaceholders` object
        /// </summary>
        /// <param name="objectId">string identifying object</param>
        /// <param name="electoralDistrictId">string identifying electoral district</param>
        /// <param name="sequenceOrder">the sequence order to show this in</param>
        /// <param name="voteVariation">vote variation type</param>
        /// <param name="numberElected">the number of elected</param>
        /// <param name="name">string for name of the contest</param>
        /// <param name="selections">array of `SelectionDescription`</param>
        /// <param name="primaryPartyIds">array of strings for `PartyIds`</param>
        /// <param name="placeholders">array of `SelectionDescription` to use as placeholders</param>
        public unsafe ContestDescriptionWithPlaceholders(
            string objectId, string electoralDistrictId, ulong sequenceOrder,
            VoteVariationType voteVariation, ulong numberElected, string name,
            SelectionDescription[] selections, string[] primaryPartyIds,
            SelectionDescription[] placeholders)
        {
            IntPtr[] selectionPointers = new IntPtr[selections.Length];
            for (var i = 0; i < selections.Length; i++)
            {
                selectionPointers[i] = selections[i].Handle.Ptr;
                selections[i].Dispose();
            }

            IntPtr[] placeholderPointers = new IntPtr[placeholders.Length];
            for (var i = 0; i < placeholders.Length; i++)
            {
                placeholderPointers[i] = placeholders[i].Handle.Ptr;
                placeholders[i].Dispose();
            }

            var status = NativeInterface.ContestDescriptionWithPlaceholders.New(
                objectId, electoralDistrictId, sequenceOrder,
                voteVariation, numberElected, name,
                selectionPointers, (ulong)selectionPointers.LongLength,
                primaryPartyIds, (ulong)primaryPartyIds.LongLength,
                placeholderPointers, (ulong)placeholderPointers.LongLength,
                out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescriptionWithPlaceholders Error Status: {status}");
            }
        }

        /// <summary>
        /// Create a `ContestDescriptionWithPlaceholders` object
        /// </summary>
        /// <param name="objectId">string identifying object</param>
        /// <param name="electoralDistrictId">string identifying electoral district</param>
        /// <param name="sequenceOrder">the sequence order to show this in</param>
        /// <param name="voteVariation">vote variation type</param>
        /// <param name="numberElected">the number of elected</param>
        /// <param name="votesAllowed">number of votes allowed</param>
        /// <param name="name">string for name of the contest</param>
        /// <param name="ballotTitle">international string for the ballot title</param>
        /// <param name="ballotSubtitle">international string for the ballot title</param>
        /// <param name="selections">array of `SelectionDescription`</param>
        /// <param name="primaryPartyIds">array of strings for `PartyIds`</param>
        /// <param name="placeholders">array of `SelectionDescription` to use as placeholders</param>
        public unsafe ContestDescriptionWithPlaceholders(
            string objectId, string electoralDistrictId, ulong sequenceOrder,
            VoteVariationType voteVariation, ulong numberElected, ulong votesAllowed,
            string name, InternationalizedText ballotTitle, InternationalizedText ballotSubtitle,
            SelectionDescription[] selections, string[] primaryPartyIds,
            SelectionDescription[] placeholders)
        {
            IntPtr[] selectionPointers = new IntPtr[selections.Length];
            for (var i = 0; i < selections.Length; i++)
            {
                selectionPointers[i] = selections[i].Handle.Ptr;
                selections[i].Dispose();
            }

            IntPtr[] placeholderPointers = new IntPtr[placeholders.Length];
            for (var i = 0; i < placeholders.Length; i++)
            {
                placeholderPointers[i] = placeholders[i].Handle.Ptr;
                placeholders[i].Dispose();
            }

            var status = NativeInterface.ContestDescriptionWithPlaceholders.New(
                objectId, electoralDistrictId, sequenceOrder,
                voteVariation, numberElected, votesAllowed,
                name, ballotTitle.Handle, ballotSubtitle.Handle,
                selectionPointers, (ulong)selectionPointers.LongLength,
                primaryPartyIds, (ulong)primaryPartyIds.LongLength,
                placeholderPointers, (ulong)placeholderPointers.LongLength,
                out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescriptionWithPlaceholders Error Status: {status}");
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// The collection of selections in this contest.
        /// </Summary>
        public unsafe SelectionDescription GetSelectionAtIndex(ulong index)
        {
            var status = NativeInterface.ContestDescriptionWithPlaceholders.GetSelectionAtIndex(
                Handle, index, out NativeSelectionDescription value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescriptionWithPlaceholders ContestDescription GetSelectionAtIndex: {status}");
            }
            return new SelectionDescription(value);
        }

        /// <Summary>
        /// The collection of placeholders in this contest.
        /// </Summary>
        public unsafe SelectionDescription GetPlaceholderAtIndex(ulong index)
        {
            var status = NativeInterface.ContestDescriptionWithPlaceholders.GetPlaceholderAtIndex(
                Handle, index, out NativeSelectionDescription value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescriptionWithPlaceholders ContestDescription GetSelectionAtIndex: {status}");
            }
            return new SelectionDescription(value);
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.ContestDescriptionWithPlaceholders.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ContestDescriptionWithPlaceholders Error CryptoHash: {status}");
            }
            return new ElementModQ(value);
        }
    }

    #endregion

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
    public class Manifest : DisposableBase
    {
        /// <Summary>
        /// Unique identifier for a GpUnit element. Associates the election with
        /// a reporting unit that represents the geographical scope of the election,
        /// such as a state or city.
        /// </Summary>
        public unsafe string ElectionScopeId
        {
            get
            {
                var status = NativeInterface.Manifest.GetElectionScopeId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Manifest Error ObjectId: {status}");
                }
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Enumerated type of election, such as partisan-primary or open-primary.
        /// </Summary>
        public unsafe ElectionType ElectionType
        {
            get
            {
                var value = NativeInterface.Manifest.GetElectionType(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The start date/time of the election.
        /// </Summary>
        public unsafe DateTime StartDate
        {
            get
            {
                var value = NativeInterface.Manifest.GetStartDate(Handle);
                return DateTimeOffset.FromUnixTimeMilliseconds((long)value).DateTime;
            }
        }

        /// <Summary>
        /// The end date/time of the election.
        /// </Summary>
        public unsafe DateTime EndDate
        {
            get
            {
                var value = NativeInterface.Manifest.GetEndDate(Handle);
                return DateTimeOffset.FromUnixTimeMilliseconds((long)value).DateTime;
            }
        }

        /// <Summary>
        /// The size of the geopolitical units collection
        /// </Summary>
        public unsafe ulong GeopoliticalUnitsSize
        {
            get
            {
                var value = NativeInterface.Manifest.GetGeopoliticalUnitsSize(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The size of the parties collection
        /// </Summary>
        public unsafe ulong PartiesSize
        {
            get
            {
                var value = NativeInterface.Manifest.GetPartiesSize(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The size of the candidates collection
        /// </Summary>
        public unsafe ulong CandidatesSize
        {
            get
            {
                var value = NativeInterface.Manifest.GetCandidatesSize(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The size of the contests collection
        /// </Summary>
        public unsafe ulong ContestsSize
        {
            get
            {
                var value = NativeInterface.Manifest.GetContestsSize(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The size of the ballot styles collection
        /// </Summary>
        public unsafe ulong BallotStylesSize
        {
            get
            {
                var value = NativeInterface.Manifest.GetBallotStylesSize(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The friendly name of the election
        /// </Summary>
        public unsafe InternationalizedText Name
        {
            get
            {
                var status = NativeInterface.Manifest.GetName(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Manifest Error ObjectId: {status}");
                }
                return new InternationalizedText(value);
            }
        }

        /// <Summary>
        /// The contact information for the election
        /// </Summary>
        public unsafe ContactInformation ContactInfo
        {
            get
            {
                var status = NativeInterface.Manifest.GetContactInfo(
                    Handle, out NativeContactInformation value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Manifest Error ObjectId: {status}");
                }
                return new ContactInformation(value);
            }
        }

        internal unsafe NativeManifest Handle;

        unsafe internal Manifest(NativeManifest handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Creates a `Manifest` object
        /// </summary>
        /// <param name="json">string of json data describing the manifest</param>
        public unsafe Manifest(string json)
        {
            var status = NativeInterface.Manifest.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Manifest Error Status: {status}");
            }
        }

        /// <summary>
        /// Creates a `Manifest` object
        /// </summary>
        /// <param name="data">byte array of data describing the manifest</param>
        /// <param name="encoding">binary encoding for the data</param>
        public unsafe Manifest(byte[] data, BinarySerializationEncoding encoding)
        {
            fixed (byte* pointer = data)
            {
                var status = encoding == BinarySerializationEncoding.BSON
                    ? NativeInterface.Manifest.FromBson(pointer, (ulong)data.Length, out Handle)
                    : NativeInterface.Manifest.FromMsgPack(pointer, (ulong)data.Length, out Handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Manifest Error Binary Ctor: {status}");
                }
            }
        }

        /// <summary>
        /// Creates a `Manifest` object
        /// </summary>
        /// <param name="electionScopeId"></param>
        /// <param name="electionType">election type</param>
        /// <param name="startDate">start date for election</param>
        /// <param name="endDate">end data for the election</param>
        /// <param name="gpUnits">array of the `GeopoliticalUnit` for election</param>
        /// <param name="parties">array of the `Party` for election</param>
        /// <param name="candidates">array of the `Candidate` for election</param>
        /// <param name="contests">array of the `ContestDescription` for election</param>
        /// <param name="ballotStyles">array of the `BallotStyle` for election</param>
        public unsafe Manifest(
            string electionScopeId, ElectionType electionType,
            DateTime startDate, DateTime endDate,
            GeopoliticalUnit[] gpUnits, Party[] parties,
            Candidate[] candidates, ContestDescription[] contests,
            BallotStyle[] ballotStyles)
        {
            IntPtr[] gpUnitPointers = new IntPtr[gpUnits.Length];
            for (var i = 0; i < gpUnits.Length; i++)
            {
                gpUnitPointers[i] = gpUnits[i].Handle.Ptr;
                gpUnits[i].Dispose();
            }

            IntPtr[] partyPointers = new IntPtr[parties.Length];
            for (var i = 0; i < parties.Length; i++)
            {
                partyPointers[i] = parties[i].Handle.Ptr;
                parties[i].Dispose();
            }

            IntPtr[] candidatePointers = new IntPtr[candidates.Length];
            for (var i = 0; i < candidates.Length; i++)
            {
                candidatePointers[i] = candidates[i].Handle.Ptr;
                candidates[i].Dispose();
            }

            IntPtr[] contestPointers = new IntPtr[contests.Length];
            for (var i = 0; i < contests.Length; i++)
            {
                contestPointers[i] = contests[i].Handle.Ptr;
                contests[i].Dispose();
            }

            IntPtr[] ballotStylePointers = new IntPtr[ballotStyles.Length];
            for (var i = 0; i < ballotStyles.Length; i++)
            {
                ballotStylePointers[i] = ballotStyles[i].Handle.Ptr;
                ballotStyles[i].Dispose();
            }

            var status = NativeInterface.Manifest.New(
                electionScopeId, electionType,
                (ulong)new DateTimeOffset(startDate).ToUnixTimeMilliseconds(),
                (ulong)new DateTimeOffset(endDate).ToUnixTimeMilliseconds(),
                gpUnitPointers, (ulong)gpUnitPointers.LongLength,
                partyPointers, (ulong)partyPointers.LongLength,
                candidatePointers, (ulong)candidatePointers.LongLength,
                contestPointers, (ulong)contestPointers.LongLength,
                ballotStylePointers, (ulong)ballotStylePointers.LongLength,
                 out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Manifest Error Status: {status}");
            }
        }

        /// <summary>
        /// Creates a `Manifest` object
        /// </summary>
        /// <param name="electionScopeId"></param>
        /// <param name="electionType">election type</param>
        /// <param name="startDate">start date for election</param>
        /// <param name="endDate">end data for the election</param>
        /// <param name="gpUnits">array of the `GeopoliticalUnit` for election</param>
        /// <param name="parties">array of the `Party` for election</param>
        /// <param name="candidates">array of the `Candidate` for election</param>
        /// <param name="contests">array of the `ContestDescription` for election</param>
        /// <param name="ballotStyles">array of the `BallotStyle` for election</param>
        /// <param name="name">name of the election</param>
        /// <param name="contact">contact information for the election</param>
        public unsafe Manifest(
             string electionScopeId, ElectionType electionType,
             DateTime startDate, DateTime endDate,
             GeopoliticalUnit[] gpUnits, Party[] parties,
             Candidate[] candidates, ContestDescription[] contests,
             BallotStyle[] ballotStyles, InternationalizedText name, ContactInformation contact)
        {
            IntPtr[] gpUnitPointers = new IntPtr[gpUnits.Length];
            for (var i = 0; i < gpUnits.Length; i++)
            {
                gpUnitPointers[i] = gpUnits[i].Handle.Ptr;
                gpUnits[i].Dispose();
            }

            IntPtr[] partyPointers = new IntPtr[parties.Length];
            for (var i = 0; i < parties.Length; i++)
            {
                partyPointers[i] = parties[i].Handle.Ptr;
                parties[i].Dispose();
            }

            IntPtr[] candidatePointers = new IntPtr[candidates.Length];
            for (var i = 0; i < candidates.Length; i++)
            {
                candidatePointers[i] = candidates[i].Handle.Ptr;
                candidates[i].Dispose();
            }

            IntPtr[] contestPointers = new IntPtr[contests.Length];
            for (var i = 0; i < contests.Length; i++)
            {
                contestPointers[i] = contests[i].Handle.Ptr;
                contests[i].Dispose();
            }

            IntPtr[] ballotStylePointers = new IntPtr[ballotStyles.Length];
            for (var i = 0; i < ballotStyles.Length; i++)
            {
                ballotStylePointers[i] = ballotStyles[i].Handle.Ptr;
                ballotStyles[i].Dispose();
            }

            var status = NativeInterface.Manifest.New(
                electionScopeId, electionType,
                (ulong)new DateTimeOffset(startDate).ToUnixTimeMilliseconds(),
                (ulong)new DateTimeOffset(endDate).ToUnixTimeMilliseconds(),
                gpUnitPointers, (ulong)gpUnitPointers.LongLength,
                partyPointers, (ulong)partyPointers.LongLength,
                candidatePointers, (ulong)candidatePointers.LongLength,
                contestPointers, (ulong)contestPointers.LongLength,
                ballotStylePointers, (ulong)ballotStylePointers.LongLength,
                name.Handle,
                contact.Handle,
                out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Manifest Error Status: {status}");
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// Collection of geopolitical units for this election.
        /// </Summary>
        public unsafe GeopoliticalUnit GetGeopoliticalUnitAtIndex(ulong index)
        {
            var status = NativeInterface.Manifest.GetGeopoliticalUnitAtIndex(
                Handle, index, out NativeGeopoliticalUnit value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Manifest Error GetGeopoliticalUnitAtIndex: {status}");
            }
            return new GeopoliticalUnit(value);
        }

        /// <Summary>
        /// Collection of parties for this election.
        /// </Summary>
        public unsafe Party GetPartyAtIndex(ulong index)
        {
            var status = NativeInterface.Manifest.GetPartyAtIndex(
                Handle, index, out NativeParty value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Manifest Error GetPartyAtIndex: {status}");
            }
            return new Party(value);
        }

        /// <Summary>
        /// Collection of candidates for this election.
        /// </Summary>
        public unsafe Candidate GetCandidateAtIndex(ulong index)
        {
            var status = NativeInterface.Manifest.GetCandidateAtIndex(
                Handle, index, out NativeCandidate value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Manifest Error GetCandidateAtIndex: {status}");
            }
            return new Candidate(value);
        }

        /// <Summary>
        /// Collection of contests for this election.
        /// </Summary>
        public unsafe ContestDescription GetContestAtIndex(ulong index)
        {
            var status = NativeInterface.Manifest.GetContestAtIndex(
                Handle, index, out NativeContestDescription value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Manifest Error GetContestAtIndex: {status}");
            }
            return new ContestDescription(value);
        }

        /// <Summary>
        /// Collection of ballot styles for this election.
        /// </Summary>
        public unsafe BallotStyle GetBallotStyleAtIndex(ulong index)
        {
            var status = NativeInterface.Manifest.GetBallotStyleAtIndex(
                Handle, index, out NativeBallotStyle value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Manifest Error GetContestAtIndex: {status}");
            }
            return new BallotStyle(value);
        }

        /// <Summary>
        /// A hash representation of the object
        /// </Summary>
        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.Manifest.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CryptoHash Error Status: {status}");
            }
            return new ElementModQ(value);
        }

        /// <Summary>
        /// Check wether the election manifest is valid and well-formed.
        /// </Summary>
        public unsafe bool IsValid()
        {
            var value = NativeInterface.Manifest.IsValid(Handle);
            return value;
        }

        /// <Summary>
        /// Export the ballot representation as JSON
        /// </Summary>
        public unsafe string ToJson()
        {
            var status = NativeInterface.Manifest.ToJson(
                Handle, out IntPtr pointer, out ulong size);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ToJson Error Status: {status}");
            }
            var json = Marshal.PtrToStringAnsi(pointer);
            NativeInterface.Memory.FreeIntPtr(pointer);
            return json;
        }

        /// <Summary>
        /// Export the ballot representation as ToBson
        /// </Summary>
        public unsafe byte[] ToBson()
        {

            var status = NativeInterface.Manifest.ToBson(
                Handle, out IntPtr data, out ulong size);

            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Manifest Error ToBson: {status}");
            }

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"Manifest Error ToBson: size is too big. Expected <= {int.MaxValue}, Actual: {size}.");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
        }

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        public unsafe byte[] ToMsgPack()
        {

            var status = NativeInterface.Manifest.ToMsgPack(
                Handle, out IntPtr data, out ulong size);

            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"Manifest Error ToMsgPack: {status}");
            }

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"Manifest Error ToMsgPack: size is too big. Expected <= {int.MaxValue}, actual: {size}.");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
        }
    }

    /// <summary>
    /// `InternalManifest` is a subset of the `Manifest` structure that specifies
    /// the components that ElectionGuard uses for conducting an election.  The key component is the
    /// `contests` collection, which applies placeholder selections to the `Manifest` contests
    /// </summary>
    public class InternalManifest : DisposableBase
    {
        /// <summary>
        /// The hash of the election manifest
        /// </summary>
        public unsafe ElementModQ ManifestHash
        {
            get
            {
                var status = NativeInterface.InternalManifest.GetManifestHash(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ManifestHash Error Status: {status}");
                }
                return new ElementModQ(value);
            }
        }

        /// <Summary>
        /// The size of the geopolitical units collection
        /// </Summary>
        public unsafe ulong GeopoliticalUnitsSize
        {
            get
            {
                var value = NativeInterface.InternalManifest.GetGeopoliticalUnitsSize(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The size of the contests collection
        /// </Summary>
        public unsafe ulong ContestsSize
        {
            get
            {
                var value = NativeInterface.InternalManifest.GetContestsSize(Handle);
                return value;
            }
        }

        /// <Summary>
        /// The size of the ballot styles collection
        /// </Summary>
        public unsafe ulong BallotStylesSize
        {
            get
            {
                var value = NativeInterface.InternalManifest.GetBallotStylesSize(Handle);
                return value;
            }
        }

        internal unsafe NativeInternalManifest Handle;

        /// <summary>
        /// Creates an `InternalManifest` object
        /// </summary>
        /// <param name="manifest">public manifest to copy</param>
        public unsafe InternalManifest(Manifest manifest)
        {
            var status = NativeInterface.InternalManifest.New(manifest.Handle, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"InternalManifest Error Status: {status}");
            }
        }

        /// <summary>
        /// Creates an `InternalManifest` object
        /// </summary>
        /// <param name="json">string of json data describing the manifest</param>
        public unsafe InternalManifest(string json)
        {
            var status = NativeInterface.InternalManifest.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"InternalManifest Error Status: {status}");
            }
        }

        /// <summary>
        /// Creates an `InternalManifest` object
        /// </summary>
        /// <param name="data">byte array of data describing the manifest</param>
        /// <param name="encoding">binary encoding for the data</param>
        public unsafe InternalManifest(byte[] data, BinarySerializationEncoding encoding)
        {
            fixed (byte* pointer = data)
            {
                var status = encoding == BinarySerializationEncoding.BSON
                    ? NativeInterface.InternalManifest.FromBson(pointer, (ulong)data.Length, out Handle)
                    : NativeInterface.InternalManifest.FromMsgPack(pointer, (ulong)data.Length, out Handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"InternalManifest Error Binary Ctor: {status}");
                }
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// Collection of geopolitical units for this election.
        /// </Summary>
        public unsafe GeopoliticalUnit GetGeopoliticalUnitAtIndex(ulong index)
        {
            var status = NativeInterface.InternalManifest.GetGeopoliticalUnitAtIndex(
                Handle, index, out NativeGeopoliticalUnit value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"InternalManifest Error GetGeopoliticalUnitAtIndex: {status}");
            }
            return new GeopoliticalUnit(value);
        }

        /// <Summary>
        /// Collection of contests for this election.
        /// </Summary>
        public unsafe ContestDescription GetContestAtIndex(ulong index)
        {
            var status = NativeInterface.InternalManifest.GetContestAtIndex(
                Handle, index, out NativeContestDescription value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"InternalManifest Error GetContestAtIndex: {status}");
            }
            return new ContestDescription(value);
        }

        /// <Summary>
        /// Collection of ballot styles for this election.
        /// </Summary>
        public unsafe BallotStyle GetBallotStyleAtIndex(ulong index)
        {
            var status = NativeInterface.InternalManifest.GetBallotStyleAtIndex(
                Handle, index, out NativeBallotStyle value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"InternalManifest Error GetContestAtIndex: {status}");
            }
            return new BallotStyle(value);
        }

        /// <Summary>
        /// Export the ballot representation as JSON
        /// </Summary>
        public unsafe string ToJson()
        {
            var status = NativeInterface.InternalManifest.ToJson(
                Handle, out IntPtr pointer, out ulong size);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ToJson Error Status: {status}");
            }
            var json = Marshal.PtrToStringAnsi(pointer);
            NativeInterface.Memory.FreeIntPtr(pointer);
            return json;
        }

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        public unsafe byte[] ToBson()
        {

            var status = NativeInterface.InternalManifest.ToBson(
                Handle, out IntPtr data, out ulong size);

            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"InternalManifest Error ToBson: {status}");
            }

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"InternalManifest Error ToBson: size is too big. Expected <= {int.MaxValue}, actual: {size}.");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
        }

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        public unsafe byte[] ToMsgPack()
        {

            var status = NativeInterface.InternalManifest.ToMsgPack(
                Handle, out IntPtr data, out ulong size);

            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"InternalManifest Error ToMsgPack: {status}");
            }

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"InternalManifest Error ToMsgPack: size is too big. Expected <= {int.MaxValue}, actual: {size}.");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
        }
    }
}
