using System.Collections;
using System.Globalization;

namespace ElectionGuard.Encryption.Cli.Encrypt
{
    internal class EncryptCommand
    {
        public static Task Encrypt(EncryptOptions encryptOptions)
        {
            try
            {
                var encryptCommand = new EncryptCommand();
                return encryptCommand.EncryptInternal(encryptOptions);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                throw;
            }
        }

        private async Task EncryptInternal(EncryptOptions encryptOptions)
        {
            encryptOptions.Validate();
            if (string.IsNullOrEmpty(encryptOptions.Context)) throw new ArgumentNullException(nameof(encryptOptions.Context));
            if (string.IsNullOrEmpty(encryptOptions.Manifest)) throw new ArgumentNullException(nameof(encryptOptions.Manifest));
            if (string.IsNullOrEmpty(encryptOptions.BallotsDir)) throw new ArgumentNullException(nameof(encryptOptions.BallotsDir));

            var encryptionMediator = await GetEncryptionMediator(encryptOptions.Context, encryptOptions.Manifest);
            var ballotFiles = GetBallotFiles(encryptOptions.BallotsDir);

            foreach (var ballotFile in ballotFiles)
            {
                Console.WriteLine($"Parsing: {ballotFile}");
                var plaintextBallot = await GetPlaintextBallot(ballotFile);
                var spoiledDeviceIds = encryptOptions.SpoiledDeviceIds.ToList();
                var submittedBallot = EncryptAndSubmit(encryptionMediator, plaintextBallot, spoiledDeviceIds, ballotFile);
                await WriteSubmittedBallot(encryptOptions, ballotFile, submittedBallot);
            }
            Console.WriteLine("Parsing Complete");
        }

        private static SubmittedBallot EncryptAndSubmit(EncryptionMediator encryptionMediator,
            PlaintextBallot plaintextBallot, IList<string> spoiledDeviceIds, string ballotFile)
        {
            var ciphertextBallot = encryptionMediator.Encrypt(plaintextBallot);
            var shouldSpoil = spoiledDeviceIds.Contains(ciphertextBallot.ObjectId, StringComparer.OrdinalIgnoreCase) ||
                              spoiledDeviceIds.Contains(Path.GetFileNameWithoutExtension(ballotFile), StringComparer.OrdinalIgnoreCase);
            var state = shouldSpoil ? BallotBoxState.Spoiled : BallotBoxState.Cast;
            var submittedBallot = new SubmittedBallot(ciphertextBallot, state);
            return submittedBallot;
        }

        private static async Task<EncryptionMediator> GetEncryptionMediator(string contextFile, string manifestFile)
        {
            var context = await GetContext(contextFile);
            var internalManifest = await GetInternalManifest(manifestFile);
            var device = GetDevice();
            var encryptionMediator = new EncryptionMediator(internalManifest, context, device);
            return encryptionMediator;
        }

        private static IEnumerable<string> GetBallotFiles(string directory)
        {
            return Directory.EnumerateFiles(directory);
        }

        private static async Task<PlaintextBallot> GetPlaintextBallot(string ballotFile)
        {
            var ballot = await File.ReadAllTextAsync(ballotFile);
            var plaintextBallot = new PlaintextBallot(ballot);
            return plaintextBallot;
        }

        private static async Task WriteSubmittedBallot(EncryptOptions encryptOptions, string ballotFile,
            SubmittedBallot submittedBallot)
        {
            var ballotJson = submittedBallot.ToJson();
            var outFile = Path.Join(encryptOptions.OutDir, Path.GetFileName(ballotFile));
            await File.WriteAllTextAsync(outFile, ballotJson);
        }

        private static EncryptionDevice GetDevice()
        {
            return new EncryptionDevice(12345UL, 23456UL, 34567UL, "Location");
        }

        private static async Task<InternalManifest> GetInternalManifest(string manifestFile)
        {
            var manifestJson = await File.ReadAllTextAsync(manifestFile);
            var manifest = new Manifest(manifestJson);
            return new InternalManifest(manifest);
        }

        private static async Task<CiphertextElectionContext> GetContext(string contextFile)
        {
            var contextJson = await File.ReadAllTextAsync(contextFile);
            return new CiphertextElectionContext(contextJson);
        }
    }
}
