using System;
using System.Threading.Tasks;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Encryption;
using ElectionGuardCore.Ui.Elections;
using Moq;
using NUnit.Framework;

namespace ElectionGuardCore.Ui.Tests.Elections
{
    [TestFixture]
    public class ReviewSelectionViewModelTest
    {
        private Mock<IAlertService> _alertServiceMock;
        private Mock<IEncryptionService> _encryptionServiceMock;
        private Mock<INavigationService> _navigationServiceMock;

        private readonly Candidate _candidate = new Candidate();
        private readonly SelectionDescription _selection = new SelectionDescription();

        private readonly Election _election = new Election
        {
            ElectionDescription = new ElectionDescription()
        };

        private readonly CiphertextElectionContext _context = new CiphertextElectionContext();
        private readonly CiphertextBallot _ciphertextBallot = new CiphertextBallot();

        [SetUp]
        public void SetUp()
        {
            _alertServiceMock = new Mock<IAlertService>();
            _alertServiceMock
                .Setup(x => x.Alert(It.IsAny<string>(), It.IsAny<string>(), It.IsAny<string>(), It.IsAny<string>()))
                .ReturnsAsync(true);
            _alertServiceMock
                .Setup(x => x.ShowBusy(It.IsAny<string>(), It.IsAny<Action>(), It.IsAny<Action>()))
                .Callback<string, Action, Action>((label, busyAction, onComplete) =>
                {
                    busyAction();
                    onComplete();
                });

            _encryptionServiceMock = new Mock<IEncryptionService>();
            _encryptionServiceMock.Setup(x => x.EncryptBallot(It.IsAny<ElectionDescription>(),
                It.IsAny<CiphertextElectionContext>(),
                It.IsAny<SelectionDescription>(),
                It.IsAny<Candidate>())).Returns(_ciphertextBallot);

            _navigationServiceMock = new Mock<INavigationService>();
        }

        [Test]
        public async Task EncryptSelectionCommand_CallsEncryptionService()
        {
            var viewModel = await CreateViewModel();
            viewModel.EncryptSelectionCommand.Execute(null);

            _encryptionServiceMock.Verify(x =>
                x.EncryptBallot(_election.ElectionDescription, _context, _selection, _candidate));
        }

        [Test]
        public async Task EncryptSelectionCommand_PassesCiphertextBallotToSelectionVerificationPage()
        {
            var viewModel = await CreateViewModel();
            viewModel.EncryptSelectionCommand.Execute(null);

            _navigationServiceMock.Verify(x => x.Push(NavigationPaths.SelectionVerificationPage,
                It.Is<SelectionVerificationViewModel.SelectionVerificationArgs>(a =>
                    a.Election == _election && a.CiphertextElectionContext == _context &&
                    a.CiphertextBallot == _ciphertextBallot)));
        }

        private async Task<ReviewSelectionViewModel> CreateViewModel()
        {
            var viewModel = new ReviewSelectionViewModel(_alertServiceMock.Object, _encryptionServiceMock.Object,
                _navigationServiceMock.Object)
            {
                Parameter = new ReviewSelectionViewModel.ReviewSelectionArgs(
                    _election, _context, _selection, _candidate)
            };
            await viewModel.Load();

            return viewModel;
        }
    }
}
