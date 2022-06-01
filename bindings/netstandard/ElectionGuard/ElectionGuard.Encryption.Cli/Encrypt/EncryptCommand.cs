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

            var context = await GetContext(encryptOptions.Context);
            var enumerateFiles = Directory.EnumerateFiles(encryptOptions.BallotsDir);

            var internalManifest = await GetInternalManifest(encryptOptions);
            var device = GetDevice();

            foreach (var ballotFile in enumerateFiles)
            {
                var ballot = await File.ReadAllTextAsync(ballotFile);
                Console.WriteLine($"Parsing: {ballotFile}");
                var plaintextBallot = new PlaintextBallot(ballot);
                var ciphertextBallot = ElectionGuard.Encrypt.Ballot(plaintextBallot, internalManifest, context, device.GetHash());
                await WriteEncryptedBallot(encryptOptions, ballotFile, ciphertextBallot);
            }
            Console.WriteLine("Parsing Complete");
        }

        private static async Task WriteEncryptedBallot(EncryptOptions encryptOptions, string ballotFile,
            CiphertextBallot ciphertextBallot)
        {
            var ballotJson = ciphertextBallot.ToJson();
            var outFile = Path.Join(encryptOptions.OutDir, Path.GetFileName(ballotFile));
            await File.WriteAllTextAsync(outFile, ballotJson);
        }

        private static ElementModQ GetNonce()
        {
            return new ElementModQ("4BD3231DC17E9E84F5B0A5D2C4160C6A2299EDAE184C291E17709913B8F9CB40");
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
