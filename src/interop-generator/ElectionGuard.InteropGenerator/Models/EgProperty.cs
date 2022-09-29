using System.Text;

namespace ElectionGuard.InteropGenerator.Models;

public class EgProperty
{
    public string Name { get; set; }
    public string Type { get; set; }
    public string? NativeHandleType { get; set; }
    public string Description { get; set; }

    public string GetEntryPoint(string className)
    {
        return ToSnakeCase($"Eg{className}Get{Name}");
    }

    private static string ToSnakeCase(string text)
    {
        if (text == null)
        {
            throw new ArgumentNullException(nameof(text));
        }
        if (text.Length < 2)
        {
            return text;
        }
        var sb = new StringBuilder();
        sb.Append(char.ToLowerInvariant(text[0]));
        for (int i = 1; i < text.Length; ++i)
        {
            char c = text[i];
            if (char.IsUpper(c))
            {
                sb.Append('_');
                sb.Append(char.ToLowerInvariant(c));
            }
            else
            {
                sb.Append(c);
            }
        }
        return sb.ToString();
    }

}