using CommandLine;

namespace ElectionGuard.Encryption.Cli.Encrypt;

[Verb("encrypt", HelpText = "Encrypt ballots using a context.")]
internal class EncryptOptions
{
    [Option('c', "context", HelpText = "ElectionGuard context json file that contains encryption details")]
    public string Context { get; set; }
}
