using ElectionGuard.InteropGenerator.Models;
using ElectionGuard.InteropGenerator.Templates;

namespace ElectionGuard.InteropGenerator.FileGenerators;

internal class CsFileGenerator : FileGeneratorBase
{
    const string CsOutputDir = "../../../../../../bindings/netstandard/ElectionGuard/ElectionGuard.Encryption/GeneratedInterop";

    public override GeneratedClass Generate(EgClass egClass)
    {
        var generator = new CsClassTemplate(egClass);
        var generatedCode = generator.TransformText();
        var fileName = egClass.ClassName + ".generated.cs";
        var filePath = Path.Combine(CsOutputDir, fileName);
        return new GeneratedClass(filePath, generatedCode);
    }
}