using ElectionGuard.InteropGenerator.Helpers;

namespace ElectionGuard.InteropGenerator.Models;

public record EgMethod (
    string Name,
    string ReturnTypeCs,
    string Description,
    EgParam[] Params)
{
    public string ReturnTypeC => TypeHelper.CsToC(ReturnTypeCs);
    
    public string GetEntryPoint(string className)
    {
        return $"Eg{className}{Name}".ToSnakeCase();
    }
}