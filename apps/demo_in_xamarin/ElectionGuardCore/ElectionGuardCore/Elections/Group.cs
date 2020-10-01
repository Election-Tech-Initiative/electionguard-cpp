using System;
using System.Numerics;

namespace ElectionGuardCore.Elections
{
    /// <summary>
    /// An element of the larger `mod p` space, i.e., in [0, P), where P is a 4096-bit prime.
    /// </summary>
    public class ElementModP
    {
        public BigInteger Data { get; set; }
    }

    /// <summary>
    /// An element of the smaller `mod q` space, i.e., in [0, Q), where Q is a 256-bit prime.
    /// </summary>
    public class ElementModQ
    {
        public BigInteger Data { get; set; }
    }
}
