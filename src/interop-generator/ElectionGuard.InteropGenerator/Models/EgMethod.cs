using ElectionGuard.InteropGenerator.Helpers;

namespace ElectionGuard.InteropGenerator.Models;

public class EgMethod
{
    public string Name { get; set; }
    public string ReturnTypeCs { get; set; }
    public string Description { get; set; }
    public EgParam[] Params { get; set; }

    public string GetEntryPoint(string className)
    {
        return NameHelpers.ToSnakeCase($"Eg{className}{Name}");
    }
}

public class EgParam
{
    public string Name { get; set; }
    public string TypeCs { get; set; }

    public string MarshallAs()
    {
        if (TypeCs == "string")
        {
            return "UnmanagedType.LPStr";
        }
        throw new NotImplementedException("Unsupported marshall type " + TypeCs);
    }
}
