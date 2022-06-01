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

            var context = await File.ReadAllTextAsync(encryptOptions.Context);
            Console.WriteLine(context);
            var enumerateFiles = Directory.EnumerateFiles(encryptOptions.BallotsDir);

            foreach (var ballotFile in enumerateFiles)
            {
                var ballot = await File.ReadAllTextAsync(ballotFile);
                Console.WriteLine($"Parsing: {ballot}");
                var plaintextBallot = new PlaintextBallot(ballot);
                //ElectionGuard.Encrypt.Ballot(plaintextBallot, internalManifest);
            }
            Console.WriteLine("Parsing Complete");
        }
    }
}
