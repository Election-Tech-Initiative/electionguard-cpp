using CommandLine;
using ElectionGuard.Encryption.Cli.Encrypt;

namespace ElectionGuard.Encryption.Cli;

class Program
{
    static async Task Main(string[] args)
    {
        await Parser.Default.ParseArguments<EncryptOptions>(args)
            .WithParsedAsync(EncryptCommand.Encrypt);
    }
}