namespace ElectionGuard.InteropGenerator;

internal class InteropClassGenerator
{
    public GeneratedClass Generate(OutputType outputType, EgClass egClass)
    {
        var generatedCode = GenerateCode(outputType, egClass);
        return new GeneratedClass(egClass.ClassName, generatedCode);
    }

    private static string GenerateCode(OutputType outputType, EgClass egClass)
    {
        switch (outputType)
        {
            case OutputType.CS:
                var csClassGenerator = new CsClassGenerator(egClass);
                return csClassGenerator.TransformText();
            case OutputType.Cpp:
                var cppClassGenerator = new CppClassGenerator();
                return cppClassGenerator.TransformText();
            default:
                throw new NotImplementedException();
        }
    }
}