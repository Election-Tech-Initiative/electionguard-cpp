using System;
namespace ElectionGuardCore.Elections
{
    public class DisjunctiveChaumPedersenProof
    {
        public ElementModP ProofZeroPad { get; set; }
        public ElementModP ProofZeroData { get; set; }
        public ElementModP ProofOnePad { get; set; }
        public ElementModP ProofOneData { get; set; }
        public ElementModQ ProofZeroChallenge { get; set; }
        public ElementModQ ProofOneChallenge { get; set; }
        public ElementModQ Challenge { get; set; }
        public ElementModQ ProofZeroResponse { get; set; }
        public ElementModQ ProofOneResponse { get; set; }

    }

    public class ConstantChaumPedersenProof
    {
        public ElementModP Pad { get; set; }
        public ElementModP Data { get; set; }
        public ElementModQ Challenge { get; set; }
        public ElementModQ Response { get; set; }
        public int Constant { get; set; }
    }
}
