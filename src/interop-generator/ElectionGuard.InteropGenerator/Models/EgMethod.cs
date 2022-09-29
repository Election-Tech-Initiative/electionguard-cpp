namespace ElectionGuard.InteropGenerator.Models;

public class EgMethod
{
    public string Name { get; set; }
    public string ReturnType { get; set; }
    public string Description { get; set; }
    public EgParam[] Params { get; set; }
}

public class EgParam
{
    public string Name { get; set; }
    public string Type { get; set; }
}
