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
    public class PlaintextBallot : DisposableBase
    {
        /// <Summary>
        /// A unique Ballot ID that is relevant to the external system and must be unique
        /// within the dataset of the election.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.PlaintextBallot.GetObjectId(
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

}
