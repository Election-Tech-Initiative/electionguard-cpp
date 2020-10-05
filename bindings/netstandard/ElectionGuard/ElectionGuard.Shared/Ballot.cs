using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NativeElementModP = NativeInterface.ElementModP.ElementModPType;
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQType;
    using NativePlaintextBallot = NativeInterface.PlaintextBallot.PlaintextBallotType;
    using NativeCiphertextBallot = NativeInterface.CiphertextBallot.CiphertextBallotType;

    public class PlaintextBallot: DisposableBase
    {
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.PlaintextBallot.GetObjectId(Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Debug.WriteLine($"PlaintextBallot Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        internal unsafe NativePlaintextBallot* Handle;

        public unsafe PlaintextBallot(string json)
        {
            var status = NativeInterface.PlaintextBallot.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Debug.WriteLine($"PlaintextBallot Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnamanged()
        {
            base.DisposeUnamanged();
            var status = NativeInterface.PlaintextBallot.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Debug.WriteLine($"DisposeUnamanged Error Status: {status}");
            }
            Handle = null;
        }

        public unsafe string ToJson()
        {
            var status = NativeInterface.PlaintextBallot.ToJson(
                Handle, out IntPtr pointer, out UIntPtr size);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Debug.WriteLine($"ToJson Error Status: {status}");
                return null;
            }
            var json = Marshal.PtrToStringAnsi(pointer);
            return json;
        }
    }

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
                    Debug.WriteLine($"CiphertextBallot Error ObjectId: {status}");
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
                    Debug.WriteLine($"TrackingHash Error Status: {status}");
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
                    Debug.WriteLine($"CiphertextBallot Error TrackingCode: {status}");
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
                Debug.WriteLine($"DisposeUnamanged Error Status: {status}");
            }
            Handle = null;
        }

        public unsafe bool IsValidEncryption(
            ElementModQ seedHash, ElementModP elGamalPublicKey, ElementModQ cryptoExtendedBaseHash)
        {
            return NativeInterface.CiphertextBallot.IsValidEncryption(
                Handle, seedHash.Handle, elGamalPublicKey.Handle, cryptoExtendedBaseHash.Handle);
        }

        public unsafe string ToJson()
        {
            var status = NativeInterface.CiphertextBallot.ToJson(
                Handle, out IntPtr pointer, out UIntPtr size);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Debug.WriteLine($"ToJson Error Status: {status}");
                return null;
            }
            var json = Marshal.PtrToStringAnsi(pointer);
            return json;
        }
    }
}
