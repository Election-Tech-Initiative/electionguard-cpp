using System;

namespace ElectionGuard
{
    public class ElementModQ
    {
        public static readonly int MAX_SIZE = 4;

        public ulong[] Data = new ulong[MAX_SIZE];

        public ElementModQ(ulong[] data)
        {
            // TODO: better safety
            if (data?.Length <= MAX_SIZE)
            {
                Data = data;
            }
        }
    }
}
