using CommandLine;

namespace ElectionGuard.Encryption.Cli.SayHi;

[Verb("sayhi", HelpText = "Just greets you nicely, that's all.")]
internal class SayHiOptions
{
    [Option('n', "name")]
    public string? Name { get; set; }
}