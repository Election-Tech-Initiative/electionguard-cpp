using System;

namespace ElectionGuard
{
    public class ElementModQ
    {
        public static readonly int MAX_SIZE = 4;

        public ulong[] data { get { return Get(); } set { createNative(value); } }
        unsafe ulong* handle;

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

                handle = NativeInterface.CreateElementModQ(pointer);
            }
        }

        public unsafe ulong[] Get()
        {
            var data = new ulong[MAX_SIZE];
            fixed (ulong* element = new ulong[MAX_SIZE])
            {
                var dhSize = NativeInterface.GetElementModQ(handle, &element);
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
