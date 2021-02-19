using System;

namespace ElectionGuard
{
    using NativeEncryptionDevice = NativeInterface.EncryptionDevice.EncryptionDeviceHandle;
    using NativeEncryptionMediator = NativeInterface.EncryptionMediator.EncryptionMediatorHandle;
    using NativeCiphertextBallot = NativeInterface.CiphertextBallot.CiphertextBallotHandle;

    public class EncryptionDevice: DisposableBase
    {
        internal unsafe NativeEncryptionDevice Handle;

        public unsafe EncryptionDevice(ulong uuid, string location)
        {
            var status = NativeInterface.EncryptionDevice.New(uuid, location, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"EncryptionDevice New Error Status: {status}");
            }
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    public class EncryptionMediator: DisposableBase
    {
        internal unsafe NativeEncryptionMediator Handle;

        public unsafe EncryptionMediator(
            InternalElectionDescription metadata,
            CiphertextElectionContext context,
            EncryptionDevice device)
        {
            var status = NativeInterface.EncryptionMediator.New(
                metadata.Handle, context.Handle, device.Handle, out Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"EncryptionMediator New Error Status: {status}");
            }
        }

        public unsafe CiphertextBallot Encrypt(
            PlaintextBallot plaintext, bool verifyProofs = false)
        {
            if (verifyProofs)
            {
                var status = NativeInterface.EncryptionMediator.EncryptAndVerify(
                Handle, plaintext.Handle, out NativeCiphertextBallot ciphertext);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"InternalElectionDescription Error Status: {status}");
                }
                return new CiphertextBallot(ciphertext);
            }
            else
            {
                var status = NativeInterface.EncryptionMediator.Encrypt(
                Handle, plaintext.Handle, out NativeCiphertextBallot ciphertext);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"InternalElectionDescription Error Status: {status}");
                }
                return new CiphertextBallot(ciphertext);
            }
            
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    public class Encrypt
    {
        public static unsafe CiphertextBallot Ballot(
            PlaintextBallot ballot,
            InternalElectionDescription metadata,
            CiphertextElectionContext context,
            ElementModQ seedHash,
            ElementModQ nonce = null,
            bool verifyProofs = true)
        {
            if (nonce == null)
            {
                var status = NativeInterface.Encrypt.Ballot(
                    ballot.Handle, metadata.Handle, context.Handle,
                    seedHash.Handle, verifyProofs,
                    out NativeCiphertextBallot ciphertext);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Encrypt Error Status: {status}");
                }
                return new CiphertextBallot(ciphertext);
            }
            else
            {
                var status = NativeInterface.Encrypt.Ballot(
                    ballot.Handle, metadata.Handle, context.Handle,
                    seedHash.Handle, nonce.Handle, verifyProofs,
                    out NativeCiphertextBallot ciphertext);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"EncryptWithNonce Error Status: {status}");
                }
                return new CiphertextBallot(ciphertext);
            }
        }
    }
}
