// DO NOT MODIFY THIS FILE
// This file is generated via ElectionGuard.InteropGenerator at /src/interop-generator

using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    public partial class PlaintextBallotContest
    {
        #region Properties
        [DllImport(
            NativeInterface.DllName,
            EntryPoint = "eg_plaintext_ballot_contest_get_object_id",
            CallingConvention = CallingConvention.Cdecl, 
            SetLastError = true
        )]
        internal static extern Status GetObjectId(
            NativeInterface.PlaintextBallotContest.PlaintextBallotContestHandle handle
            , out IntPtr objectId
        );

        /// <Summary>
        /// Get the objectId of the contest, which is the unique id for the contest in a specific ballot style described in the election manifest.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = GetObjectId(Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        [DllImport(
            NativeInterface.DllName,
            EntryPoint = "eg_plaintext_ballot_contest_get_selections_size",
            CallingConvention = CallingConvention.Cdecl, 
            SetLastError = true
        )]
        internal static extern ulong GetSelectionsSize(
            NativeInterface.PlaintextBallotContest.PlaintextBallotContestHandle handle
        );

        /// <Summary>
        /// Get the Size of the selections collection
        /// </Summary>
        public unsafe ulong SelectionsSize
        {
            get
            {
                return GetSelectionsSize(Handle);
            }
        }

        #endregion

        #region Methods
        #endregion
    }
}
