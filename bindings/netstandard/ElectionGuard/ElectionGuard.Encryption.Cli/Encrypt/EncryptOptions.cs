using CommandLine;

namespace ElectionGuard.Encryption.Cli.Encrypt;

[Verb("encrypt", HelpText = "Encrypt ballots using a context.")]
internal class EncryptOptions
{
    [Option('c', "context", Required = true, HelpText = "Json file containing an ElectionGuard context that contains encryption details.")]
    public string Context { get; set; }

    [Option('b', "ballots", Required = true, HelpText = "File folder containing ballots to encrypt.")]
    public string BallotsDir { get; set; }

    public void Validate()
    {
        if (!Directory.Exists(BallotsDir))
        {
            throw new ArgumentException($"{BallotsDir} does not exist");
        }

        if (!File.Exists(Context))
        {
            throw new ArgumentException($"{Context} does not exist");
        }
    }
}
