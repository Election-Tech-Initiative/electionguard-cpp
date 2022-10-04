namespace ElectionGuard.InteropGenerator.Models;

public record GeneratedClass(string FilePath, string GeneratedCode)
{
    public async Task Write()
    {
        await File.WriteAllTextAsync(FilePath, GeneratedCode);
    }
}