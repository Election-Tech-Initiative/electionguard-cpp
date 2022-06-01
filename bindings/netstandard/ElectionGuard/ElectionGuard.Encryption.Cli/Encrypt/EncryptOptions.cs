using CommandLine;

namespace ElectionGuard.Encryption.Cli.Encrypt;

[Verb("encrypt", HelpText = "Encrypt ballots using a context.")]
internal class EncryptOptions
{
    [Option('c', "context", Required = true, HelpText = "Json file containing an ElectionGuard context that contains encryption details.")]
    public string Context { get; set; }

    [Option('m', "manifest", Required = true, HelpText = "Json file containing an ElectionGuard manifest that contains election details.")]
    public string Manifest { get; set; }

    [Option('b', "ballots", Required = true, HelpText = "File folder containing ballots to encrypt.")]
    public string BallotsDir { get; set; }

    [Option('o', "out", Required = true, HelpText = "File folder in which to place encrypted ballots.")]
    public string OutDir { get; set; }

    public void Validate()
    {
        ValidateFiles();
        ValidateDirectories();
    }

    private void ValidateDirectories()
    {
        var requiredDirectories = new[] { BallotsDir, OutDir };
        var missingDirectories = requiredDirectories.Where(f => !Directory.Exists(f));
        foreach (var directory in missingDirectories)
        {
            throw new ArgumentException($"{directory} does not exist");
        }
    }

    private void ValidateFiles()
    {
        var requiredFiles = new[] { Context, Manifest };
        var missingFiles = requiredFiles.Where(f => !File.Exists(f));
        foreach (var file in missingFiles)
        {
            throw new ArgumentException($"{file} does not exist");
        }
    }
}
