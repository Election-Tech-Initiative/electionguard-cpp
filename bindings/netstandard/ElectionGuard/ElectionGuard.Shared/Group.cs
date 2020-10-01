using System;



namespace ElectionGuard
{
    using NaiveElementModP = NativeInterface.ElementModP.ElementModPType;
    using NaiveElementModQ = NativeInterface.ElementModQ.ElementModQType;

    public class ElementModP
    {
        public static readonly int MAX_SIZE = 64;

        public ulong[] Data { get { return Get(); } set { CreateNative(value); } }

        private unsafe NaiveElementModP* handle;

        public ElementModP(ulong[] data)
        {
            try
            {
                CreateNative(data);
            }
            catch
            {
                Console.WriteLine("construction error");
            }
        }

        private unsafe void CreateNative(ulong[] data)
        {
            fixed (ulong* pointer = new ulong[MAX_SIZE])
            {
                for (var i = 0; i < MAX_SIZE; i++)
                {
                    pointer[i] = data[i];
                }

                handle = NativeInterface.ElementModP.Create(pointer);
            }
        }

        private unsafe ulong[] Get()
        {
            var data = new ulong[MAX_SIZE];
            fixed (ulong* element = new ulong[MAX_SIZE])
            {
                var dhSize = NativeInterface.ElementModP.Get(handle, &element);
                if (dhSize != MAX_SIZE)
                {
                    Console.WriteLine("wrong size");
                }

                if (element == null)
                {
                    Console.WriteLine("element is null");
                    return null;
                }

                for (var i = 0; i < MAX_SIZE; i++)
                {
                    data[i] = element[i];
                }
            }

            return data;
        }
    }
    
    public class ElementModQ
    {
        public static readonly int MAX_SIZE = 4;

        public ulong[] Data { get { return Get(); } set { createNative(value); } }
        internal unsafe NaiveElementModQ* handle;

        public ElementModQ(ulong[] data)
        {
            // TODO: safety
            try
            {
                createNative(data);
            } catch
            {
                Console.WriteLine("construction error");
            }
        }

        private unsafe void createNative(ulong[] data)
        {
            fixed (ulong* pointer = new ulong[MAX_SIZE])
            {
                for (var i = 0; i < MAX_SIZE; i++)
                {
                    pointer[i] = data[i];
                }

                handle = NativeInterface.ElementModQ.Create(pointer);
            }
        }

        public unsafe ulong[] Get()
        {
            var data = new ulong[MAX_SIZE];
            fixed (ulong* element = new ulong[MAX_SIZE])
            {
                var dhSize = NativeInterface.ElementModQ.Get(handle, &element);
                if (dhSize != MAX_SIZE)
                {
                    Console.WriteLine("wrong size");
                }

                if (element == null)
                {
                    Console.WriteLine("element is null");
                    return null;
                }

                for (var i = 0; i < MAX_SIZE; i++)
                {
                    data[i] = element[i];
                }
            }

            return data;
        }
    }
}
