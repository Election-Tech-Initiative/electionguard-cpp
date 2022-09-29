using ElectionGuard.InteropGenerator.Models;
using System.Text.Json;

namespace ElectionGuard.InteropGenerator
{
    /// <summary>
    /// Responsible for generating all interop classes in all languages for ElectionGuard
    /// </summary>
    internal class InteropClassesGenerator
    {
        private readonly InteropClassGenerator _classGenerator;
        const string OutputDir = "../../../../../../bindings/netstandard/ElectionGuard/ElectionGuard.Encryption/GeneratedInterop";

        public InteropClassesGenerator()
        {
            _classGenerator = new InteropClassGenerator();
        }

        public async Task Generate()
        {
            var outputType = OutputType.CS;

            var propertiesPath = Path.Join(Directory.GetCurrentDirectory(), "/EgInteropClasses.json");
            await using var openStream = File.OpenRead(propertiesPath);
            var egClasses = JsonSerializer.Deserialize<EgClass[]>(openStream);
            if (egClasses == null) throw new Exception("EgInteropClasses.json was empty");
            foreach (var egClass in egClasses)
            {
                var (className, generatedCode) = _classGenerator.Generate(outputType, egClass);
                var fileName = className + ".cs";
                var filePath = Path.Combine(OutputDir, fileName);
                await File.WriteAllTextAsync(filePath, generatedCode);
            }
        }
    }
}
