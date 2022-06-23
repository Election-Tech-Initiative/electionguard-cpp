using System;
using System.Runtime.ConstrainedExecution;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    /// <summary>
    /// Enumeration used to keep the status error messages for the exception handling
    /// </summary>
    public enum Status
    {
        /// <summary>
        /// The operation finished successfully
        /// </summary>
        ELECTIONGUARD_STATUS_SUCCESS = 0,

        /// <summary>
        /// There was an error with the arguments passed in
        /// </summary>
        ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT,

        /// <summary>
        /// Data was being accessed outside its range
        /// </summary>
        ELECTIONGUARD_STATUS_ERROR_OUT_OF_RANGE,

        /// <summary>
        /// Error with writing/reading the data
        /// </summary>
        ELECTIONGUARD_STATUS_ERROR_IO_ERROR,

        /// <summary>
        /// Had issue accessing data
        /// </summary>
        ELECTIONGUARD_STATUS_ERROR_BAD_ACCESS,

        /// <summary>
        /// Error allocating the data
        /// </summary>
        ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC,

        /// <summary>
        /// The data already exists
        /// </summary>
        ELECTIONGUARD_STATUS_ERROR_ALREADY_EXISTS,

        /// <summary>
        /// Error used as a default when catching any exception
        /// </summary>
        ELECTIONGUARD_STATUS_ERROR_RUNTIME_ERROR,

        /// <summary>
        /// This code should always be the last code in the collection
        /// so that the status codes string can be correctly derived
        /// </summary>
        ELECTIONGUARD_STATUS_UNKNOWN


    }

    static internal class StatusExtensions
    {
        public static void ThrowIfError(this Status status)
        {
            switch (status)
            {
                case Status.ELECTIONGUARD_STATUS_SUCCESS:
                    return;
                case Status.ELECTIONGUARD_STATUS_ERROR_INVALID_ARGUMENT:
                    throw new ArgumentException($"{status}");
                case Status.ELECTIONGUARD_STATUS_ERROR_OUT_OF_RANGE:
                    throw new ArgumentOutOfRangeException($"{status}");
                case Status.ELECTIONGUARD_STATUS_ERROR_IO_ERROR:
                    throw new SystemException($"{status}");
                case Status.ELECTIONGUARD_STATUS_ERROR_BAD_ACCESS:
                    throw new AccessViolationException($"{status}");
                case Status.ELECTIONGUARD_STATUS_ERROR_BAD_ALLOC:
                    throw new OutOfMemoryException($"{status}");
                case Status.ELECTIONGUARD_STATUS_ERROR_ALREADY_EXISTS:
                    throw new TypeLoadException($"{status}");
                case Status.ELECTIONGUARD_STATUS_ERROR_RUNTIME_ERROR:
                    throw new AggregateException($"{status}");
                case Status.ELECTIONGUARD_STATUS_UNKNOWN:
                default:
                    throw new Exception($"{status}");
            }
        }
    }

    /// <Summary>
    /// Enumeration used when marking a ballot as cast or spoiled
    /// </Summary>
    public enum BallotBoxState
    {
        /// <Summary>
        /// A ballot that has been explicitly cast
        /// </Summary>
        Cast = 1,
        /// <Summary>
        /// A ballot that has been explicitly spoiled
        /// </Summary>
        Spoiled = 2,
        /// <Summary>
        /// A ballot whose state is unknown to ElectionGuard and will not be included in any election results
        /// </Summary>
        Unknown = 999
    }

    /// <Summary>
    /// Enumeration for the type of ElectionType
    /// see: https://developers.google.com/elections-data/reference/election-type
    /// </Summary>
    public enum ElectionType
    {
        /// <summary>
        /// an unknown value
        /// </summary>
        unknown = 0,
        /// <summary>
        /// An election that's typically held on the national day for elections.
        /// </summary>
        general = 1,
        /// <summary>
        /// A primary election that's for a specific party, where voter eligibility for this contest is based on registration.
        /// </summary>
        partisanPrimaryClosed = 2,
        /// <summary>
        /// A primary election that's for a specific party, where voters declare their desired party or choose in private
        /// </summary>
        partisanPrimaryOpen = 3,
        /// <summary>
        /// A primary election without a specified type, such as a nonpartisan primary.
        /// </summary>
        primary = 4,
        /// <summary>
        /// An election to decide a prior contest that ended with no candidate receiving a majority of the votes
        /// </summary>
        runoff = 5,
        /// <summary>
        /// An election held out of sequence for special circumstances, such as to fill a vacated office.
        /// </summary>
        special = 6,
        /// <summary>
        /// The election is a type that isn't listed in this enumeration. If used, include the item's custom type in an OtherType element.
        /// </summary>
        other = 7
    };

    /// <Summary>
    /// Enumeration for the type of geopolitical unit
    /// see: https://developers.google.com/elections-data/reference/reporting-unit-type
    /// </Summary>
    public enum ReportingUnitType
    {
        /// <summary>
        /// an unknown value
        /// </summary>
        unknown = 0,
        /// <summary>
        /// Used to report batches of ballots that might cross precinct boundaries.
        /// </summary>
        ballotBatch = 1,
        /// <summary>
        /// Used for a ballot-style area that's generally composed of precincts.
        /// </summary>
        ballotStyleArea = 2,
        /// <summary>
        /// Used as a synonym for a county.
        /// </summary>
        borough = 3,
        /// <summary>
        /// Used for a city that reports results or for the district that encompasses it.
        /// </summary>
        city = 4,
        /// <summary>
        /// Used for city council districts.
        /// </summary>
        cityCouncil = 5,
        /// <summary>
        /// Used for one or more precincts that have been combined for the purposes of reporting. 
        /// If the term ward is used interchangeably with combined precinct, use combined-precinct 
        /// for the ReportingUnitType.
        /// </summary>
        combinedPrecinct = 6,
        /// <summary>
        /// Used for national legislative body districts.
        /// </summary>
        congressional = 7,
        /// <summary>
        /// Used for a country.
        /// </summary>
        country = 8,
        /// <summary>
        /// Used for a county or for the district that encompasses it. 
        /// Synonymous with borough and parish in some localities.
        /// </summary>
        county = 9,
        /// <summary>
        /// Used for county council districts.
        /// </summary>
        countyCouncil = 10,
        /// <summary>
        /// Used for a dropbox for absentee ballots.
        /// </summary>
        dropBox = 11,
        /// <summary>
        /// Used for judicial districts.
        /// </summary>
        judicial = 12,
        /// <summary>
        /// Used as applicable for various units such as towns, 
        /// townships, villages that report votes, or for the district 
        /// that encompasses them.
        /// </summary>
        municipality = 13,
        /// <summary>
        /// Used for a polling place.
        /// </summary>
        polling_place = 14,
        /// <summary>
        /// Used if the terms for ward or district are used interchangeably with precinct.
        /// </summary>
        precinct = 15,
        /// <summary>
        /// Used for a school district.
        /// </summary>
        school = 16,
        /// <summary>
        /// Used for a special district.
        /// </summary>
        special = 17,
        /// <summary>
        /// Used for splits of precincts.
        /// </summary>
        splitPrecinct = 18,
        /// <summary>
        /// Used for a state or for the district that encompasses it.
        /// </summary>
        state = 19,
        /// <summary>
        /// Used for a state house or assembly district.
        /// </summary>
        stateHouse = 20,
        /// <summary>
        /// Used for a state senate district.
        /// </summary>
        stateSenate = 21,
        /// <summary>
        /// Used for type of municipality that reports votes or for the district that encompasses it.
        /// </summary>
        township = 22,
        /// <summary>
        /// Used for a utility district.
        /// </summary>
        utility = 23,
        /// <summary>
        /// Used for a type of municipality that reports votes or for the district that encompasses it.
        /// </summary>
        village = 24,
        /// <summary>
        /// Used for a vote center.
        /// </summary>
        voteCenter = 25,
        /// <summary>
        /// Used for combinations or groupings of precincts or other units.
        /// </summary>
        ward = 26,
        /// <summary>
        /// Used for a water district.
        /// </summary>
        water = 27,
        /// <summary>
        /// Used for other types of reporting units that aren't included in this enumeration. 
        /// If used, provide the item's custom type in an OtherType element.
        /// </summary>
        other = 28,
    };

    /// <Summary>
    /// Enumeration for the type of VoteVariationType
    /// see: https://developers.google.com/elections-data/reference/vote-variation
    /// </Summary>
    public enum VoteVariationType
    {
        /// <summary>
        /// an unknown value
        /// </summary>
        unknown = 0,
        /// <summary>
        /// Each voter can select up to one option.
        /// </summary>
        one_of_m = 1,
        /// <summary>
        /// Approval voting, where each voter can select as many options as desired.
        /// </summary>
        approval = 2,
        /// <summary>
        /// Borda count, where each voter can rank the options, and the rankings are assigned point values.
        /// </summary>
        borda = 3,
        /// <summary>
        /// Cumulative voting, where each voter can distribute their vote to up to N options.
        /// </summary>
        cumulative = 4,
        /// <summary>
        /// A 1-of-m method where the winner needs more than 50% of the vote to be elected.
        /// </summary>
        majority = 5,
        /// <summary>
        /// A method where each voter can select up to N options.
        /// </summary>
        n_of_m = 6,
        /// <summary>
        /// A 1-of-m method where the option with the most votes is elected, regardless of 
        /// whether the option has more than 50% of the vote.
        /// </summary>
        plurality = 7,
        /// <summary>
        /// A proportional representation method, which is any system that elects winners 
        /// in proportion to the total vote. For the single transferable vote (STV) method, 
        /// use rcv instead.
        /// </summary>
        proportional = 8,
        /// <summary>
        /// Range voting, where each voter can select a score for each option.
        /// </summary>
        range = 9,
        /// <summary>
        /// Ranked choice voting (RCV), where each voter can rank the options, and the 
        /// ballots are counted in rounds. Also known as instant-runoff voting (IRV) and 
        /// the single transferable vote (STV).
        /// </summary>
        rcv = 10,
        /// <summary>
        /// A 1-of-m method where the winner needs more than some predetermined fraction 
        /// of the vote to be elected, and where the fraction is more than 50%. For example, 
        /// the winner might need three-fifths or two-thirds of the vote.
        /// </summary>
        super_majority = 11,
        /// <summary>
        /// The vote variation is a type that isn't included in this enumeration. 
        /// If used, provide the item's custom type in an OtherType element.
        /// </summary>
        other = 12
    };

    internal static unsafe class NativeInterface
    {
        const string DllName = "electionguard";

        internal unsafe struct CharPtr { };

        #region Collections

        internal static unsafe class LinkedList
        {
            internal unsafe struct LinkedListType { };

            internal class LinkedListHandle
                : ElectionguardSafeHandle<LinkedListType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = LinkedList.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"LinkedList Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_linked_list_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(out LinkedListHandle handle);

            [DllImport(DllName, EntryPoint = "eg_linked_list_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(LinkedListType* handle);

            [DllImport(DllName, EntryPoint = "eg_linked_list_append",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Append(
                LinkedListHandle handle,
                [MarshalAs(UnmanagedType.LPStr)] string key,
                [MarshalAs(UnmanagedType.LPStr)] string value);

            [DllImport(DllName, EntryPoint = "eg_linked_list_delete_last",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status DeleteLast(LinkedListHandle handle);

            [DllImport(DllName, EntryPoint = "eg_linked_list_get_count",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetCount(
                LinkedListHandle handle);

            [DllImport(DllName, EntryPoint = "eg_linked_list_get_element_at",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetElementAt(
                LinkedListHandle handle,
                ulong position,
                out IntPtr key,
                out IntPtr value);

            [DllImport(DllName, EntryPoint = "eg_linked_list_get_value_at",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetValueAt(
                LinkedListHandle handle,
                ulong position,
                out IntPtr value);
        }

        #endregion

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

                    var status = ElementModP.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"ElementModP Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                ulong* in_data, out ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_new_unchecked",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status NewUnchecked(
                ulong* in_data, out ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(ElementModPType* handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_get_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetData(
                ElementModPHandle handle, ulong** out_data, out ulong out_size);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_to_hex",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
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

                    var status = ElementModQ.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"ElementModQ Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                ulong* in_data, out ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_new_unchecked",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status NewUnchecked(
                ulong* in_data, out ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(ElementModQType* handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_get_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetData(
                ElementModQHandle handle, ulong** out_data, out ulong out_size);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_to_hex",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToHex(
                ElementModQHandle handle, out IntPtr out_hex);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_from_hex",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromHex(
                [MarshalAs(UnmanagedType.LPStr)] string hex,
                out ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_from_hex_unchecked",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromHexUnchecked(
                [MarshalAs(UnmanagedType.LPStr)] string hex,
                out ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_rand_q_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Random(out ElementModQHandle handle);
        }

        internal static unsafe class Constants
        {
            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_g",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status G(out ElementModP.ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_p",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status P(out ElementModP.ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_r",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status R(out ElementModP.ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_zero_mod_p",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ZERO_MOD_P(out ElementModP.ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_one_mod_p",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ONE_MOD_P(out ElementModP.ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_p_constant_two_mod_p",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status TWO_MOD_P(out ElementModP.ElementModPHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_constant_q",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Q(out ElementModQ.ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_constant_zero_mod_q",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ZERO_MOD_Q(out ElementModQ.ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_constant_one_mod_q",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ONE_MOD_Q(out ElementModQ.ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_element_mod_q_constant_two_mod_q",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status TWO_MOD_Q(out ElementModQ.ElementModQHandle handle);

            [DllImport(DllName, EntryPoint = "eg_constant_to_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToJson(out IntPtr data, out ulong size);

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

                    var status = ElGamalKeyPair.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"ElGamalKeyPair Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_elgamal_keypair_from_secret_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                ElementModQ.ElementModQHandle in_secret_key,
                out ElGamalKeyPairHandle handle);

            [DllImport(DllName, EntryPoint = "eg_elgamal_keypair_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(ElGamalKeyPairType* handle);

            [DllImport(DllName, EntryPoint = "eg_elgamal_keypair_get_public_key",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetPublicKey(
                ElGamalKeyPairHandle handle,
                out ElementModP.ElementModPHandle out_public_key);

            [DllImport(DllName, EntryPoint = "eg_elgamal_keypair_get_secret_key",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
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

                    var status = ElGamalCiphertext.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"ElGamalCiphertext Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_elgamal_ciphertext_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(ElGamalCiphertextType* handle);

            [DllImport(DllName, EntryPoint = "eg_elgamal_ciphertext_get_pad",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetPad(
                ElGamalCiphertextHandle handle,
                out ElementModP.ElementModPHandle elgamal_public_key);

            [DllImport(DllName, EntryPoint = "eg_elgamal_ciphertext_get_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetData(
                ElGamalCiphertextHandle handle,
                out ElementModP.ElementModPHandle elgamal_public_key);

            [DllImport(DllName, EntryPoint = "eg_elgamal_ciphertext_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCryptoHash(
                ElGamalCiphertextHandle handle,
                out ElementModQ.ElementModQHandle crypto_base_hash);

            [DllImport(DllName, EntryPoint = "eg_elgamal_ciphertext_decrypt_with_secret",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status DecryptWithSecret(
                ElGamalCiphertextHandle handle,
                ElementModQ.ElementModQHandle secret_key,
                ref ulong plaintext);

        }

        internal static unsafe class ElGamal
        {
            [DllImport(DllName, EntryPoint = "eg_elgamal_encrypt",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
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

                    var status = DisjunctiveChaumPedersenProof.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"DisjunctiveChaumPedersenProof Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(DisjunctiveChaumPedersenProofType* handle);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_zero_pad",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetZeroPad(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_zero_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetZeroData(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_one_pad",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetOnePad(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_one_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetOneData(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_zero_challenge",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetZeroChallenge(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_one_challenge",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetOneChallenge(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_challenge",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetChallenge(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_zero_response",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetZeroResponse(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_get_one_response",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetOneResponse(
                DisjunctiveChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_make",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Make(
                ElGamalCiphertext.ElGamalCiphertextHandle message,
                ElementModQ.ElementModQHandle r,
                ElementModP.ElementModPHandle k,
                ElementModQ.ElementModQHandle q,
                ulong plaintext,
                out DisjunctiveChaumPedersenProofHandle handle);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_make_deterministic",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Make(
                ElGamalCiphertext.ElGamalCiphertextHandle message,
                ElementModQ.ElementModQHandle r,
                ElementModP.ElementModPHandle k,
                ElementModQ.ElementModQHandle q,
                ElementModQ.ElementModQHandle seed,
                ulong plaintext,
                out DisjunctiveChaumPedersenProofHandle handle);

            [DllImport(DllName, EntryPoint = "eg_disjunctive_chaum_pedersen_proof_is_valid",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
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

                    var status = ConstantChaumPedersenProof.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"ConstantChaumPedersenProof Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(ConstantChaumPedersenProofType* handle);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_get_pad",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetPad(
                ConstantChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_get_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetData(
                ConstantChaumPedersenProofHandle handle,
                out ElementModP.ElementModPHandle element);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_get_challenge",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetChallenge(
                ConstantChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_get_response",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetResponse(
                ConstantChaumPedersenProofHandle handle,
                out ElementModQ.ElementModQHandle element);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_make",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Make(
                ElGamalCiphertext.ElGamalCiphertextHandle message,
                ElementModQ.ElementModQHandle r,
                ElementModP.ElementModPHandle k,
                ElementModQ.ElementModQHandle seed,
                ElementModQ.ElementModQHandle hash_header,
                ulong constant,
                out ConstantChaumPedersenProofHandle handle);

            [DllImport(DllName, EntryPoint = "eg_constant_chaum_pedersen_proof_is_valid",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern bool IsValid(
                ConstantChaumPedersenProofHandle handle,
                ElGamalCiphertext.ElGamalCiphertextHandle message,
                ElementModP.ElementModPHandle k,
                ElementModQ.ElementModQHandle q);

        }


        #endregion

        #region AnnotatedString

        internal static unsafe class AnnotatedString
        {
            internal unsafe struct AnnotatedStringType { };

            internal class AnnotatedStringHandle
                : ElectionguardSafeHandle<AnnotatedStringType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = AnnotatedString.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"AnnotatedString Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_annotated_string_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string annotation,
                [MarshalAs(UnmanagedType.LPStr)] string value,
                out AnnotatedStringHandle handle);

            [DllImport(DllName, EntryPoint = "eg_annotated_string_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(AnnotatedStringType* handle);

            [DllImport(DllName, EntryPoint = "eg_annotated_string_get_annotation",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetAnnotation(
                AnnotatedStringHandle handle, out IntPtr language);


            [DllImport(DllName, EntryPoint = "eg_annotated_string_get_value",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetValue(
                AnnotatedStringHandle handle, out IntPtr value);

            [DllImport(DllName, EntryPoint = "eg_annotated_string_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                AnnotatedStringHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);
        }

        #endregion

        #region Language

        internal static unsafe class Language
        {
            internal unsafe struct LanguageType { };

            internal class LanguageHandle
                : ElectionguardSafeHandle<LanguageType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = Language.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"Language Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_language_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string value,
                [MarshalAs(UnmanagedType.LPStr)] string language,
                out LanguageHandle handle);

            [DllImport(DllName, EntryPoint = "eg_language_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(LanguageType* handle);

            [DllImport(DllName, EntryPoint = "eg_language_get_value",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetValue(
                LanguageHandle handle, out IntPtr value);

            [DllImport(DllName, EntryPoint = "eg_language_get_language",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetLanguage(
                LanguageHandle handle, out IntPtr language);

            [DllImport(DllName, EntryPoint = "eg_language_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                LanguageHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);
        }

        #endregion

        #region InternationalizedText

        // TODO: get the calling conventions correct

        internal static unsafe class InternationalizedText
        {
            internal unsafe struct InternationalizedTextType { };

            internal class InternationalizedTextHandle
                : ElectionguardSafeHandle<InternationalizedTextType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = InternationalizedText.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"InternationalizedText Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_internationalized_text_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                // TODO: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] text,
                ulong textSize,
                out InternationalizedTextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internationalized_text_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(InternationalizedTextType* handle);

            [DllImport(DllName, EntryPoint = "eg_internationalized_text_get_text_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetTextSize(
                InternationalizedTextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internationalized_text_get_text_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetTextAtIndex(
                InternationalizedTextHandle handle,
                ulong index,
                out Language.LanguageHandle text);

            [DllImport(DllName, EntryPoint = "eg_intertnationalized_text_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                InternationalizedTextHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);
        }

        #endregion

        #region ContactInformation

        internal static unsafe class ContactInformation
        {
            internal unsafe struct ContactInformationType { };

            internal class ContactInformationHandle
                : ElectionguardSafeHandle<ContactInformationType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = ContactInformation.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"ContactInformation Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_contact_information_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string name,
                out ContactInformationHandle handle);

            // TODO: add eg_contact_information_new_with_collections

            [DllImport(DllName, EntryPoint = "eg_contact_information_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(ContactInformationType* handle);

            [DllImport(DllName, EntryPoint = "eg_contact_information_get_address_line_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetAddressLineSize(
                ContactInformationHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contact_information_get_address_line_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetAddressLineAtIndex(
                ContactInformationHandle handle,
                ulong index,
                out IntPtr address);

            [DllImport(DllName, EntryPoint = "eg_contact_information_get_email_line_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetEmailLineSize(
                ContactInformationHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contact_information_get_email_line_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetEmailLineAtIndex(
                ContactInformationHandle handle,
                ulong index,
                out InternationalizedText.InternationalizedTextHandle email);

            [DllImport(DllName, EntryPoint = "eg_contact_information_get_phone_line_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetPhoneLineSize(
                ContactInformationHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contact_information_get_email_line_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetPhoneLineAtIndex(
                ContactInformationHandle handle,
                ulong index,
                out InternationalizedText.InternationalizedTextHandle phone);

            [DllImport(DllName, EntryPoint = "eg_contact_information_get_name",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetName(
                ContactInformationHandle handle, out IntPtr value);

            [DllImport(DllName, EntryPoint = "eg_contact_information_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                ContactInformationHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);
        }

        #endregion

        #region GeopoliticalUnit

        internal static unsafe class GeopoliticalUnit
        {
            internal unsafe struct GeopoliticalUnitType { };

            internal class GeopoliticalUnitHandle
                : ElectionguardSafeHandle<GeopoliticalUnitType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = GeopoliticalUnit.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"GeopoliticalUnit Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_geopolitical_unit_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string name,
                ReportingUnitType reportingUnitType,
                out GeopoliticalUnitHandle handle);

            [DllImport(DllName, EntryPoint = "eg_geopolitical_unit_new_with_contact_info",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string name,
                ReportingUnitType reportingUnitType,
                ContactInformation.ContactInformationHandle contactInformation,
                out GeopoliticalUnitHandle handle);

            [DllImport(DllName, EntryPoint = "eg_geopolitical_unit_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(GeopoliticalUnitType* handle);

            [DllImport(DllName, EntryPoint = "eg_geopolitical_unit_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                GeopoliticalUnitHandle handle, out IntPtr objectId);

            [DllImport(DllName, EntryPoint = "eg_geopolitical_unit_get_name",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetName(
                GeopoliticalUnitHandle handle, out IntPtr name);

            [DllImport(DllName, EntryPoint = "get_geopolitical_unit_get_type",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ReportingUnitType GetReportingUnitType(
                GeopoliticalUnitHandle handle);

            [DllImport(DllName, EntryPoint = "eg_geopolitical_unit_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                GeopoliticalUnitHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);
        }

        #endregion

        #region BallotStyle

        internal static unsafe class BallotStyle
        {
            internal unsafe struct BallotStyleType { };

            internal class BallotStyleHandle
                : ElectionguardSafeHandle<BallotStyleType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = BallotStyle.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"BallotStyle Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_ballot_style_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPStr)] string[] gpUnitIds,
                ulong gpUnitIdsSize,
                out BallotStyleHandle handle);

            // TODO eg_ballot_style_new_with_parties

            [DllImport(DllName, EntryPoint = "eg_ballot_style_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(BallotStyleType* handle);

            [DllImport(DllName, EntryPoint = "eg_ballot_style_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                BallotStyleHandle handle, out IntPtr objectId);

            [DllImport(DllName, EntryPoint = "eg_ballot_style_get_geopolitical_unit_ids_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetGeopoliticalUnitSize(
                BallotStyleHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ballot_style_get_geopolitical_unit_id_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetGeopoliticalInitIdAtIndex(
                BallotStyleHandle handle,
                ulong index,
                out IntPtr gpUnitId);

            [DllImport(DllName, EntryPoint = "eg_ballot_style_get_party_ids_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetPartyIdsSize(
                BallotStyleHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ballot_style_get_party_id_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetPartyIdAtIndex(
                BallotStyleHandle handle,
                ulong index,
                out IntPtr partyId);

            [DllImport(DllName, EntryPoint = "eg_ballot_style_get_image_uri",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetImageUri(
                BallotStyleHandle handle, out IntPtr imageUri);

            [DllImport(DllName, EntryPoint = "eg_ballot_style_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                BallotStyleHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);
        }

        #endregion

        #region Party

        internal static unsafe class Party
        {
            internal unsafe struct PartyType { };

            internal class PartyHandle
                : ElectionguardSafeHandle<PartyType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = Party.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"Party Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_party_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                out PartyHandle handle);

            [DllImport(DllName, EntryPoint = "eg_party_new_with_extras",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                InternationalizedText.InternationalizedTextHandle name,
                [MarshalAs(UnmanagedType.LPStr)] string abbreviation,
                [MarshalAs(UnmanagedType.LPStr)] string color,
                [MarshalAs(UnmanagedType.LPStr)] string logoUri,
                out PartyHandle handle);

            [DllImport(DllName, EntryPoint = "eg_party_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(PartyType* handle);

            [DllImport(DllName, EntryPoint = "eg_party_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                PartyHandle handle, out IntPtr objectId);

            [DllImport(DllName, EntryPoint = "eg_party_get_abbreviation",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetAbbreviation(
                PartyHandle handle, out IntPtr abbreviation);

            [DllImport(DllName, EntryPoint = "eg_party_get_name",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetName(
                PartyHandle handle, out InternationalizedText.InternationalizedTextHandle name);

            [DllImport(DllName, EntryPoint = "eg_party_get_color",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetColor(
                PartyHandle handle, out IntPtr color);

            [DllImport(DllName, EntryPoint = "eg_party_get_logo_uri",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetLogoUri(
                PartyHandle handle, out IntPtr logoUri);

            [DllImport(DllName, EntryPoint = "eg_party_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                PartyHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);
        }

        #endregion

        #region Candidate

        internal static unsafe class Candidate
        {
            internal unsafe struct CandidateType { };

            internal class CandidateHandle
                : ElectionguardSafeHandle<CandidateType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = Candidate.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"Candidate Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_candidate_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                bool isWriteIn,
                out CandidateHandle handle);

            [DllImport(DllName, EntryPoint = "eg_candidate_new_with_party",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string partyId,
                bool isWriteIn,
                out CandidateHandle handle);

            [DllImport(DllName, EntryPoint = "eg_candidate_new_with_extras",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                InternationalizedText.InternationalizedTextHandle name,
                [MarshalAs(UnmanagedType.LPStr)] string partyId,
                [MarshalAs(UnmanagedType.LPStr)] string imageUri,
                bool isWriteIn,
                out CandidateHandle handle);

            [DllImport(DllName, EntryPoint = "eg_candidate_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(CandidateType* handle);

            [DllImport(DllName, EntryPoint = "eg_candidate_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                CandidateHandle handle, out IntPtr objectId);

            [DllImport(DllName, EntryPoint = "eg_candidate_get_candidate_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCandidateId(
                CandidateHandle handle, out IntPtr candidateId);

            [DllImport(DllName, EntryPoint = "eg_candidate_get_name",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetName(
                CandidateHandle handle, out IntPtr name);

            [DllImport(DllName, EntryPoint = "eg_candidate_get_party_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetPartyId(
                CandidateHandle handle, out IntPtr partyId);

            [DllImport(DllName, EntryPoint = "eg_candidate_get_image_uri",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetImageUri(
                CandidateHandle handle, out IntPtr iamgeUri);

            [DllImport(DllName, EntryPoint = "eg_candidate_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                CandidateHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);
        }

        #endregion

        #region SelectionDescription

        internal static unsafe class SelectionDescription
        {
            internal unsafe struct SelectionDescriptionType { };

            internal class SelectionDescriptionHandle
                : ElectionguardSafeHandle<SelectionDescriptionType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = SelectionDescription.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"SelectionDescription Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_selection_description_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string candidateId,
                ulong sequenceOrder,
                out SelectionDescriptionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_selection_description_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(SelectionDescriptionType* handle);

            [DllImport(DllName, EntryPoint = "eg_selection_description_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                SelectionDescriptionHandle handle, out IntPtr objectId);

            [DllImport(DllName, EntryPoint = "eg_selection_description_get_candidate_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCandidateId(
                SelectionDescriptionHandle handle, out IntPtr candidateId);

            [DllImport(DllName, EntryPoint = "eg_selection_description_get_sequence_order",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetSequenceOrder(
                SelectionDescriptionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_selection_description_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                SelectionDescriptionHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);
        }

        #endregion

        #region ContestDescription

        internal static unsafe class ContestDescription
        {
            internal unsafe struct ContestDescriptionType { };

            internal class ContestDescriptionHandle
                : ElectionguardSafeHandle<ContestDescriptionType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = ContestDescription.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"ContestDescription Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_contest_description_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string electoralDistrictId,
                ulong sequenceOrder,
                VoteVariationType voteVariation,
                ulong numberElected,
                [MarshalAs(UnmanagedType.LPStr)] string name,
                // TODO: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] selections,
                ulong selectionsSize,
                out ContestDescriptionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_new_with_title",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string electoralDistrictId,
                ulong sequenceOrder,
                VoteVariationType voteVariation,
                ulong numberElected,
                ulong votesAllowed,
                [MarshalAs(UnmanagedType.LPStr)] string name,
                InternationalizedText.InternationalizedTextHandle ballotTitle,
                InternationalizedText.InternationalizedTextHandle ballotSubTitle,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] selections,
                ulong selectionsSize,
                out ContestDescriptionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_new_with_parties",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string electoralDistrictId,
                ulong sequenceOrder,
                VoteVariationType voteVariation,
                ulong numberElected,
                [MarshalAs(UnmanagedType.LPStr)] string name,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] selections,
                ulong selectionsSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] string[] primaryPartyIds,
                ulong primaryPartyIdsSize,
                out ContestDescriptionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_new_with_title_and_parties",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string electoralDistrictId,
                ulong sequenceOrder,
                VoteVariationType voteVariation,
                ulong numberElected,
                ulong votesAllowed,
                [MarshalAs(UnmanagedType.LPStr)] string name,
                InternationalizedText.InternationalizedTextHandle ballotTitle,
                InternationalizedText.InternationalizedTextHandle ballotSubTitle,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] selections,
                ulong selectionsSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] string[] primaryPartyIds,
                ulong primaryPartyIdsSize,
                out ContestDescriptionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(ContestDescriptionType* handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                ContestDescriptionHandle handle, out IntPtr objectId);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_electoral_district_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetElectoralDistrictId(
                ContestDescriptionHandle handle, out IntPtr electoralDistrictId);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_sequence_order",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetSequenceOrder(
                ContestDescriptionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_vote_variation",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern VoteVariationType GetVoteVariationType(
                ContestDescriptionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_number_elected",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetNumberElected(
                ContestDescriptionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_votes_allowed",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetVotesAllowed(
                ContestDescriptionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_name",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetName(
                ContestDescriptionHandle handle, out IntPtr name);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_ballot_title",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetBallotTitle(
                ContestDescriptionHandle handle,
                out InternationalizedText.InternationalizedTextHandle name);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_ballot_subtitle",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetBallotSubTitle(
                ContestDescriptionHandle handle,
                out InternationalizedText.InternationalizedTextHandle name);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_selections_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetSelectionsSize(
                ContestDescriptionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_selection_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetSelectionAtIndex(
                ContestDescriptionHandle handle,
                ulong index,
                out SelectionDescription.SelectionDescriptionHandle partyId);

            [DllImport(DllName, EntryPoint = "eg_contest_description_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                ContestDescriptionHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);
        }

        #endregion

        #region ContestDescriptionWithPlaceholders

        internal static unsafe class ContestDescriptionWithPlaceholders
        {
            internal unsafe struct ContestDescriptionWithPlaceholdersType { };

            internal class ContestDescriptionWithPlaceholdersHandle
                : ElectionguardSafeHandle<ContestDescriptionWithPlaceholdersType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = ContestDescriptionWithPlaceholders.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"ContestDescriptionWithPlaceholders Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_contest_description_with_placeholders_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string electoralDistrictId,
                ulong sequenceOrder,
                VoteVariationType voteVariation,
                ulong numberElected,
                [MarshalAs(UnmanagedType.LPStr)] string name,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] selections,
                ulong selectionsSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] placeholders,
                ulong placeholdersSize,
                out ContestDescriptionWithPlaceholdersHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_with_placeholders_new_with_title",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string electoralDistrictId,
                ulong sequenceOrder,
                VoteVariationType voteVariation,
                ulong numberElected,
                ulong votesAllowed,
                [MarshalAs(UnmanagedType.LPStr)] string name,
                InternationalizedText.InternationalizedTextHandle ballotTitle,
                InternationalizedText.InternationalizedTextHandle ballotSubTitle,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] selections,
                ulong selectionsSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] placeholders,
                ulong placeholdersSize,
                out ContestDescriptionWithPlaceholdersHandle handle);

            [DllImport(DllName,
                EntryPoint = "eg_contest_description_with_placeholders_new_with_parties",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string electoralDistrictId,
                ulong sequenceOrder,
                VoteVariationType voteVariation,
                ulong numberElected,
                [MarshalAs(UnmanagedType.LPStr)] string name,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] selections,
                ulong selectionsSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] string[] primaryPartyIds,
                ulong primaryPartyIdsSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] placeholders,
                ulong placeholdersSize,
                out ContestDescriptionWithPlaceholdersHandle handle);

            [DllImport(DllName,
                EntryPoint = "eg_contest_description_with_placeholders_new_with_title_and_parties",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string electoralDistrictId,
                ulong sequenceOrder,
                VoteVariationType voteVariation,
                ulong numberElected,
                ulong votesAllowed,
                [MarshalAs(UnmanagedType.LPStr)] string name,
                InternationalizedText.InternationalizedTextHandle ballotTitle,
                InternationalizedText.InternationalizedTextHandle ballotSubTitle,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] selections,
                ulong selectionsSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] string[] primaryPartyIds,
                ulong primaryPartyIdsSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] placeholders,
                ulong placeholdersSize,
                out ContestDescriptionWithPlaceholdersHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_with_placeholders_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(ContestDescriptionWithPlaceholdersType* handle);

            #region ContestDescription Methods

            // Since the underlying c++ class inherits from ContestDescription
            // these functions call those methods subsituting the 
            // ContestDescriptionWithPlaceholdersHandle opaque pointer type

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                ContestDescriptionWithPlaceholdersHandle handle, out IntPtr objectId);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_electoral_district_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetElectoralDistrictId(
                ContestDescriptionWithPlaceholdersHandle handle, out IntPtr electoralDistrictId);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_sequence_order",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetSequenceOrder(
                ContestDescriptionWithPlaceholdersHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_vote_variation",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern VoteVariationType GetVoteVariationType(
                ContestDescriptionWithPlaceholdersHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_number_elected",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetNumberElected(
                ContestDescriptionWithPlaceholdersHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_votes_allowed",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetVotesAllowed(
                ContestDescriptionWithPlaceholdersHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_name",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetName(
                ContestDescriptionWithPlaceholdersHandle handle, out IntPtr name);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_ballot_title",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetBallotTitle(
                ContestDescriptionWithPlaceholdersHandle handle,
                out InternationalizedText.InternationalizedTextHandle name);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_ballot_subtitle",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetBallotSubTitle(
                ContestDescriptionWithPlaceholdersHandle handle,
                out InternationalizedText.InternationalizedTextHandle name);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_selections_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetSelectionsSize(
                ContestDescriptionWithPlaceholdersHandle handle);

            [DllImport(DllName, EntryPoint = "eg_contest_description_get_selection_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetSelectionAtIndex(
                ContestDescriptionWithPlaceholdersHandle handle,
                ulong index,
                out SelectionDescription.SelectionDescriptionHandle partyId);

            [DllImport(DllName, EntryPoint = "eg_contest_description_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                ContestDescriptionWithPlaceholdersHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);

            #endregion

            [DllImport(DllName,
                EntryPoint = "eg_contest_description_with_placeholders_get_placeholders_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetPlaceholdersSize(
                ContestDescriptionWithPlaceholdersHandle handle);

            [DllImport(DllName,
                EntryPoint = "eg_contest_description_with_placeholders_get_placeholder_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetPlaceholderAtIndex(
                ContestDescriptionWithPlaceholdersHandle handle,
                ulong index,
                out SelectionDescription.SelectionDescriptionHandle partyId);

            [DllImport(DllName,
                EntryPoint = "eg_contest_description_with_placeholders_is_placeholder",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern bool IsPlaceholder(
                ContestDescriptionWithPlaceholdersHandle handle,
                SelectionDescription.SelectionDescriptionHandle selection);

            [DllImport(DllName,
                EntryPoint = "eg_contest_description_with_placeholders_selection_for_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status SelectionForId(
                ContestDescriptionWithPlaceholdersHandle handle,
                [MarshalAs(UnmanagedType.LPStr)] string selectionId,
                SelectionDescription.SelectionDescriptionHandle selection);
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

                    var status = Manifest.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"Manifest Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_election_manifest_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string electionScopeId,
                ElectionType electionType,
                ulong startDate,
                ulong endDate,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] gpUnits,
                ulong gpUnitsSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] parties,
                ulong partiesSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] candidates,
                ulong candidatesSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] contests,
                ulong contestSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] ballotStyles,
                ulong ballotStylesSize,
                out ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_new_with_contact",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string electionScopeId,
                ElectionType electionType,
                ulong startDate,
                ulong endDate,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] gpUnits,
                ulong gpUnitsSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] parties,
                ulong partiesSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] candidates,
                ulong candidatesSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] contests,
                ulong contestSize,
                // TODO ISSUE #212: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] ballotStyles,
                ulong ballotStylesSize,
                InternationalizedText.InternationalizedTextHandle name,
                ContactInformation.ContactInformationHandle contact,
                out ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(ManifestType* handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_election_scope_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetElectionScopeId(
                ManifestHandle handle, out IntPtr election_scope_id);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_type",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ElectionType GetElectionType(
                ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_start_date",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetStartDate(
                ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_end_date",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetEndDate(
                ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_geopolitical_units_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetGeopoliticalUnitsSize(
                ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_geopolitical_unit_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetGeopoliticalUnitAtIndex(
                ManifestHandle handle,
                ulong index,
                out GeopoliticalUnit.GeopoliticalUnitHandle gpUnit);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_parties_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetPartiesSize(
                ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_party_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetPartyAtIndex(
                ManifestHandle handle,
                ulong index,
                out Party.PartyHandle party);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_candidates_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetCandidatesSize(
                ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_candidate_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCandidateAtIndex(
                ManifestHandle handle,
                ulong index,
                out Candidate.CandidateHandle candidate);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_contests_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetContestsSize(
                ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_contest_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetContestAtIndex(
                ManifestHandle handle,
                ulong index,
                out ContestDescription.ContestDescriptionHandle contest);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_ballot_styles_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetBallotStylesSize(
                ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_ballot_style_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetBallotStyleAtIndex(
                ManifestHandle handle,
                ulong index,
                out BallotStyle.BallotStyleHandle ballotStyle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_name",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetName(
                ManifestHandle handle,
                out InternationalizedText.InternationalizedTextHandle name);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_get_contact_info",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetContactInfo(
                ManifestHandle handle,
                out ContactInformation.ContactInformationHandle contactInfo);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHash(
                ManifestHandle handle,
                out ElementModQ.ElementModQHandle crypto_hash);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_is_valid",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern bool IsValid(ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_from_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_from_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromBson(
                byte* data, ulong length, out ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_from_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromMsgPack(
                byte* data, ulong length, out ManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_to_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToJson(
                ManifestHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_to_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToBson(
                ManifestHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_election_manifest_to_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToMsgPack(
                ManifestHandle handle, out IntPtr data, out ulong size);
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

                    var status = InternalManifest.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"InternalManifest Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                Manifest.ManifestHandle manifest,
                out InternalManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(
                InternalManifestType* handle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_get_manifest_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetManifestHash(
                InternalManifestHandle handle,
                out ElementModQ.ElementModQHandle manifest_hash);

            [DllImport(DllName,
                EntryPoint = "eg_internal_manifest_get_geopolitical_units_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetGeopoliticalUnitsSize(
                InternalManifestHandle handle);

            [DllImport(DllName,
                EntryPoint = "eg_internal_manifest_get_geopolitical_unit_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetGeopoliticalUnitAtIndex(
                InternalManifestHandle handle,
                ulong index,
                out GeopoliticalUnit.GeopoliticalUnitHandle gpUnit);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_get_contests_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetContestsSize(
                InternalManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_get_contest_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetContestAtIndex(
                InternalManifestHandle handle,
                ulong index,
                out ContestDescription.ContestDescriptionHandle contest);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_get_ballot_styles_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetBallotStylesSize(
                InternalManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_get_ballot_style_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetBallotStyleAtIndex(
                InternalManifestHandle handle,
                ulong index,
                out BallotStyle.BallotStyleHandle ballotStyle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_from_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out InternalManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_from_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromBson(
                byte* data, ulong length, out InternalManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_from_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromMsgPack(
                byte* data, ulong length, out InternalManifestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_to_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToJson(
                InternalManifestHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_to_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToBson(
                InternalManifestHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_internal_manifest_to_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToMsgPack(
                InternalManifestHandle handle, out IntPtr data, out ulong size);
        }

        #endregion

        #region ContextConfiguration
        internal static unsafe class ContextConfiguration
        {
            internal unsafe struct ContextConfigurationType { };

            internal class ContextConfigurationHandle
                : ElectionguardSafeHandle<ContextConfigurationType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = ContextConfiguration.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"ContextConfiguration Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_election_context_config_get_allowed_overvotes",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetAllowedOverVotes(
                ContextConfigurationHandle handle,
                ref bool allowed_overvotes);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_election_context_config_get_max_ballots",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetMaxBallots(
                ContextConfigurationHandle handle,
                ref UInt64 max_ballots);


            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_config_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(ContextConfigurationType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_config_make",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Make(
                bool allow_overvotes,
                UInt64 max_ballots,
                out ContextConfigurationHandle handle);
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

                    var status = CiphertextElectionContext.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"CiphertextElectionContext Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(CiphertextElectionType* handle);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_election_context_get_configuration",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetConfiguration(
                CiphertextElectionContextHandle handle,
                out ContextConfiguration.ContextConfigurationHandle context_config);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_election_context_get_elgamal_public_key",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetElGamalPublicKey(
                CiphertextElectionContextHandle handle,
                out ElementModP.ElementModPHandle elgamal_public_key);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_election_context_get_commitment_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCommitmentHash(
                CiphertextElectionContextHandle handle,
                out ElementModQ.ElementModQHandle commitment_hash);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_election_context_get_manifest_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetManifestHash(
                CiphertextElectionContextHandle handle,
                out ElementModQ.ElementModQHandle manifest_hash);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_election_context_get_crypto_base_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCryptoBaseHash(
                CiphertextElectionContextHandle handle,
                out ElementModQ.ElementModQHandle crypto_base_hash);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_election_context_get_crypto_extended_base_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCryptoExtendedBaseHash(
                CiphertextElectionContextHandle handle,
                out ElementModQ.ElementModQHandle crypto_extended_base_hash);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_election_context_get_extended_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetExtendedData(
                CiphertextElectionContextHandle handle,
                out LinkedList.LinkedListHandle extended_data);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_make",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Make(
                ulong number_of_guardians,
                ulong quorum,
                ElementModP.ElementModPHandle elgamal_public_key,
                ElementModQ.ElementModQHandle commitment_hash,
                ElementModQ.ElementModQHandle manifest_hash,
                out CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_make_with_configuration",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Make(
                ulong number_of_guardians,
                ulong quorum,
                ElementModP.ElementModPHandle elgamal_public_key,
                ElementModQ.ElementModQHandle commitment_hash,
                ElementModQ.ElementModQHandle manifest_hash,
                ContextConfiguration.ContextConfigurationHandle configuration,
                out CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_make_with_extended_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Make(
                ulong number_of_guardians,
                ulong quorum,
                ElementModP.ElementModPHandle elgamal_public_key,
                ElementModQ.ElementModQHandle commitment_hash,
                ElementModQ.ElementModQHandle manifest_hash,
                LinkedList.LinkedListHandle extended_data,
                out CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_make_with_configuration_and_extended_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Make(
                ulong number_of_guardians,
                ulong quorum,
                ElementModP.ElementModPHandle elgamal_public_key,
                ElementModQ.ElementModQHandle commitment_hash,
                ElementModQ.ElementModQHandle manifest_hash,
                ContextConfiguration.ContextConfigurationHandle configuration,
                LinkedList.LinkedListHandle extended_data,
                out CiphertextElectionContextHandle handle);


            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_make_from_hex",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Make(
                ulong number_of_guardians,
                ulong quorum,
                [MarshalAs(UnmanagedType.LPStr)] string hex_elgamal_public_key,
                [MarshalAs(UnmanagedType.LPStr)] string hex_commitment_hash,
                [MarshalAs(UnmanagedType.LPStr)] string hex_manifest_hash,
                out CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_make_from_hex_with_extended_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Make(
                ulong number_of_guardians,
                ulong quorum,
                [MarshalAs(UnmanagedType.LPStr)] string hex_elgamal_public_key,
                [MarshalAs(UnmanagedType.LPStr)] string hex_commitment_hash,
                [MarshalAs(UnmanagedType.LPStr)] string hex_manifest_hash,
                LinkedList.LinkedListHandle extended_data,
                out CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_from_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_from_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromBson(
                uint* data, ulong length, CiphertextElectionContextHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_to_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToJson(
                CiphertextElectionContextHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_election_context_to_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToBson(
                CiphertextElectionContextHandle handle, out uint* data, out ulong size);
        }

        #endregion

        #region ExtendedData

        internal static unsafe class ExtendedData
        {
            internal unsafe struct ExtendedDataType { };

            internal class ExtendedDataHandle
                : ElectionguardSafeHandle<ExtendedDataType>
            {
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = ExtendedData.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"ExtendedData Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_extended_data_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string value,
                ulong length,
                out ExtendedDataHandle handle);

            [DllImport(DllName, EntryPoint = "eg_extended_data_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(ExtendedDataType* handle);

            [DllImport(DllName, EntryPoint = "eg_extended_data_get_value",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetValue(
                ExtendedDataHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_extended_data_get_length",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetLength(ExtendedDataHandle handle);

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

                    var status = PlaintextBallotSelection.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"PlaintextBallotSelection Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_selection_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                ulong vote,
                bool isPlaceholderSelection,
                out PlaintextBallotSelectionHandle handle);

            [DllImport(DllName,
                EntryPoint = "eg_plaintext_ballot_selection_new_with_extended_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                ulong vote,
                bool isPlaceholderSelection,
                [MarshalAs(UnmanagedType.LPStr)] string extendedData,
                ulong extendedDataLength,
                out PlaintextBallotSelectionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_selection_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(PlaintextBallotSelectionType* handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_selection_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                PlaintextBallotSelectionHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_selection_get_vote",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetVote(
                PlaintextBallotSelectionHandle handle);

            [DllImport(DllName,
                EntryPoint = "eg_plaintext_ballot_selection_get_is_placeholder",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern bool GetIsPlaceholder(
                PlaintextBallotSelectionHandle handle);

            [DllImport(DllName,
                EntryPoint = "eg_plaintext_ballot_selection_get_extended_data",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetExtendedData(
                PlaintextBallotSelectionHandle handle, out ExtendedData.ExtendedDataHandle extended_data);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_selection_is_valid",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
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

                    var status = CiphertextBallotSelection.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"CiphertextBallotSelection Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(CiphertextBallotSelectionType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_selection_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                CiphertextBallotSelectionHandle handle, out IntPtr object_id);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_selection_get_sequence_order",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetSequenceOrder(
                CiphertextBallotSelectionHandle handle);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_selection_get_description_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetDescriptionHash(
                CiphertextBallotSelectionHandle handle,
                out ElementModQ.ElementModQHandle description_hash);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_selection_get_is_placeholder",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern bool GetIsPlaceholder(
                CiphertextBallotSelectionHandle handle);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_selection_get_ciphertext",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCiphertext(
                CiphertextBallotSelectionHandle handle,
                out ElGamalCiphertext.ElGamalCiphertextHandle ciphertext);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_selection_get_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCryptoHash(
                CiphertextBallotSelectionHandle handle,
                out ElementModQ.ElementModQHandle cryptoHash);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_selection_get_nonce",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetNonce(
                CiphertextBallotSelectionHandle handle,
                out ElementModQ.ElementModQHandle nonce);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_selection_get_proof",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetProof(
                CiphertextBallotSelectionHandle handle,
                out DisjunctiveChaumPedersenProof.DisjunctiveChaumPedersenProofHandle nonce);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_selection_crypto_hash_with",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHashWith(
                CiphertextBallotSelectionHandle handle,
                ElementModQ.ElementModQHandle encryption_seed,
                out ElementModQ.ElementModQHandle crypto_hash);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_selection_is_valid_encryption",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
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

                    var status = PlaintextBallotContest.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"PlaintextBallotContest Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_contest_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                // TODO: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] selections,
                ulong selectionsSize,
                out PlaintextBallotContestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_contest_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(PlaintextBallotContestType* handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_contest_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                PlaintextBallotContestHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_contest_get_selections_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetSelectionsSize(
                PlaintextBallotContestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_contest_get_selection_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetSelectionAtIndex(
                PlaintextBallotContestHandle handle,
                ulong index,
                out PlaintextBallotSelection.PlaintextBallotSelectionHandle selection);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_contest_is_valid",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern bool IsValid(
                PlaintextBallotContestHandle handle,
                [MarshalAs(UnmanagedType.LPStr)] string expected_object_id,
                ulong expected_num_selections,
                ulong expected_num_elected,
                ulong votes_allowed);
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

                    var status = CiphertextBallotContest.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"CiphertextBallotContest Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(CiphertextBallotContestType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                CiphertextBallotContestHandle handle, out IntPtr object_id);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_contest_get_sequence_order",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetSequenceOrder(CiphertextBallotContestHandle handle);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_contest_get_description_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetDescriptionHash(
                CiphertextBallotContestHandle handle,
                out ElementModQ.ElementModQHandle description_hash);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_contest_get_selections_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetSelectionsSize(
                CiphertextBallotContestHandle handle);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_contest_get_selection_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetSelectionAtIndex(
                CiphertextBallotContestHandle handle,
                ulong index,
                out CiphertextBallotSelection.CiphertextBallotSelectionHandle selection);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_contest_get_nonce",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetNonce(
                CiphertextBallotContestHandle handle,
                out ElementModQ.ElementModQHandle nonce);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_contest_get_ciphertext_accumulation",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCiphertextAccumulation(
                CiphertextBallotContestHandle handle,
                out ElGamalCiphertext.ElGamalCiphertextHandle nonce);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_contest_get_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCryptoHash(
                CiphertextBallotContestHandle handle,
                out ElementModQ.ElementModQHandle cryptoHash);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_contest_get_proof",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetProof(
                CiphertextBallotContestHandle handle,
                out ConstantChaumPedersenProof.ConstantChaumPedersenProofHandle nonce);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_contest_crypto_hash_with",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHashWith(
                CiphertextBallotContestHandle handle,
                ElementModQ.ElementModQHandle encryption_seed,
                out ElementModQ.ElementModQHandle crypto_hash);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_contest_aggregate_nonce",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status AggregateNonce(
                CiphertextBallotContestHandle handle,
                out ElementModQ.ElementModQHandle aggregate_nonce);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_contest_elgamal_accumulate",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ElGamalAccumulate(
                CiphertextBallotContestHandle handle,
                out ElGamalCiphertext.ElGamalCiphertextHandle ciphertext_accumulation);

            [DllImport(DllName,
                EntryPoint = "eg_ciphertext_ballot_selection_is_valid_encryption",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
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

                    var status = PlaintextBallot.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"PlaintextBallot Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName,
                EntryPoint = "eg_plaintext_ballot_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                [MarshalAs(UnmanagedType.LPStr)] string objectId,
                [MarshalAs(UnmanagedType.LPStr)] string styleId,
                // TODO: type safety
                [MarshalAs(UnmanagedType.LPArray)] IntPtr[] contests,
                ulong contestsSize,
                out PlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(PlaintextBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                PlaintextBallotHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_get_style_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetStyleId(
                PlaintextBallotHandle handle, out IntPtr style_id);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_get_contests_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetContestsSize(PlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_get_contest_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetContestAtIndex(
                PlaintextBallotHandle handle,
                ulong index,
                out PlaintextBallotContest.PlaintextBallotContestHandle contest);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_from_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out PlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_from_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromBson(
                byte* data, ulong length, out PlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_from_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromMsgPack(
                byte* data, ulong length, out PlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_to_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToJson(
                PlaintextBallotHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_to_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToBson(
                PlaintextBallotHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_plaintext_ballot_to_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToMsgPack(
                PlaintextBallotHandle handle, out IntPtr data, out ulong size);

            // TODO: implement MsgPackFree
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

                    var status = CompactPlaintextBallot.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"CompactPlaintextBallot Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_compact_plaintext_ballot_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(CompactPlaintextBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_compact_plaintext_ballot_from_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromMsgPack(
                byte* data, ulong size, out CompactPlaintextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_compact_plaintext_ballot_to_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToMsgPack(
                CompactPlaintextBallotHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_compact_plaintext_ballot_msgpack_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
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

                    var status = CiphertextBallot.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"CiphertextBallot Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(CiphertextBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                CiphertextBallotHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_style_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetStyleId(
                CiphertextBallotHandle handle, out IntPtr style_id);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_manifest_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetManifestHash(
                CiphertextBallotHandle handle,
                out ElementModQ.ElementModQHandle manifest_hash_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_ballot_code_seed",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetBallotCodeSeed(
                CiphertextBallotHandle handle,
                out ElementModQ.ElementModQHandle ballot_code_seed_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_contests_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetContestsSize(CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_contest_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetContestAtIndex(
                CiphertextBallotHandle handle,
                ulong index,
                out CiphertextBallotContest.CiphertextBallotContestHandle contest_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_ballot_code",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetBallotCode(
                CiphertextBallotHandle handle,
                out ElementModQ.ElementModQHandle ballot_code_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_timestamp",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetTimestamp(
                CiphertextBallotHandle handle,
                out ulong timestamp);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_nonce",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetNonce(
                CiphertextBallotHandle handle,
                out ElementModQ.ElementModQHandle nonce_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCryptoHash(
                CiphertextBallotHandle handle,
                out ElementModQ.ElementModQHandle hash_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_crypto_hash_with",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CryptoHashWith(
                CiphertextBallotHandle handle,
                ElementModQ.ElementModQHandle manifest_hash,
                out ElementModQ.ElementModQHandle crypto_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_is_valid_encryption",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern bool IsValidEncryption(
                CiphertextBallotHandle handle,
                ElementModQ.ElementModQHandle manifest_hash,
                ElementModP.ElementModPHandle public_key,
                ElementModQ.ElementModQHandle crypto_extended_base_hash);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_from_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_from_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromBson(
                byte* data, ulong length, out CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_from_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromMsgPack(
                byte* data, ulong length, out CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToJson(
                CiphertextBallotHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_json_with_nonces",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToJsonWithNonces(
                CiphertextBallotHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToBson(
                CiphertextBallotHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_bson_with_nonces",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToBsonWithNonces(
                CiphertextBallotHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToMsgPack(
                CiphertextBallotHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_to_msgpack_with_nonces",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToMsgPackWithNonces(
                CiphertextBallotHandle handle, out IntPtr data, out ulong size);
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

                    var status = CompactCiphertextBallot.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"CompactCiphertextBallot Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_compact_ciphertext_ballot_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(CompactCiphertextBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_compact_ciphertext_ballot_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                CompactCiphertextBallotHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_compact_ciphertext_ballot_from_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromMsgPack(
                byte* data, ulong size, out CompactCiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_compact_ciphertext_ballot_to_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToMsgPack(
                CompactCiphertextBallotHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_compact_ciphertext_ballot_msgpack_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status MsgPackFree(IntPtr data);

        }

        #endregion

        #region SubmittedBallot

        internal static unsafe class SubmittedBallot
        {
            internal unsafe struct SubmittedBallotType { };

            internal class SubmittedBallotHandle
                : ElectionguardSafeHandle<SubmittedBallotType>
            {
                [ReliabilityContract(Consistency.WillNotCorruptState, Cer.MayFail)]
                protected override bool Free()
                {
                    if (IsClosed) return true;

                    var status = SubmittedBallot.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"SubmittedBallot Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_submitted_ballot_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(SubmittedBallotType* handle);

            [DllImport(DllName, EntryPoint = "eg_submitted_ballot_get_state",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern BallotBoxState GetState(SubmittedBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_submitted_ballot_from",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status From(
                CiphertextBallot.CiphertextBallotHandle ciphertext,
                BallotBoxState state,
                out SubmittedBallotHandle handle);

            #region CiphertextBallot Methods

            // Since the underlying c++ class inherits from CiphertextBallot
            // these functions call those methods subsituting the SubmittedBallot opaque pointer type

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_object_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetObjectId(
                SubmittedBallotHandle handle, out IntPtr object_id);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_style_id",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetStyleId(
               SubmittedBallotHandle handle, out IntPtr style_id);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_manifest_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetManifestHash(
                SubmittedBallotHandle handle,
                out ElementModQ.ElementModQHandle manifest_hash_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_ballot_code_seed",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetBallotCodeSeed(
                SubmittedBallotHandle handle,
                out ElementModQ.ElementModQHandle ballot_code_seed_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_contests_size",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern ulong GetContestsSize(SubmittedBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_contest_at_index",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetContestAtIndex(
                SubmittedBallotHandle handle,
                ulong index,
                out CiphertextBallotContest.CiphertextBallotContestHandle contest_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_ballot_code",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetBallotCode(
                SubmittedBallotHandle handle,
                out ElementModQ.ElementModQHandle ballot_code_ref);

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_timestamp",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetTimestamp(
                SubmittedBallotHandle handle,
                out ulong timestamp);

            // GetNonce is not provided

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_get_crypto_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status GetCryptoHash(
                SubmittedBallotHandle handle,
                out ElementModQ.ElementModQHandle hash_ref);

            // CryptoHashWith is not provided

            [DllImport(DllName, EntryPoint = "eg_ciphertext_ballot_is_valid_encryption",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern bool IsValidEncryption(
                SubmittedBallotHandle handle,
                ElementModQ.ElementModQHandle manifest_hash,
                ElementModP.ElementModPHandle public_key,
                ElementModQ.ElementModQHandle crypto_extended_base_hash);

            #endregion

            [DllImport(DllName, EntryPoint = "eg_submitted_ballot_from_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out SubmittedBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_submitted_ballot_from_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromBson(
                byte* data, ulong length, out SubmittedBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_submitted_ballot_from_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromMsgPack(
                byte* data, ulong length, out SubmittedBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_submitted_ballot_to_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToJson(
                SubmittedBallotHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_submitted_ballot_to_bson",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToBson(
                SubmittedBallotHandle handle, out IntPtr data, out ulong size);

            [DllImport(DllName, EntryPoint = "eg_submitted_ballot_to_msgpack",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToMsgPack(
                SubmittedBallotHandle handle, out IntPtr data, out ulong size);

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

                    var status = EncryptionDevice.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"EncryptionDevice Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_encryption_device_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                ulong deviceUuid,
                ulong sessionUuid,
                ulong launchCode,
                [MarshalAs(UnmanagedType.LPStr)] string location,
                out EncryptionDeviceHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_device_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
                
            internal static extern Status Free(EncryptionDeviceType* handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_device_get_hash",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]

            internal static extern Status GetHash(
                EncryptionDeviceHandle handle,
                out ElementModQ.ElementModQHandle device_hash);

            //hooking up jsonular calls
            [DllImport(DllName, EntryPoint = "eg_encryption_device_from_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status FromJson(
                [MarshalAs(UnmanagedType.LPStr)] string data,
                out EncryptionDeviceHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_device_to_json",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status ToJson(
                EncryptionDeviceHandle handle, out IntPtr data, out ulong size);


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

                    var status = EncryptionMediator.Free(TypedPtr);
                    if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                    {
                        throw new ElectionGuardException($"EncryptionMediator Error Free: {status}", status);
                    }
                    return true;
                }
            }

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_new",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status New(
                InternalManifest.InternalManifestHandle manifest,
                CiphertextElectionContext.CiphertextElectionContextHandle context,
                EncryptionDevice.EncryptionDeviceHandle device,
                out EncryptionMediatorHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_free",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Free(EncryptionMediatorType* handle);

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_compact_encrypt_ballot",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CompactEncrypt(
                EncryptionMediatorHandle handle,
                PlaintextBallot.PlaintextBallotHandle plainutext,
                out CompactCiphertextBallot.CompactCiphertextBallotHandle ciphertext);

            [DllImport(DllName,
                EntryPoint = "eg_encryption_mediator_compact_encrypt_ballot_verify_proofs",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CompactEncryptAndVerify(
                EncryptionMediatorHandle handle,
                PlaintextBallot.PlaintextBallotHandle plainutext,
                out CompactCiphertextBallot.CompactCiphertextBallotHandle ciphertext);

            [DllImport(DllName, EntryPoint = "eg_encryption_mediator_encrypt_ballot",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Encrypt(
                EncryptionMediatorHandle handle,
                PlaintextBallot.PlaintextBallotHandle plainutext,
                out CiphertextBallot.CiphertextBallotHandle ciphertext);

            [DllImport(DllName,
                EntryPoint = "eg_encryption_mediator_encrypt_ballot_verify_proofs",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status EncryptAndVerify(
                EncryptionMediatorHandle handle,
                PlaintextBallot.PlaintextBallotHandle plainutext,
                out CiphertextBallot.CiphertextBallotHandle ciphertext);
        }


        internal static unsafe class Encrypt
        {
            [DllImport(DllName, EntryPoint = "eg_encrypt_selection",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Selection(
                PlaintextBallotSelection.PlaintextBallotSelectionHandle plaintext,
                SelectionDescription.SelectionDescriptionHandle description,
                ElementModP.ElementModPHandle publicKey,
                ElementModQ.ElementModQHandle crypto_extended_base_hash,
                ElementModQ.ElementModQHandle nonceSeed,
                bool shouldVerifyProofs,
                out CiphertextBallotSelection.CiphertextBallotSelectionHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encrypt_contest",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Contest(
                PlaintextBallotContest.PlaintextBallotContestHandle plaintext,
                ContestDescription.ContestDescriptionHandle description,
                ElementModP.ElementModPHandle publicKey,
                ElementModQ.ElementModQHandle crypto_extended_base_hash,
                ElementModQ.ElementModQHandle nonceSeed,
                bool shouldVerifyProofs,
                out CiphertextBallotContest.CiphertextBallotContestHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encrypt_ballot",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Ballot(
                PlaintextBallot.PlaintextBallotHandle plaintext,
                InternalManifest.InternalManifestHandle internal_manifest,
                CiphertextElectionContext.CiphertextElectionContextHandle context,
                ElementModQ.ElementModQHandle ballot_code_seed,
                bool shouldVerifyProofs,
                out CiphertextBallot.CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encrypt_ballot_with_nonce",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Ballot(
                PlaintextBallot.PlaintextBallotHandle plaintext,
                InternalManifest.InternalManifestHandle internal_manifest,
                CiphertextElectionContext.CiphertextElectionContextHandle context,
                ElementModQ.ElementModQHandle ballot_code_seed,
                ElementModQ.ElementModQHandle nonce,
                bool shouldVerifyProofs,
                out CiphertextBallot.CiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encrypt_compact_ballot",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CompactBallot(
                PlaintextBallot.PlaintextBallotHandle plaintext,
                InternalManifest.InternalManifestHandle internal_manifest,
                CiphertextElectionContext.CiphertextElectionContextHandle context,
                ElementModQ.ElementModQHandle ballot_code_seed,
                bool shouldVerifyProofs,
                out CompactCiphertextBallot.CompactCiphertextBallotHandle handle);

            [DllImport(DllName, EntryPoint = "eg_encrypt_compact_ballot_with_nonce",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status CompactBallot(
                PlaintextBallot.PlaintextBallotHandle plaintext,
                InternalManifest.InternalManifestHandle internal_manifest,
                CiphertextElectionContext.CiphertextElectionContextHandle context,
                ElementModQ.ElementModQHandle ballot_code_seed,
                ElementModQ.ElementModQHandle nonce,
                bool shouldVerifyProofs,
                out CompactCiphertextBallot.CompactCiphertextBallotHandle handle);
        }

        #region Precompute
        internal static unsafe class PrecomputeBuffers
        {
            [DllImport(DllName, EntryPoint = "eg_precompute_init",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Init(int max_buffers);

            [DllImport(DllName, EntryPoint = "eg_precompute_populate",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Populate(ElementModP.ElementModPHandle publicKey);

            [DllImport(DllName, EntryPoint = "eg_precompute_stop",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Stop();

            [DllImport(DllName, EntryPoint = "eg_precompute_status",
                CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
            internal static extern Status Status(out int count, out int queue_size);
        }

        #endregion

    }

    #endregion

}
