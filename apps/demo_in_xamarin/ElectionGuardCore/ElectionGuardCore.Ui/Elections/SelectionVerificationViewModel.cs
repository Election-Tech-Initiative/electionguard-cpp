using System.Threading.Tasks;
using System.Windows.Input;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Encryption;

namespace ElectionGuardCore.Ui.Elections
{
    public class SelectionVerificationViewModel : PageViewModelBase
    {
        private readonly IClipboardService _clipboardService;
        private readonly IAlertService _alertService;
        private readonly IElectionService _electionService;
        private readonly INavigationService _navigationService;

        public SelectionVerificationViewModel(IClipboardService clipboardService, IAlertService alertService,
            IElectionService electionService, INavigationService navigationService)
        {
            _clipboardService = clipboardService;
            _alertService = alertService;
            _electionService = electionService;
            _navigationService = navigationService;

            CopyTrackingCodeCommand = new RelayCommand(CopyTrackingCode);
            CastCommand = new RelayCommand(Cast);
            SpoilCommand = new RelayCommand(Spoil);
        }

        public override string Title => "Verification";

        public override Task Load()
        {
            Args = (SelectionVerificationArgs)Parameter;
            return Task.CompletedTask;
        }

        private SelectionVerificationArgs _args;
        private SelectionVerificationArgs Args
        {
            get => _args;
            set
            {
                _args = value;
                CiphertextBallot = _args.CiphertextBallot;
            }
        }

        private CiphertextBallot _ciphertextBallot;
        public CiphertextBallot CiphertextBallot
        {
            get => _ciphertextBallot;
            set
            {
                _ciphertextBallot = value;
                OnPropertyChanged();
            }
        }

        public ICommand CopyTrackingCodeCommand { get; }

        private async void CopyTrackingCode(object parameter)
        {
            await _clipboardService.Copy(CiphertextBallot.TrackingCode);
            await _alertService.Alert("Copied to clipboard", null, "OK");
        }

        public ICommand CastCommand { get; }

        private async void Cast(object parameter)
        {
            await Submit(true);
        }

        public ICommand SpoilCommand { get; }

        private async void Spoil(object parameter)
        {
            await Submit(false);
        }

        private async Task Submit(bool voted)
        {
            // TODO submit result to API
            _electionService.Votes[Args.ElectionDescription.ActiveContest.ObjectId] = voted;
            await _navigationService.Push(NavigationPaths.SelectionSubmittedPage,
                new SelectionSubmittedViewModel.SelectionSubmittedArgs(voted, Args.ElectionDescription));
        }

        public class SelectionVerificationArgs
        {
            public readonly ElectionDescription ElectionDescription;
            public readonly CiphertextElectionContext CiphertextElectionContext;
            public readonly CiphertextBallot CiphertextBallot;

            public SelectionVerificationArgs(
                ElectionDescription metadata, CiphertextElectionContext context, CiphertextBallot ciphertextBallot)
            {
                ElectionDescription = metadata;
                CiphertextElectionContext = context;
                CiphertextBallot = ciphertextBallot;
            }
        }
    }
}
