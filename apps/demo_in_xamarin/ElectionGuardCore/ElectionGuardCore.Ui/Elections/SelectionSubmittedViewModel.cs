using System.Threading.Tasks;
using System.Windows.Input;
using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Ui.Elections
{
    public class SelectionSubmittedViewModel : PageViewModelBase
    {
        private readonly INavigationService _navigationService;

        public SelectionSubmittedViewModel(INavigationService navigationService)
        {
            _navigationService = navigationService;

            CloseCommand = new RelayCommand(Close);
            ReVoteCommand = new RelayCommand(ReVote);
        }

        public override string Title => "Thank You";

        public override Task Load()
        {
            Args = (SelectionSubmittedArgs)Parameter;
            return Task.CompletedTask;
        }

        private SelectionSubmittedArgs _args;
        private SelectionSubmittedArgs Args
        {
            get => _args;
            set
            {
                _args = value;
                OnPropertyChanged(nameof(Message));
                OnPropertyChanged(nameof(CanReVote));
            }
        }

        public string Message
        {
            get
            {
                if (Args == null)
                {
                    return null;
                }
                return Args.VoteCast ? "Your vote has been counted!" : "Your vote has been spoiled.";
            }
        }

        public bool CanReVote
        {
            get
            {
                if (Args == null)
                {
                    return false;
                }
                return !Args.VoteCast;
            }
        }

        public ICommand CloseCommand { get; }

        private async void Close(object parameter)
        {
            await _navigationService.PopToRoot();
        }

        public ICommand ReVoteCommand { get; }

        private async void ReVote(object parameter)
        {
            await _navigationService.PopToRoot();
            await _navigationService.Push(NavigationPaths.ContestSelectionListPage,
                new ContestSelectionListViewModel.ContestSelectionListArgs(Args.Election,
                    Args.CiphertextElectionContext));
        }

        public class SelectionSubmittedArgs
        {
            public readonly bool VoteCast;
            public readonly Election Election;
            public readonly CiphertextElectionContext CiphertextElectionContext;

            public SelectionSubmittedArgs(bool voteCast, Election election, CiphertextElectionContext context)
            {
                VoteCast = voteCast;
                Election = election;
                CiphertextElectionContext = context;
            }
        }
    }
}
