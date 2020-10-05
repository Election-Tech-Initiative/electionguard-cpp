using System.Collections.Generic;
using System.Threading.Tasks;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Ui.Elections;
using Moq;
using NUnit.Framework;

namespace ElectionGuardCore.Ui.Tests.Elections
{
    [TestFixture]
    public class SelectionVerificationViewModelTest
    {
        private Mock<IClipboardService> _clipboardServiceMock;
        private Mock<IAlertService> _alertServiceMock;
        private Mock<IElectionService> _electionServiceMock;
        private Mock<INavigationService> _navigationServiceMock;

        private readonly Election _election = new Election
        {
            ElectionDescription = new ElectionDescription
            {
                Contests = new List<ContestDescription>
                {
                    new ContestDescription
                    {
                        ObjectId = "contest1"
                    }
                }
            }
        };
        private readonly CiphertextElectionContext _context = new CiphertextElectionContext();
        private readonly CiphertextBallot _ciphertextBallot = new CiphertextBallot
        {
            TrackingCode = "12345"  // I've got the same combination on my luggage!
        };

        [SetUp]
        public void SetUp()
        {
            _clipboardServiceMock = new Mock<IClipboardService>();
            _alertServiceMock = new Mock<IAlertService>();
            _electionServiceMock = new Mock<IElectionService>();
            _navigationServiceMock = new Mock<INavigationService>();
        }

        [Test]
        public async Task CopyTrackingCodeCommand_CopiesTrackingCode()
        {
            var viewModel = await CreateViewModel();
            viewModel.CopyTrackingCodeCommand.Execute(null);

            _clipboardServiceMock.Verify(x => x.Copy(_ciphertextBallot.TrackingCode));
        }

        [Test]
        public async Task CastCommand_PassesTrueToSelectionSubmittedPage()
        {
            var viewModel = await CreateViewModel();
            viewModel.CastCommand.Execute(null);

            _navigationServiceMock.Verify(x => x.Push(NavigationPaths.SelectionSubmittedPage,
                It.Is<SelectionSubmittedViewModel.SelectionSubmittedArgs>(a =>
                    a.VoteCast && a.Election == _election &&
                    a.CiphertextElectionContext == _context)));
        }

        [Test]
        public async Task SpoilCommand_PassesFalseToSelectionSubmittedPage()
        {
            var viewModel = await CreateViewModel();
            viewModel.SpoilCommand.Execute(null);

            _navigationServiceMock.Verify(x => x.Push(NavigationPaths.SelectionSubmittedPage,
                It.Is<SelectionSubmittedViewModel.SelectionSubmittedArgs>(a =>
                    !a.VoteCast && a.Election == _election &&
                    a.CiphertextElectionContext == _context)));
        }

        private async Task<SelectionVerificationViewModel> CreateViewModel()
        {
            var viewModel = new SelectionVerificationViewModel(_clipboardServiceMock.Object, _alertServiceMock.Object,
                _electionServiceMock.Object, _navigationServiceMock.Object)
            {
                Parameter = new SelectionVerificationViewModel.SelectionVerificationArgs(_election, _context,
                    _ciphertextBallot)
            };
            await viewModel.Load();

            return viewModel;
        }
    }
}
