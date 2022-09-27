internal class ClassGenerator
{
    public GeneratedClass Generate(OutputType outputType)
    {
        var generatedCode = GenerateCode(outputType);
        return new GeneratedClass("PlaintextBallotSelection", generatedCode);
    }

    private static string GenerateCode(OutputType outputType)
    {
        switch (outputType)
        {
            case OutputType.CS:
                var csClassGenerator = new CsClassGenerator();
                return csClassGenerator.TransformText();
            case OutputType.Cpp:
                var cppClassGenerator = new CppClassGenerator();
                return cppClassGenerator.TransformText();
            default:
                throw new NotImplementedException();
        }
    }
}
