using NUnit.Framework;

using ElectionGuardInterop;

namespace ElectionGuard.Tests
{
    [TestFixture]
    class TestBallot
    {
        string data = @"{""style_id"":""ballot-style-1"",""contests"":[{""ballot_selections"":[{""object_id"":""contest-1-selection-1-id"",""vote"":1},{""object_id"":""contest-1-selection-2-id"",""vote"":0},{""object_id"":""contest-1-selection-3-id"",""vote"":0}],""object_id"":""contest-1-id""},{""ballot_selections"":[{""object_id"":""contest-2-selection-1-id"",""vote"":1},{""object_id"":""contest-2-selection-2-id"",""vote"":0}],""object_id"":""contest-2-id""}],""object_id"":""ballot-id-123""}";

        [Test]
        public void Test_Ballot_Property_Getters()
        {
            // Act
            var ballot = new PlaintextBallot(data);

            // Assert

            // verify the ballot property accessors
            var ballotId = ballot.ObjectId;
            Assert.That(ballotId == "ballot-id-123");

            int index = 0;
            // iterate over the contests in the ballot
            foreach (var contest in ballot.Contests)
            {
                // verify the contest property accessors
                var contestId = contest.ObjectId;
                Assert.That(contestId == $"contest-{index + 1}-id");

                var jindex = 0;
                // iterate over the selections in the contest
                foreach (var selection in contest.Selections)
                {
                    // verify the selection property accessors
                    var selectionId = selection.ObjectId;
                    Assert.That(selectionId == $"contest-{index + 1}-selection-{jindex + 1}-id");
                    jindex++;
                }

                index++;
            }

            var bson = ballot.Bson;
            var fromBson = PlaintextBallot.FromBson(bson);
            Assert.That(ballot.ObjectId == fromBson.ObjectId);
        }
    }
}
