using System.Threading.Tasks;
using System.Windows.Input;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Encryption;

namespace ElectionGuardCore.Ui.Elections
{
    public class ReviewSelectionViewModel : PageViewModelBase
    {
        private readonly IAlertService _alertService;
        private readonly IEncryptionService _encryptionService;
        private readonly INavigationService _navigationService;

        public ReviewSelectionViewModel(IAlertService alertService, IEncryptionService encryptionService,
            INavigationService navigationService)
        {
            _encryptionService = encryptionService;
            _alertService = alertService;
            _navigationService = navigationService;

            EncryptSelectionCommand = new RelayCommand(EncryptSelecton);
        }

        public override string Title => "Review";

        public override Task Load()
        {
            Args = (ReviewSelectionArgs)Parameter;
            return Task.CompletedTask;
        }

        private ReviewSelectionArgs _args;
        private ReviewSelectionArgs Args
        {
            get => _args;
            set
            {
                _args = value;
                OnPropertyChanged(nameof(CandidateName));
            }
        }

        public string CandidateName => Args?.Selection.BallotName.GetTextValue("en");

        public ICommand EncryptSelectionCommand { get; }

        private async void EncryptSelecton(object parameter)
        {
            if (await _alertService.Alert("Encrypt ballot",
                "You are about to encrypt your ballot. This cannot be undone. Do you wish to continue?", "Yes", "No"))
            {
                EncryptionResult result = null;
                await _alertService.ShowBusy("Encrypting ballot…",
                    () => result = _encryptionService.EncryptBallot(Args.ElectionDescription, Args.Selection.ObjectId),
                    async () => await _navigationService.Push(NavigationPaths.SelectionVerificationPage,
                        new SelectionVerificationViewModel.SelectionVerificationArgs(Args.ElectionDescription, result))
                );
            }
        }

        public class ReviewSelectionArgs
        {
            public readonly Candidate Selection;
            public readonly ElectionDescription ElectionDescription;

            public ReviewSelectionArgs(Candidate selection, ElectionDescription electionDescription)
            {
                Selection = selection;
                ElectionDescription = electionDescription;
            }
        }
    }
}
