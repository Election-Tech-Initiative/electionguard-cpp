using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    // Declare native types for convenience
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQType;
    using NativeElGamalCiphertext = NativeInterface.ElGamalCiphertext.ElGamalCiphertextType;
    using NativePlaintextBallotSelection = NativeInterface.PlaintextBallotSelection.PlaintextBallotSelectionHandle;
    using NativeCiphertextBallotSelection = NativeInterface.CiphertextBallotSelection.CiphertextBallotSelectionType;
    using NativePlaintextBallotContest = NativeInterface.PlaintextBallotContest.PlaintextBallotContestHandle;
    using NativeCiphertextBallotContest = NativeInterface.CiphertextBallotContest.CiphertextBallotContestType;
    using NativePlaintextBallot = NativeInterface.PlaintextBallot.PlaintextBallotHandle;
    using NativeCiphertextBallot = NativeInterface.CiphertextBallot.CiphertextBallotType;

    #region PlaintextBallotSelction

    /// <summary>
    /// A BallotSelection represents an individual selection on a ballot.
    ///
    /// This class accepts a `vote` integer field which has no constraints
    /// in the ElectionGuard Data Specification, but is constrained logically
    /// in the application to resolve to `True` or `False`aka only 0 and 1 is
    /// supported for now.
    ///
    /// This class can also be designated as `isPlaceholderSelection` which has no
    /// context to the data specification but is useful for running validity checks internally
    ///
    /// an `extendedData` field exists to support any arbitrary data to be associated
    /// with the selection.  In practice, this field is the cleartext representation
    /// of a write-in candidate value.  In the current implementation these values are
    /// discarded when encrypting.
    /// </summary>
    public class PlaintextBallotSelection : DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.PlaintextBallotSelection.GetObjectId(Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"PlaintextBallotContest Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        /// <summary>
        /// Determines if this is a placeholder selection
        /// </summary>
        public unsafe bool IsPlaceholder
        {
            get
            {
                return NativeInterface.PlaintextBallotSelection.GetIsPlaceholder(Handle);
            }
        }

        internal unsafe NativePlaintextBallotSelection Handle;

        unsafe internal PlaintextBallotSelection(NativePlaintextBallotSelection handle)
        {
            Handle = handle;
        }

        public unsafe PlaintextBallotSelection(
            string objectId, long vote, bool isPlaceholder = false)
        {
            var status = NativeInterface.PlaintextBallotSelection.New(
                objectId, vote, isPlaceholder, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalElectionDescription Error Status: {status}");
            }
        }

        public unsafe PlaintextBallotSelection(
            string objectId, long vote, bool isPlaceholder, string extendedData)
        {
            var status = NativeInterface.PlaintextBallotSelection.New(
                objectId, vote, isPlaceholder, extendedData, extendedData.Length, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalElectionDescription Error Status: {status}");
            }
        }

        /// <summary>
        /// Given a PlaintextBallotSelection validates that the object matches an expected object
        /// and that the plaintext value can resolve to a valid representation
        /// </summary>
        public unsafe bool IsValid(string expectedObjectId)
        {
            return NativeInterface.PlaintextBallotSelection.IsValid(Handle, expectedObjectId);
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    #endregion

    #region CiphertextBallotSelection

    /// <summary>
    /// A CiphertextBallotSelection represents an individual encrypted selection on a ballot.
    ///
    /// This class accepts a `description_hash` and a `ciphertext` as required parameters
    /// in its constructor.
    ///
    /// When a selection is encrypted, the `description_hash` and `ciphertext` required fields must
    /// be populated at construction however the `nonce` is also usually provided by convention.
    ///
    /// After construction, the `crypto_hash` field is populated automatically in the `__post_init__` cycle
    ///
    /// A consumer of this object has the option to discard the `nonce` and/or discard the `proof`,
    /// or keep both values.
    ///
    /// By discarding the `nonce`, the encrypted representation and `proof`
    /// can only be regenerated if the nonce was derived from the ballot's master nonce.  If the nonce
    /// used for this selection is truly random, and it is discarded, then the proofs cannot be regenerated.
    ///
    /// By keeping the `nonce`, or deriving the selection nonce from the ballot nonce, an external system can
    /// regenerate the proofs on demand.  This is useful for storage or memory constrained systems.
    ///
    /// By keeping the `proof` the nonce is not required fotor verify the encrypted selection.
    /// </summary>
    public class CiphertextBallotSelection : DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.CiphertextBallotSelection.GetObjectId(Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallotSelection Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        /// <summary>
        /// The SelectionDescription hash
        /// </summary>
        public unsafe ElementModQ DescriptionHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallotSelection.GetDescriptionHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallotSelection Error DescriptionHash: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// Determines if this is a placeholder selection
        /// </summary>
        public unsafe bool IsPlaceholder
        {
            get
            {
                return NativeInterface.CiphertextBallotSelection.GetIsPlaceholder(Handle);
            }
        }

        /// <summary>
        /// The encrypted representation of the vote field
        /// </summary>
        public unsafe ElGamalCiphertext Ciphertext
        {
            get
            {
                var status = NativeInterface.CiphertextBallotSelection.GetCiphertext(
                    Handle, out NativeElGamalCiphertext* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallotSelection Error Ciphertext: {status}");
                    return null;
                }
                return new ElGamalCiphertext(value);
            }
        }

        /// <summary>
        /// The hash of the encrypted values
        /// </summary>
        public unsafe ElementModQ CryptoHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallotSelection.GetCryptoHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallotSelection Error CryptoHash: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// The nonce used to generate the encryption. Sensitive &amp; should be treated as a secret
        /// </summary>
        public unsafe ElementModQ Nonce
        {
            get
            {
                var status = NativeInterface.CiphertextBallotSelection.GetNonce(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallotSelection Error Nonce: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeCiphertextBallotSelection* Handle;

        unsafe internal CiphertextBallotSelection(NativeCiphertextBallotSelection* handle)
        {
            Handle = handle;
        }

        /// <sumary>
        /// Given an encrypted BallotSelection, validates the encryption state against a specific seed hash and public key.
        /// Calling this function expects that the object is in a well-formed encrypted state
        /// with the elgamal encrypted `message` field populated along with
        /// the DisjunctiveChaumPedersenProof`proof` populated.
        /// the ElementModQ `description_hash` and the ElementModQ `crypto_hash` are also checked.
        ///
        /// <param name="seed_hash">The hash of the SelectionDescription, or
        ///                         whatever `ElementModQ` was used to populate the `description_hash` field. </param>
        /// <param name="elgamalPublicKey">The election public key</param>
        /// <param name="cryptoExtendedBaseHash">The extended base hash of the election</param>
        /// </summary>
        public unsafe bool IsValidEncryption(
            ElementModQ seedHash, ElementModP elGamalPublicKey, ElementModQ cryptoExtendedBaseHash)
        {
            return NativeInterface.CiphertextBallotSelection.IsValidEncryption(
                Handle, seedHash.Handle, elGamalPublicKey.Handle, cryptoExtendedBaseHash.Handle);
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null) return;
            var status = NativeInterface.CiphertextBallotSelection.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"CiphertextBallotSelection Error DisposeUnmanaged: {status}");
            }
            Handle = null;
        }
    }

    #endregion

    #region PlaintextBallotContest

    public class PlaintextBallotContest : DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.PlaintextBallotContest.GetObjectId(Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"PlaintextBallotContest Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe ulong SelectionsSize
        {
            get
            {
                var size = NativeInterface.PlaintextBallotContest.GetSelectionsSize(Handle);
                return size;
            }
        }

        internal unsafe NativePlaintextBallotContest Handle;

        unsafe internal PlaintextBallotContest(NativePlaintextBallotContest handle)
        {
            Handle = handle;
        }

        public unsafe PlaintextBallotSelection GetSelectionAt(ulong index)
        {
            var status = NativeInterface.PlaintextBallotContest.GetSelectionAtIndex(
                Handle, index, out NativePlaintextBallotSelection value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"PlaintextBallotContest Error GetSelectionAt: {status}");
                return null;
            }
            return new PlaintextBallotSelection(value);
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    #endregion

    #region CiphertextBallotContest

    public class CiphertextBallotContest : DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.CiphertextBallotContest.GetObjectId(Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallotContest Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe ElementModQ DescriptionHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallotContest.GetDescriptionHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallotContest Error DescriptionHash: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        public unsafe ulong SelectionsSize
        {
            get
            {
                var size = NativeInterface.CiphertextBallotContest.GetSelectionsSize(Handle);
                return size;
            }
        }

        public unsafe ElementModQ CryptoHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallotContest.GetCryptoHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallotContest Error CryptoHash: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        public unsafe ElementModQ Nonce
        {
            get
            {
                var status = NativeInterface.CiphertextBallotContest.GetNonce(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallotContest Error Nonce: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeCiphertextBallotContest* Handle;

        unsafe internal CiphertextBallotContest(NativeCiphertextBallotContest* handle)
        {
            Handle = handle;
        }

        public unsafe CiphertextBallotSelection GetSelectionAt(ulong index)
        {
            var status = NativeInterface.CiphertextBallotContest.GetSelectionAtIndex(
                Handle, index, out NativeCiphertextBallotSelection* value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"CiphertextBallotContest Error GetSelectionAt: {status}");
                return null;
            }
            return new CiphertextBallotSelection(value);
        }

        public unsafe bool IsValidEncryption(
            ElementModQ seedHash, ElementModP elGamalPublicKey, ElementModQ cryptoExtendedBaseHash)
        {
            return NativeInterface.CiphertextBallotContest.IsValidEncryption(
                Handle, seedHash.Handle, elGamalPublicKey.Handle, cryptoExtendedBaseHash.Handle);
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null) return;
            var status = NativeInterface.CiphertextBallotContest.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"CiphertextBallotContest Error DisposeUnmanaged: {status}");
            }
            Handle = null;
        }
    }

    #endregion

    #region PlaintextBallot

    public class PlaintextBallot : DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.PlaintextBallot.GetObjectId(Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"PlaintextBallot Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string BallotStyle
        {
            get
            {
                var status = NativeInterface.PlaintextBallot.GetBallotStyle(Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"PlaintextBallot Error BallotStyle: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe ulong ContestsSize
        {
            get
            {
                var size = NativeInterface.PlaintextBallot.GetContestsSize(Handle);
                return size;
            }
        }

        internal unsafe NativePlaintextBallot Handle;

        public unsafe PlaintextBallot(string json)
        {
            var status = NativeInterface.PlaintextBallot.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"PlaintextBallot Error Status: {status}");
            }
        }

        public unsafe PlaintextBallotContest GetContestAt(ulong index)
        {
            var status = NativeInterface.PlaintextBallot.GetContestAtIndex(
                Handle, index, out NativePlaintextBallotContest value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"PlaintextBallot Error BallotStyle: {status}");
                return null;
            }
            return new PlaintextBallotContest(value);
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null) return;
            Handle.Dispose();
            Handle = null;
        }

        public unsafe string ToJson()
        {
            var status = NativeInterface.PlaintextBallot.ToJson(
                Handle, out IntPtr pointer, out UIntPtr size);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"PlaintextBallot Error ToJson: {status}");
                return null;
            }
            var json = Marshal.PtrToStringAnsi(pointer);
            return json;
        }
    }

    #endregion

    #region CiphertextBallot

    public class CiphertextBallot : DisposableBase
    {
        internal unsafe NativeCiphertextBallot* Handle;

        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetObjectId(Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallot Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe string BallotStyle
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetBallotStyle(Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallot Error BallotStyle: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe ElementModQ DescriptionHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetDescriptionHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallot Error DescriptionHash: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        public unsafe ElementModQ PreviousTrackingHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetPreviousTrackingHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallot Error PreviousTrackingHash: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        public unsafe ulong ContestsSize
        {
            get
            {
                var size = NativeInterface.CiphertextBallot.GetContestsSize(Handle);
                return size;
            }
        }

        public unsafe ElementModQ TrackingHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetTrackingHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallot Error TrackingHash: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        public unsafe string TrackingCode
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetTrackingCode(Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallot Error TrackingCode: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        public unsafe ElementModQ Nonce
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetNonce(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CiphertextBallot Error Nonce: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        public unsafe CiphertextBallot(string json)
        {
            var status = NativeInterface.CiphertextBallot.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"CiphertextBallot Error Ctor: {status}");
            }
        }

        unsafe internal CiphertextBallot(NativeCiphertextBallot* handle)
        {
            Handle = handle;
        }

        public unsafe CiphertextBallotContest GetContestAt(ulong index)
        {
            var status = NativeInterface.CiphertextBallot.GetContestAtIndex(
                Handle, index, out NativeCiphertextBallotContest* value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"CiphertextBallot Error GetContestAt: {status}");
                return null;
            }
            return new CiphertextBallotContest(value);
        }

        public unsafe bool IsValidEncryption(
            ElementModQ seedHash, ElementModP elGamalPublicKey, ElementModQ cryptoExtendedBaseHash)
        {
            return NativeInterface.CiphertextBallot.IsValidEncryption(
                Handle, seedHash.Handle, elGamalPublicKey.Handle, cryptoExtendedBaseHash.Handle);
        }

        // TODO: ISSUE #129: To bson

        public unsafe string ToJson(bool withNonces = false)
        {
            var status = withNonces
                ? NativeInterface.CiphertextBallot.ToJsonWithNonces(
                Handle, out IntPtr pointer, out UIntPtr size)
                : NativeInterface.CiphertextBallot.ToJson(
                Handle, out pointer, out size);

            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"CiphertextBallot Error ToJson: {status}");
                return null;
            }
            var json = Marshal.PtrToStringAnsi(pointer, (int)size);
            Marshal.FreeHGlobal(pointer);
            return json;
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null) return;
            var status = NativeInterface.CiphertextBallot.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"CiphertextBallot Error DisposeUnmanaged: {status}");
            }
            Handle = null;
        }
    }

    #endregion
}
