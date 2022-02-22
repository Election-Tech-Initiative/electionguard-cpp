using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NativeCiphertextElectionContext = NativeInterface.CiphertextElectionContext.CiphertextElectionContextHandle;
    using NativeElementModP = NativeInterface.ElementModP.ElementModPHandle;
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQHandle;
    using NativeLinkedList = NativeInterface.LinkedList.LinkedListHandle;


    /// <summary>
    /// `CiphertextElectionContext` is the ElectionGuard representation of a specific election
    /// Note: The ElectionGuard Data Spec deviates from the NIST model in that
    /// this object includes fields that are populated in the course of encrypting an election
    /// Specifically, `crypto_base_hash`, `crypto_extended_base_hash` and `elgamal_public_key`
    /// are populated with election-specific information necessary for encrypting the election.
    /// Refer to the [Electionguard Specification](https://github.com/microsoft/electionguard) for more information.
    ///
    /// To make an instance of this class, don't construct it directly. Use
    /// `make_ciphertext_election_context` instead.
    /// </summary>
    public class CiphertextElectionContext : DisposableBase
    {
        /// <summary>
        /// the `joint public key (K)` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        public unsafe ElementModP ElGamalPublicKey
        {
            get
            {
                var status = NativeInterface.CiphertextElectionContext.GetElGamalPublicKey(
                    Handle, out NativeElementModP value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException("ElGamalPublicKey Error Status", status);
                }
                return new ElementModP(value);
            }
        }

        /// <summary>
        /// the `commitment hash H(K 1,0 , K 2,0 ... , K n,0 )` of the public commitments
        /// guardians make to each other in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        public unsafe ElementModQ CommitmentHash
        {
            get
            {
                var status = NativeInterface.CiphertextElectionContext.GetCommitmentHash(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException("CommitmentHash Error Status", status);
                }
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// The hash of the election manifest
        /// </summary>
        public unsafe ElementModQ ManifestHash
        {
            get
            {
                var status = NativeInterface.CiphertextElectionContext.GetManifestHash(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException("ManifestHash Error Status", status);
                }
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// the `base hash code (𝑄)` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        public unsafe ElementModQ CryptoBaseHash
        {
            get
            {
                var status = NativeInterface.CiphertextElectionContext.GetCryptoBaseHash(
                     Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException("CryptoBaseHash Error Status", status);
                }
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// the `extended base hash code (𝑄')` in the [ElectionGuard Spec](https://github.com/microsoft/electionguard/wiki)
        /// </summary>
        public unsafe ElementModQ CryptoExtendedBaseHash
        {
            get
            {
                var status = NativeInterface.CiphertextElectionContext.GetCryptoExtendedBaseHash(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException("CryptoExtendedBaseHash Error Status", status);
                }
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// Get a linked list containing the extended data of the election.
        /// </summary>
        public unsafe LinkedList ExtendedData
        {
            get
            {
                var status = NativeInterface.CiphertextElectionContext.GetExtendedData(
                    Handle, out NativeLinkedList value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException("GetExtendedData Error Status", status);
                }
                return new LinkedList(value);
            }
        }

        internal unsafe NativeCiphertextElectionContext Handle;

        public unsafe CiphertextElectionContext(string json)
        {
            var status = NativeInterface.CiphertextElectionContext.FromJson(json, out Handle);
            status.ThrowIfError();
        }

        /// <summary>
        ///  Makes a CiphertextElectionContext object.
        ///
        /// <param name="number_of_guardians"> The number of guardians necessary to generate the public key </param>
        /// <param name="quorum"> The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians` </param>
        /// <param name="publicKey"> the public key of the election </param>
        /// <param name="commitmentHash"> the hash of the commitments the guardians make to each other </param>
        /// <param name="manifestHash"> the hash of the election metadata </param>
        /// </summary>
        public unsafe CiphertextElectionContext(ulong numberOfGuardians,
                ulong quorum,
                ElementModP publicKey,
                ElementModQ commitmentHash,
                ElementModQ manifestHash)
        {
            var status = NativeInterface.CiphertextElectionContext.Make(
                numberOfGuardians, quorum, publicKey.Handle,
                commitmentHash.Handle, manifestHash.Handle, out Handle);
            status.ThrowIfError();
        }

        /// <summary>
        ///  Makes a CiphertextElectionContext object.
        ///
        /// <param name="numberOfGuardians"> The number of guardians necessary to generate the public key </param>
        /// <param name="quorum"> The quorum of guardians necessary to decrypt an election.  Must be less than `number_of_guardians` </param>
        /// <param name="publicKey"> the public key of the election </param>
        /// <param name="commitmentHash"> the hash of the commitments the guardians make to each other </param>
        /// <param name="manifestHash"> the hash of the election metadata </param>
        /// <param name="extendedData"> an unordered map of key value strings revelant to the consuming application </param>
        /// </summary>
        public unsafe CiphertextElectionContext(ulong numberOfGuardians,
                ulong quorum,
                ElementModP publicKey,
                ElementModQ commitmentHash,
                ElementModQ manifestHash,
                LinkedList extendedData)
        {
            var status = NativeInterface.CiphertextElectionContext.Make(
                numberOfGuardians, quorum, publicKey.Handle,
                commitmentHash.Handle, manifestHash.Handle, extendedData.Handle, out Handle);
            status.ThrowIfError();
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }

        /// <Summary>
        /// Export the representation as JSON
        /// </Summary>
        public unsafe string ToJson()
        {
            var status = NativeInterface.CiphertextElectionContext.ToJson(
                Handle, out IntPtr pointer, out ulong size);
            status.ThrowIfError();
            var json = Marshal.PtrToStringAnsi(pointer);
            return json;
        }
    }
}
