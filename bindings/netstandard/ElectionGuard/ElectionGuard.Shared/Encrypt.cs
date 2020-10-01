using System;

namespace ElectionGuard
{
    using NativeEncryptionDevice = NativeInterface.EncryptionDevice.EncryptionDeviceType;
    using NativeEncryptionMediator = NativeInterface.EncryptionMediator.EncryptionMediatorType;

    public class EncryptionDevice
    {
        internal unsafe NativeEncryptionDevice* Handle;

        public unsafe EncryptionDevice(ulong uuid, string location)
        {
            Handle = NativeInterface.EncryptionDevice.Create(uuid, location);
        }
    }

    public class EncryptionMediator
    {
        internal unsafe NativeEncryptionMediator* Handle;

        public unsafe EncryptionMediator(
            InternalElectionDescription metadata, CiphertextElectionContext context, EncryptionDevice device)
        {
            Handle = NativeInterface.EncryptionMediator.Create(metadata.Handle, context.Handle, device.Handle);
        }

        public unsafe CiphertextBallot Encrypt(PlaintextBallot plaintext)
        {
            var ciphertext = NativeInterface.EncryptionMediator.Encrypt(Handle, plaintext.Handle);
            return new CiphertextBallot(ciphertext);
        }
    }

    public class Encrypt
    {
        public static unsafe CiphertextBallot Ballot(
            PlaintextBallot ballot,
            InternalElectionDescription metadata,
            CiphertextElectionContext context,
            ElementModQ seedHash, ElementModQ nonce = null)
        {
            if (nonce == null)
            {
                var ciphertext = NativeInterface.Encrypt.Ballot(
                    ballot.Handle, metadata.Handle, context.Handle, seedHash.Handle, true);

                return new CiphertextBallot(ciphertext);
            }
            else
            {
                var ciphertext = NativeInterface.Encrypt.Ballot(
                    ballot.Handle, metadata.Handle, context.Handle, seedHash.Handle, nonce.Handle, true);

                return new CiphertextBallot(ciphertext);
            }
        }
    }
}
