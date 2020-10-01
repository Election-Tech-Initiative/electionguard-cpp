using System;

namespace ElectionGuardCore.Elections
{
    /// <summary>
    /// An "exponential ElGamal ciphertext" (i.e., with the plaintext in the exponent to allow for
    /// homomorphic addition).
    /// </summary>
    public class ElGamalCiphertext
    {
        public ElementModP Pad { get; set; }
        public ElementModP Data { get; set; }
    }
}
