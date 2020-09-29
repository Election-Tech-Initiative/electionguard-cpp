using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Input;
using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Ui.Elections
{
    public class ContestSelectionListViewModel : PageViewModelBase
    {
        public override string Title => "Vote";

        public ContestSelectionListViewModel()
        {
            SelectCandidateCommand = new RelayCommand(SelectCandidate);
        }

        public override Task Load()
        {
            ElectionDescription = (ElectionDescription)Parameter;
            return Task.CompletedTask;
        }

        private ElectionDescription _electionDescription;
        private ElectionDescription ElectionDescription
        {
            get => _electionDescription;
            set
            {
                _electionDescription = value;

                OnPropertyChanged(nameof(ContestTitle));
                RefreshCandidates();
            }
        }

        public string ContestTitle => ElectionDescription?.ActiveContest.BallotTitle.Text
            .FirstOrDefault(t => t.Language == "en")?.Value;

        private List<CandidateViewModel> _candidates;
        public List<CandidateViewModel> Candidates
        {
            get => _candidates;
            private set
            {
                _candidates = value;
                OnPropertyChanged();
            }
        }

        private void RefreshCandidates()
        {
            if (ElectionDescription?.ActiveContest?.BallotSelections == null)
            {
                Candidates = null;
                return;
            }

            var candidates = new List<CandidateViewModel>();
            foreach (var ballotSelection in ElectionDescription.ActiveContest.BallotSelections.OrderBy(b =>
                b.SequenceOrder))
            {
                var candidate =
                    ElectionDescription.Candidates.FirstOrDefault(c => c.ObjectId == ballotSelection.CandidateId);
                if (candidate != null)
                {
                    candidates.Add(new CandidateViewModel(candidate));
                }
            }
            Candidates = candidates;
        }

        public ICommand SelectCandidateCommand { get; }

        private void SelectCandidate(object parameter)
        {
            var candidate = (CandidateViewModel)parameter;

            var selectedCandidate = Candidates.FirstOrDefault(c => c.IsSelected);
            if (selectedCandidate != null)
            {
                selectedCandidate.IsSelected = false;
            }

            if (candidate != selectedCandidate)
            {
                candidate.IsSelected = true;
            }
        }

        public class CandidateViewModel : ViewModelBase
        {
            private readonly Candidate _candidate;

            public CandidateViewModel(Candidate candidate)
            {
                _candidate = candidate;
            }

            public string BallotName => _candidate.BallotName.Text.FirstOrDefault()?.Value;

            private bool _isSelected;
            public bool IsSelected
            {
                get => _isSelected;
                set
                {
                    _isSelected = value;
                    OnPropertyChanged();
                    OnPropertyChanged(nameof(BackgroundColor));
                }
            }

            public string BackgroundColor => IsSelected ? Colors.LightGreen : Colors.White;
        }
    }
}
