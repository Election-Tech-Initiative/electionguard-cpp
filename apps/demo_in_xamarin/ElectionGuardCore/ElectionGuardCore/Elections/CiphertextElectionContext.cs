using System;
namespace ElectionGuardCore.Elections
{
    /// <summary>
    /// `CiphertextElectionContext` is the ElectionGuard representation of a specific election
    /// Note: The ElectionGuard Data Spec deviates from the NIST model in that
    /// this object includes fields that are populated in the course of encrypting an election
    /// Specifically, `CryptoBaseHash`, `CryptoExtendedBaseHash` and `ElgamalPublicKey`
    /// are populated with election-specific information necessary for encrypting the election.
    /// Refer to the[Electionguard Specification](https://github.com/microsoft/electionguard) for more information.
    /// 
    /// </summary>
    public class CiphertextElectionContext
    {
        public int NumberOfGuardians { get; set; }
        public int Quorum { get; set; }
        public string ElgamalPublicKey { get; set; }
        public string DescriptionHash { get; set; }
        public string CryptoBaseHash { get; set; }
        public string CryptoExtendedBaseHash { get; set; }
    }
}
