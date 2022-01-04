using System;
namespace ElectionGuard
{
    // Declare native types for convenience
    using NativeElementModP = NativeInterface.ElementModP.ElementModPHandle;
    using NativeElementModQ = NativeInterface.ElementModQ.ElementModQHandle;
    using NativeElGamalCiphertext = NativeInterface.ElGamalCiphertext.ElGamalCiphertextHandle;
    using NativeDisjunctiveChaumPedersenProof = NativeInterface.DisjunctiveChaumPedersenProof.DisjunctiveChaumPedersenProofHandle;
    using NativeConstantChaumPedersenProof = NativeInterface.ConstantChaumPedersenProof.ConstantChaumPedersenProofHandle;

    /// <Summary>
    /// The Disjunctive Chaum Pederson proof is a Non-Interactive Zero-Knowledge Proof
    /// that represents the proof of ballot correctness (that a value is either zero or one).
    /// This proof demonstrates that an ElGamal encryption pair (𝛼,𝛽) is an encryption of zero or one
    /// (given knowledge of encryption nonce R).
    ///
    /// This object should not be constructed directly.  Use DisjunctiveChaumPedersenProof::make
    ///
    /// see: https://www.electionguard.vote/spec/0.95.0/5_Ballot_encryption/#outline-for-proofs-of-ballot-correctness
    /// </Summary>
    public class DisjunctiveChaumPedersenProof : DisposableBase
    {
        /// <Summary>
        /// a0 in the spec
        /// </Summary>
        public unsafe ElementModP ZeroPad
        {
            get
            {
                var status = NativeInterface.DisjunctiveChaumPedersenProof.GetZeroPad(
                    Handle, out NativeElementModP value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ZeroPad Error Status: {status}");
                    return null;
                }
                return new ElementModP(value);
            }
        }

        /// <Summary>
        /// b0 in the spec
        /// </Summary>
        public unsafe ElementModP ZeroData
        {
            get
            {
                var status = NativeInterface.DisjunctiveChaumPedersenProof.GetZeroData(
                    Handle, out NativeElementModP value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ZeroData Error Status: {status}");
                    return null;
                }
                return new ElementModP(value);
            }
        }

        /// <Summary>
        /// a1 in the spec
        /// </Summary>
        public unsafe ElementModP OnePad
        {
            get
            {
                var status = NativeInterface.DisjunctiveChaumPedersenProof.GetOnePad(
                    Handle, out NativeElementModP value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"OnePad Error Status: {status}");
                    return null;
                }
                return new ElementModP(value);
            }
        }

        /// <Summary>
        /// b1 in the spec
        /// </Summary>
        public unsafe ElementModP OneData
        {
            get
            {
                var status = NativeInterface.DisjunctiveChaumPedersenProof.GetOneData(
                    Handle, out NativeElementModP value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"OneData Error Status: {status}");
                    return null;
                }
                return new ElementModP(value);
            }
        }

        /// <Summary>
        /// c0 in the spec
        /// </Summary>
        public unsafe ElementModQ ZeroChallenge
        {
            get
            {
                var status = NativeInterface.DisjunctiveChaumPedersenProof.GetZeroChallenge(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ZeroChallenge Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        /// <Summary>
        /// c1 in the spec
        /// </Summary>
        public unsafe ElementModQ OneChallenge
        {
            get
            {
                var status = NativeInterface.DisjunctiveChaumPedersenProof.GetOneChallenge(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"OneChallenge Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        /// <Summary>
        /// c in the spec
        /// </Summary>
        public unsafe ElementModQ Challenge
        {
            get
            {
                var status = NativeInterface.DisjunctiveChaumPedersenProof.GetChallenge(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Challenge Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        /// <Summary>
        /// v0 in the spec
        /// </Summary>
        public unsafe ElementModQ ZeroResponse
        {
            get
            {
                var status = NativeInterface.DisjunctiveChaumPedersenProof.GetZeroResponse(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"ZeroResponse Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        /// <Summary>
        /// v1 in the spec
        /// </Summary>
        public unsafe ElementModQ OneResponse
        {
            get
            {
                var status = NativeInterface.DisjunctiveChaumPedersenProof.GetOneResponse(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"OneResponse Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeDisjunctiveChaumPedersenProof Handle;

        unsafe internal DisjunctiveChaumPedersenProof(NativeDisjunctiveChaumPedersenProof handle)
        {
            Handle = handle;
        }

        /// <Summary>
        /// make function for a `DisjunctiveChaumPedersenProof`
        ///
        /// This overload does not accept a seed value and calculates
        /// proofs independent of the original encryption. (faster performance)
        /// <param name="message"> The ciphertext message</param>
        /// <param name="r"> The nonce used creating the ElGamal ciphertext</param>
        /// <param name="k"> The public key of the election</param>
        /// <param name="q"> A value used when generating the challenge,
        ///          usually the election extended base hash (𝑄')</param>
        /// <param name="plaintext">The constant value to prove, zero or one</param>
        /// <returns>A unique pointer</returns>
        /// </Summary>
        unsafe public DisjunctiveChaumPedersenProof(
            ElGamalCiphertext message,
            ElementModQ r,
            ElementModP k,
            ElementModQ q,
            ulong plaintext)
        {
            var status = NativeInterface.DisjunctiveChaumPedersenProof.Make(
                message.Handle, r.Handle, k.Handle, q.Handle, plaintext, out Handle);
            status.ThrowIfError();
        }

        /// <Summary>
        /// make function for a `DisjunctiveChaumPedersenProof`
        ///
        /// This overload accepts a seed value and calculates
        /// proofs deterministically based on the seed. (slower, but reproduceable proofs)
        /// <param name="message"> The ciphertext message</param>
        /// <param name="r"> The nonce used creating the ElGamal ciphertext</param>
        /// <param name="k"> The public key of the election</param>
        /// <param name="q"> A value used when generating the challenge,
        ///          usually the election extended base hash (𝑄')</param>
        /// <param name="seed">Used to generate other random values here</param>
        /// <param name="plaintext">The constant value to prove, zero or one</param>
        /// <returns>A unique pointer</returns>
        /// </Summary>
        unsafe public DisjunctiveChaumPedersenProof(
            ElGamalCiphertext message,
            ElementModQ r,
            ElementModP k,
            ElementModQ q,
            ElementModQ seed,
            ulong plaintext)
        {
            var status = NativeInterface.DisjunctiveChaumPedersenProof.Make(
                message.Handle, r.Handle, k.Handle, q.Handle, seed.Handle, plaintext, out Handle);
            status.ThrowIfError();
        }

        /// <Summary>
        /// Validates a "disjunctive" Chaum-Pedersen (zero or one) proof.
        ///
        /// <param name="message"> The ciphertext message</param>
        /// <param name="k"> The public key of the election</param>
        /// <param name="q"> The extended base hash of the election</param>
        /// <returns> True if everything is consistent. False otherwise. </returns>
        /// </Summary>
        public unsafe bool IsValid(ElGamalCiphertext message, ElementModP k, ElementModQ q)
        {
            return NativeInterface.DisjunctiveChaumPedersenProof.IsValid(
                Handle, message.Handle, k.Handle, q.Handle
            );
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }
    }

    /// <Summary>
    /// The Constant Chaum PedersenProof is a Non-Interactive Zero-Knowledge Proof
    /// that represents the proof of satisfying the selection limit (that the voter has not over voted).
    /// The proof demonstrates that the elgamal accumulation of the encrypted selections
    /// on the ballot forms an aggregate contest encryption matches the combination of random nonces (R)
    /// used to encrypt the selections and that the encrypted values do not exceed the selection limit L.
    ///
    /// This object should not be made directly.  Use ConstantChaumPedersenProof::make
    ///
    /// see: https://www.electionguard.vote/spec/0.95.0/5_Ballot_encryption/#proof-of-satisfying-the-selection-limit
    /// </Summary>
    public class ConstantChaumPedersenProof : DisposableBase
    {
        /// <Summary>
        /// a in the spec
        /// </Summary>
        public unsafe ElementModP Pad
        {
            get
            {
                var status = NativeInterface.ConstantChaumPedersenProof.GetPad(
                    Handle, out NativeElementModP value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Pad Error Status: {status}");
                    return null;
                }
                return new ElementModP(value);
            }
        }

        /// <Summary>
        /// b in the spec
        /// </Summary>
        public unsafe ElementModP Data
        {
            get
            {
                var status = NativeInterface.ConstantChaumPedersenProof.GetData(
                    Handle, out NativeElementModP value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Data Error Status: {status}");
                    return null;
                }
                return new ElementModP(value);
            }
        }

        /// <Summary>
        /// c in the spec
        /// </Summary>
        public unsafe ElementModQ Challenge
        {
            get
            {
                var status = NativeInterface.ConstantChaumPedersenProof.GetChallenge(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Challenge Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        /// <Summary>
        /// v in the spec
        /// </Summary>
        public unsafe ElementModQ Response
        {
            get
            {
                var status = NativeInterface.ConstantChaumPedersenProof.GetResponse(
                    Handle, out NativeElementModQ value);
                if (status != Status.ELECTIONGUARD_STATUS_SUCCESS)
                {
                    Console.WriteLine($"Response Error Status: {status}");
                    return null;
                }
                return new ElementModQ(value);
            }
        }

        internal unsafe NativeConstantChaumPedersenProof Handle;

        unsafe internal ConstantChaumPedersenProof(NativeConstantChaumPedersenProof handle)
        {
            Handle = handle;
        }

        /// <Summary>
        /// make function for a `ConstantChaumPedersenProof`
        ///
        /// <param name="message"> The ciphertext message</param>
        /// <param name="r"> The nonce used creating the ElGamal ciphertext</param>
        /// <param name="k"> The public key of the election</param>
        /// <param name="seed"> A value used when generating the challenge,
        ///          usually the election extended base hash (𝑄')</param>
        /// <param name="hashHeader">Used to generate other random values here</param>
        /// <param name="constant">The constant value to prove</param>
        /// <returns>An instance</returns>
        /// </Summary>
        unsafe public ConstantChaumPedersenProof(
            ElGamalCiphertext message,
            ElementModQ r,
            ElementModP k,
            ElementModQ seed,
            ElementModQ hashHeader,
            ulong constant)
        {
            var status = NativeInterface.ConstantChaumPedersenProof.Make(
                message.Handle, r.Handle, k.Handle, seed.Handle, hashHeader.Handle, constant, out Handle);
            status.ThrowIfError();
        }

        /// <Summary>
        /// Validates a `ConstantChaumPedersenProof`
        ///
        /// <param name="message"> The ciphertext message</param>
        /// <param name="k"> The public key of the election</param>
        /// <param name="q"> The extended base hash of the election</param>
        /// <returns> True if everything is consistent. False otherwise. </returns>
        /// </Summary>
        public unsafe bool IsValid(ElGamalCiphertext message, ElementModP k, ElementModQ q)
        {
            return NativeInterface.ConstantChaumPedersenProof.IsValid(
                Handle, message.Handle, k.Handle, q.Handle
            );
        }

        protected override unsafe void DisposeUnmanaged()
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }
    }
}
