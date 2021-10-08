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

    public class AnnotatedString : DisposableBase
    {
        public unsafe string Annotation
        {
            get
            {
                var status = NativeInterface.AnnotatedString.GetAnnotation(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"AnnotatedString Error Annotation: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string Value
        {
            get
            {
                var status = NativeInterface.AnnotatedString.GetValue(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"AnnotatedString Error Value: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        internal unsafe NativeAnnotatedString Handle;

        unsafe internal AnnotatedString(
            NativeAnnotatedString handle)
        {
            Handle = handle;
        }

        public unsafe AnnotatedString(string annotation, string value)
        {
            var status = NativeInterface.AnnotatedString.New(annotation, value, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"AnnotatedString Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
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
                Console.WriteLine($"CryptoHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region Language

    public class Language : DisposableBase
    {
        public unsafe string Value
        {
            get
            {
                var status = NativeInterface.Language.GetValue(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Language Error Value: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string LanguageAbbreviation
        {
            get
            {
                var status = NativeInterface.Language.GetLanguage(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Language Error LanguageAbbreviation: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        internal unsafe NativeLanguage Handle;

        unsafe internal Language(
            NativeLanguage handle)
        {
            Handle = handle;
        }

        public unsafe Language(string value, string language)
        {
            var status = NativeInterface.Language.New(value, language, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Language Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
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
                Console.WriteLine($"CryptoHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region InternationalizedText

    public class InternationalizedText : DisposableBase
    {
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

        public unsafe InternationalizedText(Language[] text)
        {
            IntPtr[] nativeText = new IntPtr[text.Length];
            for (var i = 0; i < text.Length; i++)
            {
                nativeText[i] = text[i].Handle.Ptr;
                text[i].Dispose();
            }
            var status = NativeInterface.InternationalizedText.New(nativeText, text.Length, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternationalizedText Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        public unsafe Language GetTextAt(ulong index)
        {
            var status = NativeInterface.InternationalizedText.GetTextAtIndex(
                Handle, index, out NativeLanguage value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternationalizedText Error GetTextAt: {status}");
                return null;
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
                Console.WriteLine($"CryptoHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region ContactInformation

    public class ContactInformation : DisposableBase
    {
        public unsafe ulong AddressLineSize
        {
            get
            {
                var size = NativeInterface.ContactInformation.GetAddressLineSize(
                    Handle);
                return size;
            }
        }

        public unsafe ulong EmailLineSize
        {
            get
            {
                var size = NativeInterface.ContactInformation.GetEmailLineSize(
                    Handle);
                return size;
            }
        }

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

        public unsafe ContactInformation(string name)
        {
            var status = NativeInterface.ContactInformation.New(name, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ContactInformation Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        public unsafe string GetAddressLineAt(ulong index)
        {
            var status = NativeInterface.ContactInformation.GetAddressLineAtIndex(
                Handle, index, out IntPtr value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ContactInformation Error GetAddressLineAt: {status}");
                return null;
            }
            return Marshal.PtrToStringAnsi(value);
        }

        public unsafe InternationalizedText GetEmailLineAt(ulong index)
        {
            var status = NativeInterface.ContactInformation.GetEmailLineAtIndex(
                Handle, index, out NativeInternationalizedText value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ContactInformation Error GetEmailLineAt: {status}");
                return null;
            }
            return new InternationalizedText(value);
        }

        public unsafe InternationalizedText GetPhoneLineAt(ulong index)
        {
            var status = NativeInterface.ContactInformation.GetPhoneLineAtIndex(
                Handle, index, out NativeInternationalizedText value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ContactInformation Error GetPhoneLineAt: {status}");
                return null;
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
                Console.WriteLine($"CryptoHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region GeopoliticalUnit

    public class GeopoliticalUnit : DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.GeopoliticalUnit.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"GeopoliticalUnit Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string Name
        {
            get
            {
                var status = NativeInterface.GeopoliticalUnit.GetName(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"GeopoliticalUnit Error Name: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

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

        public unsafe GeopoliticalUnit(
            string objectId, string name, ReportingUnitType reportingUnitType)
        {
            var status = NativeInterface.GeopoliticalUnit.New(
                objectId, name, reportingUnitType, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ContactInformation Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
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
                Console.WriteLine($"CryptoHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region BallotStyle

    public class BallotStyle : DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.BallotStyle.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"BallotStyle Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe ulong GeopoliticalUnitIdsSize
        {
            get
            {
                var size = NativeInterface.BallotStyle.GetGeopoliticalUnitSize(
                    Handle);
                return size;
            }
        }

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

        public unsafe BallotStyle(string objectId, string[] gpUnitIds)
        {
            var status = NativeInterface.BallotStyle.New(objectId, gpUnitIds, gpUnitIds.Length, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"BallotStyle Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        public unsafe String GetGeopoliticalUnitIdAt(ulong index)
        {
            var status = NativeInterface.BallotStyle.GetGeopoliticalInitIdAtIndex(
                Handle, index, out IntPtr value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"BallotStyle Error GetGeopoliticalUnitIdAt: {status}");
                return null;
            }
            return Marshal.PtrToStringAnsi(value);
        }

        public unsafe String GetPartyIdAt(ulong index)
        {
            var status = NativeInterface.BallotStyle.GetPartyIdAtIndex(
                Handle, index, out IntPtr value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"BallotStyle Error GetPartyIdAt: {status}");
                return null;
            }
            return Marshal.PtrToStringAnsi(value);
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
                Console.WriteLine($"CryptoHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region Party

    public class Party : DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.Party.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Party Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string Abbreviation
        {
            get
            {
                var status = NativeInterface.Party.GetAbbreviation(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Party Error Abbreviation: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe InternationalizedText Name
        {
            get
            {
                var status = NativeInterface.Party.GetName(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Party Error Name: {status}");
                    return null;
                }
                return new InternationalizedText(value);
            }
        }

        public unsafe string Color
        {
            get
            {
                var status = NativeInterface.Party.GetColor(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Party Error Color: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string LogoUri
        {
            get
            {
                var status = NativeInterface.Party.GetLogoUri(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Party Error LogoUri: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        internal unsafe NativeParty Handle;

        unsafe internal Party(
            NativeParty handle)
        {
            Handle = handle;
        }

        public unsafe Party(string objectId)
        {
            var status = NativeInterface.Party.New(objectId, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Party Error Status: {status}");
            }
        }

        public unsafe Party(
            string objectId, InternationalizedText name,
            string abbreviation, string color, string logoUri)
        {
            var status = NativeInterface.Party.New(
                objectId, name.Handle, abbreviation, color, logoUri, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Party Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
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
                Console.WriteLine($"CryptoHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region Candidate

    public class Candidate : DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.Candidate.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Candidate Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string CandidateId
        {
            get
            {
                var status = NativeInterface.Candidate.GetCandidateId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Candidate Error CandidateId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string Name
        {
            get
            {
                var status = NativeInterface.Candidate.GetName(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Candidate Error Name: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string PartyId
        {
            get
            {
                var status = NativeInterface.Candidate.GetPartyId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Candidate Error PartyId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string ImageUri
        {
            get
            {
                var status = NativeInterface.Candidate.GetImageUri(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Candidate Error ImageUri: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        internal unsafe NativeCandidate Handle;

        unsafe internal Candidate(
            NativeCandidate handle)
        {
            Handle = handle;
        }

        public unsafe Candidate(string objectId, bool isWriteIn)
        {
            var status = NativeInterface.Candidate.New(objectId, isWriteIn, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Candidate Error Status: {status}");
            }
        }

        public unsafe Candidate(
            string objectId, InternationalizedText name,
            string partyId, string imageUri, bool isWriteIn)
        {
            var status = NativeInterface.Candidate.New(
                objectId, name.Handle, partyId, imageUri, isWriteIn, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Candidate Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
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
                Console.WriteLine($"CryptoHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region SelectionDescription

    public class SelectionDescription : DisposableBase
    {

        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.SelectionDescription.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"SelectionDescription Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string CandidateId
        {
            get
            {
                var status = NativeInterface.SelectionDescription.GetCandidateId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"SelectionDescription Error CandidateId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

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

        public unsafe SelectionDescription(
            string objectId, string candidateId, ulong sequenceOrder)
        {
            var status = NativeInterface.SelectionDescription.New(
                objectId, candidateId, sequenceOrder, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"SelectionDescription Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
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
                Console.WriteLine($"CryptoHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region ContestDescription

    public class ContestDescription : DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.ContestDescription.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ContestDescription Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string ElectoralDistrictId
        {
            get
            {
                var status = NativeInterface.ContestDescription.GetElectoralDistrictId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ContestDescription Error ElectoralDistrictId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe ulong SequenceOrder
        {
            get
            {
                var value = NativeInterface.ContestDescription.GetSequenceOrder(Handle);
                return value;
            }
        }

        public unsafe VoteVariationType VoteVariationType
        {
            get
            {
                var value = NativeInterface.ContestDescription.GetVoteVariationType(Handle);
                return value;
            }
        }

        public unsafe ulong NumberElected
        {
            get
            {
                var value = NativeInterface.ContestDescription.GetNumberElected(Handle);
                return value;
            }
        }

        public unsafe ulong VotesAllowed
        {
            get
            {
                var value = NativeInterface.ContestDescription.GetVotesAllowed(Handle);
                return value;
            }
        }

        public unsafe string Name
        {
            get
            {
                var status = NativeInterface.ContestDescription.GetName(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ContestDescription Error Name: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe InternationalizedText BallotTitle
        {
            get
            {
                var status = NativeInterface.ContestDescription.GetBallotTitle(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ContestDescription Error BallotTitle: {status}");
                    return null;
                }
                return new InternationalizedText(value);
            }
        }

        public unsafe InternationalizedText BallotSubTitle
        {
            get
            {
                var status = NativeInterface.ContestDescription.GetBallotSubTitle(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ContestDescription Error BallotSubTitle: {status}");
                    return null;
                }
                return new InternationalizedText(value);
            }
        }

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
                Console.WriteLine($"ContestDescription Error Status: {status}");
            }
        }

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
                Console.WriteLine($"ContestDescription Error Status: {status}");
            }
        }

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
                Console.WriteLine($"ContestDescription Error Status: {status}");
            }
        }

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
                Console.WriteLine($"ContestDescription Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        public unsafe SelectionDescription GetSelectionAtIndex(ulong index)
        {
            var status = NativeInterface.ContestDescription.GetSelectionAtIndex(
                Handle, index, out NativeSelectionDescription value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ContestDescription Error GetSelectionAtIndex: {status}");
                return null;
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
                Console.WriteLine($"CryptoHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }
    }

    #endregion

    #region ContestDescriptionWithPlaceholders

    public class ContestDescriptionWithPlaceholders : DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.ContestDescriptionWithPlaceholders.GetObjectId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ContestDescriptionWithPlaceholders Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string ElectoralDistrictId
        {
            get
            {
                var status = NativeInterface.ContestDescriptionWithPlaceholders.GetElectoralDistrictId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ContestDescriptionWithPlaceholders Error ElectoralDistrictId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe ulong SequenceOrder
        {
            get
            {
                var value = NativeInterface.ContestDescriptionWithPlaceholders.GetSequenceOrder(Handle);
                return value;
            }
        }

        public unsafe VoteVariationType VoteVariationType
        {
            get
            {
                var value = NativeInterface.ContestDescriptionWithPlaceholders.GetVoteVariationType(Handle);
                return value;
            }
        }

        public unsafe ulong NumberElected
        {
            get
            {
                var value = NativeInterface.ContestDescriptionWithPlaceholders.GetNumberElected(Handle);
                return value;
            }
        }

        public unsafe ulong VotesAllowed
        {
            get
            {
                var value = NativeInterface.ContestDescriptionWithPlaceholders.GetVotesAllowed(Handle);
                return value;
            }
        }

        public unsafe string Name
        {
            get
            {
                var status = NativeInterface.ContestDescriptionWithPlaceholders.GetName(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ContestDescriptionWithPlaceholders Error Name: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe InternationalizedText BallotTitle
        {
            get
            {
                var status = NativeInterface.ContestDescriptionWithPlaceholders.GetBallotTitle(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ContestDescriptionWithPlaceholders Error BallotTitle: {status}");
                    return null;
                }
                return new InternationalizedText(value);
            }
        }

        public unsafe InternationalizedText BallotSubTitle
        {
            get
            {
                var status = NativeInterface.ContestDescriptionWithPlaceholders.GetBallotSubTitle(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ContestDescriptionWithPlaceholders Error BallotSubTitle: {status}");
                    return null;
                }
                return new InternationalizedText(value);
            }
        }

        public unsafe ulong SelectionsSize
        {
            get
            {
                var value = NativeInterface.ContestDescriptionWithPlaceholders.GetSelectionsSize(Handle);
                return value;
            }
        }

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
                Console.WriteLine($"ContestDescriptionWithPlaceholders Error Status: {status}");
            }
        }

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
                Console.WriteLine($"ContestDescription Error Status: {status}");
            }
        }

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
                Console.WriteLine($"ContestDescription Error Status: {status}");
            }
        }

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
                Console.WriteLine($"ContestDescription Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        public unsafe SelectionDescription GetSelectionAtIndex(ulong index)
        {
            var status = NativeInterface.ContestDescriptionWithPlaceholders.GetSelectionAtIndex(
                Handle, index, out NativeSelectionDescription value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ContestDescriptionWithPlaceholders ContestDescription GetSelectionAtIndex: {status}");
                return null;
            }
            return new SelectionDescription(value);
        }

        public unsafe SelectionDescription GetPlaceholderAtIndex(ulong index)
        {
            var status = NativeInterface.ContestDescriptionWithPlaceholders.GetPlaceholderAtIndex(
                Handle, index, out NativeSelectionDescription value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ContestDescriptionWithPlaceholders ContestDescription GetSelectionAtIndex: {status}");
                return null;
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
                Console.WriteLine($"ContestDescriptionWithPlaceholders Error CryptoHash: {status}");
                return null;
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
                    Console.WriteLine($"Manifest Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe ElectionType ElectionType
        {
            get
            {
                var value = NativeInterface.Manifest.GetElectionType(Handle);
                return value;
            }
        }

        // TODO: datetime type
        public unsafe DateTime StartDate
        {
            get
            {
                var value = NativeInterface.Manifest.GetStartDate(Handle);
                return DateTimeOffset.FromUnixTimeMilliseconds((long)value).DateTime;
            }
        }

        public unsafe DateTime EndDate
        {
            get
            {
                var value = NativeInterface.Manifest.GetEndDate(Handle);
                return DateTimeOffset.FromUnixTimeMilliseconds((long)value).DateTime;
            }
        }

        public unsafe ulong GeopoliticalUnitsSize
        {
            get
            {
                var value = NativeInterface.Manifest.GetGeopoliticalUnitsSize(Handle);
                return value;
            }
        }

        public unsafe ulong PartiesSize
        {
            get
            {
                var value = NativeInterface.Manifest.GetPartiesSize(Handle);
                return value;
            }
        }

        public unsafe ulong CandidatesSize
        {
            get
            {
                var value = NativeInterface.Manifest.GetCandidatesSize(Handle);
                return value;
            }
        }

        public unsafe ulong ContestsSize
        {
            get
            {
                var value = NativeInterface.Manifest.GetContestsSize(Handle);
                return value;
            }
        }

        public unsafe ulong BallotStylesSize
        {
            get
            {
                var value = NativeInterface.Manifest.GetBallotStylesSize(Handle);
                return value;
            }
        }

        public unsafe InternationalizedText Name
        {
            get
            {
                var status = NativeInterface.Manifest.GetName(
                    Handle, out NativeInternationalizedText value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Manifest Error ObjectId: {status}");
                    return null;
                }
                return new InternationalizedText(value);
            }
        }

        public unsafe ContactInformation ContactInfo
        {
            get
            {
                var status = NativeInterface.Manifest.GetContactInfo(
                    Handle, out NativeContactInformation value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Manifest Error ObjectId: {status}");
                    return null;
                }
                return new ContactInformation(value);
            }
        }

        internal unsafe NativeManifest Handle;

        unsafe internal Manifest(NativeManifest handle)
        {
            Handle = handle;
        }

        public unsafe Manifest(string json)
        {
            var status = NativeInterface.Manifest.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Manifest Error Status: {status}");
            }
        }

        public unsafe Manifest(byte[] data, BinarySerializationEncoding encoding)
        {
            fixed (byte* pointer = data)
            {
                var status = encoding == BinarySerializationEncoding.BSON
                    ? NativeInterface.Manifest.FromBson(pointer, (ulong)data.Length, out Handle)
                    : NativeInterface.Manifest.FromMsgPack(pointer, (ulong)data.Length, out Handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Manifest Error Binary Ctor: {status}");
                }
            }
        }

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
                Console.WriteLine($"Manifest Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        public unsafe GeopoliticalUnit GetGeopoliticalUnitAtIndex(ulong index)
        {
            var status = NativeInterface.Manifest.GetGeopoliticalUnitAtIndex(
                Handle, index, out NativeGeopoliticalUnit value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Manifest Error GetGeopoliticalUnitAtIndex: {status}");
                return null;
            }
            return new GeopoliticalUnit(value);
        }

        public unsafe Party GetPartyAtIndex(ulong index)
        {
            var status = NativeInterface.Manifest.GetPartyAtIndex(
                Handle, index, out NativeParty value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Manifest Error GetPartyAtIndex: {status}");
                return null;
            }
            return new Party(value);
        }

        public unsafe Candidate GetCandidateAtIndex(ulong index)
        {
            var status = NativeInterface.Manifest.GetCandidateAtIndex(
                Handle, index, out NativeCandidate value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Manifest Error GetCandidateAtIndex: {status}");
                return null;
            }
            return new Candidate(value);
        }

        public unsafe ContestDescription GetContestAtIndex(ulong index)
        {
            var status = NativeInterface.Manifest.GetContestAtIndex(
                Handle, index, out NativeContestDescription value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Manifest Error GetContestAtIndex: {status}");
                return null;
            }
            return new ContestDescription(value);
        }

        public unsafe BallotStyle GetBallotStyleAtIndex(ulong index)
        {
            var status = NativeInterface.Manifest.GetBallotStyleAtIndex(
                Handle, index, out NativeBallotStyle value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Manifest Error GetContestAtIndex: {status}");
                return null;
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
                Console.WriteLine($"CryptoHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }

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
                Handle, out IntPtr pointer, out UIntPtr size);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ToJson Error Status: {status}");
                return null;
            }
            var json = Marshal.PtrToStringAnsi(pointer);
            return json;
        }

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        public unsafe byte[] ToBson()
        {

            var status = NativeInterface.Manifest.ToBson(
                Handle, out IntPtr data, out UIntPtr size);

            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Manifest Error ToBson: {status}");
                return null;
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
                Handle, out IntPtr data, out UIntPtr size);

            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Manifest Error ToMsgPack: {status}");
                return null;
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
                    Console.WriteLine($"ManifestHash Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        public unsafe ulong GeopoliticalUnitsSize
        {
            get
            {
                var value = NativeInterface.InternalManifest.GetGeopoliticalUnitsSize(Handle);
                return value;
            }
        }

        public unsafe ulong ContestsSize
        {
            get
            {
                var value = NativeInterface.InternalManifest.GetContestsSize(Handle);
                return value;
            }
        }

        public unsafe ulong BallotStylesSize
        {
            get
            {
                var value = NativeInterface.InternalManifest.GetBallotStylesSize(Handle);
                return value;
            }
        }

        internal unsafe NativeInternalManifest Handle;

        public unsafe InternalManifest(Manifest manifest)
        {
            var status = NativeInterface.InternalManifest.New(manifest.Handle, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalManifest Error Status: {status}");
            }
        }

        public unsafe InternalManifest(string json)
        {
            var status = NativeInterface.InternalManifest.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalManifest Error Status: {status}");
            }
        }

        public unsafe InternalManifest(byte[] data, BinarySerializationEncoding encoding)
        {
            fixed (byte* pointer = data)
            {
                var status = encoding == BinarySerializationEncoding.BSON
                    ? NativeInterface.InternalManifest.FromBson(pointer, (ulong)data.Length, out Handle)
                    : NativeInterface.InternalManifest.FromMsgPack(pointer, (ulong)data.Length, out Handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"InternalManifest Error Binary Ctor: {status}");
                }
            }
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        public unsafe GeopoliticalUnit GetGeopoliticalUnitAtIndex(ulong index)
        {
            var status = NativeInterface.InternalManifest.GetGeopoliticalUnitAtIndex(
                Handle, index, out NativeGeopoliticalUnit value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalManifest Error GetGeopoliticalUnitAtIndex: {status}");
                return null;
            }
            return new GeopoliticalUnit(value);
        }

        public unsafe ContestDescription GetContestAtIndex(ulong index)
        {
            var status = NativeInterface.InternalManifest.GetContestAtIndex(
                Handle, index, out NativeContestDescription value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalManifest Error GetContestAtIndex: {status}");
                return null;
            }
            return new ContestDescription(value);
        }

        public unsafe BallotStyle GetBallotStyleAtIndex(ulong index)
        {
            var status = NativeInterface.InternalManifest.GetBallotStyleAtIndex(
                Handle, index, out NativeBallotStyle value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalManifest Error GetContestAtIndex: {status}");
                return null;
            }
            return new BallotStyle(value);
        }

        /// <Summary>
        /// Export the ballot representation as JSON
        /// </Summary>
        public unsafe string ToJson()
        {
            var status = NativeInterface.InternalManifest.ToJson(
                Handle, out IntPtr pointer, out UIntPtr size);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ToJson Error Status: {status}");
                return null;
            }
            var json = Marshal.PtrToStringAnsi(pointer);
            return json;
        }

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        public unsafe byte[] ToBson()
        {

            var status = NativeInterface.InternalManifest.ToBson(
                Handle, out IntPtr data, out UIntPtr size);

            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalManifest Error ToBson: {status}");
                return null;
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
                Handle, out IntPtr data, out UIntPtr size);

            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalManifest Error ToMsgPack: {status}");
                return null;
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
        }
    }
}
