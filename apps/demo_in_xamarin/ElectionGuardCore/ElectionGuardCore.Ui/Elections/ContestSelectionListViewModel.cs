using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Ui.Elections
{
    public class ContestSelectionListViewModel : ViewModelBase
    {
        public override string Title => "Vote";

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

        public class CandidateViewModel
        {
            private readonly Candidate _candidate;

            public CandidateViewModel(Candidate candidate)
            {
                _candidate = candidate;
            }

            public string BallotName => _candidate.BallotName.Text.FirstOrDefault()?.Value;
        }
    }
}
