using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ElectionGuardInterop;

namespace ElectionGuard.Bench
{
    
    public static class BenchEncrypt
    {
        static readonly string plaintext_json = "{\"object_id\":\"some-external-id-string-123\",\"style_id\":\"some-ballot-style-id\",\"contests\":[{\"object_id\":\"some-referendum-contest-object-id\",\"ballot_selections\":[{\"object_id\":\"some-candidate-id-1\",\"vote\":1}]}]}";
        static readonly string manifest_json = "{\"ballot_styles\":[{\"geopolitical_unit_ids\":[\"some-geopoltical-unit-id\"],\"object_id\":\"some-ballot-style-id\"}],\"candidates\":[{\"object_id\":\"some-candidate-id-1\"},{\"object_id\":\"some-candidate-id-2\"},{\"object_id\":\"some-candidate-id-3\"}],\"contests\":[{\"ballot_selections\":[{\"candidate_id\":\"some-candidate-id-1\",\"object_id\":\"some-object-id-affirmative\",\"sequence_order\":0},{\"candidate_id\":\"some-candidate-id-2\",\"object_id\":\"some-object-id-negative\",\"sequence_order\":1}],\"electoral_district_id\":\"some-geopoltical-unit-id\",\"name\":\"some-referendum-contest-name\",\"number_elected\":1,\"object_id\":\"some-referendum-contest-object-id\",\"sequence_order\":0,\"vote_variation\":\"one_of_m\"},{\"ballot_selections\":[{\"candidate_id\":\"some-candidate-id-1\",\"object_id\":\"some-object-id-candidate-1\",\"sequence_order\":0},{\"candidate_id\":\"some-candidate-id-2\",\"object_id\":\"some-object-id-candidate-2\",\"sequence_order\":1},{\"candidate_id\":\"some-candidate-id-3\",\"object_id\":\"some-object-id-candidate-3\",\"sequence_order\":2}],\"electoral_district_id\":\"some-geopoltical-unit-id\",\"name\":\"some-candidate-contest-name\",\"number_elected\":2,\"object_id\":\"some-candidate-contest-object-id\",\"sequence_order\":1,\"vote_variation\":\"one_of_m\"}],\"election_scope_id\":\"some-scope-id\",\"end_date\":\"2021-02-04T13:30:10Z\",\"geopolitical_units\":[{\"name\":\"some-gp-unit-name\",\"object_id\":\"some-geopoltical-unit-id\",\"type\":\"unknown\"}],\"parties\":[{\"object_id\":\"some-party-id-1\"},{\"object_id\":\"some-party-id-2\"}],\"start_date\":\"2021-02-04T13:30:10Z\",\"type\":\"unknown\"}";

        public static bool Bench_Encrypt_Ballot_Simple()
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            // Configure the election context
            var keypair = ElGamalKeyPair.FromSecret(Constants.TWO_MOD_Q);
            var description = new Manifest(manifest_json);
            var manifest = new InternalManifest(description);
            var context = new CiphertextElectionContext(
                1UL, 1UL, keypair.PublicKey, Constants.TWO_MOD_Q, manifest.ManifestHash);
            var device = new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
            var mediator = new EncryptionMediator(manifest, context, device);

            var ballot = new PlaintextBallot(plaintext_json);

            // Act
            var ciphertext = mediator.Encrypt(ballot, shouldVerifyProofs: false);

            // json serialization
            var json = ciphertext.Json;
            var fromJson = new CiphertextBallot(json);

            // binary serialization
            var bson = ciphertext.Bson;
            var fromBson = CiphertextBallot.FromBson(bson);

            sw.Stop();
            Console.WriteLine("Elapsed={0}", sw.Elapsed);
            return true;
        }
    }
}
