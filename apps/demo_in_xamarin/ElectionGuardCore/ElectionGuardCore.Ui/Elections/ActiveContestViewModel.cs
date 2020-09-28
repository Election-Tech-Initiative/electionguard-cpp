using System.Linq;
using System.Threading.Tasks;
using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Ui.Elections
{
    public class ActiveContestViewModel : ViewModelBase
    {
        private readonly IElectionService _electionService;

        private ElectionDescription _electionDescription;
        public ElectionDescription ElectionDescription
        {
            get => _electionDescription;
            private set
            {
                _electionDescription = value;
                OnPropertyChanged();
                OnPropertyChanged(nameof(ElectionName));
                OnPropertyChanged(nameof(ActiveContest));
                OnPropertyChanged(nameof(ActiveContestName));
                OnPropertyChanged(nameof(HasVotedInActiveContest));
                OnPropertyChanged(nameof(CanVote));
                OnPropertyChanged(nameof(CannotVote));
            }
        }

        public string ElectionName => ElectionDescription?.Name?.Text?.FirstOrDefault()?.Value;
        public ContestDescription ActiveContest => ElectionDescription?.Contests?.FirstOrDefault();
        public string ActiveContestName => ActiveContest?.Name ?? "There is currently not an active contest available";

        public bool HasVotedInActiveContest
        {
            get
            {
                var voted = false;
                if (ActiveContest?.ObjectId != null && _electionService.Votes.ContainsKey(ActiveContest.ObjectId))
                {
                    voted = _electionService.Votes[ActiveContest.ObjectId];
                }

                return voted;
            }
        }

        public bool CanVote => !HasVotedInActiveContest;
        public bool CannotVote => !CanVote;

        public ActiveContestViewModel(IElectionService electionService)
        {
            _electionService = electionService;
        }

        public override string Title => "Election Guard Demo";

        public override async Task Load()
        {
            ElectionDescription = await _electionService.GetElectionDescription();
        }
    }
}
