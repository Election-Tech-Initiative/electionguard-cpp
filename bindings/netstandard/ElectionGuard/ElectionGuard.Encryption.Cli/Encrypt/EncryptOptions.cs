using CommandLine;

namespace ElectionGuard.Encryption.Cli.Encrypt;

[Verb("encrypt", HelpText = "Encrypt ballots using a context.")]
internal class EncryptOptions
{
    [Option('c', "context", Required = true, HelpText = "Json file containing an ElectionGuard context that contains encryption details.")]
    public string? Context { get; set; }

    [Option('m', "manifest", Required = true, HelpText = "Json file containing an ElectionGuard manifest that contains election details.")]
    public string? Manifest { get; set; }

    [Option('b', "ballots", Required = true, HelpText = "File folder containing ballots to encrypt.")]
    public string? BallotsDir { get; set; }

    [Option('d', "device", Required = true, HelpText = "Json file containing device information like launch code and location.")]
    public string? Device { get; set; }

    [Option('s', "spoiled_ids", Required = false, Separator = ',', HelpText = "Json file containing device information like launch code and location.")]
    public IEnumerable<string> SpoiledDeviceIds { get; set; }

    [Option('o', "out", Required = true, HelpText = "File folder in which to place encrypted ballots.")]
    public string? OutDir { get; set; }

    public void Validate()
    {
        ValidateFiles();
        ValidateDirectories();
    }

    private void ValidateDirectories()
    {
        if (string.IsNullOrEmpty(BallotsDir)) throw new ArgumentNullException(nameof(BallotsDir));
        if (string.IsNullOrEmpty(OutDir)) throw new ArgumentNullException(nameof(OutDir));

        if (!Directory.Exists(BallotsDir)) throw new ArgumentException($"ballots directory does not exist"); ;
        if (!Directory.Exists(OutDir))
        {
            Directory.CreateDirectory(OutDir);
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
