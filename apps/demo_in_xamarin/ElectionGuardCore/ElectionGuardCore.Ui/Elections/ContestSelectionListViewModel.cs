using System.Threading.Tasks;
using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Ui.Elections
{
    public class ContestSelectionListViewModel : ViewModelBase
    {
        public override string Title => "Vote";

        public override Task Load()
        {
            ActiveContest = (ContestDescription)Parameter;
            return Task.CompletedTask;
        }

        private ContestDescription _activeContest;
        public ContestDescription ActiveContest
        {
            get => _activeContest;
            private set
            {
                _activeContest = value;
                OnPropertyChanged();
            }
        }
    }
}
