using System;
using ElectionGuard.Encryption.Utils;
using NUnit.Framework;

namespace ElectionGuard.Encryption.Tests
{
    [TestFixture]
    public class TestChaumPedersen
    {
        [Test]
        public void Test_DisjunctiveChaumPedersen()
        {
            var nonce = new ElementModQ(Constants.ONE_MOD_Q.Data);
            var keyPair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            const ulong vote = 0UL;
            var ciphertext = Elgamal.Encrypt(vote, nonce, keyPair.PublicKey);

            var proof = new DisjunctiveChaumPedersenProof(
                ciphertext, nonce, keyPair.PublicKey, new ElementModQ(Constants.ONE_MOD_Q.Data), vote);

            Assert.That(proof.IsValid(ciphertext, keyPair.PublicKey, Constants.ONE_MOD_Q));
        }

        [Test]
        public void Test_DisjunctiveChaumPedersen_deterministic()
        {
            var nonce = Constants.ONE_MOD_Q;
            var seed = Constants.TWO_MOD_Q;
            var keyPair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            const ulong vote = 0UL;
            var ciphertext = Elgamal.Encrypt(vote, nonce, keyPair.PublicKey);

            var proof = new DisjunctiveChaumPedersenProof(
                ciphertext, nonce, keyPair.PublicKey, Constants.ONE_MOD_Q, seed, vote);

            Assert.That(proof.IsValid(ciphertext, keyPair.PublicKey, Constants.ONE_MOD_Q));
        }
    }
}
