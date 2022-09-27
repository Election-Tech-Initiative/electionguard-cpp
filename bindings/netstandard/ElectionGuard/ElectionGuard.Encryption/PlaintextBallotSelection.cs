// DO NOT MODIFY THIS FILE
// This file is generated via ElectionGuard.InteropGenerator at /src/interop-generator

using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    public partial class PlaintextBallotSelection
    {
        /// <Summary>
        /// Get the objectId of the selection which is the unique id for\nthe selection in a specific contest described in the election manifest.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = NativeInterface.PlaintextBallotSelection.GetObjectId(
                    Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        /// <Summary>
        /// Determines if this is a placeholder selection
        /// </Summary>
        public unsafe bool IsPlaceholder
        {
            get
            {
                return NativeInterface.PlaintextBallotSelection.GetIsPlaceholder(Handle);
            }
        }

        /// <Summary>
        /// Get the plaintext vote
        /// </Summary>
        public unsafe ulong Vote
        {
            get
            {
                return NativeInterface.PlaintextBallotSelection.GetVote(Handle);
            }
        }

    }
}