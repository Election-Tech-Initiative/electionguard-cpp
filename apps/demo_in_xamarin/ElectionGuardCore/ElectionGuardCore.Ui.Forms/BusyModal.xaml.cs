using Xamarin.Forms.Xaml;

namespace ElectionGuardCore.Ui.Forms
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class BusyModal
    {
        public BusyModal(string label)
        {
            InitializeComponent();
            Label.Text = label;
        }
    }
}