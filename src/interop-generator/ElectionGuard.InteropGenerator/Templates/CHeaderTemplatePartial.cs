using ElectionGuard.InteropGenerator.Models;

namespace ElectionGuard.InteropGenerator.Templates;

public partial class CHeaderTemplate
{
    public CHeaderTemplate(EgClass egClass)
    {
        EgClass = egClass;
    }

    public EgClass EgClass { get; set; }
}
