using System.Threading.Tasks;
using ElectionGuardCore.Encryption;

namespace ElectionGuardCore.Ui.Elections
{
    public class SelectionVerificationViewModel : PageViewModelBase
    {
        public override string Title => "Verification";

        public override Task Load()
        {
            EncryptionResult = (EncryptionResult)Parameter;
            return Task.CompletedTask;
        }

        private EncryptionResult _encryptionResult;
        public EncryptionResult EncryptionResult
        {
            get => _encryptionResult;
            set
            {
                _encryptionResult = value;
                OnPropertyChanged();
            }
        }
    }
}
