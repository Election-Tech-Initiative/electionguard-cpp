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

        private readonly ElectionDescription _electionDescription = new ElectionDescription
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
            _electionServiceMock.Setup(x => x.GetElectionDescription()).ReturnsAsync(_electionDescription);
            _electionServiceMock.Setup(x => x.GetCiphertextElectionContext()).ReturnsAsync(_electionContext);

            _navigationServiceMock = new Mock<INavigationService>();
        }

        [Test]
        public async Task Load_LoadsElectionDescriptionFromService()
        {
            var viewModel = await CreateViewModel();
            viewModel.ElectionDescription.Should().Be(_electionDescription);
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
        [TestCase("contest1", false)]
        [TestCase("contest1", true)]
        public async Task HasVotedInActiveContest_ReflectsElectionServiceVotes(string contestId, bool hasVoted)
        {
            if (contestId != null)
            {
                _electionServiceMock.Setup(x => x.Votes).Returns(new Dictionary<string, bool> {{contestId, hasVoted}});
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
                    a.ElectionDescription == _electionDescription
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
