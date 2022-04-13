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

            precompute.StartPrecomputeAsync(keypair.PublicKey, cts.Token);
            var status = precompute.GetStatus();
            precompute.StopPrecompute();

            Assert.AreEqual(PrecomputeState.Running, status.CurrentState);
        }

        [Test]
        public void Test_Precompute_Status_Stopped()
        {
            Precompute precompute = new Precompute();
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);

            CancellationTokenSource cts = new CancellationTokenSource();

            precompute.StartPrecomputeAsync(keypair.PublicKey, cts.Token);
            precompute.StopPrecompute();
            Thread.Sleep(1000);

            var status = precompute.GetStatus();

            Assert.AreEqual(PrecomputeState.UserStopped, status.CurrentState);
        }

    }
}
