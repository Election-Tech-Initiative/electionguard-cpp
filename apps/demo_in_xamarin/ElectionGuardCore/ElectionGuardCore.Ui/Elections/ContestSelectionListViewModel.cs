using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Input;
using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Ui.Elections
{
    public class ContestSelectionListViewModel : PageViewModelBase
    {
        private readonly INavigationService _navigationService;

        public ContestSelectionListViewModel(INavigationService navigationService)
        {
            _navigationService = navigationService;

            SelectCandidateCommand = new RelayCommand(SelectCandidate);
            _reviewSelectionCommand = new RelayCommand(ReviewSelection, CanReviewSelection);
        }

        public override string Title => "Vote";

        public override Task Load()
        {
            ElectionDescription = (ElectionDescription)Parameter;
            _reviewSelectionCommand.OnCanExecuteChanged();  // prevent button from staying enabled when 
                                                            // reloading view and selection is lost
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

        public string ContestTitle => ElectionDescription?.ActiveContest.BallotTitle.GetTextValue("en");

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

        private CandidateViewModel SelectedCandidate => Candidates?.FirstOrDefault(c => c.IsSelected);

        public ICommand SelectCandidateCommand { get; }

        private void SelectCandidate(object parameter)
        {
            var candidate = (CandidateViewModel)parameter;

            var selectedCandidate = SelectedCandidate;
            if (selectedCandidate != null)
            {
                selectedCandidate.IsSelected = false;
            }

            if (candidate.ObjectId != selectedCandidate?.ObjectId)
            {
                candidate.IsSelected = true;
            }

            _reviewSelectionCommand.OnCanExecuteChanged();
        }

        private readonly RelayCommand _reviewSelectionCommand;
        public ICommand ReviewSelectionCommand => _reviewSelectionCommand;

        private bool CanReviewSelection(object parameter) => SelectedCandidate != null;

        private async void ReviewSelection(object parameter)
        {
            await _navigationService.Push(NavigationPaths.ReviewSelectionPage,
                new ReviewSelectionViewModel.ReviewSelectionArgs(SelectedCandidate.Candidate,
                    ElectionDescription.ActiveContest));
        }

        public class CandidateViewModel : ViewModelBase
        {
            public CandidateViewModel(Candidate candidate)
            {
                if (candidate == null)
                {
                    throw new ArgumentNullException(nameof(candidate));
                }

                Candidate = candidate;
            }

            public Candidate Candidate { get; }

            public string ObjectId => Candidate.ObjectId;
            public string BallotName => Candidate.BallotName.GetTextValue("en");

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
