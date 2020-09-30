using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows.Input;
using ElectionGuardCore.Encryption;

namespace ElectionGuardCore.Ui.Elections
{
    public class SelectionVerificationViewModel : PageViewModelBase
    {
        private readonly IClipboardService _clipboardService;
        private readonly IAlertService _alertService;
        private readonly INavigationService _navigationService;

        public SelectionVerificationViewModel(IClipboardService clipboardService, IAlertService alertService,
            INavigationService navigationService)
        {
            _clipboardService = clipboardService;
            _alertService = alertService;
            _navigationService = navigationService;

            CopyTrackingCodeCommand = new RelayCommand(CopyTrackingCode);
            CastCommand = new RelayCommand(Cast);
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

        public ICommand CastCommand { get; }

        private async void Cast(object parameter)
        {
            var tasks = new List<Task>
            {
                _navigationService.PopModal(),
                _navigationService.PushModal(NavigationPaths.SelectionSubmittedPage)
            };
            await Task.WhenAll(tasks);
        }
    }
}
