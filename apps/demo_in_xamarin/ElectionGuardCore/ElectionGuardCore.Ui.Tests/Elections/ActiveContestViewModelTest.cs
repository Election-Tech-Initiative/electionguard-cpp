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

        [SetUp]
        public void SetUp()
        {
            _electionServiceMock = new Mock<IElectionService>();
            _electionServiceMock.Setup(x => x.GetElectionDescription()).ReturnsAsync(_electionDescription);
        }

        [Test]
        public async Task OnLoad_LoadsElectionDescriptionFromService()
        {
            var viewModel = new ActiveContestViewModel(_electionServiceMock.Object);
            await viewModel.Load();

            viewModel.ElectionDescription.Should().Be(_electionDescription);
        }

        [Test]
        public async Task ElectionName_ShouldBeFirstElectionDescriptionName()
        {
            var viewModel = new ActiveContestViewModel(_electionServiceMock.Object);
            await viewModel.Load();

            viewModel.ElectionName.Should().Be("Hello");
        }

        [Test]
        public async Task ActiveContestName_ShouldBeFirstContestName()
        {
            var viewModel = new ActiveContestViewModel(_electionServiceMock.Object);
            await viewModel.Load();

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

            var viewModel = new ActiveContestViewModel(_electionServiceMock.Object);
            await viewModel.Load();

            viewModel.HasVotedInActiveContest.Should().Be(hasVoted);
            viewModel.CanVote.Should().Be(!hasVoted);
            viewModel.CannotVote.Should().Be(hasVoted);
        }
    }
}
