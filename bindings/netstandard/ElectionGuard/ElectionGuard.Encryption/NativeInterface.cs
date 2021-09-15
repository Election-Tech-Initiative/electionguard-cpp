using System;
using System.Runtime.InteropServices;
using System.Runtime.ConstrainedExecution;

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
        const string DllName = "electionguard";

        internal unsafe struct CharPtr { };

        #region Group

        internal static unsafe class ElementModP
        {
            internal unsafe struct ElementModPType { };

            internal class ElementModPHandle
                : ElectionguardSafeHandle<ElementModPType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = ElementModP.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"ElementModP Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_new")]
            internal static extern Status New(
                ulong* in_data, out ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_new_unchecked")]
            internal static extern Status NewUnchecked(
                ulong* in_data, out ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_free")]
            internal static extern Status Free(ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_get_data")]
            internal static extern Status GetData(
                ElementModPHandle handle, ulong** out_data, out UIntPtr out_size);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_to_hex")]
            internal static extern Status ToHex(
                ElementModPHandle handle, out IntPtr data);
        }

        internal static unsafe class ElementModQ
        {
            internal unsafe struct ElementModQType { };

            internal class ElementModQHandle
                : ElectionguardSafeHandle<ElementModQType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = ElementModQ.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"ElementModQ Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_new")]
            internal static extern Status New(
                ulong* in_data, out ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_new_unchecked")]
            internal static extern Status NewUnchecked(
                ulong* in_data, out ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_free")]
            internal static extern Status Free(ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_get_data")]
            internal static extern Status GetData(
                ElementModQHandle handle, ulong** out_data, out UIntPtr out_size);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_to_hex")]
            internal static extern Status ToHex(
                ElementModQHandle handle, out IntPtr out_hex);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_rand_q_new")]
            internal static extern Status Random(out ElementModQHandle handle);
        }

        internal static unsafe class Constants
        {
            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_g")]
            internal static extern Status G(out ElementModP.ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_p")]
            internal static extern Status P(out ElementModP.ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_zero_mod_p")]
            internal static extern Status ZERO_MOD_P(out ElementModP.ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_one_mod_p")]
            internal static extern Status ONE_MOD_P(out ElementModP.ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_two_mod_p")]
            internal static extern Status TWO_MOD_P(out ElementModP.ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_p")]
            internal static extern Status Q(out ElementModQ.ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_constant_zero_mod_q")]
            internal static extern Status ZERO_MOD_Q(out ElementModQ.ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_constant_one_mod_q")]
            internal static extern Status ONE_MOD_Q(out ElementModQ.ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_constant_two_mod_q")]
            internal static extern Status TWO_MOD_Q(out ElementModQ.ElementModQHandle handle);
        }

        #endregion

        #region Elgamal

        internal static unsafe class ElGamalKeyPair
        {
            internal unsafe struct ElGamalKeyPairType { };

            internal class ElGamalKeyPairHandle
                : ElectionguardSafeHandle<ElGamalKeyPairType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = ElGamalKeyPair.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"ElGamalKeyPair Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_elgamal_keypair_from_secret_new")]
            internal static extern Status New(
                ElementModQ.ElementModQHandle in_secret_key,
                out ElGamalKeyPairHandle handle);

            [DllImport(DllName, EntryPoint = "eg_elgamal_keypair_free")]
            internal static extern Status Free(ElGamalKeyPairHandle handle);

            [DllImport(DllName, EntryPoint = "eg_elgamal_keypair_get_public_key")]
            internal static extern Status GetPublicKey(
                ElGamalKeyPairHandle handle,
                out ElementModP.ElementModPHandle out_public_key);

            [DllImport(DllName, EntryPoint = "eg_elgamal_keypair_get_secret_key")]
            internal static extern Status GetSecretKey(
                ElGamalKeyPairHandle handle,
                out ElementModQ.ElementModQHandle out_secret_key);

        }

        internal static unsafe class ElGamalCiphertext
        {
            internal unsafe struct ElGamalCiphertextType { };

            internal class ElGamalCiphertextHandle
                : ElectionguardSafeHandle<ElGamalCiphertextType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = ElGamalCiphertext.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"ElGamalCiphertext Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_elgamal_ciphertext_free")]
            internal static extern Status Free(ElGamalCiphertextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_elgamal_ciphertext_get_pad")]
            internal static extern Status GetPad(
                ElGamalCiphertextHandle handle,
                out ElementModP.ElementModPHandle elgamal_public_key);

            [DllImport(DllName, EntryPoint = "eg_elgamal_ciphertext_get_data")]
            internal static extern Status GetData(
                ElGamalCiphertextHandle handle,
                out ElementModP.ElementModPHandle elgamal_public_key);

            [DllImport(DllName, EntryPoint = "eg_elgamal_ciphertext_crypto_hash")]
            internal static extern Status GetCryptoHash(
                ElGamalCiphertextHandle handle,
                out ElementModQ.ElementModQHandle crypto_base_hash);

            [DllImport(DllName, EntryPoint = "eg_elgamal_ciphertext_decrypt_with_secret")]
            internal static extern Status DecryptWithSecret(
                ElGamalCiphertextHandle handle,
                ElementModQ.ElementModQHandle secret_key,
                ref ulong plaintext);

        }

        internal static unsafe class ElGamal
        {
            [DllImport(DllName, EntryPoint = "eg_elgamal_encrypt")]
            internal static extern Status Encrypt(
                ulong plaintext,
                ElementModQ.ElementModQHandle nonce,
                ElementModP.ElementModPHandle public_key,
                out ElGamalCiphertext.ElGamalCiphertextHandle handle);
        }

        #endregion

        #region ChaumPedersen

        internal static unsafe class DisjunctiveChaumPedersenProof
        {
            internal unsafe struct DisjunctiveChaumPedersenProofType { };

            internal class DisjunctiveChaumPedersenProofHandle
                : ElectionguardSafeHandle<DisjunctiveChaumPedersenProofType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = DisjunctiveChaumPedersenProof.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"DisjunctiveChaumPedersenProof Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_free")]
            internal static extern Status Free(DisjunctiveChaumPedersenProofHandle handle);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_zero_pad")]
            internal static extern Status GetZeroPad(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_zero_data")]
            internal static extern Status GetZeroData(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_one_pad")]
            internal static extern Status GetOnePad(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_one_data")]
            internal static extern Status GetOneData(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_zero_challenge")]
            internal static extern Status GetZeroChallenge(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_one_challenge")]
            internal static extern Status GetOneChallenge(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_challenge")]
            internal static extern Status GetChallenge(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_zero_response")]
            internal static extern Status GetZeroResponse(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_one_response")]
            internal static extern Status GetOneResponse(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_make")]
            internal static extern Status Make(
                ElGamalCiphertext.ElGamalCiphertextHandle message,
                ElementModQ.ElementModQHandle r,
                ElementModP.ElementModPHandle k,
                ElementModQ.ElementModQHandle q,
                ElementModQ.ElementModQHandle seed,
                ulong plaintext,
                out DisjunctiveChaumPedersenProofHandle handle);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_is_valid")]
            internal static extern bool IsValid(
                DisjunctiveChaumPedersenProofHandle handle,
                ElGamalCiphertext.ElGamalCiphertextHandle message,
                ElementModP.ElementModPHandle k,
                ElementModQ.ElementModQHandle q);

        }

        internal static unsafe class ConstantChaumPedersenProof
        {
            internal unsafe struct ConstantChaumPedersenProofType { };

            internal class ConstantChaumPedersenProofHandle
                : ElectionguardSafeHandle<ConstantChaumPedersenProofType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = ConstantChaumPedersenProof.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"ConstantChaumPedersenProof Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_free")]
            internal static extern Status Free(ConstantChaumPedersenProofHandle handle);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_get_pad")]
            internal static extern Status GetPad(
                ConstantChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_get_data")]
            internal static extern Status GetData(
                ConstantChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_get_challenge")]
            internal static extern Status GetChallenge(
                ConstantChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_get_response")]
            internal static extern Status GetResponse(
                ConstantChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_make")]
            internal static extern Status Make(
                ElGamalCiphertext.ElGamalCiphertextHandle message,
                ElementModQ.ElementModQHandle r,
                ElementModP.ElementModPHandle k,
                ElementModQ.ElementModQHandle seed,
                ElementModQ.ElementModQHandle hash_header,
                ulong constant,
                out ConstantChaumPedersenProofHandle handle);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_is_valid")]
            internal static extern bool IsValid(
                ConstantChaumPedersenProofHandle handle,
                ElGamalCiphertext.ElGamalCiphertextHandle message,
                ElementModP.ElementModPHandle k,
                ElementModQ.ElementModQHandle q);

        }


        #endregion

        #region Manifest

        internal static unsafe class Manifest
        {
            internal unsafe struct ManifestType { };

            internal class ManifestHandle
                : ElectionguardSafeHandle<ManifestType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = Manifest.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"Manifest Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_election_manifest_free")]
            internal static extern Status Free(ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_election_scope_id")]
            internal static extern Status GetElectionScopeId(
                ManifestHandle handle, out IntPtr election_scope_id);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_crypto_hash")]
            internal static extern Status CryptoHash(
                ManifestHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_from_json")]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_from_bson")]
            internal static extern Status FromBson(
                uint* data, ulong length, ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_to_json")]
            internal static extern Status ToJson(
                ManifestHandle handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_to_bson")]
            internal static extern Status ToBson(
                ManifestHandle handle, out uint* data, out UIntPtr size);
        }

        #endregion

        #region InternalManifest

        internal static unsafe class InternalManifest
        {
            internal unsafe struct InternalManifestType { };

            internal class InternalManifestHandle
                : ElectionguardSafeHandle<InternalManifestType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = InternalManifest.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"InternalManifest Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_new")]
            internal static extern Status New(
                Manifest.ManifestHandle manifest,
                out InternalManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_free")]
            internal static extern Status Free(
                InternalManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_get_manifest_hash")]
            internal static extern Status GetManifestHash(
                InternalManifestHandle handle,
                out ElementModQ.ElementModQHandle manifest_hash);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_from_json")]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out InternalManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_from_bson")]
            internal static extern Status FromBson(
                uint* data, ulong length, InternalManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_to_json")]
            internal static extern Status ToJson(
                InternalManifestHandle handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_to_bson")]
            internal static extern Status ToBson(
                InternalManifestHandle handle, out uint* data, out UIntPtr size);
        }

        #endregion

        #region CiphertextElectionContext

        internal static unsafe class CiphertextElectionContext
        {
            internal unsafe struct CiphertextElectionType { };

            internal class CiphertextElectionContextHandle
                : ElectionguardSafeHandle<CiphertextElectionType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = CiphertextElectionContext.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"CiphertextElectionContext Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_free")]
            internal static extern Status Free(CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_elgamal_public_key")]
            internal static extern Status GetElGamalPublicKey(
                CiphertextElectionContextHandle handle,
                out ElementModP.ElementModPHandle elgamal_public_key);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_commitment_hash")]
            internal static extern Status GetCommitmentHash(
                CiphertextElectionContextHandle handle,
                out ElementModQ.ElementModQHandle commitment_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_manifest_hash")]
            internal static extern Status GetManifestHash(
                CiphertextElectionContextHandle handle,
                out ElementModQ.ElementModQHandle manifest_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_crypto_base_hash")]
            internal static extern Status GetCryptoBaseHash(
                CiphertextElectionContextHandle handle,
                out ElementModQ.ElementModQHandle crypto_base_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_get_crypto_extended_base_hash")]
            internal static extern Status GetCryptoExtendedBaseHash(
                CiphertextElectionContextHandle handle,
                out ElementModQ.ElementModQHandle crypto_extended_base_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_make")]
            internal static extern Status Make(
                ulong number_of_guardians,
                ulong quorum,
                ElementModP.ElementModPHandle elgamal_public_key,
                ElementModQ.ElementModQHandle commitment_hash,
                ElementModQ.ElementModQHandle manifest_hash,
                out CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_make_from_hex")]
            internal static extern Status Make(
                ulong number_of_guardians,
                ulong quorum,
                [MarshalAs(UnmanagedType.LPStr)] string hex_elgamal_public_key,
                [MarshalAs(UnmanagedType.LPStr)] string hex_commitment_hash,
                [MarshalAs(UnmanagedType.LPStr)] string hex_manifest_hash,
                out CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_from_json")]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_from_bson")]
            internal static extern Status FromBson(
                uint* data, ulong length, CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_to_json")]
            internal static extern Status ToJson(
                CiphertextElectionContextHandle handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_to_bson")]
            internal static extern Status ToBson(
                CiphertextElectionContextHandle handle, out uint* data, out UIntPtr size);
        }

        #endregion

        #region Ballot

        internal static unsafe class PlaintextBallotSelection
        {
            internal unsafe struct PlaintextBallotSelectionType { };

            internal class PlaintextBallotSelectionHandle
                : ElectionguardSafeHandle<PlaintextBallotSelectionType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = PlaintextBallotSelection.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"PlaintextBallotSelection Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_selection_new")]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                long vote,
                bool isPlaceholderSelection,
                out PlaintextBallotSelectionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_selection_new_with_extended_data")]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                long vote,
                bool isPlaceholderSelection,
                [MarshalAs(UnmanagedType.LPStr)] string extendedData,
                long extendedDataLength,
                out PlaintextBallotSelectionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_selection_free")]
            internal static extern Status Free(PlaintextBallotSelectionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_selection_get_object_id")]
            internal static extern Status GetObjectId(
                PlaintextBallotSelectionHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_selection_get_is_placeholder")]
            internal static extern bool GetIsPlaceholder(
                PlaintextBallotSelectionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_selection_is_valid")]
            internal static extern bool IsValid(
                PlaintextBallotSelectionHandle handle,
                [MarshalAs(UnmanagedType.LPStr)] string expectedObjectId);
        }

        internal static unsafe class CiphertextBallotSelection
        {
            internal unsafe struct CiphertextBallotSelectionType { };

            internal class CiphertextBallotSelectionHandle
                : ElectionguardSafeHandle<CiphertextBallotSelectionType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = CiphertextBallotSelection.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"CiphertextBallotSelection Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_free")]
            internal static extern Status Free(CiphertextBallotSelectionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_get_object_id")]
            internal static extern Status GetObjectId(
                CiphertextBallotSelectionHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_get_description_hash")]
            internal static extern Status GetDescriptionHash(
                CiphertextBallotSelectionHandle handle,
                out ElementModQ.ElementModQHandle description_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_get_is_placeholder")]
            internal static extern bool GetIsPlaceholder(CiphertextBallotSelectionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_get_ciphertext")]
            internal static extern Status GetCiphertext(
                CiphertextBallotSelectionHandle handle,
                out ElGamalCiphertext.ElGamalCiphertextHandle ciphertext);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_get_crypto_hash")]
            internal static extern Status GetCryptoHash(
                CiphertextBallotSelectionHandle handle,
                out ElementModQ.ElementModQHandle cryptoHash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_get_nonce")]
            internal static extern Status GetNonce(
                CiphertextBallotSelectionHandle handle,
                out ElementModQ.ElementModQHandle nonce);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_get_proof")]
            internal static extern Status GetProof(
                CiphertextBallotSelectionHandle handle,
                out DisjunctiveChaumPedersenProof.DisjunctiveChaumPedersenProofHandle nonce);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_is_valid_encryption")]
            internal static extern bool IsValidEncryption(
                CiphertextBallotSelectionHandle handle,
                ElementModQ.ElementModQHandle encryption_seed,
                ElementModP.ElementModPHandle public_key,
                ElementModQ.ElementModQHandle crypto_extended_base_hash);

        }

        internal static unsafe class PlaintextBallotContest
        {
            internal unsafe struct PlaintextBallotContestType { };

            internal class PlaintextBallotContestHandle
                : ElectionguardSafeHandle<PlaintextBallotContestType>
            {
                [ReliabilityContract(Consistency.WillNotCorruptState, Cer.MayFail)]
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = PlaintextBallotContest.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"PlaintextBallotContest Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_contest_free")]
            internal static extern Status Free(PlaintextBallotContestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_contest_get_object_id")]
            internal static extern Status GetObjectId(
                PlaintextBallotContestHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_contest_get_selections_size")]
            internal static extern ulong GetSelectionsSize(
                PlaintextBallotContestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_contest_get_selection_at_index")]
            internal static extern Status GetSelectionAtIndex(
                PlaintextBallotContestHandle handle,
                ulong index,
                out PlaintextBallotSelection.PlaintextBallotSelectionHandle selection);
        }

        internal static unsafe class CiphertextBallotContest
        {
            internal unsafe struct CiphertextBallotContestType { };

            internal class CiphertextBallotContestHandle
                : ElectionguardSafeHandle<CiphertextBallotContestType>
            {
                [ReliabilityContract(Consistency.WillNotCorruptState, Cer.MayFail)]
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = CiphertextBallotContest.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"CiphertextBallotContest Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_free")]
            internal static extern Status Free(CiphertextBallotContestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_get_object_id")]
            internal static extern Status GetObjectId(
                CiphertextBallotContestHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_get_description_hash")]
            internal static extern Status GetDescriptionHash(
                CiphertextBallotContestHandle handle,
                out ElementModQ.ElementModQHandle description_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_get_selections_size")]
            internal static extern ulong GetSelectionsSize(
                CiphertextBallotContestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_get_selection_at_index")]
            internal static extern Status GetSelectionAtIndex(
                CiphertextBallotContestHandle handle,
                ulong index,
                out CiphertextBallotSelection.CiphertextBallotSelectionHandle selection);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_get_nonce")]
            internal static extern Status GetNonce(
                CiphertextBallotContestHandle handle,
                out ElementModQ.ElementModQHandle nonce);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_get_ciphertext_accumulation")]
            internal static extern Status GetCiphertextAccumulation(
                CiphertextBallotContestHandle handle,
                out ElGamalCiphertext.ElGamalCiphertextHandle nonce);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_get_crypto_hash")]
            internal static extern Status GetCryptoHash(
                CiphertextBallotContestHandle handle,
                out ElementModQ.ElementModQHandle cryptoHash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_get_proof")]
            internal static extern Status GetProof(
                CiphertextBallotContestHandle handle,
                out ConstantChaumPedersenProof.ConstantChaumPedersenProofHandle nonce);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_is_valid_encryption")]
            internal static extern bool IsValidEncryption(
                CiphertextBallotContestHandle handle,
                ElementModQ.ElementModQHandle encryption_seed,
                ElementModP.ElementModPHandle public_key,
                ElementModQ.ElementModQHandle crypto_extended_base_hash);
        }

        internal static unsafe class PlaintextBallot
        {
            internal unsafe struct PlaintextBallotType { };

            internal class PlaintextBallotHandle
                : ElectionguardSafeHandle<PlaintextBallotType>
            {
                [ReliabilityContract(Consistency.WillNotCorruptState, Cer.MayFail)]
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = PlaintextBallot.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"PlaintextBallot Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_free")]
            internal static extern Status Free(PlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_get_object_id")]
            internal static extern Status GetObjectId(
                PlaintextBallotHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_get_style_id")]
            internal static extern Status GetStyleId(
                PlaintextBallotHandle handle, out IntPtr style_id);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_get_contests_size")]
            internal static extern ulong GetContestsSize(PlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_get_contest_at_index")]
            internal static extern Status GetContestAtIndex(
                PlaintextBallotHandle handle,
                ulong index,
                out PlaintextBallotContest.PlaintextBallotContestHandle contest);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_from_json")]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out PlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_from_bson")]
            internal static extern Status FromBson(
                uint* data, ulong length, PlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_to_json")]
            internal static extern Status ToJson(
                PlaintextBallotHandle handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_to_bson")]
            internal static extern Status ToBson(
                PlaintextBallotHandle handle, out uint* data, out UIntPtr size);
        }

        internal static unsafe class CompactPlaintextBallot
        {
            internal unsafe struct CompactPlaintextBallotType { };

            internal class CompactPlaintextBallotHandle
                : ElectionguardSafeHandle<CompactPlaintextBallotType>
            {
                [ReliabilityContract(Consistency.WillNotCorruptState, Cer.MayFail)]
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = CompactPlaintextBallot.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"CompactPlaintextBallot Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_compact_plaintext_ballot_free")]
            internal static extern Status Free(CompactPlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_compact_plaintext_ballot_from_msgpack")]
            internal static extern Status FromMsgPack(
                byte* data, ulong size, out CompactPlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_compact_plaintext_ballot_to_msgpack")]
            internal static extern Status ToMsgPack(
                CompactPlaintextBallotHandle handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_compact_plaintext_ballot_msgpack_free")]
            internal static extern Status MsgPackFree(IntPtr data);
        }

        internal static unsafe class CiphertextBallot
        {
            internal unsafe struct CiphertextBallotType { };

            internal class CiphertextBallotHandle
                : ElectionguardSafeHandle<CiphertextBallotType>
            {
                [ReliabilityContract(Consistency.WillNotCorruptState, Cer.MayFail)]
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = CiphertextBallot.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"CiphertextBallot Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_free")]
            internal static extern Status Free(CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_object_id")]
            internal static extern Status GetObjectId(
                CiphertextBallotHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_style_id")]
            internal static extern Status GetStyleId(
                CiphertextBallotHandle handle, out IntPtr style_id);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_manifest_hash")]
            internal static extern Status GetManifestHash(
                CiphertextBallotHandle handle,
                out ElementModQ.ElementModQHandle manifest_hash_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_ballot_code_seed")]
            internal static extern Status GetBallotCodeSeed(
                CiphertextBallotHandle handle,
                out ElementModQ.ElementModQHandle ballot_code_seed_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_contests_size")]
            internal static extern ulong GetContestsSize(CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_contest_at_index")]
            internal static extern Status GetContestAtIndex(
                CiphertextBallotHandle handle,
                ulong index,
                out CiphertextBallotContest.CiphertextBallotContestHandle contest_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_ballot_code")]
            internal static extern Status GetBallotCode(
                CiphertextBallotHandle handle,
                out ElementModQ.ElementModQHandle ballot_code_ref);

            // TODO: ISSUE #129: get timestamp

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_nonce")]
            internal static extern Status GetNonce(
                CiphertextBallotHandle handle,
                out ElementModQ.ElementModQHandle nonce_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_crypto_hash")]
            internal static extern Status GetCryptoHash(
                CiphertextBallotHandle handle,
                out ElementModQ.ElementModQHandle hash_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_is_valid_encryption")]
            internal static extern bool IsValidEncryption(
                CiphertextBallotHandle handle,
                ElementModQ.ElementModQHandle manifest_hash,
                ElementModP.ElementModPHandle public_key,
                ElementModQ.ElementModQHandle crypto_extended_base_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_from_json")]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_from_bson")]
            internal static extern Status FromBson(
                uint* data, ulong length, CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_json")]
            internal static extern Status ToJson(
                CiphertextBallotHandle handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_json_with_nonces")]
            internal static extern Status ToJsonWithNonces(
                CiphertextBallotHandle handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_bson")]
            internal static extern Status ToBson(
                CiphertextBallotHandle handle, out uint* data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_bson_with_nonces")]
            internal static extern Status ToBsonWithNonces(
                CiphertextBallotHandle handle, out uint* data, out UIntPtr size);
        }

        internal static unsafe class CompactCiphertextBallot
        {
            internal unsafe struct CompactCiphertextBallotType { };

            internal class CompactCiphertextBallotHandle
                : ElectionguardSafeHandle<CompactCiphertextBallotType>
            {
                [ReliabilityContract(Consistency.WillNotCorruptState, Cer.MayFail)]
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = CompactCiphertextBallot.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"CompactCiphertextBallot Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_compact_ciphertext_ballot_free")]
            internal static extern Status Free(CompactCiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_compact_ciphertext_ballot_get_object_id")]
            internal static extern Status GetObjectId(
                CompactCiphertextBallotHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_compact_ciphertext_ballot_from_msgpack")]
            internal static extern Status FromMsgPack(
                byte* data, ulong size, out CompactCiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_compact_ciphertext_ballot_to_msgpack")]
            internal static extern Status ToMsgPack(
                CompactCiphertextBallotHandle handle, out IntPtr data, out UIntPtr size);

            [DllImport(DllName, EntryPoint = "eg_compact_ciphertext_ballot_msgpack_free")]
            internal static extern Status MsgPackFree(IntPtr data);

        }

        #endregion

        #region Encrypt

        internal static unsafe class EncryptionDevice
        {
            internal unsafe struct EncryptionDeviceType { };

            internal class EncryptionDeviceHandle
                : ElectionguardSafeHandle<EncryptionDeviceType>
            {
                [ReliabilityContract(Consistency.WillNotCorruptState, Cer.MayFail)]
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = EncryptionDevice.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"EncryptionDevice Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_encryption_device_new")]
            internal static extern Status New(
                ulong deviceUuid,
                ulong sessionUuid,
                ulong launchCode,
                [MarshalAs(UnmanagedType.LPStr)] string location,
                out EncryptionDeviceHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_device_free")]
            internal static extern Status Free(EncryptionDeviceHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_device_get_hash")]
            internal static extern Status GetHash(
                EncryptionDeviceHandle handle,
                out ElementModQ.ElementModQHandle device_hash);
        }

        internal static unsafe class EncryptionMediator
        {
            internal unsafe struct EncryptionMediatorType { };

            internal class EncryptionMediatorHandle
                : ElectionguardSafeHandle<EncryptionMediatorType>
            {
                [ReliabilityContract(Consistency.WillNotCorruptState, Cer.MayFail)]
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = EncryptionMediator.Free(this);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        Console.WriteLine($"EncryptionMediator Error Free: {status}");
                        return false;
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_new")]
            internal static extern Status New(
                InternalManifest.InternalManifestHandle manifest,
                CiphertextElectionContext.CiphertextElectionContextHandle context,
                EncryptionDevice.EncryptionDeviceHandle device,
                out EncryptionMediatorHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_free")]
            internal static extern Status Free(EncryptionMediatorHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_compact_encrypt_ballot")]
            internal static extern Status CompactEncrypt(
                EncryptionMediatorHandle handle,
                PlaintextBallot.PlaintextBallotHandle plainutext,
                out CompactCiphertextBallot.CompactCiphertextBallotHandle ciphertext);

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_compact_encrypt_ballot_verify_proofs")]
            internal static extern Status CompactEncryptAndVerify(
                EncryptionMediatorHandle handle,
                PlaintextBallot.PlaintextBallotHandle plainutext,
                out CompactCiphertextBallot.CompactCiphertextBallotHandle ciphertext);

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_encrypt_ballot")]
            internal static extern Status Encrypt(
                EncryptionMediatorHandle handle,
                PlaintextBallot.PlaintextBallotHandle plainutext,
                out CiphertextBallot.CiphertextBallotHandle ciphertext);

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_encrypt_ballot_verify_proofs")]
            internal static extern Status EncryptAndVerify(
                EncryptionMediatorHandle handle,
                PlaintextBallot.PlaintextBallotHandle plainutext,
                out CiphertextBallot.CiphertextBallotHandle ciphertext);
        }


        internal static unsafe class Encrypt
        {
            // TODO: when the manifest is completely exposed
            // [DllImport(DllName, EntryPoint = "eg_encrypt_selection")]
            // internal static extern Status Selection(
            //     PlaintextBallotSelection.PlaintextBallotSelectionHandle plaintext,
            //     SelectionDescription.SelectionDescriptionHandle metadata,
            //     CiphertextElectionContext.CiphertextElectionContextHandle context,
            //     ElementModQ.ElementModQHandle ballot_code_seed,
            //     ElementModQ.ElementModQHandle nonce,
            //     bool shouldVerifyProofs,
            //     out CiphertextBallot.CiphertextBallotHandle handle);

            // [DllImport(DllName, EntryPoint = "eg_encrypt_contest")]
            // internal static extern Status Contest(
            //     PlaintextBallotContest.PlaintextBallotContestHandle plaintext,
            //     ContestDescription.InternalManifestHandle metadata,
            //     CiphertextElectionContext.CiphertextElectionContextHandle context,
            //     ElementModQ.ElementModQHandle ballot_code_seed,
            //     ElementModQ.ElementModQHandle nonce,
            //     bool shouldVerifyProofs,
            //     out CiphertextBallot.CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encrypt_ballot")]
            internal static extern Status Ballot(
                PlaintextBallot.PlaintextBallotHandle plaintext,
                InternalManifest.InternalManifestHandle internal_manifest,
                CiphertextElectionContext.CiphertextElectionContextHandle context,
                ElementModQ.ElementModQHandle ballot_code_seed,
                bool shouldVerifyProofs,
                out CiphertextBallot.CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encrypt_ballot_with_nonce")]
            internal static extern Status Ballot(
                PlaintextBallot.PlaintextBallotHandle plaintext,
                InternalManifest.InternalManifestHandle internal_manifest,
                CiphertextElectionContext.CiphertextElectionContextHandle context,
                ElementModQ.ElementModQHandle ballot_code_seed,
                ElementModQ.ElementModQHandle nonce,
                bool shouldVerifyProofs,
                out CiphertextBallot.CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encrypt_compact_ballot")]
            internal static extern Status CompactBallot(
                PlaintextBallot.PlaintextBallotHandle plaintext,
                InternalManifest.InternalManifestHandle internal_manifest,
                CiphertextElectionContext.CiphertextElectionContextHandle context,
                ElementModQ.ElementModQHandle ballot_code_seed,
                bool shouldVerifyProofs,
                out CompactCiphertextBallot.CompactCiphertextBallotHandle handle);
        }
    }

    #endregion
}
