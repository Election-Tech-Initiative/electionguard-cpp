using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using NUnit.Framework;

namespace ElectionGuard.Encryption.Tests
{
    [TestFixture]
    class TestPrecompute
    {
        readonly int MAX_COMPLETE_DELAY = 7000;
        readonly int SMALL_BUFFER_SIZE = 10;
        readonly int DEFAULT_BUFFER_SIZE = 5000;
        readonly int LARGE_BUFFER_SIZE = 6000;

        [Test]
        public void Test_Precompute_Status_NoStarted()
        {
            Precompute precompute = new Precompute();

            var status = precompute.GetStatus();

            Assert.AreEqual(PrecomputeState.NotStarted, status.CurrentState);
        }

        [Test]
        public void Test_Precompute_Status_Running()
        {
            var waitHandle = new AutoResetEvent(false);

            Precompute precompute = new Precompute();
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);

            precompute.CompletedEvent += (PrecomputeStatus completedStatus) =>
            {
                Assert.AreEqual(PrecomputeState.UserStopped, completedStatus.CurrentState);
                waitHandle.Set();
            };
            precompute.StartPrecomputeAsync(keypair.PublicKey, LARGE_BUFFER_SIZE);
            var runningStatus = precompute.GetStatus();
            precompute.StopPrecompute();

            var waitReturn = waitHandle.WaitOne(MAX_COMPLETE_DELAY);

            int count = -1;
            int queue_size = -1;
            precompute.GetProgress(out count, out queue_size);
            var status = precompute.GetStatus();

            Assert.AreEqual(LARGE_BUFFER_SIZE, queue_size);
            Assert.AreEqual(PrecomputeState.Running, runningStatus.CurrentState);
            Assert.AreEqual(true, waitReturn);
        }

        [Test]
        public void Test_Precompute_Status_Stopped()
        {
            var waitHandle = new AutoResetEvent(false);

            Precompute precompute = new Precompute();
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);

            precompute.CompletedEvent += (PrecomputeStatus completedStatus) =>
            {
                Assert.AreEqual(PrecomputeState.UserStopped, completedStatus.CurrentState);
                waitHandle.Set();
            };
            precompute.StartPrecomputeAsync(keypair.PublicKey);
            var statusRunning = precompute.GetStatus();
            precompute.StopPrecompute();

            var waitReturn = waitHandle.WaitOne(MAX_COMPLETE_DELAY);

            int count = -1;
            int queue_size = -1;
            precompute.GetProgress(out count, out queue_size);
            var status = precompute.GetStatus();

            Assert.AreEqual(DEFAULT_BUFFER_SIZE, queue_size);
            Assert.AreNotEqual(-1, count);

            Assert.AreEqual(PrecomputeState.Running, statusRunning.CurrentState);
            Assert.AreEqual(PrecomputeState.UserStopped, status.CurrentState);

            Assert.AreEqual(true, waitReturn);
        }

        [Test]
        public void Test_Precompute_Status_Complete()
        {
            var waitHandle = new AutoResetEvent(false);

            Precompute precompute = new Precompute();
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);

            precompute.CompletedEvent += (PrecomputeStatus completedStatus) =>
            {
                Assert.AreEqual(SMALL_BUFFER_SIZE, completedStatus.CompletedExponentiationsCount);
                Assert.AreEqual(1.0, completedStatus.Percentage);
                Assert.AreEqual(PrecomputeState.Completed, completedStatus.CurrentState);
                waitHandle.Set();
            };
            precompute.StartPrecomputeAsync(keypair.PublicKey, SMALL_BUFFER_SIZE);

            var waitReturn = waitHandle.WaitOne(MAX_COMPLETE_DELAY);

            int count = -1;
            int queue_size = -1;
            precompute.GetProgress(out count, out queue_size);
            var status = precompute.GetStatus();

            Assert.AreEqual(SMALL_BUFFER_SIZE, queue_size);
            Assert.AreEqual(SMALL_BUFFER_SIZE, count);

            Assert.AreEqual(PrecomputeState.Completed, status.CurrentState);
            Assert.AreEqual(true, waitReturn);
        }

    }
}
