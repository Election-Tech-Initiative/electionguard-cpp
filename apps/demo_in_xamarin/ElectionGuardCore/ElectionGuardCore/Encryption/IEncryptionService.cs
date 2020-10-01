using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Encryption
{
    public interface IEncryptionService
    {
        CiphertextBallot EncryptBallot(
            ElectionDescription metadata, CiphertextElectionContext context, Candidate selection);
    }
}
