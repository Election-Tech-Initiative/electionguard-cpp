// DO NOT MODIFY THIS FILE
// This file is generated via ElectionGuard.InteropGenerator at /src/interop-generator

using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    public partial class PlaintextBallot
    {
        #region Properties
        /// <Summary>
        /// A unique Ballot ID that is relevant to the external system and must be unique within the dataset of the election.
        /// </Summary>
        public unsafe string ObjectId
        {
            get
            {
                var status = External.GetObjectId(Handle, out IntPtr value);
                status.ThrowIfError();
                var data = Marshal.PtrToStringAnsi(value);
                NativeInterface.Memory.FreeIntPtr(value);
                return data;
            }
        }

        #endregion

        #region Methods
        #endregion

        #region Extern
        private unsafe static class External {
            [DllImport(
                NativeInterface.DllName,
                EntryPoint = "eg_plaintext_ballot_get_object_id",
                CallingConvention = CallingConvention.Cdecl, 
                SetLastError = true
            )]
            internal static extern Status GetObjectId(
                NativeInterface.PlaintextBallot.PlaintextBallotHandle handle
                , out IntPtr objectId
            );

        }
        #endregion
    }
}
