using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    internal static class ElectionGuard
    {
        static internal unsafe ulong* SetElementModQ(ElementModQ element)
        {
            // allocate space in unmanaged memory and store the ElementModQ
            // pass the pointer to the unmanagement memory to the ElementModQ creation method

            fixed (ulong* pointer = new ulong[ElementModQ.MAX_SIZE])
            {
                for (var i = 0; i < ElementModQ.MAX_SIZE; i++)
                {
                    pointer[i] = element.Data[i];
                }

                return ElectionGuardWrapper.CreateElementModQ(pointer);
            }
        }

        /// <summary>
        /// Gets an ElementModQ out of unmanaged memory using the EG C interface
        /// </summary>
        /// <param name="pointer">Pointer to the ElementModQ in memory</param>
        static internal unsafe ElementModQ GetElementModQ(ulong* pointer)
        {
            var elementData = new ulong[ElementModQ.MAX_SIZE];

            fixed (ulong* element = new ulong[ElementModQ.MAX_SIZE])
            {
                var dhSize = ElectionGuardWrapper.GetElementModQ(pointer, &element);

                if (dhSize != ElementModQ.MAX_SIZE)
                {
                    Console.WriteLine("wrong size");
                }

                if (element == null)
                {
                    Console.WriteLine("element is null");
                    return null;
                }

                for (var i = 0; i < ElementModQ.MAX_SIZE; i++)
                {
                    elementData[i] = element[i];
                }
            }

            return new ElementModQ(elementData);
        }
    }
}
