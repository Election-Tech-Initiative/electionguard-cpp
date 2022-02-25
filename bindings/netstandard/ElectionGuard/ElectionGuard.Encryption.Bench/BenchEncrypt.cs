using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ElectionGuard.Encryption.Utils;

namespace ElectionGuard.Encryption.Bench
{

    public class BenchEncrypt : Fixture
    {
        ElementModQ nonce;
        ElGamalKeyPair keypair;
        Manifest manifest;
        InternalManifest internalManifest;
        CiphertextElectionContext context;
        EncryptionDevice device;
        EncryptionMediator mediator;
        PlaintextBallot ballot;

        public BenchEncrypt()
        {
            var secret = new ElementModQ("A9FA69F9686810ED82DAF9020FE80DFE0FC0FDCBF7FA55B93C811F0BA2650101");
            nonce = new ElementModQ("4BD3231DC17E9E84F5B0A5D2C4160C6A2299EDAE184C291E17709913B8F9CB40");
            keypair = ElGamalKeyPair.FromSecret(secret);
            manifest = ManifestGenerator.GetManifestFromFile();
            internalManifest = new InternalManifest(manifest);
            context = new CiphertextElectionContext(
                1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, internalManifest.ManifestHash);
            device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            mediator = new EncryptionMediator(internalManifest, context, device);

            ballot = BallotGenerator.GetFakeBallot(internalManifest);
            //Console.WriteLine(ballot.ToJson());
        }

        public override void Run()
        {
            Bench_Encrypt_BallotFull_NoProofCheck();
            Bench_Encrypt_BallotFull_WithProofCheck();
            Bench_Encrypt_Ballot_Compact_NoProofCheck();
            // TODO: Bench_Encrypt_Ballot_Compact_WithProofCheck();
        }

        public void Bench_Encrypt_BallotFull_NoProofCheck()
        {
            Console.WriteLine("Bench_Encrypt_BallotFull_NoProofCheck");
            Run(() =>
            {
                var ciphertext = Encrypt.Ballot(ballot, internalManifest, context, device.GetHash(), nonce, false);
            });
        }

        public void Bench_Encrypt_BallotFull_WithProofCheck()
        {
            Console.WriteLine("Bench_Encrypt_BallotFull_WithProofCheck");
            Run(() =>
            {
                var ciphertext = Encrypt.Ballot(ballot, internalManifest, context, device.GetHash(), nonce, true);
            });
        }

        public void Bench_Encrypt_Ballot_Compact_NoProofCheck()
        {
            Console.WriteLine("Bench_Encrypt_Ballot_Compact_NoProofCheck");
            Run(() =>
            {
                var ciphertext = Encrypt.CompactBallot(ballot, internalManifest, context, device.GetHash(), nonce, false);
            });
        }

        public void Bench_Encrypt_Ballot_Compact_WithProofCheck()
        {
            Console.WriteLine("Bench_Encrypt_Ballot_Compact_WithProofCheck");
            Run(() =>
            {
                var ciphertext = Encrypt.CompactBallot(ballot, internalManifest, context, device.GetHash(), nonce, true);
            });
        }
    }
}
