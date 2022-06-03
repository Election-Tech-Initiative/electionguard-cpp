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

            var encryptionMediator = await GetEncryptionMediator(encryptOptions);
            var ballotFiles = GetBallotFiles(encryptOptions.BallotsDir);

            foreach (var ballotFile in ballotFiles)
            {
                Console.WriteLine($"Parsing: {ballotFile}");
                var plaintextBallot = await GetPlaintextBallot(ballotFile);
                var submittedBallot = EncryptAndSubmit(encryptionMediator, plaintextBallot);
                await WriteSubmittedBallot(encryptOptions, ballotFile, submittedBallot);
            }
            Console.WriteLine("Parsing Complete");
        }

        private static SubmittedBallot EncryptAndSubmit(EncryptionMediator encryptionMediator, PlaintextBallot plaintextBallot)
        {
            var ciphertextBallot = encryptionMediator.Encrypt(plaintextBallot);
            // todo: spoil?
            var submittedBallot = new SubmittedBallot(ciphertextBallot, BallotBoxState.Cast);
            return submittedBallot;
        }

        private static async Task<EncryptionMediator> GetEncryptionMediator(EncryptOptions encryptOptions)
        {
            var context = await GetContext(encryptOptions.Context);
            var internalManifest = await GetInternalManifest(encryptOptions);
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

        private static async Task<InternalManifest> GetInternalManifest(EncryptOptions encryptOptions)
        {
            var manifestJson = await File.ReadAllTextAsync(encryptOptions.Manifest);
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
