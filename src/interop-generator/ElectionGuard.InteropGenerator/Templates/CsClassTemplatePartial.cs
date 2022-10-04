using ElectionGuard.InteropGenerator.Models;

namespace ElectionGuard.InteropGenerator.Templates
{
    public partial class CsClassTemplate
    {
        public CsClassTemplate(EgClass egClass)
        {
            EgClass = egClass;
        }

        public EgClass EgClass { get; set; }
    }
}
