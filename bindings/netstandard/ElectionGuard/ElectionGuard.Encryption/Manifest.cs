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
    using NativeConstestDescriptionWithPlaceholders = NativeInterface.ContestDescriptionWithPlaceholders.ContestDescriptionWithPlaceholdersHandle;
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
        // TODO: ikmplement

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

        // TODO: ikmplement

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

        internal unsafe NativeManifest Handle;

        public unsafe Manifest(string json)
        {
            var status = NativeInterface.Manifest.FromJson(json, out Handle);
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

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
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
    }

}
