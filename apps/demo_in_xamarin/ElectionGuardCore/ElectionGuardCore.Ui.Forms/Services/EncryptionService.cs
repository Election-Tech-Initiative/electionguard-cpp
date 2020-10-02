using System.Threading;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Encryption;
using SDK = ElectionGuard;
using System.Collections.Generic;
using System.Linq;
using System;
using Newtonsoft.Json;
using System.Numerics;
using System.Diagnostics;
using Newtonsoft.Json.Serialization;
using Newtonsoft.Json.Converters;

namespace ElectionGuardCore.Ui.Forms.Services
{
    using local = ElectionGuardCore.Elections;
    public class EncryptionService : IEncryptionService
    {
        private readonly JsonSerializerSettings _serializerSettings;

        // TODO: generate securely & deterministically
        private ulong DeviceId = BitConverter.ToUInt64(Guid.NewGuid().ToByteArray(), 0);

        public EncryptionService()
        {
            _serializerSettings = new JsonSerializerSettings
            {
                ContractResolver = new DefaultContractResolver
                {
                    NamingStrategy = new SnakeCaseNamingStrategy()
                },
                Converters = new List<JsonConverter>
                {
                    new StringEnumConverter(typeof(SnakeCaseNamingStrategy)),
                    new ElementConverter()
                }
            };
        }

        private class InternalElectionDescription
        {
            public List<ContestDescription> Contests { get; set; }
            public string DescriptionHash { get; set; }
        }

        public CiphertextBallot EncryptBallot(
            ElectionDescription metadata, CiphertextElectionContext context,
            SelectionDescription selection, Candidate candidate)
        {
            // Only supports elections with a single contest

            if (selection.CandidateId != candidate.ObjectId)
            {
                throw new ArgumentException("Candidate must match selection");
            }

            // Fill in a ciphertext ballot
            PlaintextBallot plaintext = FillPlaintextBallotFromSingleSelection(metadata, selection);

            // serialize all the things into the SDK
            var hexContext = ToHex(context);
            var internalMetadata = new InternalElectionDescription
            {
                Contests = metadata.Contests,
                DescriptionHash = hexContext.DescriptionHash
            };

            var metadata_json = JsonConvert.SerializeObject(internalMetadata, _serializerSettings);
            
            var context_json = JsonConvert.SerializeObject(hexContext, _serializerSettings);
            

            var metadata_ = new SDK.InternalElectionDescription(metadata_json);
            var context_ = new SDK.CiphertextElectionContext(context_json);
            var device = new SDK.EncryptionDevice(DeviceId, "Some-Location-String");
            var mediator = new SDK.EncryptionMediator(metadata_, context_, device);

            // create an SDK representation of the plaintext ballot
            var json = JsonConvert.SerializeObject(plaintext, _serializerSettings);
            var plaintext_ = new SDK.PlaintextBallot(json);

            // encrypt
            var ciphertext_ = mediator.Encrypt(plaintext_);

            // convert the SDK ciphertext into a DTO object
            var ciphertext = JsonConvert.DeserializeObject<CiphertextBallot>(ciphertext_.ToJson(), _serializerSettings);
            return ciphertext;
        }

        private CiphertextElectionContext ToHex(CiphertextElectionContext context)
        {
            return new CiphertextElectionContext
            {
                ElgamalPublicKey = BigInteger.Parse(context.ElgamalPublicKey).ToString("X"),
                DescriptionHash = BigInteger.Parse(context.DescriptionHash).ToString("X"),
                CryptoBaseHash = BigInteger.Parse(context.CryptoBaseHash).ToString("X"),
                CryptoExtendedBaseHash = BigInteger.Parse(context.CryptoExtendedBaseHash).ToString("X"),
            };
        }

        private PlaintextBallot FillPlaintextBallotFromSingleSelection(
            ElectionDescription metadata, SelectionDescription selection)
        {
            Debug.WriteLine($"selection: {selection.ObjectId}");
            PlaintextBallot plaintext = new PlaintextBallot
            {
                ObjectId = Guid.NewGuid().ToString(),
                BallotStyle = metadata.BallotStyles.First().ObjectId,
                Contests = new List<PlaintextBallotContest>
                {
                    {
                        new PlaintextBallotContest
                        {
                            ObjectId = metadata.ActiveContest.ObjectId,
                            BallotSelections = new List<PlaintextBallotSelection>()
                        }
                    }
                }
            };
            foreach (var ballotSelection in metadata.ActiveContest.BallotSelections.OrderBy(b =>
                b.SequenceOrder))
            {
                var option = new PlaintextBallotSelection
                {
                    ObjectId = ballotSelection.ObjectId,
                    Vote = ballotSelection.ObjectId == selection.ObjectId ? "1" : "0",
                    IsPlaceholderSelection = false
                };
                Debug.WriteLine($"selection: {selection.ObjectId} vote: {option.Vote}");
                plaintext.Contests.First().BallotSelections.Add(option);
            }

            return plaintext;
        }
    }
}
