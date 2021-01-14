using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    // Declare native types for convenience
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQType;
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

        protected override unsafe void DisposeUnamanged()
        {
            base.DisposeUnamanged();
            var status = NativeInterface.PlaintextBallotSelection.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnamanged Error Status: {status}");
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
                    Console.WriteLine($"PlaintextBallotContest Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        internal unsafe NativeCiphertextBallotSelection* Handle;

        unsafe internal CiphertextBallotSelection(NativeCiphertextBallotSelection* handle)
        {
            Handle = handle;
        }

        protected override unsafe void DisposeUnamanged()
        {
            base.DisposeUnamanged();
            var status = NativeInterface.CiphertextBallotSelection.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnamanged Error Status: {status}");
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

        internal unsafe NativePlaintextBallotContest* Handle;

        unsafe internal PlaintextBallotContest(NativePlaintextBallotContest* handle)
        {
            Handle = handle;
        }

        protected override unsafe void DisposeUnamanged()
        {
            base.DisposeUnamanged();
            var status = NativeInterface.PlaintextBallotContest.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnamanged Error Status: {status}");
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
                    Console.WriteLine($"PlaintextBallotContest Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        internal unsafe NativeCiphertextBallotContest* Handle;

        unsafe internal CiphertextBallotContest(NativeCiphertextBallotContest* handle)
        {
            Handle = handle;
        }

        protected override unsafe void DisposeUnamanged()
        {
            base.DisposeUnamanged();
            var status = NativeInterface.CiphertextBallotContest.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnamanged Error Status: {status}");
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

        protected override unsafe void DisposeUnamanged()
        {
            base.DisposeUnamanged();
            var status = NativeInterface.PlaintextBallot.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnamanged Error Status: {status}");
            }
            Handle = null;
        }

        public unsafe string ToJson()
        {
            var status = NativeInterface.PlaintextBallot.ToJson(
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

        public unsafe ElementModQ TrackingHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetTrackingHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"TrackingHash Error Status: {status}");
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

        public unsafe CiphertextBallot(string json)
        {
            var status = NativeInterface.CiphertextBallot.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"CiphertextBallot Error Status: {status}");
            }
        }

        unsafe internal CiphertextBallot(NativeCiphertextBallot* handle)
        {
            Handle = handle;
        }

        protected override unsafe void DisposeUnamanged()
        {
            base.DisposeUnamanged();
            var status = NativeInterface.CiphertextBallot.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnamanged Error Status: {status}");
            }
            Handle = null;
        }

        public unsafe bool IsValidEncryption(
            ElementModQ seedHash, ElementModP elGamalPublicKey, ElementModQ cryptoExtendedBaseHash)
        {
            return NativeInterface.CiphertextBallot.IsValidEncryption(
                Handle, seedHash.Handle, elGamalPublicKey.Handle, cryptoExtendedBaseHash.Handle);
        }

        public unsafe string ToJson(bool withNonces = false)
        {
            var status = withNonces
                ? NativeInterface.CiphertextBallot.ToJsonWithNonces(
                Handle, out IntPtr pointer, out UIntPtr size)
                : NativeInterface.CiphertextBallot.ToJson(
                Handle, out pointer, out size);

            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ToJson Error Status: {status}");
                return null;
            }
            var json = Marshal.PtrToStringAnsi(pointer, (int)size);
            Marshal.FreeHGlobal(pointer);
            return json;
        }
    }

    #endregion
}
