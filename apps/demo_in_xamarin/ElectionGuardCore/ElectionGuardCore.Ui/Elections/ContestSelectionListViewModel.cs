﻿using System;
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
            Args = (ContestSelectionListArgs)Parameter;
            _reviewSelectionCommand.OnCanExecuteChanged();  // prevent button from staying enabled when 
                                                            // reloading view and selection is lost
            return Task.CompletedTask;
        }

        private ContestSelectionListArgs _args;
        private ContestSelectionListArgs Args
        {
            get => _args;
            set
            {
                _args = value;
                ElectionDescription = _args.ElectionDescription;
            }
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
                    Args.ElectionDescription.Candidates
                    .FirstOrDefault(c => c.ObjectId == ballotSelection.CandidateId);
                if (candidate != null)
                {
                    candidates.Add(new CandidateViewModel(candidate, ballotSelection));
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
                new ReviewSelectionViewModel.ReviewSelectionArgs(
                    Args.ElectionDescription, Args.ElectionContext,
                    SelectedCandidate.Selection, SelectedCandidate.Candidate)
                );
        }

        public class ContestSelectionListArgs
        {
            public readonly ElectionDescription ElectionDescription;
            public readonly CiphertextElectionContext ElectionContext;

            public ContestSelectionListArgs(ElectionDescription metadata, CiphertextElectionContext context)
            {
                ElectionDescription = metadata;
                ElectionContext = context;
            }
        }

        public class CandidateViewModel : ViewModelBase
        {
            public CandidateViewModel(Candidate candidate, SelectionDescription selection)
            {
                if (candidate == null)
                {
                    throw new ArgumentNullException(nameof(candidate));
                }

                if (selection == null)
                {
                    throw new ArgumentNullException(nameof(selection));
                }

                if (selection.CandidateId != candidate.ObjectId)
                {
                    throw new ArgumentException("Candidate must match selection");
                }

                Candidate = candidate;
                Selection = selection;
            }

            public Candidate Candidate { get; }
            public SelectionDescription Selection { get; }

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
