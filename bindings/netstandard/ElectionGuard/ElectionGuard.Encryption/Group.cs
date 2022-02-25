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
        public static readonly ulong MAX_SIZE = 64;

        /// <Summary>
        /// Get the integer representation of the element
        /// </Summary>
        public ulong[] Data { get { return GetNative(); } internal set { NewNative(value); } }
        internal unsafe NaiveElementModP Handle;

        public ElementModP(ulong[] data)
        {
            try
            {
                NewNative(data);
            }
            catch
            {
                Console.WriteLine("construction error");
            }
        }

        unsafe internal ElementModP(NaiveElementModP handle)
        {
            Handle = handle;
        }

        protected override unsafe void DisposeUnmanaged()
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
                Console.WriteLine($"ToHex Error Status: {status}");
            }
            var value = Marshal.PtrToStringAnsi(pointer);
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
                    Console.WriteLine($"createNative Error Status: {status}");
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
                    Console.WriteLine("wrong size");
                    return null;
                }

                if (element == null)
                {
                    Console.WriteLine("element is null");
                    return null;
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
        public static readonly ulong MAX_SIZE = 4;

        /// <Summary>
        /// Get the integer representation of the element
        /// </Summary>
        public ulong[] Data { get { return GetNative(); } internal set { NewNative(value); } }
        internal unsafe NaiveElementModQ Handle;

        public ElementModQ(ulong[] data)
        {
            try
            {
                NewNative(data);
            }
            catch
            {
                Console.WriteLine("construction error");
            }
        }

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
                Console.WriteLine($"ToHex Error Status: {status}");
            }
            var value = Marshal.PtrToStringAnsi(pointer);
            return value;
        }

        protected override unsafe void DisposeUnmanaged()
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
                    Console.WriteLine($"createNative Error Status: {status}");
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
                    Console.WriteLine("wrong size");
                    return null;
                }

                if (element == null)
                {
                    Console.WriteLine("element is null");
                    return null;
                }

                for (ulong i = 0; i < MAX_SIZE; i++)
                {
                    data[i] = element[i];
                }
            }

            return data;
        }
    }

    public static class Constants
    {
        public unsafe static ElementModP G
        {
            get
            {
                var status = NativeInterface.Constants.G(out NaiveElementModP handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"G Error Status: {status}");
                }
                return new ElementModP(handle);
            }
        }

        public unsafe static ElementModP P
        {
            get
            {
                var status = NativeInterface.Constants.P(out NaiveElementModP handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"P Error Status: {status}");
                }
                return new ElementModP(handle);
            }
        }

        public unsafe static ElementModP ZERO_MOD_P
        {
            get
            {
                var status = NativeInterface.Constants.ZERO_MOD_P(out NaiveElementModP handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ZERO_MOD_P Error Status: {status}");
                }
                return new ElementModP(handle);
            }
        }

        public unsafe static ElementModP ONE_MOD_P
        {
            get
            {
                var status = NativeInterface.Constants.ZERO_MOD_P(out NaiveElementModP handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ONE_MOD_P Error Status: {status}");
                }
                return new ElementModP(handle);
            }
        }

        public unsafe static ElementModP TWO_MOD_P
        {
            get
            {
                var status = NativeInterface.Constants.ZERO_MOD_P(out NaiveElementModP handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"TWO_MOD_P Error Status: {status}");
                }
                return new ElementModP(handle);
            }
        }

        public unsafe static ElementModQ Q
        {
            get
            {
                var status = NativeInterface.Constants.Q(out NaiveElementModQ handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Q Error Status: {status}");
                }
                return new ElementModQ(handle);
            }
        }

        public unsafe static ElementModQ ZERO_MOD_Q
        {
            get
            {
                var status = NativeInterface.Constants.ZERO_MOD_Q(out NaiveElementModQ handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ZERO_MOD_Q Error Status: {status}");
                }
                return new ElementModQ(handle);
            }
        }

        public unsafe static ElementModQ ONE_MOD_Q
        {
            get
            {
                var status = NativeInterface.Constants.ONE_MOD_Q(out NaiveElementModQ handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ONE_MOD_Q Error Status: {status}");
                }
                return new ElementModQ(handle);
            }
        }

        public unsafe static ElementModQ TWO_MOD_Q
        {
            get
            {
                var status = NativeInterface.Constants.TWO_MOD_Q(out NaiveElementModQ handle);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"TWO_MOD_Q Error Status: {status}");
                }
                return new ElementModQ(handle);
            }
        }
    }
}
