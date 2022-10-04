using ElectionGuard.InteropGenerator.Models;

namespace ElectionGuard.InteropGenerator.FileGenerators;

public abstract class FileGeneratorBase
{
    public abstract GeneratedClass Generate(EgClass egClass);
}
