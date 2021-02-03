using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    // Declare native types for convenience
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQType;
    using NativeElGamalCiphertext = NativeInterface.ElGamalCiphertext.ElGamalCiphertextType;
    using NativePlaintextBallotSelection = NativeInterface.PlaintextBallotSelection.PlaintextBallotSelectionType;
    using NativeCiphertextBallotSelection = NativeInterface.CiphertextBallotSelection.CiphertextBallotSelectionType;
    using NativePlaintextBallotContest = NativeInterface.PlaintextBallotContest.PlaintextBallotContestType;
    using NativeCiphertextBallotContest = NativeInterface.CiphertextBallotContest.CiphertextBallotContestType;
    using NativePlaintextBallot = NativeInterface.PlaintextBallot.PlaintextBallotType;
    using NativeCiphertextBallot = NativeInterface.CiphertextBallot.CiphertextBallotType;

    #region PlaintextBallotSelction

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

        internal unsafe NativePlaintextBallotSelection* Handle;

        unsafe internal PlaintextBallotSelection(NativePlaintextBallotSelection* handle)
        {
            Handle = handle;
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();
            var status = NativeInterface.PlaintextBallotSelection.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"PlaintextBallotSelection Error DisposeUnmanaged: {status}");
            }
            Handle = null;
        }
    }

    #endregion

    #region CiphertextBallotSelection

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

        public unsafe bool IsValidEncryption(
            ElementModQ seedHash, ElementModP elGamalPublicKey, ElementModQ cryptoExtendedBaseHash)
        {
            return NativeInterface.CiphertextBallotSelection.IsValidEncryption(
                Handle, seedHash.Handle, elGamalPublicKey.Handle, cryptoExtendedBaseHash.Handle);
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();
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

    public class PlaintextBallotContest: DisposableBase
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

        internal unsafe NativePlaintextBallotContest* Handle;

        unsafe internal PlaintextBallotContest(NativePlaintextBallotContest* handle)
        {
            Handle = handle;
        }

        public unsafe PlaintextBallotSelection GetSelectionAt(ulong index)
        {
            var status = NativeInterface.PlaintextBallotContest.GetSelectionAtIndex(
                Handle, index, out NativePlaintextBallotSelection* value);
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
            var status = NativeInterface.PlaintextBallotContest.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"PlaintextBallotContest Error DisposeUnmanaged: {status}");
            }
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

    public class PlaintextBallot: DisposableBase
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

        internal unsafe NativePlaintextBallot* Handle;

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
                Handle, index, out NativePlaintextBallotContest* value);
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
            var status = NativeInterface.PlaintextBallot.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"PlaintextBallot Error DisposeUnmanaged: {status}");
            }
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

    public class CiphertextBallot: DisposableBase
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
