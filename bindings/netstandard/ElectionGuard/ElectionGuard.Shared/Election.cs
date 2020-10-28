using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NativeInternalElectionDescription = NativeInterface.InternalElectionDescription.InternalElectionDescriptionType;
    using NativeCiphertextElectionContext = NativeInterface.CiphertextElectionContext.CiphertextElectionType;
    using NativeElementModP = NativeInterface.ElementModP.ElementModPType;
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQType;

    public class InternalElectionDescription: DisposableBase
    {
        public unsafe ElementModQ DescriptionHash
        {
            get
            {
                var status = NativeInterface.InternalElectionDescription.GetDescriptionHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"DescriptionHash Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeInternalElectionDescription* Handle;

        public unsafe InternalElectionDescription(string json)
        {
            var status = NativeInterface.InternalElectionDescription.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalElectionDescription Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnamanged()
        {
            base.DisposeUnamanged();
            var status = NativeInterface.InternalElectionDescription.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnamanged Error Status: {status}");
            }
            Handle = null;
        }

        public unsafe string ToJson()
        {
            var status = NativeInterface.InternalElectionDescription.ToJson(
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

    public class CiphertextElectionContext: DisposableBase
    {
        public unsafe ElementModP ElGamalPublicKey
        {
            get
            {
                var status = NativeInterface.CiphertextElectionContext.GetElGamalPublicKey(
                    Handle, out NativeElementModP* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ElGamalPublicKey Error Status: {status}");
                    return null;
                }
                return new ElementModP(value);
            }
        }

        public unsafe ElementModQ DescriptionHash
        {
            get
            {
                var status = NativeInterface.CiphertextElectionContext.GetDescriptionHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"DescriptionHash Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        public unsafe ElementModQ CryptoBaseHash
        {
            get
            {
                var status = NativeInterface.CiphertextElectionContext.GetCryptoBaseHash(
                     Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CryptoBaseHash Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        public unsafe ElementModQ CryptoExtendedBaseHash
        {
            get
            {
                var status = NativeInterface.CiphertextElectionContext.GetCryptoExtendedBaseHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CryptoExtendedBaseHash Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeCiphertextElectionContext* Handle;

        public unsafe CiphertextElectionContext(string json)
        {
            var status = NativeInterface.CiphertextElectionContext.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"CiphertextElectionContext Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnamanged()
        {
            base.DisposeUnamanged();
            var status = NativeInterface.CiphertextElectionContext.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnamanged Error Status: {status}");
            }
            Handle = null;
        }

        public unsafe string ToJson()
        {
            var status = NativeInterface.CiphertextElectionContext.ToJson(
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
