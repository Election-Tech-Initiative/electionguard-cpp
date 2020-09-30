using ElectionGuardCore.Ui.Elections;
using Xamarin.Forms.Xaml;

namespace ElectionGuardCore.Ui.Forms.Elections
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SelectionVerificationPage
    {
        public SelectionVerificationPage(SelectionVerificationViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }
    }
}