using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Encryption
{
    public interface IEncryptionService
    {
        EncryptionResult EncryptBallot(ElectionDescription election, string candidateId);
    }
}
