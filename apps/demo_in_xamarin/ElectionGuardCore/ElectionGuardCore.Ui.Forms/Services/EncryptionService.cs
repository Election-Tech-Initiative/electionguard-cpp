using System.Threading;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Encryption;
using SDK = ElectionGuard;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System;
using Newtonsoft.Json;
using System.Numerics;
using Newtonsoft.Json.Serialization;
using Newtonsoft.Json.Converters;

namespace ElectionGuardCore.Ui.Forms.Services
{
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
            var hexContext = context.ToHex();
            var internalMetadata = new InternalElectionDescription
            {
                Contests = metadata.Contests,
                DescriptionHash = hexContext.DescriptionHash
            };

            var serializedMetadata = JsonConvert.SerializeObject(internalMetadata, _serializerSettings);
            var serializedContext = JsonConvert.SerializeObject(hexContext, _serializerSettings);      

            var sdkMetadata = new SDK.InternalElectionDescription(serializedMetadata);
            var sdkContext = new SDK.CiphertextElectionContext(serializedContext);
            var device = new SDK.EncryptionDevice(DeviceId, "Some-Location-String");
            var mediator = new SDK.EncryptionMediator(sdkMetadata, sdkContext, device);

            // create an SDK representation of the plaintext ballot
            var serializedPlaintextBallot = JsonConvert.SerializeObject(plaintext, _serializerSettings);
            var sdkPlaintextBallot = new SDK.PlaintextBallot(serializedPlaintextBallot);

            // encrypt
            var ciphertext_ = mediator.Encrypt(sdkPlaintextBallot);

            if(!ciphertext_.IsValidEncryption(
                sdkContext.DescriptionHash, sdkContext.ElGamalPublicKey, sdkContext.CryptoExtendedBaseHash))
            {
                Debug.WriteLine($"Error encrypting ballot {sdkPlaintextBallot.ObjectId}");
                return null;
            }

            // convert the SDK ciphertext into a DTO object
            var ciphertext = JsonConvert.DeserializeObject<CiphertextBallot>(
                ciphertext_.ToJson(), _serializerSettings);
            return ciphertext;
        }

        private PlaintextBallot FillPlaintextBallotFromSingleSelection(
            ElectionDescription metadata, SelectionDescription selection)
        {
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
                plaintext.Contests.First().BallotSelections.Add(option);
            }

            return plaintext;
        }
    }
}
