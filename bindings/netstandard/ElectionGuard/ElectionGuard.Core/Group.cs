using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NaiveElementModP = NativeInterface.ElementModP.ElementModPHandle;
    using NaiveElementModQ = NativeInterface.ElementModQ.ElementModQHandle;

    /// <summary>
    /// An element of the larger `mod p` space, i.e., in [0, P), where P is a 4096-bit prime.
    /// </summary>
    public class ElementModP : DisposableBase
    {
        /// <summary>
        /// Number of 64-bit ints that make up the 4096-bit prime
        /// </summary>
        public static readonly ulong MAX_SIZE = 64;

        /// <Summary>
        /// Get the integer representation of the element
        /// </Summary>
        public ulong[] Data { get { return GetNative(); } internal set { NewNative(value); } }
        internal unsafe NaiveElementModP Handle;

        /// <summary>
        /// Creates a `ElementModP` object
        /// </summary>
        /// <param name="data">the data used to initialize the `ElementModP`</param>
        public ElementModP(ulong[] data)
        {
            try
            {
                NewNative(data);
            }
            catch (Exception ex)
            {
                throw new ElectionGuardException("construction error", ex);
            }
        }

        unsafe internal ElementModP(NaiveElementModP handle)
        {
            Handle = handle;
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
        /// exports a hex representation of the integer value in Big Endian format
        /// </Summary>
        public unsafe string ToHex()
        {
            var status = NativeInterface.ElementModP.ToHex(Handle, out IntPtr pointer);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ToHex Error Status: {status}");
            }
            var value = Marshal.PtrToStringAnsi(pointer);
            NativeInterface.Memory.FreeIntPtr(pointer);
            return value;
        }

        private unsafe void NewNative(ulong[] data)
        {
            fixed (ulong* pointer = new ulong[MAX_SIZE])
            {
                for (ulong i = 0; i < MAX_SIZE; i++)
                {
                    pointer[i] = data[i];
                }

                var status = NativeInterface.ElementModP.New(pointer, out Handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"createNative Error Status: {status}");
                }
            }
        }

        private unsafe ulong[] GetNative()
        {
            if (Handle == null)
            {
                return null;
            }

            var data = new ulong[MAX_SIZE];
            fixed (ulong* element = new ulong[MAX_SIZE])
            {
                var status = NativeInterface.ElementModP.GetData(Handle, &element, out ulong size);
                if (size != MAX_SIZE)
                {
                    throw new ElectionGuardException($"wrong size, expected: {MAX_SIZE} actual: {size}");
                }

                if (element == null)
                {
                    throw new ElectionGuardException("element is null");
                }

                for (ulong i = 0; i < MAX_SIZE; i++)
                {
                    data[i] = element[i];
                }
            }

            return data;
        }
    }

    /// <summary>
    /// An element of the smaller `mod q` space, i.e., in [0, Q), where Q is a 256-bit prime.
    /// </summary>
    public class ElementModQ : DisposableBase
    {
        /// <summary>
        /// Number of 64-bit ints that make up the 256-bit prime
        /// </summary>
        public static readonly ulong MAX_SIZE = 4;

        /// <Summary>
        /// Get the integer representation of the element
        /// </Summary>
        public ulong[] Data { get { return GetNative(); } internal set { NewNative(value); } }
        internal unsafe NaiveElementModQ Handle;

        /// <summary>
        /// Create a `ElementModQ`
        /// </summary>
        /// <param name="data">data used to initialize the `ElementModQ`</param>
        public ElementModQ(ulong[] data)
        {
            try
            {
                NewNative(data);
            }
            catch (Exception ex)
            {
                throw new ElectionGuardException("construction error", ex);
            }
        }

        /// <summary>
        /// Create a `ElementModQ`
        /// </summary>
        /// <param name="hex">string representing the hex bytes of the initializationd data</param>
        /// <param name="uncheckedInput">if data is checked or not</param>
        public ElementModQ(string hex, bool uncheckedInput = false)
        {
            var status = uncheckedInput ?
                NativeInterface.ElementModQ.FromHexUnchecked(hex, out Handle)
                : NativeInterface.ElementModQ.FromHex(hex, out Handle);
            status.ThrowIfError();
        }

        unsafe internal ElementModQ(NaiveElementModQ handle)
        {
            Handle = handle;
        }

        /// <Summary>
        /// exports a hex representation of the integer value in Big Endian format
        /// </Summary>
        public unsafe string ToHex()
        {
            var status = NativeInterface.ElementModQ.ToHex(Handle, out IntPtr pointer);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                throw new ElectionGuardException($"ToHex Error Status: {status}");
            }
            var value = Marshal.PtrToStringAnsi(pointer);
            NativeInterface.Memory.FreeIntPtr(pointer);
            return value;
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

        private unsafe void NewNative(ulong[] data)
        {
            fixed (ulong* pointer = new ulong[MAX_SIZE])
            {
                for (ulong i = 0; i < MAX_SIZE; i++)
                {
                    pointer[i] = data[i];
                }

                var status = NativeInterface.ElementModQ.New(pointer, out Handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"createNative Error Status: {status}");
                }
            }
        }

        private unsafe ulong[] GetNative()
        {
            if (Handle == null)
            {
                return null;
            }

            var data = new ulong[MAX_SIZE];
            fixed (ulong* element = new ulong[MAX_SIZE])
            {
                var status = NativeInterface.ElementModQ.GetData(Handle, &element, out ulong size);
                if (size != MAX_SIZE)
                {
                    throw new ElectionGuardException($"wrong size, expected: {MAX_SIZE}, actual: {size}");
                }

                if (element == null)
                {
                    throw new ElectionGuardException("element is null");
                }

                for (ulong i = 0; i < MAX_SIZE; i++)
                {
                    data[i] = element[i];
                }
            }

            return data;
        }
    }

    /// <summary>
    /// Static class of constant values used in the encryption system
    /// </summary>
    public static class Constants
    {
        /// <summary>
        /// G in Hacl_Bignum4096 format
        /// </summary>
        public unsafe static ElementModP G
        {
            get
            {
                var status = NativeInterface.Constants.G(out NaiveElementModP handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"G Error Status: {status}");
                }
                return new ElementModP(handle);
            }
        }

        /// <summary>
        /// Max P value in Hacl_Bignum4096 format
        /// </summary>
        public unsafe static ElementModP P
        {
            get
            {
                var status = NativeInterface.Constants.P(out NaiveElementModP handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"P Error Status: {status}");
                }
                return new ElementModP(handle);
            }
        }

        /// <summary>
        /// R value in Hacl_Bignum4096 format
        /// </summary>
        public unsafe static ElementModP R
        {
            get
            {
                var status = NativeInterface.Constants.R(out NaiveElementModP handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"R Error Status: {status}");
                }
                return new ElementModP(handle);
            }
        }

        /// <summary>
        /// zero as data for `ElementModP`
        /// </summary>
        public unsafe static ElementModP ZERO_MOD_P
        {
            get
            {
                var status = NativeInterface.Constants.ZERO_MOD_P(out NaiveElementModP handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ZERO_MOD_P Error Status: {status}");
                }
                return new ElementModP(handle);
            }
        }

        /// <summary>
        /// one as data for `ElementModP`
        /// </summary>
        public unsafe static ElementModP ONE_MOD_P
        {
            get
            {
                var status = NativeInterface.Constants.ZERO_MOD_P(out NaiveElementModP handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ONE_MOD_P Error Status: {status}");
                }
                return new ElementModP(handle);
            }
        }

        /// <summary>
        /// two as data for `ElementModP`
        /// </summary>
        public unsafe static ElementModP TWO_MOD_P
        {
            get
            {
                var status = NativeInterface.Constants.ZERO_MOD_P(out NaiveElementModP handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"TWO_MOD_P Error Status: {status}");
                }
                return new ElementModP(handle);
            }
        }

        /// <summary>
        /// Max Q value in Hacl_Bignum256 format
        /// </summary>
        public unsafe static ElementModQ Q
        {
            get
            {
                var status = NativeInterface.Constants.Q(out NaiveElementModQ handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"Q Error Status: {status}");
                }
                return new ElementModQ(handle);
            }
        }

        /// <summary>
        /// zero as data for `ElementModQ`
        /// </summary>
        public unsafe static ElementModQ ZERO_MOD_Q
        {
            get
            {
                var status = NativeInterface.Constants.ZERO_MOD_Q(out NaiveElementModQ handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ZERO_MOD_Q Error Status: {status}");
                }
                return new ElementModQ(handle);
            }
        }

        /// <summary>
        /// one as data for `ElementModQ`
        /// </summary>
        public unsafe static ElementModQ ONE_MOD_Q
        {
            get
            {
                var status = NativeInterface.Constants.ONE_MOD_Q(out NaiveElementModQ handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"ONE_MOD_Q Error Status: {status}");
                }
                return new ElementModQ(handle);
            }
        }

        /// <summary>
        /// two as data for `ElementModQ`
        /// </summary>
        public unsafe static ElementModQ TWO_MOD_Q
        {
            get
            {
                var status = NativeInterface.Constants.TWO_MOD_Q(out NaiveElementModQ handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    throw new ElectionGuardException($"TWO_MOD_Q Error Status: {status}");
                }
                return new ElementModQ(handle);
            }
        }

        /// <Summary>
        /// Export the representation as JSON
        /// </Summary>
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Style", "IDE0059:Unnecessary assignment of a value", Justification = "<Pending>")]
        public unsafe static string ToJson()
        {
            var status = NativeInterface.Constants.ToJson(
                 out IntPtr pointer, out ulong size);
            status.ThrowIfError();
            var json = Marshal.PtrToStringAnsi(pointer);
            NativeInterface.Memory.FreeIntPtr(pointer);
            return json;
        }

    }
}
