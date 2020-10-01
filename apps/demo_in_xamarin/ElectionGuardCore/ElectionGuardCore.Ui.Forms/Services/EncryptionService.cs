using System.Threading;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Encryption;

namespace ElectionGuardCore.Ui.Forms.Services
{
    public class EncryptionService : IEncryptionService
    {
        public CiphertextBallot EncryptBallot(
            ElectionDescription metadata, CiphertextElectionContext context, Candidate candidate)
        {

            // TODO implement
            Thread.Sleep(5000);
            return new CiphertextBallot { TrackingCode = "checker ACND2 sorry K2JJD connect 9ANDK four 8C372 unreal G338F blizzard FGG1J super 4RR81 tornado J7BCA" };
        }
    }
}
