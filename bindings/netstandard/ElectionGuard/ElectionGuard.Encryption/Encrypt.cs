using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQHandle;
    using NativeEncryptionDevice = NativeInterface.EncryptionDevice.EncryptionDeviceHandle;
    using NativeEncryptionMediator = NativeInterface.EncryptionMediator.EncryptionMediatorHandle;
    using NativeCiphertextBallot = NativeInterface.CiphertextBallot.CiphertextBallotHandle;
    using NativeCiphertextBallotContest = NativeInterface.CiphertextBallotContest.CiphertextBallotContestHandle;
    using NativeCiphertextBallotSelection = NativeInterface.CiphertextBallotSelection.CiphertextBallotSelectionHandle;
    using NativeCompactCiphertextBallot = NativeInterface.CompactCiphertextBallot.CompactCiphertextBallotHandle;

    /// <summary>
    /// Metadata for encryption device
    ///
    /// The encryption device is a stateful container that represents abstract hardware
    /// authorized to participate in a specific election.
    ///
    /// </summary>
    public class EncryptionDevice : DisposableBase
    {
        internal unsafe NativeEncryptionDevice Handle;

        /// <summary>
        /// Create a new EncryptionDevice
        /// </summary>
        /// <param name="deviceUuid">a unique identifier tied to the device hardware</param>
        /// <param name="sessionUuid">a unique identifier tied to the runtime session</param>
        /// <param name="launchCode">a unique identifer tied to the election</param>
        /// <param name="location">an arbitrary string meaningful to the external system
        ///                        such as a friendly name, description, or some other value</param>
        public unsafe EncryptionDevice(
            ulong deviceUuid,
            ulong sessionUuid,
            ulong launchCode,
            string location)
        {
            var status = NativeInterface.EncryptionDevice.New(
                deviceUuid, sessionUuid, launchCode, location, out Handle);
            status.ThrowIfError();
        }

        /// <summary>
        /// Get a new hash value
        ///
        /// <return>An `ElementModQ`</return>
        /// </summary>
        public unsafe ElementModQ GetHash()
        {
            var status = NativeInterface.EncryptionDevice.GetHash(Handle, out NativeElementModQ value);
            return new ElementModQ(value);
        }

        /// <summary>
        /// produces encription device when given json
        /// </summary>
        /// <param name="json"></param>
        public unsafe EncryptionDevice(string json)
        {
            var status = NativeInterface.EncryptionDevice.FromJson(json, out Handle);
            status.ThrowIfError();
        }
        /// <Summary>
        /// Export the encryptiondevice representation as JSON
        /// </Summary>
        public unsafe string ToJson()
        {
            var status = NativeInterface.EncryptionDevice.ToJson(Handle, out IntPtr pointer, out ulong size);
            status.ThrowIfError();
            var json = Marshal.PtrToStringAnsi(pointer);
            return json;
        }



#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
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

        /// <summary>
        /// Create an `EncryptionMediator` object
        /// </summary>
        /// <param name="manifest"></param>
        /// <param name="context"></param>
        /// <param name="device"></param>
        public unsafe EncryptionMediator(
            InternalManifest manifest,
            CiphertextElectionContext context,
            EncryptionDevice device)
        {
            var status = NativeInterface.EncryptionMediator.New(
                manifest.Handle, context.Handle, device.Handle, out Handle);
            status.ThrowIfError();
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
                status.ThrowIfError();
                return new CiphertextBallot(ciphertext);
            }
            else
            {
                var status = NativeInterface.EncryptionMediator.Encrypt(
                Handle, plaintext.Handle, out NativeCiphertextBallot ciphertext);
                status.ThrowIfError();
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
                status.ThrowIfError();
                return new CompactCiphertextBallot(ciphertext);
            }
            else
            {
                var status = NativeInterface.EncryptionMediator.CompactEncrypt(
                Handle, plaintext.Handle, out NativeCompactCiphertextBallot ciphertext);
                status.ThrowIfError();
                return new CompactCiphertextBallot(ciphertext);
            }

        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    /// <summary>
    /// Metadata for encryption
    ///
    /// The encrypt object is used for encrypting ballots.
    ///
    /// </summary>
    public class Encrypt
    {
        /// <summary>
        /// Encrypt a specific `BallotSelection` in the context of a specific `BallotContest`
        /// </summary>
        /// <param name="plaintext">the selection in the valid input form</param>
        /// <param name="description">the `SelectionDescription` from the `ContestDescription`
        ///                           which defines this selection's structure</param>
        /// <param name="elgamalPublicKey">the public key (K) used to encrypt the ballot</param>
        /// <param name="cryptoExtendedBaseHash">the extended base hash of the election</param>
        /// <param name="nonceSeed">an `ElementModQ` used as a header to seed the `Nonce` generated
        ///                          for this selection. this value can be (or derived from) the
        ///                          Contest nonce, but no relationship is required</param>
        /// <param name="shouldVerifyProofs">specify if the proofs should be verified prior to returning (default True)</param>
        /// <returns>A `CiphertextBallotSelection`</returns>
        public static unsafe CiphertextBallotSelection Selection(
            PlaintextBallotSelection plaintext,
            SelectionDescription description,
            ElementModP elgamalPublicKey,
            ElementModQ cryptoExtendedBaseHash,
            ElementModQ nonceSeed,
            bool shouldVerifyProofs = true
        )
        {
            var status = NativeInterface.Encrypt.Selection(
                    plaintext.Handle, description.Handle, elgamalPublicKey.Handle,
                    cryptoExtendedBaseHash.Handle, nonceSeed.Handle, shouldVerifyProofs,
                    out NativeCiphertextBallotSelection ciphertext);
            status.ThrowIfError();
            return new CiphertextBallotSelection(ciphertext);
        }

        /// <summary>
        /// Encrypt a specific `BallotContest` in the context of a specific `Ballot`
        ///
        /// This method accepts a contest representation that only includes `True` selections.
        /// It will fill missing selections for a contest with `False` values, and generate `placeholder`
        /// selections to represent the number of seats available for a given contest.  By adding `placeholder`
        /// votes
        /// </summary>
        /// <param name="plaintext">the selection in the valid input form</param>
        /// <param name="description">the `ContestDescriptionWithPlaceholders` from the `ContestDescription`
        ///                           which defines this contest's structure</param>
        /// <param name="elgamalPublicKey">the public key (K) used to encrypt the ballot</param>
        /// <param name="cryptoExtendedBaseHash">the extended base hash of the election</param>
        /// <param name="nonceSeed">an `ElementModQ` used as a header to seed the `Nonce` generated
        ///                          for this contest. this value can be (or derived from) the
        ///                          Ballot nonce, but no relationship is required</param>
        /// <param name="shouldVerifyProofs">specify if the proofs should be verified prior to returning (default True)</param>
        /// <returns>A `CiphertextBallotContest`</returns>
        public static unsafe CiphertextBallotContest Contest(
            PlaintextBallotContest plaintext,
            ContestDescription description,
            ElementModP elgamalPublicKey,
            ElementModQ cryptoExtendedBaseHash,
            ElementModQ nonceSeed,
            bool shouldVerifyProofs = true
        )
        {
            var status = NativeInterface.Encrypt.Contest(
                    plaintext.Handle, description.Handle, elgamalPublicKey.Handle,
                    cryptoExtendedBaseHash.Handle, nonceSeed.Handle, shouldVerifyProofs,
                    out NativeCiphertextBallotContest ciphertext);
            status.ThrowIfError();
            return new CiphertextBallotContest(ciphertext);
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
        /// </summary>
        /// <param name="ballot">the selection in the valid input form</param>
        /// <param name="internalManifest">the `InternalManifest` which defines this ballot's structure</param>
        /// <param name="context">all the cryptographic context for the election</param>
        /// <param name="ballotCodeSeed">Hash from previous ballot or hash from device</param>
        /// <param name="nonce">an optional value used to seed the `Nonce` generated for this ballot
        ///                     if this value is not provided, the secret generating mechanism of the OS provides its own</param>
        /// <param name="shouldVerifyProofs">specify if the proofs should be verified prior to returning (default True)</param>
        /// <returns>A `CiphertextBallot`</returns>
        public static unsafe CiphertextBallot Ballot(
            PlaintextBallot ballot,
            InternalManifest internalManifest,
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
                status.ThrowIfError();
                return new CiphertextBallot(ciphertext);
            }
            else
            {
                var status = NativeInterface.Encrypt.Ballot(
                    ballot.Handle, internalManifest.Handle, context.Handle,
                    ballotCodeSeed.Handle, nonce.Handle, shouldVerifyProofs,
                    out NativeCiphertextBallot ciphertext);
                status.ThrowIfError();
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
        /// </summary>
        /// <param name="ballot">the selection in the valid input form</param>
        /// <param name="internalManifest">the `InternalManifest` which defines this ballot's structure</param>
        /// <param name="context">all the cryptographic context for the election</param>
        /// <param name="ballotCodeSeed">Hash from previous ballot or hash from device</param>
        /// <param name="nonce">an optional value used to seed the `Nonce` generated for this ballot
        ///                     if this value is not provided, the secret generating mechanism of the OS provides its own</param>
        /// <param name="shouldVerifyProofs">specify if the proofs should be verified prior to returning (default True)</param>
        /// <returns>A `CiphertextBallot`</returns>
        public static unsafe CompactCiphertextBallot CompactBallot(
            PlaintextBallot ballot,
            InternalManifest internalManifest,
            CiphertextElectionContext context,
            ElementModQ ballotCodeSeed,
            ElementModQ nonce = null,
            bool shouldVerifyProofs = true)
        {
            if (nonce == null)
            {
                var status = NativeInterface.Encrypt.CompactBallot(
                    ballot.Handle, internalManifest.Handle, context.Handle,
                    ballotCodeSeed.Handle, shouldVerifyProofs,
                    out NativeCompactCiphertextBallot ciphertext);
                status.ThrowIfError();
                return new CompactCiphertextBallot(ciphertext);
            }
            else
            {
                var status = NativeInterface.Encrypt.CompactBallot(
                    ballot.Handle, internalManifest.Handle, context.Handle,
                    ballotCodeSeed.Handle, nonce.Handle, shouldVerifyProofs,
                    out NativeCompactCiphertextBallot ciphertext);
                status.ThrowIfError();
                return new CompactCiphertextBallot(ciphertext);
            }

        }
    }
}
