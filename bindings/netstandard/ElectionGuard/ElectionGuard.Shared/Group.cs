using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NaiveElementModP = NativeInterface.ElementModP.ElementModPType;
    using NaiveElementModQ = NativeInterface.ElementModQ.ElementModQType;

    public class ElementModP : DisposableBase
    {
        public static readonly ulong MAX_SIZE = 64;

        public ulong[] Data { get { return GetNative(); } set { NewNative(value); } }
        internal unsafe NaiveElementModP* Handle;

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

        unsafe internal ElementModP(NaiveElementModP* handle)
        {
            Handle = handle;
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null) return;
            var status = NativeInterface.ElementModP.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnmanaged Error Status: {status}");
            }
            Handle = null;
        }

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
                var status = NativeInterface.ElementModP.GetData(Handle, &element, out UIntPtr size);
                if (size.ToUInt64() != MAX_SIZE)
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

    public class ElementModQ : DisposableBase
    {
        public static readonly ulong MAX_SIZE = 4;

        public ulong[] Data { get { return GetNative(); } set { NewNative(value); } }
        internal unsafe NaiveElementModQ* Handle;

        public ElementModQ(ulong[] data)
        {
            try
            {
                NewNative(data);
            } catch
            {
                Console.WriteLine("construction error");
            }
        }

        unsafe internal ElementModQ(NaiveElementModQ* handle)
        {
            Handle = handle;
        }

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

            if (Handle == null) return;
            var status = NativeInterface.ElementModQ.Free(Handle);
            if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
            {
                Console.WriteLine($"DisposeUnmanaged Error Status: {status}");
            }
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
                var status = NativeInterface.ElementModQ.GetData(Handle, &element, out UIntPtr size);
                if (size.ToUInt64() != MAX_SIZE)
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
}
