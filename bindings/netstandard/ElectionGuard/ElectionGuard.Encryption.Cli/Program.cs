using CommandLine;
using ElectionGuard.Encryption.Cli.Encrypt;
using ElectionGuard.Encryption.Cli.SayHi;

namespace ElectionGuard.Encryption.Cli;

class Program
{
    static async Task Main(string[] args)
    {
        await Parser.Default.ParseArguments<EncryptOptions, SayHiOptions>(args)
            .WithParsed((SayHiOptions opts) =>
            {
                Console.WriteLine($"hi {opts.Name}");
            })
            .WithParsedAsync<EncryptOptions>(EncryptCommand.Encrypt);
    }
}