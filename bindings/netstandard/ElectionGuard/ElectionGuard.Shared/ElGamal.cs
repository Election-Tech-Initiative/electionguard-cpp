using System;
namespace ElectionGuard
{
    // Declare native types for convenience
    using NativeElementModP = NativeInterface.ElementModP.ElementModPType;
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQType;
    using NativeElGamalCiphertext = NativeInterface.ElGamalCiphertext.ElGamalCiphertextType;

    public class ElGamalCiphertext : DisposableBase
    {
        public unsafe ElementModP Pad
        {
            get
            {
                var status = NativeInterface.ElGamalCiphertext.GetPad(
                    Handle, out NativeElementModP* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Pad Error Status: {status}");
                    return null;
                }
                return new ElementModP(value);
            }
        }

        public unsafe ElementModP Data
        {
            get
            {
                var status = NativeInterface.ElGamalCiphertext.GetData(
                    Handle, out NativeElementModP* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Data Error Status: {status}");
                    return null;
                }
                return new ElementModP(value);
            }
        }

        public unsafe ElementModQ CryptoHash
        {
            get
            {
                var status = NativeInterface.ElGamalCiphertext.GetCryptoHash(
                    Handle, out NativeElementModQ* value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CryptoHash Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeElGamalCiphertext* Handle;

        unsafe internal ElGamalCiphertext(NativeElGamalCiphertext* handle)
        {
            Handle = handle;
        }

        // TODO: ISSUE #129: decrypt with secret

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();
            var status = NativeInterface.ElGamalCiphertext.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnamanged Error Status: {status}");
            }
            Handle = null;
        }
    }
}
