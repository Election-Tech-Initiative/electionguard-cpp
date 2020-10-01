using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Ui.Elections;
using FluentAssertions;
using Moq;
using NUnit.Framework;

namespace ElectionGuardCore.Ui.Tests.Elections
{
    [TestFixture]
    public class ContestSelectionListViewModelTest
    {
        private Mock<INavigationService> _navigationServiceMock;

        private readonly ElectionDescription _electionDescription = new ElectionDescription
        {
            Candidates = new List<Candidate>
            {
                new Candidate
                {
                    ObjectId = "robert",
                    BallotName = new InternationalizedText
                    {
                        Text = new List<LanguageText>
                        {
                            new LanguageText { Language = "es", Value = "Roberto" },
                            new LanguageText { Language = "en", Value = "Robert" }
                        }
                    }
                },
                new Candidate
                {
                    ObjectId = "mary",
                    BallotName = new InternationalizedText
                    {
                        Text = new List<LanguageText>
                        {
                            new LanguageText { Language = "es", Value = "Maria" },
                            new LanguageText { Language = "en", Value = "Mary" }
                        }
                    }
                },
            },
            Contests = new List<ContestDescription>
            {
                new ContestDescription
                {
                    BallotTitle = new InternationalizedText
                    {
                        Text = new List<LanguageText>
                        {
                            new LanguageText { Language = "es", Value = "Hola" },
                            new LanguageText { Language = "en", Value = "Hello" },
                            new LanguageText { Language = "en", Value = "Hello again" }
                        }
                    },
                    BallotSelections = new List<SelectionDescription>
                    {
                        new SelectionDescription { CandidateId = "robert", SequenceOrder = 0 },
                        new SelectionDescription { CandidateId = "mary", SequenceOrder = 1 }
                    }
                }
            }
        };

        [SetUp]
        public void SetUp()
        {
            _navigationServiceMock = new Mock<INavigationService>();
        }

        [Test]
        public async Task ContestTitle_IsFirstEnglishTitle()
        {
            var viewModel = await CreateViewModel();
            viewModel.ContestTitle.Should().Be("Hello");
        }

        [Test]
        public async Task Candidates_ReflectsActiveContestBallotSelections()
        {
            var viewModel = await CreateViewModel();
            viewModel.Candidates.Should().HaveCount(2);
            viewModel.Candidates.Should().Contain(c => c.ObjectId == "robert");
            viewModel.Candidates.Should().Contain(c => c.ObjectId == "mary");
        }

        [Test]
        public async Task GivenNoSelection_SelectCandidateCommand_SelectsCandidate()
        {
            var viewModel = await CreateViewModel();
            viewModel.SelectCandidateCommand.Execute(viewModel.Candidates.Single(c => c.ObjectId == "mary"));

            viewModel.Candidates.Single(c => c.ObjectId == "mary").IsSelected.Should().BeTrue();
            viewModel.Candidates.Single(c => c.ObjectId == "robert").IsSelected.Should().BeFalse();
        }

        [Test]
        public async Task GivenSelection_SelectCandidateCommand_DeselectsCandidate()
        {
            var viewModel = await CreateViewModel();
            viewModel.Candidates.Single(c => c.ObjectId == "mary").IsSelected = true;

            viewModel.SelectCandidateCommand.Execute(viewModel.Candidates.Single(c => c.ObjectId == "mary"));

            viewModel.Candidates.Single(c => c.ObjectId == "mary").IsSelected.Should().BeFalse();
            viewModel.Candidates.Single(c => c.ObjectId == "robert").IsSelected.Should().BeFalse();
        }

        [Test]
        public async Task GivenNoSelection_ReviewSelectionCommand_CannotExecute()
        {
            var viewModel = await CreateViewModel();
            viewModel.ReviewSelectionCommand.CanExecute(null).Should().BeFalse();
        }

        [Test]
        public async Task GivenSelection_ReviewSelectionCommand_CanExecute()
        {
            var viewModel = await CreateViewModel();
            viewModel.Candidates.Single(c => c.ObjectId == "mary").IsSelected = true;

            viewModel.ReviewSelectionCommand.CanExecute(null).Should().BeTrue();
        }

        [Test]
        public async Task ReviewSelectionCommand_PassesSelectionToReviewSelectionPage()
        {
            var viewModel = await CreateViewModel();
            var candidate = viewModel.Candidates.Single(c => c.ObjectId == "mary");
            candidate.IsSelected = true;

            viewModel.ReviewSelectionCommand.Execute(null);

            _navigationServiceMock.Verify(x => x.Push(NavigationPaths.ReviewSelectionPage,
                It.Is<ReviewSelectionViewModel.ReviewSelectionArgs>(a =>
                    a.Selection == candidate.Candidate && a.ElectionDescription == _electionDescription)));
        }

        [Test]
        public void CandidateViewModel_Candidate_ReflectsCandidate()
        {
            var candidate = _electionDescription.Candidates.Single(c => c.ObjectId == "mary");
            var viewModel = new ContestSelectionListViewModel.CandidateViewModel(candidate);

            viewModel.Candidate.Should().Be(candidate);
        }

        [Test]
        public void CandidateViewModel_ObjectId_ReflectsCandidate()
        {
            var candidate = _electionDescription.Candidates.Single(c => c.ObjectId == "mary");
            var viewModel = new ContestSelectionListViewModel.CandidateViewModel(candidate);

            viewModel.ObjectId.Should().Be("mary");
        }

        [Test]
        public void CandidateViewModel_BallotName_IsFirstEnglishName()
        {
            var candidate = _electionDescription.Candidates.Single(c => c.ObjectId == "mary");
            var viewModel = new ContestSelectionListViewModel.CandidateViewModel(candidate);

            viewModel.BallotName.Should().Be("Mary");
        }

        [Test]
        public void CandidateViewModel_GivenIsNotSelected_BackgroundColor_IsWhite()
        {
            var candidate = _electionDescription.Candidates.Single(c => c.ObjectId == "mary");
            var viewModel = new ContestSelectionListViewModel.CandidateViewModel(candidate);

            viewModel.BackgroundColor.Should().Be(Colors.White);
        }

        [Test]
        public void CandidateViewModel_GivenIsSelected_BackgroundColor_IsLightGreen()
        {
            var candidate = _electionDescription.Candidates.Single(c => c.ObjectId == "mary");
            var viewModel = new ContestSelectionListViewModel.CandidateViewModel(candidate)
            {
                IsSelected = true
            };

            viewModel.BackgroundColor.Should().Be(Colors.LightGreen);
        }

        private async Task<ContestSelectionListViewModel> CreateViewModel()
        {
            var viewModel = new ContestSelectionListViewModel(_navigationServiceMock.Object)
            {
                Parameter = _electionDescription
            };
            await viewModel.Load();

            return viewModel;
        }
    }
}
