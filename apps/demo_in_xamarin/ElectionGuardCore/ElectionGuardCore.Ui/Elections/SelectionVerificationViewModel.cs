using System.Threading.Tasks;
using System.Windows.Input;
using ElectionGuardCore.Encryption;

namespace ElectionGuardCore.Ui.Elections
{
    public class SelectionVerificationViewModel : PageViewModelBase
    {
        private readonly IClipboardService _clipboardService;

        public SelectionVerificationViewModel(IClipboardService clipboardService)
        {
            _clipboardService = clipboardService;

            CopyTrackingCodeCommand = new RelayCommand(CopyTrackingCode);
        }

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

        public ICommand CopyTrackingCodeCommand { get; }

        private async void CopyTrackingCode(object parameter)
        {
            await _clipboardService.Copy(EncryptionResult.TrackingCode);
        }
    }
}
