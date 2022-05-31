using CommandLine;
using ElectionGuard.Encryption.Cli.Encrypt;
using ElectionGuard.Encryption.Cli.SayHi;

namespace ElectionGuard.Encryption.Cli;

class Program
{
    static void Main(string[] args)
    {
        Parser.Default.ParseArguments<EncryptOptions, SayHiOptions>(args)
            .MapResult(
                (EncryptOptions opts) =>
                {
                    new EncryptCommand().Encrypt();
                    return 0;
                },
                (SayHiOptions opts) =>
                {
                    Console.WriteLine("hi");
                    return 0;
                },
                errs => 1
                );
    }
}