using System.Threading.Tasks;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Ui.Elections;
using FluentAssertions;
using Moq;
using NUnit.Framework;

namespace ElectionGuardCore.Ui.Tests.Elections
{
    [TestFixture]
    public class SelectionSubmittedViewModelTest
    {
        private Mock<INavigationService> _navigationServiceMock;

        private readonly ElectionDescription _electionDescription = new ElectionDescription();
        private readonly CiphertextElectionContext _context = new CiphertextElectionContext();

        [SetUp]
        public void SetUp()
        {
            _navigationServiceMock = new Mock<INavigationService>();
        }

        [Test]
        public async Task GivenCastBallot_CanReVote_IsFalse()
        {
            var viewModel = await CreateViewModel(true);
            viewModel.CanReVote.Should().BeFalse();
        }

        [Test]
        public async Task GivenSpoiledBallot_CanReVote_IsTrue()
        {
            var viewModel = await CreateViewModel(false);
            viewModel.CanReVote.Should().BeTrue();
        }

        [Test]
        public async Task CloseCommand_PopsToRoot()
        {
            var viewModel = await CreateViewModel(default);
            viewModel.CloseCommand.Execute(null);

            _navigationServiceMock.Verify(x => x.PopToRoot());
        }

        [Test]
        public async Task ReVoteCommand_NavigatesBackToContestSelectionListPage()
        {
            var viewModel = await CreateViewModel(default);
            viewModel.ReVoteCommand.Execute(null);

            _navigationServiceMock.Verify(x => x.PopToRoot());
            _navigationServiceMock.Verify(x => x.Push(NavigationPaths.ContestSelectionListPage,
                It.Is<ContestSelectionListViewModel.ContestSelectionListArgs>(a =>
                    a.ElectionDescription == _electionDescription && a.ElectionContext == _context)));
        }

        private async Task<SelectionSubmittedViewModel> CreateViewModel(bool voteCast)
        {
            var viewModel = new SelectionSubmittedViewModel(_navigationServiceMock.Object)
            {
                Parameter = new SelectionSubmittedViewModel.SelectionSubmittedArgs(voteCast, _electionDescription,
                    _context)
            };
            await viewModel.Load();

            return viewModel;
        }
    }
}
