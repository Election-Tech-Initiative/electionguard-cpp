using ElectionGuard.InteropGenerator.Helpers;
using ElectionGuard.InteropGenerator.Models;
using ElectionGuard.InteropGenerator.Templates;

namespace ElectionGuard.InteropGenerator.FileGenerators;

internal class CFileGenerator : FileGeneratorBase
{
    const string COutputDir = "../../../../../../include/electionguard";

    public override GeneratedClass Generate(EgClass egClass)
    {
        var generator = new CHeaderTemplate(egClass);
        var generatedCode = generator.TransformText();
        var snakeCaseClassName = NameHelpers.ToSnakeCase(egClass.ClassName);
        var fileName = snakeCaseClassName + ".generated.h";
        var filePath = Path.Combine(COutputDir, fileName);
        return new GeneratedClass(filePath, generatedCode);
    }
}
