using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{

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

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_create")]
            internal static extern ElementModPType* Create(ulong* element);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_get")]
            internal static extern int Get(ElementModPType* element, ulong** data);
        }

        internal static unsafe class ElementModQ
        {
            internal unsafe struct ElementModQType { };

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_create")]
            internal static extern ElementModQType* Create(ulong* element);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_get")]
            internal static extern int Get(ElementModQType* element, ulong** data);
        }

        internal static unsafe class InternalElectionDescription
        {
            internal unsafe struct InternalElectionDescriptionType { };

            [DllImport(DllName, EntryPoint = "eg_internal_election_description_get_description_hash")]
            internal static extern ElementModQ.ElementModQType* GetDescriptionHash(
                InternalElectionDescriptionType* metadata);

            [DllImport(DllName, EntryPoint = "eg_internal_election_description_from_json")]
            internal static extern InternalElectionDescriptionType* FromJson([MarshalAs(UnmanagedType.LPStr)] string data);

            [DllImport(DllName, EntryPoint = "eg_internal_election_description_to_json")]
            internal static extern ulong ToJson(InternalElectionDescriptionType* metadata, out IntPtr out_data);
        }

        internal static unsafe class CiphertextElectionContext
        {
            internal unsafe struct CiphertextElectionType { };

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_elgamal_public_key")]
            internal static extern ElementModP.ElementModPType* GetElGamalPublicKey (
                CiphertextElectionType* context);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_description_hash")]
            internal static extern ElementModQ.ElementModQType* GetDescriptionHash(
                CiphertextElectionType* context);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_crypto_base_hash")]
            internal static extern ElementModQ.ElementModQType* GetCryptoBaseHash(
                CiphertextElectionType* context);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_crypto_extended_base_hash")]
            internal static extern ElementModQ.ElementModQType* GetCryptoExtendedBaseHash(
                CiphertextElectionType* context);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_from_json")]
            internal static extern CiphertextElectionType* FromJson([MarshalAs(UnmanagedType.LPStr)] string data);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_to_json")]
            internal static extern ulong ToJson(CiphertextElectionType* context, out IntPtr out_data);
        }

        internal static unsafe class PlaintextBallot
        {
            internal unsafe struct PlaintextBallotType { };

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_get_object_id")]
            internal static extern char* GetObjectId(PlaintextBallotType* plaintext);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_from_json")]
            internal static extern PlaintextBallotType* FromJson([MarshalAs(UnmanagedType.LPStr)] string data);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_to_json")]
            internal static extern ulong ToJson(PlaintextBallotType* plaintext, out IntPtr out_data);
        }

        internal static unsafe class CiphertextBallot
        {
            internal unsafe struct CiphertextBallotType { };

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_object_id")]
            internal static extern char* GetObjectId(CiphertextBallotType* ciphertext);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_tracking_hash")]
            internal static extern ElementModQ.ElementModQType* GetTrackingHash(CiphertextBallotType* ciphertext);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_tracking_code")]
            internal static extern char* GetTrackingCode(CiphertextBallotType* ciphertext);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_timestamp")]
            internal static extern ulong GetTimestamp(CiphertextBallotType* ciphertext);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_is_valid_encryption")]
            internal static extern bool IsValidEncryption(CiphertextBallotType* ciphertext,
                ElementModQ.ElementModQType* seed_hash,
                ElementModP.ElementModPType* public_key,
                ElementModQ.ElementModQType* crypto_extended_base_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_from_json")]
            internal static extern CiphertextBallotType* FromJson([MarshalAs(UnmanagedType.LPStr)] string data);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_json")]
            internal static extern ulong ToJson(CiphertextBallotType* ciphertext, out IntPtr out_data);
        }

        internal static unsafe class EncryptionDevice
        {
            internal unsafe struct EncryptionDeviceType { };

            [DllImport(DllName, EntryPoint = "eg_encryption_device_create")]
            internal static extern EncryptionDeviceType* Create(ulong uuid, [MarshalAs(UnmanagedType.LPStr)] string location);

            [DllImport(DllName, EntryPoint = "eg_encryption_device_get_hash")]
            internal static extern ElementModQ.ElementModQType* GetHash(EncryptionDeviceType* device);
        }

        internal static unsafe class EncryptionMediator
        {
            internal unsafe struct EncryptionMediatorType { };

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_create")]
            internal static extern EncryptionMediatorType* Create(
                InternalElectionDescription.InternalElectionDescriptionType* metadata,
                CiphertextElectionContext.CiphertextElectionType* context,
                EncryptionDevice.EncryptionDeviceType* device);

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_encrypt")]
            internal static extern CiphertextBallot.CiphertextBallotType* Encrypt(
                EncryptionMediatorType* mediator,
                PlaintextBallot.PlaintextBallotType* ballot);
        }


        internal static unsafe class Encrypt
        {
            [DllImport(DllName, EntryPoint = "eg_encrypt_ballot")]
            internal static extern CiphertextBallot.CiphertextBallotType* Ballot(
                PlaintextBallot.PlaintextBallotType* plaintext,
                 InternalElectionDescription.InternalElectionDescriptionType* metadata,
                 CiphertextElectionContext.CiphertextElectionType* context,
                 ElementModQ.ElementModQType* seedHash, bool shouldVerifyProofs);

            [DllImport(DllName, EntryPoint = "eg_encrypt_ballot_with_nonce")]
            internal static extern CiphertextBallot.CiphertextBallotType* Ballot(
                PlaintextBallot.PlaintextBallotType* plaintext,
                 InternalElectionDescription.InternalElectionDescriptionType* metadata,
                 CiphertextElectionContext.CiphertextElectionType* context,
                 ElementModQ.ElementModQType* seedHash,
                 ElementModQ.ElementModQType* nonce, bool shouldVerifyProofs);
        }


    }
}
