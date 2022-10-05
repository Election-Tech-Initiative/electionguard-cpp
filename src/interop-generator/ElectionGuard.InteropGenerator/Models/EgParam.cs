using ElectionGuard.InteropGenerator.Helpers;

namespace ElectionGuard.InteropGenerator.Models;

public record EgParam(
    string Name, 
    string TypeCs
)
{
    public string TypeC => TypeHelper.CsToC(TypeCs);

    public string MarshallAs()
    {
        if (TypeCs == "string")
        {
            return "UnmanagedType.LPStr";
        }
        throw new NotImplementedException("Unsupported marshall type " + TypeCs);
    }
}