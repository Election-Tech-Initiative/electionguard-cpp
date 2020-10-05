using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    internal enum Status
    {
        ELECTIONGUARD_STATUS_SUCCESS = 0,
        ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT,
        ELECTIONGUARD_STATUS_ERROR_OUT_OF_RANGE,
        ELECTIONGUARD_STATUS_ERROR_IO_ERROR,
        ELECTIONGUARD_STATUS_ERROR_BAD_ACCESS,
        ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC,
        ELECTIONGUARD_STATUS_ERROR_ALREADY_EXISTS,
        ELECTIONGUARD_STATUS_ERROR_RUNTIME_ERROR,

        /// This code should always be the last code in the collection
        // so that the status codes string can be correctly derived
        ELECTIONGUARD_STATUS_UNKNOWN
    }

    internal static unsafe class NativeInterface
    {
#if Android
        const string DllName = "libelectionguard.so";
#else
    const string DllName = "__Internal";
#endif

        internal unsafe struct CharPtr { };

        internal static unsafe class ElementModP
        {
            internal unsafe struct ElementModPType { };

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_new")]
            internal static extern Status New(ulong* in_data, out ElementModPType* handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_new_unchecked")]
            internal static extern Status NewUnchecked(ulong* in_data, out ElementModPType* handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_free")]
            internal static extern Status Free(ElementModPType* handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_get_data")]
            internal static extern Status GetData(ElementModPType* handle, ulong** out_data, out UIntPtr out_size);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_to_hex")]
            internal static extern Status ToHex(ElementModPType* handle, out IntPtr data);
        }

        internal static unsafe class ElementModQ
        {
            internal unsafe struct ElementModQType { };

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_new")]
            internal static extern Status New(ulong* in_data, out ElementModQType* handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_new_unchecked")]
            internal static extern Status NewUnchecked(ulong* in_data, out ElementModQType* handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_free")]
            internal static extern Status Free(ElementModQType* handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_get_data")]
            internal static extern Status GetData(ElementModQType* handle, ulong** out_data, out UIntPtr out_size);

           [DllImport(DllName, EntryPoint = "eg_element_mod_q_to_hex")]
           internal static extern Status ToHex(ElementModQType* handle, out IntPtr out_hex);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_rand_q_new")]
            internal static extern Status Random(out ElementModQType* handle);
        }

        internal static unsafe class InternalElectionDescription
        {
            internal unsafe struct InternalElectionDescriptionType { };

            [DllImport(DllName, EntryPoint = "eg_internal_election_description_free")]
            internal static extern Status Free(InternalElectionDescriptionType* handle);

            [DllImport(DllName, EntryPoint = "eg_internal_election_description_get_description_hash")]
            internal static extern Status GetDescriptionHash(
                InternalElectionDescriptionType* handle, out ElementModQ.ElementModQType* description_hash);

            [DllImport(DllName, EntryPoint = "eg_internal_election_description_from_json")]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data, out InternalElectionDescriptionType* handle);

            [DllImport(DllName, EntryPoint = "eg_internal_election_description_from_bson")]
            internal static extern Status FromBson(
                uint *data, ulong length, InternalElectionDescriptionType* handle);

            [DllImport(DllName, EntryPoint = "eg_internal_election_description_to_json")]
            internal static extern Status ToJson(
                InternalElectionDescriptionType* handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_internal_election_description_to_bson")]
            internal static extern Status ToBson(
                InternalElectionDescriptionType* handle, out uint *data, out UIntPtr size);
        }

        internal static unsafe class CiphertextElectionContext
        {
            internal unsafe struct CiphertextElectionType { };

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_free")]
            internal static extern Status Free(CiphertextElectionType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_elgamal_public_key")]
            internal static extern Status GetElGamalPublicKey(
                CiphertextElectionType* handle, out ElementModP.ElementModPType* elgamal_public_key);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_description_hash")]
            internal static extern Status GetDescriptionHash(
                CiphertextElectionType* handle, out ElementModQ.ElementModQType* description_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_crypto_base_hash")]
            internal static extern Status GetCryptoBaseHash(
                CiphertextElectionType* handle, out ElementModQ.ElementModQType* crypto_base_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_crypto_extended_base_hash")]
            internal static extern Status GetCryptoExtendedBaseHash(
                CiphertextElectionType* handle, out ElementModQ.ElementModQType* crypto_extended_base_has);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_make")]
            internal static extern Status Make(
                ulong number_of_guardians, ulong quorum, ElementModP.ElementModPType* elgamal_public_key,
                ElementModQ.ElementModQType* description_hash, out CiphertextElectionType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_make_from_hex")]
            internal static extern Status Make(
                ulong number_of_guardians, ulong quorum, [MarshalAs(UnmanagedType.LPStr)] string hex_elgamal_public_key,
                [MarshalAs(UnmanagedType.LPStr)] string hex_description_hash, out CiphertextElectionType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_from_json")]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data, out CiphertextElectionType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_from_bson")]
            internal static extern Status FromBson(
                uint* data, ulong length, CiphertextElectionType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_to_json")]
            internal static extern Status ToJson(
                CiphertextElectionType* handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_to_bson")]
            internal static extern Status ToBson(
                CiphertextElectionType* handle, out uint* data, out UIntPtr size);
        }

        internal static unsafe class PlaintextBallot
        {
            internal unsafe struct PlaintextBallotType { };

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_free")]
            internal static extern Status Free(PlaintextBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_get_object_id")]
            internal static extern Status GetObjectId(PlaintextBallotType* handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_from_json")]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data, out PlaintextBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_from_bson")]
            internal static extern Status FromBson(
                uint* data, ulong length, PlaintextBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_to_json")]
            internal static extern Status ToJson(
                PlaintextBallotType* handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_to_bson")]
            internal static extern Status ToBson(
                PlaintextBallotType* handle, out uint* data, out UIntPtr size);
        }

        internal static unsafe class CiphertextBallot
        {
            internal unsafe struct CiphertextBallotType { };

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_free")]
            internal static extern Status Free(CiphertextBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_object_id")]
            internal static extern Status GetObjectId(CiphertextBallotType* handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_tracking_hash")]
            internal static extern Status GetTrackingHash(
                CiphertextBallotType* handle, out ElementModQ.ElementModQType* tracking_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_tracking_code")]
            internal static extern Status GetTrackingCode(CiphertextBallotType* handle, out IntPtr tracking_code);

            //[DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_timestamp")]
            //internal static extern Status GetTimestamp(CiphertextBallotType* handle, out ulong *timestamp);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_is_valid_encryption")]
            internal static extern bool IsValidEncryption(CiphertextBallotType* handle,
                ElementModQ.ElementModQType* seed_hash,
                ElementModP.ElementModPType* public_key,
                ElementModQ.ElementModQType* crypto_extended_base_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_from_json")]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data, out CiphertextBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_from_bson")]
            internal static extern Status FromBson(
                uint* data, ulong length, CiphertextBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_json")]
            internal static extern Status ToJson(
                CiphertextBallotType* handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_bson")]
            internal static extern Status ToBson(
                CiphertextBallotType* handle, out uint* data, out UIntPtr size);
        }

        internal static unsafe class EncryptionDevice
        {
            internal unsafe struct EncryptionDeviceType { };

            [DllImport(DllName, EntryPoint = "eg_encryption_device_new")]
            internal static extern Status New(
                ulong uuid, [MarshalAs(UnmanagedType.LPStr)] string location, out EncryptionDeviceType* handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_device_free")]
            internal static extern Status Free(EncryptionDeviceType* handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_device_get_hash")]
            internal static extern Status GetHash(
                EncryptionDeviceType* handle, out ElementModQ.ElementModQType* device_hash);
        }

        internal static unsafe class EncryptionMediator
        {
            internal unsafe struct EncryptionMediatorType { };

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_new")]
            internal static extern Status New(
                InternalElectionDescription.InternalElectionDescriptionType* metadata,
                CiphertextElectionContext.CiphertextElectionType* context,
                EncryptionDevice.EncryptionDeviceType* device, out EncryptionMediatorType* handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_free")]
            internal static extern Status Free(EncryptionMediatorType* handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_encrypt_ballot")]
            internal static extern Status Encrypt(
                EncryptionMediatorType* handle,
                PlaintextBallot.PlaintextBallotType* plainutext, out CiphertextBallot.CiphertextBallotType* ciphertext);
        }


        internal static unsafe class Encrypt
        {
            [DllImport(DllName, EntryPoint = "eg_encrypt_ballot")]
            internal static extern Status Ballot(
                PlaintextBallot.PlaintextBallotType* plaintext,
                 InternalElectionDescription.InternalElectionDescriptionType* metadata,
                 CiphertextElectionContext.CiphertextElectionType* context,
                 ElementModQ.ElementModQType* seedHash, bool shouldVerifyProofs,
                 out CiphertextBallot.CiphertextBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_encrypt_ballot_with_nonce")]
            internal static extern Status Ballot(
                PlaintextBallot.PlaintextBallotType* plaintext,
                 InternalElectionDescription.InternalElectionDescriptionType* metadata,
                 CiphertextElectionContext.CiphertextElectionType* context,
                 ElementModQ.ElementModQType* seedHash,
                 ElementModQ.ElementModQType* nonce, bool shouldVerifyProofs,
                 out CiphertextBallot.CiphertextBallotType* handle);
        }
    }
}
