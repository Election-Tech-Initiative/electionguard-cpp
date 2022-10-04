using ElectionGuard.InteropGenerator.Helpers;

namespace ElectionGuard.InteropGenerator.Models;

public class EgProperty
{
    public string Name { get; set; }
    public string Type { get; set; }
    public string? NativeHandleType { get; set; }
    public string Description { get; set; }

    public string GetEntryPoint(string className)
    {
        return NameHelpers.ToSnakeCase($"Eg{className}Get{Name}");
    }
}