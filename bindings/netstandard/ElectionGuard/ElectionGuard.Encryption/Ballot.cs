using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NativeCiphertextBallot = NativeInterface.CiphertextBallot.CiphertextBallotHandle;
    using NativeCiphertextBallotContest = NativeInterface.CiphertextBallotContest.CiphertextBallotContestHandle;
    using NativeCiphertextBallotSelection = NativeInterface.CiphertextBallotSelection.CiphertextBallotSelectionHandle;
    using NativeCompactCiphertextBallot = NativeInterface.CompactCiphertextBallot.CompactCiphertextBallotHandle;
    using NativeCompactPlaintextBallot = NativeInterface.CompactPlaintextBallot.CompactPlaintextBallotHandle;
    using NativeConstantChaumPedersenProof = NativeInterface.ConstantChaumPedersenProof.ConstantChaumPedersenProofHandle;
    using NativeDisjunctiveChaumPedersenProof = NativeInterface.DisjunctiveChaumPedersenProof.DisjunctiveChaumPedersenProofHandle;
    // Declare native types for convenience
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQHandle;
    using NativeElGamalCiphertext = NativeInterface.ElGamalCiphertext.ElGamalCiphertextHandle;
    using NativeExtendedData = NativeInterface.ExtendedData.ExtendedDataHandle;
    using NativePlaintextBallot = NativeInterface.PlaintextBallot.PlaintextBallotHandle;
    using NativePlaintextBallotContest = NativeInterface.PlaintextBallotContest.PlaintextBallotContestHandle;
    using NativePlaintextBallotSelection = NativeInterface.PlaintextBallotSelection.PlaintextBallotSelectionHandle;
    using NativeSubmittedBallot = NativeInterface.SubmittedBallot.SubmittedBallotHandle;

    /// <summary>
    /// Indicates the binary format
    /// </summary>
    public enum BinarySerializationEncoding
    {
        /// <summary>
        /// BSON encoding
        /// </summary>
        BSON,

        /// <summary>
        /// MessagePack encoding
        /// </summary>
        MsgPack
    }

    #region ExtendedData

    /// <summary>
    /// ExtendedData represents any arbitrary data expressible as a string with a length.
    ///
    /// This class is used primarily as a field on a selection to indicate a write-in candidate text value
    /// </summary>
    public class ExtendedData : DisposableBase
    {
        /// <Summary>
        /// The string value
        /// </Summary>
        public unsafe string Value
        {
            get
            {
                var status = NativeInterface.ExtendedData.GetValue(
                    Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// The length of the string value
        /// </Summary>
        public unsafe ulong Length
        {
            get
            {
                return NativeInterface.ExtendedData.GetLength(Handle);
            }
        }

        internal unsafe NativeExtendedData Handle;

        unsafe internal ExtendedData(NativeExtendedData handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Creates an ExtendedData object
        /// </summary>
        /// <param name="value">the string value</param>
        /// <param name="length">the length of the string</param>
        public unsafe ExtendedData(string value, ulong length)
        {
            var status = NativeInterface.ExtendedData.New(
                value, length, out Handle);
            status.ThrowIfError();
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

    #endregion

    #region PlaintextBallotSelection

    /// <summary>
    /// A BallotSelection represents an individual selection on a ballot.
    ///
    /// This class accepts a `vote` integer field which has no constraints
    /// in the ElectionGuard Data Specification, but is constrained logically
    /// in the application to resolve to `True` or `False`aka only 0 and 1 is
    /// supported for now.
    ///
    /// This class can also be designated as `isPlaceholderSelection` which has no
    /// context to the data specification but is useful for running validity checks internally
    ///
    /// an `extendedData` field exists to support any arbitrary data to be associated
    /// with the selection.  In practice, this field is the cleartext representation
    /// of a write-in candidate value.  In the current implementation these values are
    /// discarded when encrypting.
    /// </summary>
    public partial class PlaintextBallotSelection : DisposableBase
    {
        internal unsafe NativePlaintextBallotSelection Handle;

        unsafe internal PlaintextBallotSelection(NativePlaintextBallotSelection handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a PlaintextBallotSelection
        /// </summary>
        /// <param name="objectId"></param>
        /// <param name="vote"></param>
        /// <param name="isPlaceholder"></param>
        public unsafe PlaintextBallotSelection(
            string objectId, ulong vote, bool isPlaceholder = false)
        {
            var status = NativeInterface.PlaintextBallotSelection.New(
                objectId, vote, isPlaceholder, out Handle);
            status.ThrowIfError();
        }

        /// <summary>
        /// Create a PlaintextBallotSelection
        /// </summary>
        /// <param name="objectId"></param>
        /// <param name="vote"></param>
        /// <param name="isPlaceholder"></param>
        /// <param name="extendedData"></param>
        public unsafe PlaintextBallotSelection(
            string objectId, ulong vote, bool isPlaceholder, string extendedData)
        {
            var status = NativeInterface.PlaintextBallotSelection.New(
                objectId, vote, isPlaceholder,
                extendedData, (ulong)extendedData.Length, out Handle);
            status.ThrowIfError();
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

    #endregion

    #region CiphertextBallotSelection

    /// <summary>
    /// A CiphertextBallotSelection represents an individual encrypted selection on a ballot.
    ///
    /// This class accepts a `description_hash` and a `ciphertext` as required parameters
    /// in its constructor.
    ///
    /// When a selection is encrypted, the `description_hash` and `ciphertext` required fields must
    /// be populated at construction however the `nonce` is also usually provided by convention.
    ///
    /// After construction, the `crypto_hash` field is populated automatically in the `__post_init__` cycle
    ///
    /// A consumer of this object has the option to discard the `nonce` and/or discard the `proof`,
    /// or keep both values.
    ///
    /// By discarding the `nonce`, the encrypted representation and `proof`
    /// can only be regenerated if the nonce was derived from the ballot's seed nonce.  If the nonce
    /// used for this selection is truly random, and it is discarded, then the proofs cannot be regenerated.
    ///
    /// By keeping the `nonce`, or deriving the selection nonce from the ballot nonce, an external system can
    /// regenerate the proofs on demand.  This is useful for storage or memory constrained systems.
    ///
    /// By keeping the `proof` the nonce is not required fotor verify the encrypted selection.
    /// </summary>
    public class CiphertextBallotSelection : DisposableBase
    {
        /// <Summary>
        /// Get the objectId of the selection, which is the unique id for
        /// the selection in a specific contest described in the election manifest.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.CiphertextBallotSelection.GetObjectId(
                    Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Get the sequence order of the selection
        /// </Summary>
        public unsafe ulong SequenceOrder
        {
            get
            {
                return NativeInterface.CiphertextBallotSelection.GetSequenceOrder(Handle);
            }
        }

        /// <summary>
        /// The hash of the string representation of the Selection Description from the election manifest
        /// </summary>
        public unsafe ElementModQ DescriptionHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallotSelection.GetDescriptionHash(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// Determines if this is a placeholder selection
        /// </summary>
        public unsafe bool IsPlaceholder
        {
            get
            {
                return NativeInterface.CiphertextBallotSelection.GetIsPlaceholder(Handle);
            }
        }

        /// <summary>
        /// The encrypted representation of the vote field
        /// </summary>
        public unsafe ElGamalCiphertext Ciphertext
        {
            get
            {
                var status = NativeInterface.CiphertextBallotSelection.GetCiphertext(
                    Handle, out NativeElGamalCiphertext value);
                status.ThrowIfError();
                return new ElGamalCiphertext(value);
            }
        }

        /// <summary>
        /// The hash of the encrypted values
        /// </summary>
        public unsafe ElementModQ CryptoHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallotSelection.GetCryptoHash(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// The nonce used to generate the encryption. Sensitive &amp; should be treated as a secret
        /// </summary>
        public unsafe ElementModQ Nonce
        {
            get
            {
                var status = NativeInterface.CiphertextBallotSelection.GetNonce(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// The proof that demonstrates the selection is an encryption of 0 or 1,
        /// and was encrypted using the `nonce`
        /// </summary>
        public unsafe DisjunctiveChaumPedersenProof Proof
        {
            get
            {
                var status = NativeInterface.CiphertextBallotSelection.GetProof(
                    Handle, out NativeDisjunctiveChaumPedersenProof value);
                status.ThrowIfError();
                return new DisjunctiveChaumPedersenProof(value);
            }
        }

        internal unsafe NativeCiphertextBallotSelection Handle;

        unsafe internal CiphertextBallotSelection(NativeCiphertextBallotSelection handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Given an encrypted BallotSelection, generates a hash, suitable for rolling up
        /// into a hash / tracking code for an entire ballot. Of note, this particular hash examines
        /// the `encryptionSeed` and `message`, but not the proof.
        /// This is deliberate, allowing for the possibility of ElectionGuard variants running on
        /// much more limited hardware, wherein the Disjunctive Chaum-Pedersen proofs might be computed
        /// later on.
        ///
        /// In most cases the encryption_seed should match the `description_hash`
        /// </summary>
        public unsafe ElementModQ CryptoHashWith(ElementModQ encryptionSeed)
        {
            var status = NativeInterface.CiphertextBallotSelection.CryptoHashWith(
                    Handle, encryptionSeed.Handle, out NativeElementModQ cryptoHash);
            status.ThrowIfError();
            return new ElementModQ(cryptoHash);
        }

        /// <summary>
        /// Given an encrypted BallotSelection, validates the encryption state against a specific seed hash and public key.
        /// Calling this function expects that the object is in a well-formed encrypted state
        /// with the elgamal encrypted `message` field populated along with
        /// the DisjunctiveChaumPedersenProof`proof` populated.
        /// the ElementModQ `description_hash` and the ElementModQ `crypto_hash` are also checked.
        ///
        /// <param name="encryptionSeed">The hash of the SelectionDescription, or
        ///                         whatever `ElementModQ` was used to populate the `description_hash` field. </param>
        /// <param name="elGamalPublicKey">The election public key</param>
        /// <param name="cryptoExtendedBaseHash">The extended base hash of the election</param>
        /// </summary>
        public unsafe bool IsValidEncryption(
            ElementModQ encryptionSeed,
            ElementModP elGamalPublicKey,
            ElementModQ cryptoExtendedBaseHash)
        {
            return NativeInterface.CiphertextBallotSelection.IsValidEncryption(
                Handle, encryptionSeed.Handle,
                elGamalPublicKey.Handle, cryptoExtendedBaseHash.Handle);
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

    #endregion

    #region PlaintextBallotContest

    /// <summary>
    /// A PlaintextBallotContest represents the selections made by a voter for a specific ContestDescription
    ///
    /// This class can be either a partial or a complete representation of a contest dataset.  Specifically,
    /// a partial representation must include at a minimum the "affirmative" selections of a contest.
    /// A complete representation of a ballot must include both affirmative and negative selections of
    /// the contest, AND the placeholder selections necessary to satisfy the ConstantChaumPedersen proof
    /// in the CiphertextBallotContest.
    ///
    /// Typically partial contests may be passed into Electionguard for memory constrained systems,
    /// while complete contests are passed into ElectionGuard when running encryption on an existing dataset.
    /// </summary>
    public partial class PlaintextBallotContest : DisposableBase
    {
        internal unsafe NativePlaintextBallotContest Handle;

        unsafe internal PlaintextBallotContest(
            NativePlaintextBallotContest handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a PlaintextBallotContest
        /// </summary>
        /// <param name="objectId"></param>
        /// <param name="selections"></param>
        public unsafe PlaintextBallotContest(string objectId, PlaintextBallotSelection[] selections)
        {
            IntPtr[] selectionPointers = new IntPtr[selections.Length];
            for (var i = 0; i < selections.Length; i++)
            {
                selectionPointers[i] = selections[i].Handle.Ptr;
                selections[i].Dispose();
            }

            var status = NativeInterface.PlaintextBallotContest.New(
                objectId, selectionPointers, (ulong)selectionPointers.LongLength, out Handle);
            status.ThrowIfError();
        }

        /// <Summary>
        /// Get a selection at a specific index.
        /// </Summary>
        public unsafe PlaintextBallotSelection GetSelectionAt(ulong index)
        {
            var status = NativeInterface.PlaintextBallotContest.GetSelectionAtIndex(
                Handle, index, out NativePlaintextBallotSelection value);
            status.ThrowIfError();
            return new PlaintextBallotSelection(value);
        }

        /// <Summary>
        /// Given a PlaintextBallotContest returns true if the state is representative of the expected values.
        ///
        /// Note: because this class supports partial representations, undervotes are considered a valid state.
        /// </Summary>
        public unsafe bool IsValid(
            string expectedObjectId,
            ulong expectedNumSelections,
            ulong expectedNumElected,
            ulong votesAllowed = 0)
        {
            return NativeInterface.PlaintextBallotContest.IsValid(
                Handle,
                expectedObjectId,
                expectedNumSelections,
                expectedNumElected,
                votesAllowed);
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

    #endregion

    #region CiphertextBallotContest

    /// <summary>
    /// A CiphertextBallotContest represents the selections made by a voter for a specific ContestDescription
    ///
    /// CiphertextBallotContest can only be a complete representation of a contest dataset.  While
    /// PlaintextBallotContest supports a partial representation, a CiphertextBallotContest includes all data
    /// necessary for a verifier to verify the contest.  Specifically, it includes both explicit affirmative
    /// and negative selections of the contest, as well as the placeholder selections that satisfy
    /// the ConstantChaumPedersen proof.
    ///
    /// Similar to `CiphertextBallotSelection` the consuming application can choose to discard or keep both
    /// the `nonce` and the `proof` in some circumstances.  For deterministic nonce's derived from the
    /// seed nonce, both values can be regenerated.  If the `nonce` for this contest is completely random,
    /// then it is required in order to regenerate the proof.
    /// </summary>
    public class CiphertextBallotContest : DisposableBase
    {
        /// <Summary>
        /// Get the objectId of the contest, which is the unique id for
        /// the contest in a specific ballot style described in the election manifest.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.CiphertextBallotContest.GetObjectId(
                    Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Get the sequence order of the contest
        /// </Summary>
        public unsafe ulong SequenceOrder
        {
            get
            {
                return NativeInterface.CiphertextBallotContest.GetSequenceOrder(Handle);
            }
        }

        /// <summary>
        /// The hash of the string representation of the Contest Description from the election manifest
        /// </summary>
        public unsafe ElementModQ DescriptionHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallotContest.GetDescriptionHash(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// Get the Size of the selections collection
        /// </summary>
        public unsafe ulong SelectionsSize
        {
            get
            {
                var size = NativeInterface.CiphertextBallotContest.GetSelectionsSize(Handle);
                return size;
            }
        }

        /// <summary>
        /// Hash of the encrypted values
        /// </summary>
        public unsafe ElementModQ CryptoHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallotContest.GetCryptoHash(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// The nonce used to generate the encryption. Sensitive &amp; should be treated as a secret
        /// </summary>
        public unsafe ElementModQ Nonce
        {
            get
            {
                var status = NativeInterface.CiphertextBallotContest.GetNonce(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// The proof demonstrates the sum of the selections does not exceed the maximum
        /// available selections for the contest, and that the proof was generated with the nonce
        /// </summary>
        public unsafe ConstantChaumPedersenProof Proof
        {
            get
            {
                var status = NativeInterface.CiphertextBallotContest.GetProof(
                    Handle, out NativeConstantChaumPedersenProof value);
                status.ThrowIfError();
                return new ConstantChaumPedersenProof(value);
            }
        }

        internal unsafe NativeCiphertextBallotContest Handle;

        unsafe internal CiphertextBallotContest(NativeCiphertextBallotContest handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Get a selection at a specific index.
        /// </summary>
        public unsafe CiphertextBallotSelection GetSelectionAt(ulong index)
        {
            var status = NativeInterface.CiphertextBallotContest.GetSelectionAtIndex(
                Handle, index, out NativeCiphertextBallotSelection value);
            status.ThrowIfError();
            return new CiphertextBallotSelection(value);
        }

        /// <summary>
        /// Given an encrypted BallotContest, generates a hash, suitable for rolling up
        /// into a hash / tracking code for an entire ballot. Of note, this particular hash examines
        /// the `encryptionSeed` and `message`, but not the proof.
        /// This is deliberate, allowing for the possibility of ElectionGuard variants running on
        /// much more limited hardware, wherein the Constant Chaum-Pedersen proofs might be computed
        /// later on.
        ///
        /// In most cases the encryption_seed should match the `description_hash`
        /// </summary>
        public unsafe ElementModQ CryptoHashWith(ElementModQ encryptionSeed)
        {
            var status = NativeInterface.CiphertextBallotContest.CryptoHashWith(
                    Handle, encryptionSeed.Handle, out NativeElementModQ value);
            status.ThrowIfError();
            return new ElementModQ(value);
        }

        /// <summary>
        /// An aggregate nonce for the contest composed of the nonces of the selections.
        /// Used when constructing the proof of selection limit
        /// </summary>
        public unsafe ElementModQ AggregateNonce()
        {
            var status = NativeInterface.CiphertextBallotContest.AggregateNonce(
                    Handle, out NativeElementModQ value);
            status.ThrowIfError();
            return new ElementModQ(value);
        }

        /// <summary>
        /// Add the individual ballot_selections `message` fields together, suitable for use
        /// when constructing the proof of selection limit.
        /// </summary>
        public unsafe ElGamalCiphertext ElGamalAccumulate()
        {
            var status = NativeInterface.CiphertextBallotContest.ElGamalAccumulate(
                    Handle, out NativeElGamalCiphertext value);
            status.ThrowIfError();
            return new ElGamalCiphertext(value);
        }

        /// <summary>
        /// Given an encrypted BallotContest, validates the encryption state against
        /// a specific encryption seed and public key
        /// by verifying the accumulated sum of selections match the proof.
        /// Calling this function expects that the object is in a well-formed encrypted state
        /// with the `ballot_selections` populated with valid encrypted ballot selections,
        /// the ElementModQ `description_hash`, the ElementModQ `crypto_hash`,
        /// and the ConstantChaumPedersenProof all populated.
        /// Specifically, the seed hash in this context is the hash of the ContestDescription,
        /// or whatever `ElementModQ` was used to populate the `description_hash` field.
        /// </summary>
        public unsafe bool IsValidEncryption(
            ElementModQ encryptionSeed,
            ElementModP elGamalPublicKey,
            ElementModQ cryptoExtendedBaseHash)
        {
            return NativeInterface.CiphertextBallotContest.IsValidEncryption(
                Handle, encryptionSeed.Handle, elGamalPublicKey.Handle,
                cryptoExtendedBaseHash.Handle);
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

    #endregion

    #region PlaintextBallot

    /// <summary>
    /// A PlaintextBallot represents a voters selections for a given ballot and ballot style.
    ///
    /// This class can be either a partial or a complete representation of the expected values of a ballot dataset.
    /// Specifically, a partial representation must include at a minimum the "affirmative" selections
    /// of every contest where a selection is made.  A partial representation may exclude contests for which
    /// no selection is made.
    ///
    /// A complete representation of a ballot must include both affirmative and negative selections of
    /// every contest, AND the placeholder selections necessary to satisfy the NIZKPs for each contest and selection.
    /// </summary>
    public partial class PlaintextBallot : DisposableBase
    {
        /// <Summary>
        /// The Object Id of the ballot style in the election manifest.  This value is used
        /// to determine which contests to expect on the ballot, to fill in missing values,
        /// and to validate that the ballot is well-formed
        /// </Summary>
        public unsafe string StyleId
        {
            get
            {
                var status = NativeInterface.PlaintextBallot.GetStyleId(
                    Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <summary>
        /// The size of the Contests collection
        /// </summary>
        public unsafe ulong ContestsSize
        {
            get
            {
                var size = NativeInterface.PlaintextBallot.GetContestsSize(Handle);
                return size;
            }
        }

        internal unsafe NativePlaintextBallot Handle;

        /// <summary>
        /// Create a Plaintext Ballot
        /// </summary>
        /// <param name="json">json representation</param>
        public unsafe PlaintextBallot(string json)
        {
            var status = NativeInterface.PlaintextBallot.FromJson(json, out Handle);
            status.ThrowIfError();
        }

        /// <summary>
        /// Create a Plaintext Ballot
        /// </summary>
        /// <param name="data">binary representation</param>
        /// <param name="encoding">the encoding type</param>
        public unsafe PlaintextBallot(byte[] data, BinarySerializationEncoding encoding)
        {
            fixed (byte* pointer = data)
            {
                var status = encoding == BinarySerializationEncoding.BSON
                    ? NativeInterface.PlaintextBallot.FromBson(pointer, (ulong)data.Length, out Handle)
                    : NativeInterface.PlaintextBallot.FromMsgPack(pointer, (ulong)data.Length, out Handle);
                status.ThrowIfError();
            }
        }

        /// <summary>
        /// Create a PlaintextBallot
        /// </summary>
        /// <param name="objectId"></param>
        /// <param name="styleId"></param>
        /// <param name="contests"></param>
        public unsafe PlaintextBallot(
            string objectId, string styleId, PlaintextBallotContest[] contests)
        {
            IntPtr[] contestPointers = new IntPtr[contests.Length];
            for (var i = 0; i < contests.Length; i++)
            {
                contestPointers[i] = contests[i].Handle.Ptr;
                contests[i].Dispose();
            }

            var status = NativeInterface.PlaintextBallot.New(
                objectId, styleId, contestPointers, (ulong)contestPointers.LongLength, out Handle);
            status.ThrowIfError();
        }

        /// <summary>
        /// Get the contest at the specified index
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public unsafe PlaintextBallotContest GetContestAt(ulong index)
        {
            var status = NativeInterface.PlaintextBallot.GetContestAtIndex(
                Handle, index, out NativePlaintextBallotContest value);
            status.ThrowIfError();
            return new PlaintextBallotContest(value);
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


        /// <Summary>
        /// Export the ballot representation as JSON
        /// </Summary>
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Style", "IDE0059:Unnecessary assignment of a value", Justification = "<Pending>")]
        public unsafe string ToJson()
        {
            var status = NativeInterface.PlaintextBallot.ToJson(
                Handle, out IntPtr pointer, out ulong size);
            status.ThrowIfError();
            var json = Marshal.PtrToStringAnsi(pointer);
            NativeInterface.Memory.FreeIntPtr(pointer);
            return json;
        }

        /// <Summary>
        /// Export the ballot representation as BSON
        /// </Summary>
        public unsafe byte[] ToBson()
        {

            var status = NativeInterface.PlaintextBallot.ToBson(
                Handle, out IntPtr data, out ulong size);
            status.ThrowIfError();

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"PlaintextBallot Error ToBson: size is too big");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            // TODO: use PlaintextBallot.MsgPackFree
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
        }

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        public unsafe byte[] ToMsgPack()
        {

            var status = NativeInterface.PlaintextBallot.ToMsgPack(
                Handle, out IntPtr data, out ulong size);
            status.ThrowIfError();

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"PlaintextBallot Error ToMsgPack: size is too big");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
        }

    }

    #endregion

    #region CompactPlaintextBallot

    /// <summary>
    /// A CompactCiphertextBallot is a CompactPlaintextBallot that includes the encryption parameters
    /// to properly re-encrypt the same ballot.
    ///
    /// This class is space optimized to serve specific use cases where an encrypted ballot is used
    /// to verify that plaintext selections have not been tampered with.
    ///
    /// Don't make this directly. Use `make` instead.
    /// </summary>
    public class CompactPlaintextBallot : DisposableBase
    {
        internal unsafe NativeCompactPlaintextBallot Handle;

        /// <summary>
        /// Create a CompactPlaintextBallot
        /// </summary>
        /// <param name="data"></param>
        public unsafe CompactPlaintextBallot(byte[] data)
        {
            fixed (byte* pointer = data)
            {
                var status = NativeInterface.CompactPlaintextBallot.FromMsgPack(pointer, (ulong)data.Length, out Handle);
                status.ThrowIfError();
            }
        }

        unsafe internal CompactPlaintextBallot(NativeCompactPlaintextBallot handle)
        {
            Handle = handle;
        }

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        public unsafe byte[] ToMsgPack()
        {

            var status = NativeInterface.CompactPlaintextBallot.ToMsgPack(
                Handle, out IntPtr data, out ulong size);

            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"CompactPlaintextBallot Error ToMsgPack: {status}");
            }

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"CompactPlaintextBallot Error ToMsgPack: size is too big");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactPlaintextBallot.MsgPackFree(data);
            return byteArray;
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    #endregion

    #region CiphertextBallot

    /// <summary>
    /// A CiphertextBallot represents a voters encrypted selections for a given ballot and ballot style.
    ///
    /// When a ballot is in it's complete, encrypted state, the `nonce` is the seed nonce
    /// from which all other nonces can be derived to encrypt the ballot.  Allong with the `nonce`
    /// fields on `Ballotcontest` and `BallotSelection`, this value is sensitive.
    ///
    /// Don't make this directly. Use `make_ciphertext_ballot` instead.
    /// </summary>
    public class CiphertextBallot : DisposableBase
    {
        internal unsafe NativeCiphertextBallot Handle;

        /// <summary>
        /// The unique ballot id that is meaningful to the consuming application.
        /// </summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetObjectId(
                    Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// The Object Id of the ballot style in the election manifest.  This value is used
        /// to determine which contests to expect on the ballot, to fill in missing values,
        /// and to validate that the ballot is well-formed
        /// </Summary>
        public unsafe string StyleId
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetStyleId(
                    Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <summary>
        /// Hash of the complete Election Manifest to which this ballot belongs
        /// </summary>
        public unsafe ElementModQ ManifestHash
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetManifestHash(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// The seed hash for the ballot.  It may be the encryption device hash,
        /// the hash of a previous ballot or the hash of some other value
        /// that is meaningful to the consuming application.
        /// </summary>
        public unsafe ElementModQ BallotCodeSeed
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetBallotCodeSeed(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// Get the size of the contests collection
        /// </summary>
        public unsafe ulong ContestsSize
        {
            get
            {
                var size = NativeInterface.CiphertextBallot.GetContestsSize(Handle);
                return size;
            }
        }

        /// <summary>
        /// The unique ballot code for this ballot that is derived from
        /// the ballot seed, the timestamp, and the hash of the encrypted values
        /// </summary>
        public unsafe ElementModQ BallotCode
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetBallotCode(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// The timestamp indicating when the ballot was encrypted
        /// as measured by the encryption device.  This value does not
        /// provide units as it is up to the host system to indicate the scale.
        /// Typically a host may use seconds since epoch or ticks since epoch
        /// </summary>
        public unsafe ulong Timestamp
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetTimestamp(
                    Handle, out ulong value);
                status.ThrowIfError();
                return value;
            }
        }

        /// <summary>
        /// The nonce value used to encrypt all values in the ballot
        /// </summary>
        public unsafe ElementModQ Nonce
        {
            get
            {
                var status = NativeInterface.CiphertextBallot.GetNonce(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// Create a CiphertextBallot
        /// </summary>
        /// <param name="json"></param>
        public unsafe CiphertextBallot(string json)
        {
            var status = NativeInterface.CiphertextBallot.FromJson(json, out Handle);
            status.ThrowIfError();
        }

        /// <summary>
        /// Create a CiphertextBallot
        /// </summary>
        /// <param name="data"></param>
        /// <param name="encoding"></param>
        public unsafe CiphertextBallot(byte[] data, BinarySerializationEncoding encoding)
        {
            fixed (byte* pointer = data)
            {
                var status = encoding == BinarySerializationEncoding.BSON
                    ? NativeInterface.CiphertextBallot.FromBson(pointer, (ulong)data.Length, out Handle)
                    : NativeInterface.CiphertextBallot.FromMsgPack(pointer, (ulong)data.Length, out Handle);
                status.ThrowIfError();
            }
        }

        unsafe internal CiphertextBallot(NativeCiphertextBallot handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Get a contest at a specific index
        /// </summary>
        public unsafe CiphertextBallotContest GetContestAt(ulong index)
        {
            var status = NativeInterface.CiphertextBallot.GetContestAtIndex(
                Handle, index, out NativeCiphertextBallotContest value);
            status.ThrowIfError();
            return new CiphertextBallotContest(value);
        }

        /// <summary>
        /// Given an encrypted Ballot, validates the encryption state
        /// against a specific ballot seed and public key
        /// by verifying the states of this ballot's members (BallotContest's and BallotSelection's).
        /// Calling this function expects that the object is in a well-formed encrypted state
        /// with the `contests` populated with valid encrypted ballot selections,
        /// and the ElementModQ `manifest_hash` also populated.
        /// Specifically, the seed in this context is the hash of the Election Manifest,
        /// or whatever `ElementModQ` was used to populate the `manifest_hash` field.
        /// </summary>
        public unsafe bool IsValidEncryption(
            ElementModQ manifestHash, ElementModP elGamalPublicKey, ElementModQ cryptoExtendedBaseHash)
        {
            return NativeInterface.CiphertextBallot.IsValidEncryption(
                Handle, manifestHash.Handle, elGamalPublicKey.Handle, cryptoExtendedBaseHash.Handle);
        }


        /// <Summary>
        /// Export the ballot representation as JSON
        /// </Summary>
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Style", "IDE0059:Unnecessary assignment of a value", Justification = "<Pending>")]
        public unsafe string ToJson(bool withNonces = false)
        {
            var status = withNonces
                ? NativeInterface.CiphertextBallot.ToJsonWithNonces(
                Handle, out IntPtr pointer, out ulong size)
                : NativeInterface.CiphertextBallot.ToJson(
                Handle, out pointer, out size);
            status.ThrowIfError();
            var json = Marshal.PtrToStringAnsi(pointer);
            NativeInterface.Memory.FreeIntPtr(pointer);
            return json;
        }

        /// <Summary>
        /// Export the ballot representation as BSON
        /// </Summary>
        public unsafe byte[] ToBson(bool withNonces = false)
        {

            var status = withNonces
                ? NativeInterface.CiphertextBallot.ToBsonWithNonces(
                Handle, out IntPtr data, out ulong size)
                : NativeInterface.CiphertextBallot.ToBson(
                Handle, out data, out size);

            status.ThrowIfError();

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"CiphertextBallot Error ToBson: size is too big");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
        }

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        public unsafe byte[] ToMsgPack(bool withNonces = false)
        {

            var status = withNonces
                ? NativeInterface.CiphertextBallot.ToMsgPack(
                Handle, out IntPtr data, out ulong size)
                : NativeInterface.CiphertextBallot.ToMsgPack(
                Handle, out data, out size);
            status.ThrowIfError();

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"CiphertextBallot Error ToMsgPack: size is too big");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    #endregion

    #region CompactCiphertextBallot

    /// <summary>
    /// A CompactCiphertextBallot is a CompactPlaintextBallot that includes the encryption parameters
    /// to properly re-encrypt the same ballot.
    ///
    /// This class is space optimized to serve specific use cases where an encrypted ballot is used
    /// to verify that plaintext selections have not been tampered with.
    ///
    /// Don't make this directly. Use `make` instead.
    /// </summary>
    public class CompactCiphertextBallot : DisposableBase
    {
        internal unsafe NativeCompactCiphertextBallot Handle;

        /// <Summary>
        /// A unique Ballot ID that is relevant to the external system and must be unique
        /// within the dataset of the election.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.CompactCiphertextBallot.GetObjectId(
                    Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <summary>
        /// Create a CompactCiphertextBallot
        /// </summary>
        /// <param name="data"></param>
        public unsafe CompactCiphertextBallot(byte[] data)
        {
            fixed (byte* pointer = data)
            {
                var status = NativeInterface.CompactCiphertextBallot.FromMsgPack(pointer, (ulong)data.Length, out Handle);
                status.ThrowIfError();
            }
        }

        unsafe internal CompactCiphertextBallot(NativeCompactCiphertextBallot handle)
        {
            Handle = handle;
        }

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        public unsafe byte[] ToMsgPack()
        {

            var status = NativeInterface.CompactCiphertextBallot.ToMsgPack(
                Handle, out IntPtr data, out ulong size);
            status.ThrowIfError();

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"CompactCiphertextBallot Error ToMsgPack: size is too big");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    #endregion

    #region SubmittedBallot

    /// <summary>
    /// A `SubmittedBallot` represents a ballot that is submitted for inclusion in election results.
    /// A submitted ballot is or is about to be either cast or spoiled.
    /// The state supports the `BallotBoxState.UNKNOWN` enumeration to indicate that this object is mutable
    /// and has not yet been explicitly assigned a specific state.
    ///
    /// Note, additionally, this ballot includes all proofs but no nonces.
    ///
    /// Do not make this class directly. Use `make_ciphertext_submitted_ballot` or `from_ciphertext_ballot` instead.
    /// </summary>
    public class SubmittedBallot : DisposableBase
    {
        /// <summary>
        /// Get the BallotBoxState
        /// </summary>
        public unsafe BallotBoxState State
        {
            get
            {
                return NativeInterface.SubmittedBallot.GetState(Handle);
            }
        }

        /// <summary>
        /// Get the ObjectId
        /// </summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.SubmittedBallot.GetObjectId(
                    Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// The Object Id of the ballot style in the election manifest.  This value is used
        /// to determine which contests to expect on the ballot, to fill in missing values,
        /// and to validate that the ballot is well-formed
        /// </Summary>
        public unsafe string StyleId
        {
            get
            {
                var status = NativeInterface.SubmittedBallot.GetStyleId(
                    Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <summary>
        /// Hash of the complete Election Manifest to which this ballot belongs
        /// </summary>
        public unsafe ElementModQ ManifestHash
        {
            get
            {
                var status = NativeInterface.SubmittedBallot.GetManifestHash(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// The seed hash for the ballot.  It may be the encryption device hash,
        /// the hash of a previous ballot or the hash of some other value
        /// that is meaningful to the consuming application.
        /// </summary>
        public unsafe ElementModQ BallotCodeSeed
        {
            get
            {
                var status = NativeInterface.SubmittedBallot.GetBallotCodeSeed(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// Get the size of the contests collection
        /// </summary>
        public unsafe ulong ContestsSize
        {
            get
            {
                var size = NativeInterface.SubmittedBallot.GetContestsSize(Handle);
                return size;
            }
        }

        /// <summary>
        /// The unique ballot code for this ballot that is derived from
        /// the ballot seed, the timestamp, and the hash of the encrypted values
        /// </summary>
        public unsafe ElementModQ BallotCode
        {
            get
            {
                var status = NativeInterface.SubmittedBallot.GetBallotCode(
                    Handle, out NativeElementModQ value);
                status.ThrowIfError();
                return new ElementModQ(value);
            }
        }

        /// <summary>
        /// The timestamp indicating when the ballot was encrypted
        /// as measured by the encryption device.  This value does not
        /// provide units as it is up to the host system to indicate the scale.
        /// Typically a host may use seconds since epoch or ticks since epoch
        /// </summary>
        public unsafe ulong Timestamp
        {
            get
            {
                var status = NativeInterface.SubmittedBallot.GetTimestamp(
                    Handle, out ulong value);
                status.ThrowIfError();
                return value;
            }
        }

        internal unsafe NativeSubmittedBallot Handle;

        unsafe internal SubmittedBallot(NativeSubmittedBallot handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Create a SubmittedBallot
        /// </summary>
        /// <param name="ciphertext"></param>
        /// <param name="state"></param>
        public unsafe SubmittedBallot(CiphertextBallot ciphertext, BallotBoxState state)
        {
            var status = NativeInterface.SubmittedBallot.From(
                ciphertext.Handle, state, out Handle);
            status.ThrowIfError();
        }

        /// <summary>
        /// Crate a SubmittedBallot
        /// </summary>
        /// <param name="json"></param>
        public unsafe SubmittedBallot(string json)
        {
            var status = NativeInterface.SubmittedBallot.FromJson(json, out Handle);
            status.ThrowIfError();
        }

        /// <summary>
        /// Create a SubmittedBallot
        /// </summary>
        /// <param name="data"></param>
        /// <param name="encoding"></param>
        public unsafe SubmittedBallot(byte[] data, BinarySerializationEncoding encoding)
        {
            fixed (byte* pointer = data)
            {
                var status = encoding == BinarySerializationEncoding.BSON
                    ? NativeInterface.SubmittedBallot.FromBson(pointer, (ulong)data.Length, out Handle)
                    : NativeInterface.SubmittedBallot.FromMsgPack(pointer, (ulong)data.Length, out Handle);
                status.ThrowIfError();
            }
        }

        /// <summary>
        /// Get a contest at a specific index
        /// </summary>
        public unsafe CiphertextBallotContest GetContestAt(ulong index)
        {
            var status = NativeInterface.SubmittedBallot.GetContestAtIndex(
                Handle, index, out NativeCiphertextBallotContest value);
            status.ThrowIfError();
            return new CiphertextBallotContest(value);
        }

        /// <summary>
        /// Given an encrypted Ballot, validates the encryption state
        /// against a specific ballot seed and public key
        /// by verifying the states of this ballot's members (BallotContest's and BallotSelection's).
        /// Calling this function expects that the object is in a well-formed encrypted state
        /// with the `contests` populated with valid encrypted ballot selections,
        /// and the ElementModQ `manifest_hash` also populated.
        /// Specifically, the seed in this context is the hash of the Election Manifest,
        /// or whatever `ElementModQ` was used to populate the `manifest_hash` field.
        /// </summary>
        public unsafe bool IsValidEncryption(
            ElementModQ manifestHash, ElementModP elGamalPublicKey, ElementModQ cryptoExtendedBaseHash)
        {
            return NativeInterface.SubmittedBallot.IsValidEncryption(
                Handle, manifestHash.Handle, elGamalPublicKey.Handle, cryptoExtendedBaseHash.Handle);
        }

        /// <Summary>
        /// Export the ballot representation as JSON
        /// </Summary>
        public unsafe string ToJson()
        {
            var status = NativeInterface.SubmittedBallot.ToJson(
                Handle, out IntPtr pointer, out ulong size);
            status.ThrowIfError();
            var json = Marshal.PtrToStringAnsi(pointer);
            NativeInterface.Memory.FreeIntPtr(pointer);
            return json;
        }

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        public unsafe byte[] ToBson()
        {

            var status = NativeInterface.SubmittedBallot.ToBson(
                Handle, out IntPtr data, out ulong size);
            status.ThrowIfError();

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"SubmittedBallot Error ToBson: size is too big");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
        }

        /// <Summary>
        /// Export the ballot representation as MsgPack
        /// </Summary>
        public unsafe byte[] ToMsgPack()
        {

            var status = NativeInterface.SubmittedBallot.ToMsgPack(
                Handle, out IntPtr data, out ulong size);

            status.ThrowIfError();

            if (size > int.MaxValue)
            {
                throw new ElectionGuardException($"SubmittedBallot Error ToMsgPack: size is too big");
            }

            var byteArray = new byte[(int)size];
            Marshal.Copy(data, byteArray, 0, (int)size);
            NativeInterface.CompactCiphertextBallot.MsgPackFree(data);
            return byteArray;
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

    #endregion

}
