using System;
using NUnit.Framework;

namespace ElectionGuard.Tests
{
    public class Tests
    {
        [SetUp]
        public void Setup()
        {
            
        }

        [Test]
        public void Test_Ballot_Serialization()
        {
            string json = @"{""ballot_style"":""ballot-style-1"",""contests"":[{""ballot_selections"":[{""object_id"":""contest-1-selection-1-id"",""vote"":""1""},{""object_id"":""contest-1-selection-2-id"",""vote"":""0""},{""object_id"":""contest-1-selection-3-id"",""vote"":""0""}],""object_id"":""contest-1-id""},{""ballot_selections"":[{""object_id"":""contest-2-selection-1-id"",""vote"":""1""},{""object_id"":""contest-2-selection-2-id"",""vote"":""0""}],""object_id"":""contest-2-id""}],""object_id"":""ballot-id-123""}";

            var ballot = new PlaintextBallot(json);
            var objectId = ballot.ObjectId;

            Assert.That(objectId == "ballot-id-123");

        }
    }
}
