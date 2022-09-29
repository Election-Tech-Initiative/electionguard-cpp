// Args

var outputType = OutputType.CS;
var outputDir = "../../../../../../bindings/netstandard/ElectionGuard/ElectionGuard.Encryption";

// Dependencies
var classGenerator = new ClassGenerator();

var generatedClass = classGenerator.Generate(outputType);

var fileName = generatedClass.ClassName + ".cs";
var filePath = Path.Combine(outputDir, fileName);
await File.WriteAllTextAsync(filePath, generatedClass.GeneratedCode);
