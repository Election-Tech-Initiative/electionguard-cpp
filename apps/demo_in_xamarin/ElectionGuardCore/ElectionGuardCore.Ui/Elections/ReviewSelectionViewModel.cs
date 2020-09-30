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

        public ReviewSelectionViewModel(IAlertService alertService, IEncryptionService encryptionService)
        {
            _encryptionService = encryptionService;
            _alertService = alertService;

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

        public class ReviewSelectionArgs
        {
            public readonly Candidate Selection;
            public readonly ElectionDescription Election;

            public ReviewSelectionArgs(Candidate selection, ElectionDescription election)
            {
                Selection = selection;
                Election = election;
            }
        }

        public ICommand EncryptSelectionCommand { get; }

        private async void EncryptSelecton(object parameter)
        {
            if (await _alertService.Prompt("Encrypt ballot",
                "You are about to encrypt your ballot. This cannot be undone. Do you wish to continue?", "Yes", "No"))
            {
                _encryptionService.EncryptBallot(Args.Election, Args.Selection.ObjectId);
            }
        }
    }
}
