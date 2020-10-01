using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NaiveElementModP = NativeInterface.ElementModP.ElementModPType;
    using NaiveElementModQ = NativeInterface.ElementModQ.ElementModQType;
    using NativePlaintextBallot = NativeInterface.PlaintextBallot.PlaintextBallotType;
    using NativeCiphertextBallot = NativeInterface.CiphertextBallot.CiphertextBallotType;

    public class PlaintextBallot
    {
        internal unsafe NativePlaintextBallot* Handle;

        public unsafe PlaintextBallot(string json)
        {
            Handle = NativeInterface.PlaintextBallot.FromJson(json);
        }

        public unsafe string ToJson()
        {
            _ = NativeInterface.PlaintextBallot.ToJson(Handle, out IntPtr pointer);
            var json = Marshal.PtrToStringAnsi(pointer);
            return json;
        }
    }

    public class CiphertextBallot
    {
        internal unsafe NativeCiphertextBallot* Handle;

        public string ObjectId { get; }
        public unsafe ElementModQ TrackingHash { get {
                var val = NativeInterface.CiphertextBallot.GetTrackingHash(Handle);
                return new ElementModQ(val);
            }
        }
        public string TrackingCode { get; }

        public unsafe ulong Timestamp { get {
                return NativeInterface.CiphertextBallot.GetTimestamp(Handle);
            }
        }

        public unsafe CiphertextBallot(string json)
        {
            Handle = NativeInterface.CiphertextBallot.FromJson(json);
        }

        unsafe internal CiphertextBallot(NativeCiphertextBallot* handle)
        {
            Handle = handle;
        }

        public unsafe bool IsValidEncryption(
            ElementModQ seedHash, ElementModP elGamalPublicKey, ElementModQ cryptoExtendedBaseHash)
        {
            return NativeInterface.CiphertextBallot.IsValidEncryption(
                Handle, seedHash.Handle, elGamalPublicKey.Handle, cryptoExtendedBaseHash.Handle);
        }

        public unsafe string ToJson()
        {
            _ = NativeInterface.CiphertextBallot.ToJson(Handle, out IntPtr pointer);
            var json = Marshal.PtrToStringAnsi(pointer);
            return json;
        }
    }
}
