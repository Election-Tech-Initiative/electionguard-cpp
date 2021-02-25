using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NativeElectionDescription = NativeInterface.ElectionDescription.ElectionDescriptionHandle;
    using NativeInternalElectionDescription = NativeInterface.InternalElectionDescription.InternalElectionDescriptionHandle;
    using NativeCiphertextElectionContext = NativeInterface.CiphertextElectionContext.CiphertextElectionContextHandle;
    using NativeElementModP = NativeInterface.ElementModP.ElementModPHandle;
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQHandle;

    public class ElectionDescription : DisposableBase
    {
        public unsafe string ElectionScopeId
        {
            get
            {
                var status = NativeInterface.ElectionDescription.GetElectionScopeId(
                    Handle, out IntPtr value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ElectionDescription Error ObjectId: {status}");
                    return null;
                }
                return Marshal.PtrToStringAnsi(value);
            }
        }

        internal unsafe NativeElectionDescription Handle;

        public unsafe ElectionDescription(string json)
        {
            var status = NativeInterface.ElectionDescription.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalElectionDescription Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        public unsafe ElementModQ CryptoHash()
        {
            var status = NativeInterface.ElectionDescription.CryptoHash(
                    Handle, out NativeElementModQ value);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DescriptionHash Error Status: {status}");
                return null;
            }
            return new ElementModQ(value);
        }

        public unsafe string ToJson()
        {
            var status = NativeInterface.ElectionDescription.ToJson(
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

    public class InternalElectionDescription: DisposableBase
    {
        public unsafe ElementModQ DescriptionHash
        {
            get
            {
                var status = NativeInterface.InternalElectionDescription.GetDescriptionHash(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"DescriptionHash Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeInternalElectionDescription Handle;

        public unsafe InternalElectionDescription(ElectionDescription election)
        {
            var status = NativeInterface.InternalElectionDescription.New(election.Handle, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalElectionDescription Error Status: {status}");
            }
        }

        public unsafe InternalElectionDescription(string json)
        {
            var status = NativeInterface.InternalElectionDescription.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"InternalElectionDescription Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
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
                    Handle, out NativeElementModP value);
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
                    Handle, out NativeElementModQ value);
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
                     Handle, out NativeElementModQ value);
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
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CryptoExtendedBaseHash Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeCiphertextElectionContext Handle;

        public unsafe CiphertextElectionContext(string json)
        {
            var status = NativeInterface.CiphertextElectionContext.FromJson(json, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"CiphertextElectionContext Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null) return;
            var status = NativeInterface.CiphertextElectionContext.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnmanaged Error Status: {status}");
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
