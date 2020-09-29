using System.Linq;
using System.Threading.Tasks;
using System.Windows.Input;
using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Ui.Elections
{
    public class ActiveContestViewModel : ViewModelBase
    {
        private readonly IElectionService _electionService;
        private readonly INavigationService _navigationService;

        public ActiveContestViewModel(IElectionService electionService, INavigationService navigationService)
        {
            _electionService = electionService;
            _navigationService = navigationService;

            BeginVoteCommand = new RelayCommand(BeginVote);
        }

        public override string Title => "Election Guard Demo";

        public override async Task Load()
        {
            // TODO show loading indicator
            ElectionDescription = await _electionService.GetElectionDescription();
        }

        private ElectionDescription _electionDescription;
        public ElectionDescription ElectionDescription
        {
            get => _electionDescription;
            private set
            {
                _electionDescription = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(ElectionName));
                OnPropertyChanged(nameof(ActiveContestName));
                OnPropertyChanged(nameof(HasVotedInActiveContest));
                OnPropertyChanged(nameof(CanVote));
                OnPropertyChanged(nameof(CannotVote));
            }
        }

        public string ElectionName => ElectionDescription?.Name?.Text?.FirstOrDefault()?.Value;

        public string ActiveContestName => ElectionDescription?.ActiveContest?.Name ??
                                           "There is currently not an active contest available";

        public bool HasVotedInActiveContest
        {
            get
            {
                var voted = false;
                if (ElectionDescription?.ActiveContest?.ObjectId != null &&
                    _electionService.Votes != null &&
                    _electionService.Votes.ContainsKey(ElectionDescription.ActiveContest.ObjectId))
                {
                    voted = _electionService.Votes[ElectionDescription.ActiveContest.ObjectId];
                }

                return voted;
            }
        }

        public bool CanVote => !HasVotedInActiveContest;
        public bool CannotVote => !CanVote;

        public ICommand BeginVoteCommand { get; }

        private async void BeginVote(object parameter)
        {
            await _navigationService.Push(NavigationPaths.ContestSelectionListPage, ElectionDescription);
        }
    }
}
