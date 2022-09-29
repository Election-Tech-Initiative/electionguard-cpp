namespace ElectionGuard.InteropGenerator
{
    public partial class CsClassGenerator
    {
        public CsClassGenerator(EgClass egClass)
        {
            EgClass = egClass;
        }

        public EgClass EgClass { get; set; }
    }
}
