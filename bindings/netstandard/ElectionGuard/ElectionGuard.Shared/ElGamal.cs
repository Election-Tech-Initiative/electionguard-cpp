using System;
namespace ElectionGuard
{
    // Declare native types for convenience
    using NativeElementModP = NativeInterface.ElementModP.ElementModPHandle;
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQHandle;
    using NativeElGamalKeyPair = NativeInterface.ElGamalKeyPair.ElGamalKeyPairHandle;
    using NativeElGamalCiphertext = NativeInterface.ElGamalCiphertext.ElGamalCiphertextHandle;

    public class ElGamalKeyPair : DisposableBase
    {
        public unsafe ElementModP PublicKey
        {
            get
            {
                var status = NativeInterface.ElGamalKeyPair.GetPublicKey(
                    Handle, out NativeElementModP value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"PublicKey Error Status: {status}");
                    return null;
                }
                return new ElementModP(value);
            }
        }

        public unsafe ElementModQ SecretKey
        {
            get
            {
                var status = NativeInterface.ElGamalKeyPair.GetSecretKey(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"SecretKey Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeElGamalKeyPair Handle;

        unsafe internal ElGamalKeyPair(NativeElGamalKeyPair handle)
        {
            Handle = handle;
        }

        unsafe internal ElGamalKeyPair(ElementModQ secretKey)
        {
            var status = NativeInterface.ElGamalKeyPair.New(
                secretKey.Handle, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"ElGamalKeyPair Error Status: {status}");
            }
        }

        unsafe static public ElGamalKeyPair FromSecret(ElementModQ secretKey)
        {
            return new ElGamalKeyPair(secretKey);
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    public class ElGamalCiphertext : DisposableBase
    {
        public unsafe ElementModP Pad
        {
            get
            {
                var status = NativeInterface.ElGamalCiphertext.GetPad(
                    Handle, out NativeElementModP value);
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
                    Handle, out NativeElementModP value);
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
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"CryptoHash Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeElGamalCiphertext Handle;

        unsafe internal ElGamalCiphertext(NativeElGamalCiphertext handle)
        {
            Handle = handle;
        }

        unsafe public ulong? Decrypt(ElementModQ secretKey)
        {
            ulong plaintext = 0;
            var status = NativeInterface.ElGamalCiphertext.DecryptWithSecret(
                    Handle, secretKey.Handle, ref plaintext);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Decrypt Error Status: {status}");
                return null;
            }
            return plaintext;
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }
    }
    public static unsafe class Elgamal
    {
        unsafe public static ElGamalCiphertext Encrypt(
            ulong plaintext, ElementModQ nonce, ElementModP publicKey)
        {
            var status = NativeInterface.ElGamal.Encrypt(
                    plaintext, nonce.Handle, publicKey.Handle,
                    out NativeElGamalCiphertext ciphertext);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Encrypt Error Status: {status}");
                return null;
            }
            return new ElGamalCiphertext(ciphertext);
        }
    }
}
