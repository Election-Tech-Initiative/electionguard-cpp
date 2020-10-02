using System.Threading.Tasks;
using System.Windows.Input;
using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Ui.Elections
{
    public class ActiveContestViewModel : PageViewModelBase
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
            Election = await _electionService.GetElection();
            ElectionContext = await _electionService.GetCiphertextElectionContext();
        }

        private Election _election;
        public Election Election
        {
            get => _election;
            set
            {
                _election = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(ElectionName));
                OnPropertyChanged(nameof(ActiveContestName));
                OnPropertyChanged(nameof(HasVotedInActiveContest));
                OnPropertyChanged(nameof(CanVote));
                OnPropertyChanged(nameof(CannotVote));
            }
        }

        private CiphertextElectionContext _electionContext;
        public CiphertextElectionContext ElectionContext
        {
            get => _electionContext;
            private set
            {
                _electionContext = value;
                OnPropertyChanged();
            }
        }

        public string ElectionName => Election?.ElectionDescription.Name.GetTextValue("en");

        public string ActiveContestName => Election?.ElectionDescription.ActiveContest?.Name ??
                                           "There is currently not an active contest available";

        public bool HasVotedInActiveContest
        {
            get
            {
                var voted = false;
                if (Election?.ElectionDescription.ActiveContest?.ObjectId != null &&
                    _electionService.Votes != null &&
                    _electionService.Votes.ContainsKey(Election.ElectionDescription.ActiveContest.ObjectId))
                {
                    voted = _electionService.Votes[Election.ElectionDescription.ActiveContest.ObjectId];
                }

                return voted;
            }
        }

        public bool CanVote => !HasVotedInActiveContest;
        public bool CannotVote => !CanVote;

        public ICommand BeginVoteCommand { get; }

        private async void BeginVote(object parameter)
        {
            await _navigationService.Push(NavigationPaths.ContestSelectionListPage,
                new ContestSelectionListViewModel.ContestSelectionListArgs(Election, ElectionContext));
        }
    }
}
