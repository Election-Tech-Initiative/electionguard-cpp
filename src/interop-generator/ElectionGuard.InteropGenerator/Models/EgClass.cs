namespace ElectionGuard.InteropGenerator.Models;

public class EgClass
{
    public string ClassName { get; set; }
    public EgProperty[] Properties { get; set; }
    public EgMethod[] Methods { get; set; }
}
