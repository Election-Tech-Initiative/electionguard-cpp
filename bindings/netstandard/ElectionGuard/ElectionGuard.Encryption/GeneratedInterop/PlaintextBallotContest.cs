// DO NOT MODIFY THIS FILE
// This file is generated via ElectionGuard.InteropGenerator at /src/interop-generator

using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    public partial class PlaintextBallotContest
    {
        [DllImport(NativeInterface.DllName,
            EntryPoint = "eg_plaintext_ballot_contest_get_object_id",
            CallingConvention = CallingConvention.Cdecl, SetLastError = true)]
        internal static extern Status GetObjectId(
            NativeInterface.PlaintextBallotContest.PlaintextBallotContestHandle handle
            , out IntPtr object_id
        );

        /// <Summary>
        /// Get the objectId of the contest, which is the unique id for the contest in a specific ballot style described in the election manifest.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = GetObjectId(
                    Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

    }
}
