using System.Threading.Tasks;
using System.Windows.Input;
using ElectionGuardCore.Encryption;

namespace ElectionGuardCore.Ui.Elections
{
    public class SelectionVerificationViewModel : PageViewModelBase
    {
        private readonly IClipboardService _clipboardService;
        private readonly IAlertService _alertService;

        public SelectionVerificationViewModel(IClipboardService clipboardService, IAlertService alertService)
        {
            _clipboardService = clipboardService;
            _alertService = alertService;

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
            await _alertService.Alert("Copied to clipboard", null, "OK");
        }
    }
}
