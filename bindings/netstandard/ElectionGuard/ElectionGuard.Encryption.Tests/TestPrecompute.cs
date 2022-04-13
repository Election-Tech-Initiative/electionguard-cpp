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
            Precompute precompute = new Precompute();
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);

            CancellationTokenSource cts = new CancellationTokenSource();

            precompute.StartPrecomputeAsync(keypair.PublicKey, cts.Token, 5000);
            var status = precompute.GetStatus();
            precompute.StopPrecompute();
            Thread.Sleep(1000);

            Assert.AreEqual(PrecomputeState.Running, status.CurrentState);
        }

        [Test]
        public void Test_Precompute_Status_Stopped()
        {
            Precompute precompute = new Precompute();
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);

            CancellationTokenSource cts = new CancellationTokenSource();

            precompute.StartPrecomputeAsync(keypair.PublicKey, cts.Token);
            var statusRunning = precompute.GetStatus();
            precompute.StopPrecompute();
            Thread.Sleep(1000);

            int count = -1;
            int queue_size = -1;
            precompute.GetProgress(out count, out queue_size);
            var status = precompute.GetStatus();

            Assert.AreEqual(5000, queue_size);

            Assert.AreEqual(PrecomputeState.Running, statusRunning.CurrentState);
            Assert.AreEqual(PrecomputeState.UserStopped, status.CurrentState);
        }

        [Test]
        public void Test_Precompute_Status_Complete()
        {
            Precompute precompute = new Precompute();
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);

            CancellationTokenSource cts = new CancellationTokenSource();

            precompute.StartPrecomputeAsync(keypair.PublicKey, cts.Token, 10);
            Thread.Sleep(2000);
            precompute.StopPrecompute();
            Thread.Sleep(1000);

            int count = -1;
            int queue_size = -1;
            precompute.GetProgress(out count, out queue_size);
            var status = precompute.GetStatus();

            Assert.AreEqual(10, queue_size);
            Assert.AreEqual(10, count);

            Assert.AreEqual(PrecomputeState.Completed, status.CurrentState);
        }

    }
}
