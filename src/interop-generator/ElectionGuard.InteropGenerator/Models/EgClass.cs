namespace ElectionGuard.InteropGenerator.Models;

public record EgClass(
    string ClassName, 
    EgProperty[] Properties, 
    EgMethod[] Methods
    );