using System;
using System.Diagnostics;
using NUnit.Framework;

namespace ElectionGuard.Encrypt.Tests
{
    [TestFixture]
    public class TestBallot
    {
        [Test]
        public void Test_Ballot_Property_Getters()
        {
            // Arrange
            const string data = @"{""style_id"":""ballot-style-1"",""contests"":[{""ballot_selections"":[{""object_id"":""contest-1-selection-1-id"",""vote"":1},{""object_id"":""contest-1-selection-2-id"",""vote"":0},{""object_id"":""contest-1-selection-3-id"",""vote"":0}],""object_id"":""contest-1-id""},{""ballot_selections"":[{""object_id"":""contest-2-selection-1-id"",""vote"":1},{""object_id"":""contest-2-selection-2-id"",""vote"":0}],""object_id"":""contest-2-id""}],""object_id"":""ballot-id-123""}";

            // Act
            var ballot = new PlaintextBallot(data);

            // Assert

            // verify the ballot property accessors
            var ballotId = ballot.ObjectId;
            Assert.That(ballotId == "ballot-id-123");

            // iterate over the contests in the ballot
            for (ulong i = 0; i < ballot.ContestsSize; i++)
            {
                var contest = ballot.GetContestAt(i);

                // verify the contest property accessors
                var contestId = contest.ObjectId;
                Assert.That(contestId == $"contest-{i + 1}-id");

                // iterate over the selections in the contest
                for (ulong j = 0; j < contest.SelectionsSize; j++)
                {
                    var selection = contest.GetSelectionAt(j);

                    // verify the selection property accessors
                    var selectionId = selection.ObjectId;
                    Assert.That(selectionId == $"contest-{i + 1}-selection-{j + 1}-id");
                }
            }

            var bson = ballot.ToBson();
            var fromBson = new PlaintextBallot(bson, BinarySerializationEncoding.BSON);
            Assert.That(ballot.ObjectId == fromBson.ObjectId);
        }

        [Test]
        public void Test_Plaintext_Ballot_Selection_Is_Valid()
        {
            // Arrange
            const string objectId = "some-object-id";

            // Act
            var subject = new PlaintextBallotSelection(objectId, 1);

            // Assert
            Assert.That(subject.IsValid(objectId) == true);
        }

        [Test]
        public void Test_Plaintext_Ballot_Selection_Is_InValid_With_Different_objectIds()
        {
            // Arrange
            const string objectId = "some-object-id";

            // Act
            var subject = new PlaintextBallotSelection(objectId, 1);

            // Assert
            Assert.That(subject.IsValid("some-other-object-id") == false);
        }

        [Test]
        public void Test_Plaintext_Ballot_Selection_Is_InValid_With_overvote()
        {
            // Arrange
            const string objectId = "some-object-id";

            // Act
            var subject = new PlaintextBallotSelection(objectId, 2);

            // Assert
            Assert.That(subject.IsValid(objectId) == false);
        }
    }
}
