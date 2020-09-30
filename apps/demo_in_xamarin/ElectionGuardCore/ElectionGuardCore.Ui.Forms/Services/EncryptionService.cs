using System.Threading;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Encryption;

namespace ElectionGuardCore.Ui.Forms.Services
{
    public class EncryptionService : IEncryptionService
    {
        public EncryptionResult EncryptBallot(ElectionDescription election, string candidateId)
        {
            // TODO implement
            Thread.Sleep(5000);
            return new EncryptionResult();
        }
    }
}
