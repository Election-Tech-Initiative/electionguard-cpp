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
            }
        }

        public ActiveContestViewModel(IElectionService electionService)
        {
            _electionService = electionService;
        }

        public override async Task Load()
        {
            ElectionDescription = await _electionService.GetElectionDescription();
        }
    }
}
