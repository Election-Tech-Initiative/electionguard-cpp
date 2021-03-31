using System;

namespace ElectionGuard
{
    using NativeEncryptionDevice = NativeInterface.EncryptionDevice.EncryptionDeviceHandle;
    using NativeEncryptionMediator = NativeInterface.EncryptionMediator.EncryptionMediatorHandle;
    using NativeCiphertextBallot = NativeInterface.CiphertextBallot.CiphertextBallotHandle;
    using NativeCompactCiphertextBallot = NativeInterface.CompactCiphertextBallot.CompactCiphertextBallotHandle;

    /// <summary>
    /// Metadata for encryption device
    ///
    /// The encryption device is a stateful container that represents abstract hardware
    /// authorized to participate in a specific election.
    ///
    /// <param name="deviceUuid">a unique identifier tied to the device hardware</param>
    /// <param name="sessionUuid">a unique identifier tied to the runtime session</param>
    /// <param name="launchCode">a unique identifer tied to the election</param>
    /// <param name="location">an arbitrary string meaningful to the external system
    ///                        such as a friendly name, description, or some other value</param>
    /// </summary>
    public class EncryptionDevice : DisposableBase
    {
        internal unsafe NativeEncryptionDevice Handle;

        public unsafe EncryptionDevice(
            ulong deviceUuid,
            ulong sessionUuid,
            ulong launchCode,
            string location)
        {
            var status = NativeInterface.EncryptionDevice.New(
                deviceUuid, sessionUuid, launchCode, location, out Handle);
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

    /// <summary>
    /// An object for caching election and encryption state.
    ///
    /// the encryption mediator composes ballots by querying the encryption device
    /// for a hash of its metadata and incremental timestamps/
    ///
    /// this is a convenience wrapper around the encrypt methods
    /// and may not be suitable for all use cases.
    /// </summary>
    public class EncryptionMediator : DisposableBase
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

        /// <summary>
        /// Encrypt the specified ballot using the cached election context.
        /// </summary>
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

        /// <summary>
        /// Encrypt the specified ballot into its compact form using the cached election context.
        /// </summary>
        public unsafe CompactCiphertextBallot CompactEncrypt(
            PlaintextBallot plaintext, bool verifyProofs = false)
        {
            if (verifyProofs)
            {
                var status = NativeInterface.EncryptionMediator.CompactEncryptAndVerify(
                Handle, plaintext.Handle, out NativeCompactCiphertextBallot ciphertext);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"InternalElectionDescription Error Status: {status}");
                }
                return new CompactCiphertextBallot(ciphertext);
            }
            else
            {
                var status = NativeInterface.EncryptionMediator.CompactEncrypt(
                Handle, plaintext.Handle, out NativeCompactCiphertextBallot ciphertext);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"InternalElectionDescription Error Status: {status}");
                }
                return new CompactCiphertextBallot(ciphertext);
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
        /// <summary>
        /// Encrypt a specific `Ballot` in the context of a specific `CiphertextElectionContext`
        ///
        /// This method accepts a ballot representation that only includes `True` selections.
        /// It will fill missing selections for a contest with `False` values, and generate `placeholder`
        /// selections to represent the number of seats available for a given contest.  By adding `placeholder`
        /// votes
        ///
        /// This method also allows for ballots to exclude passing contests for which the voter made no selections.
        /// It will fill missing contests with `False` selections and generate `placeholder` selections that are marked `True`.
        ///
        /// <param name="ballot">the selection in the valid input form</param>
        /// <param name="internalManifest">the `InternalElectionDescription` which defines this ballot's structure</param>
        /// <param name="context">all the cryptographic context for the election</param>
        /// <param name="ballotCodeSeed">Hash from previous ballot or hash from device</param>
        /// <param name="nonce">an optional value used to seed the `Nonce` generated for this ballot
        ///                     if this value is not provided, the secret generating mechanism of the OS provides its own</param>
        /// <param name="shouldVerifyProofs">specify if the proofs should be verified prior to returning (default True)</param>
        /// <returns>A `CiphertextBallot`</returns>
        /// </summary>
        public static unsafe CiphertextBallot Ballot(
            PlaintextBallot ballot,
            InternalElectionDescription internalManifest,
            CiphertextElectionContext context,
            ElementModQ ballotCodeSeed,
            ElementModQ nonce = null,
            bool shouldVerifyProofs = true)
        {
            if (nonce == null)
            {
                var status = NativeInterface.Encrypt.Ballot(
                    ballot.Handle, internalManifest.Handle, context.Handle,
                    ballotCodeSeed.Handle, shouldVerifyProofs,
                    out NativeCiphertextBallot ciphertext);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Encrypt Ballot Error Status: {status}");
                }
                return new CiphertextBallot(ciphertext);
            }
            else
            {
                var status = NativeInterface.Encrypt.Ballot(
                    ballot.Handle, internalManifest.Handle, context.Handle,
                    ballotCodeSeed.Handle, nonce.Handle, shouldVerifyProofs,
                    out NativeCiphertextBallot ciphertext);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"EncryptWithNonce Ballot Error Status: {status}");
                }
                return new CiphertextBallot(ciphertext);
            }
        }

        /// <summary>
        /// Encrypt a specific `Ballot` in the context of a specific `CiphertextElectionContext`
        ///
        /// This method accepts a ballot representation that only includes `True` selections.
        /// It will fill missing selections for a contest with `False` values, and generate `placeholder`
        /// selections to represent the number of seats available for a given contest.  By adding `placeholder`
        /// votes
        ///
        /// This method also allows for ballots to exclude passing contests for which the voter made no selections.
        /// It will fill missing contests with `False` selections and generate `placeholder` selections that are marked `True`.
        ///
        /// This version of the encrypt method returns a `compact` version of the ballot that includes a minimal representation
        /// of the plaintext ballot along with the crypto parameters that are required to expand the ballot
        ///
        /// <param name="ballot">the selection in the valid input form</param>
        /// <param name="internalManifest">the `InternalElectionDescription` which defines this ballot's structure</param>
        /// <param name="context">all the cryptographic context for the election</param>
        /// <param name="ballotCodeSeed">Hash from previous ballot or hash from device</param>
        /// <param name="nonceSeed">an optional value used to seed the `Nonce` generated for this ballot
        ///                     if this value is not provided, the secret generating mechanism of the OS provides its own</param>
        /// <param name="shouldVerifyProofs">specify if the proofs should be verified prior to returning (default True)</param>
        /// <returns>A `CiphertextBallot`</returns>
        /// </summary>
        public static unsafe CompactCiphertextBallot CompactBallot(
            PlaintextBallot ballot,
            InternalElectionDescription internalManifest,
            CiphertextElectionContext context,
            ElementModQ ballotCodeSeed,
            bool verifyProofs = true)
        {

            var status = NativeInterface.Encrypt.CompactBallot(
                ballot.Handle, internalManifest.Handle, context.Handle,
                ballotCodeSeed.Handle, verifyProofs,
                out NativeCompactCiphertextBallot ciphertext);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"Encrypt CompactBallot Error Status: {status}");
            }
            return new CompactCiphertextBallot(ciphertext);
        }
    }
}
