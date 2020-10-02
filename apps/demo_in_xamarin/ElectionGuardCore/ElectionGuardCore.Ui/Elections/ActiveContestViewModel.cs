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

            HasVotedInActiveContest = await _electionService.HasVoted(Election.Id);
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

        private bool _hasVotedInActiveContest;
        public bool HasVotedInActiveContest
        {
            get => _hasVotedInActiveContest;
            private set
            {
                _hasVotedInActiveContest = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(CanVote));
                OnPropertyChanged(nameof(CannotVote));
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
