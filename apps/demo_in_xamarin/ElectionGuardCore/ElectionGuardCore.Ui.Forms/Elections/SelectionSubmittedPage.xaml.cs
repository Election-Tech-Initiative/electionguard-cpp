using ElectionGuardCore.Ui.Elections;
using Xamarin.Forms.Xaml;

namespace ElectionGuardCore.Ui.Forms.Elections
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class SelectionSubmittedPage
    {
        public SelectionSubmittedPage(SelectionSubmittedViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }
    }
}