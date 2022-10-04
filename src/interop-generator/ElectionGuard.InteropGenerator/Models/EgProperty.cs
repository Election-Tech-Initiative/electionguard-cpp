using ElectionGuard.InteropGenerator.Helpers;

namespace ElectionGuard.InteropGenerator.Models;

public class EgProperty
{
    public string Name { get; set; }
    public string TypeCs { get; set; }
    public string? NativeHandleType { get; set; }
    public string Description { get; set; }

    public string GetEntryPoint(string className)
    {
        return NameHelpers.ToSnakeCase($"Eg{className}Get{Name}");
    }

    public string GetCReturnType()
    {
        switch (TypeCs)
        {
            case "string":
                return "eg_electionguard_status_t";
            case "ulong":
                return "uint64_t";
            default:
                return "eg_electionguard_status_t";
        }
    }
}