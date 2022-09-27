// DO NOT MODIFY THIS FILE
// This file is generated via ElectionGuard.InteropGenerator at /src/interop-generator

namespace ElectionGuard
{
    public partial class PlaintextBallotSelection
    {
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