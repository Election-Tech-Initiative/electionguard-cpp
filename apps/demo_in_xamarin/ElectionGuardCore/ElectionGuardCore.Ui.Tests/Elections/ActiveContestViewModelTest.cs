using System.Collections.Generic;
using System.Threading.Tasks;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Ui.Elections;
using FluentAssertions;
using Moq;
using NUnit.Framework;

namespace ElectionGuardCore.Ui.Tests.Elections
{
    [TestFixture]
    public class ActiveContestViewModelTest
    {
        private Mock<IElectionService> _electionServiceMock;
        private Mock<INavigationService> _navigationServiceMock;

        private readonly Election _election = new Election
        {
            Id = "election1",
            ElectionDescription = new ElectionDescription
            {
                Name = new InternationalizedText
                {
                    Text = new List<LanguageText>
                    {
                        new LanguageText
                        {
                            Language = "en",
                            Value = "Hello"
                        },
                        new LanguageText
                        {
                            Language = "es",
                            Value = "Hola"
                        }
                    }
                },
                Contests = new List<ContestDescription>
                {
                    new ContestDescription
                    {
                        ObjectId = "contest1",
                        Name = "Contest1"
                    },
                    new ContestDescription
                    {
                        ObjectId = "contest2",
                        Name = "Contest2"
                    }
                }
            }
        };

        private readonly CiphertextElectionContext _electionContext = new CiphertextElectionContext
        {
            NumberOfGuardians = 5,
            Quorum = 3,
            ElgamalPublicKey = "12345",
            DescriptionHash = "23456",
            CryptoBaseHash = "34567",
            CryptoExtendedBaseHash = "45678"
        };

        [SetUp]
        public void SetUp()
        {
            _electionServiceMock = new Mock<IElectionService>();
            _electionServiceMock.Setup(x => x.GetElection()).ReturnsAsync(_election);
            _electionServiceMock.Setup(x => x.GetCiphertextElectionContext(_election.Id))
                .ReturnsAsync(_electionContext);

            _navigationServiceMock = new Mock<INavigationService>();
        }

        [Test]
        public async Task Load_LoadsElectionFromService()
        {
            var viewModel = await CreateViewModel();
            viewModel.Election.Should().Be(_election);
        }

        [Test]
        public async Task ElectionName_ShouldBeFirstElectionDescriptionName()
        {
            var viewModel = await CreateViewModel();
            viewModel.ElectionName.Should().Be("Hello");
        }

        [Test]
        public async Task ActiveContestName_ShouldBeFirstContestName()
        {
            var viewModel = await CreateViewModel();
            viewModel.ActiveContestName.Should().Be("Contest1");
        }

        [Test]
        [TestCase(null, false)]
        [TestCase("election1", false)]
        [TestCase("election1", true)]
        public async Task HasVotedInActiveContest_ReflectsElectionServiceVotes(string electionId, bool hasVoted)
        {
            if (electionId != null)
            {
                _electionServiceMock.Setup(x => x.HasVoted(electionId)).ReturnsAsync(hasVoted);
            }

            var viewModel = await CreateViewModel();

            viewModel.HasVotedInActiveContest.Should().Be(hasVoted);
            viewModel.CanVote.Should().Be(!hasVoted);
            viewModel.CannotVote.Should().Be(hasVoted);
        }

        [Test]
        public async Task BeginVoteCommand_PassesElectionToVoteOptionListPage()
        {
            var viewModel = await CreateViewModel();
            viewModel.BeginVoteCommand.Execute(null);

            _navigationServiceMock.Verify(x =>
                x.Push(NavigationPaths.ContestSelectionListPage,
                It.Is<ContestSelectionListViewModel.ContestSelectionListArgs>(a =>
                    a.Election == _election
                    && a.ElectionContext == _electionContext)));
        }

        private async Task<ActiveContestViewModel> CreateViewModel()
        {
            var viewModel = new ActiveContestViewModel(_electionServiceMock.Object, _navigationServiceMock.Object);
            await viewModel.Load();

            return viewModel;
        }
    }
}
