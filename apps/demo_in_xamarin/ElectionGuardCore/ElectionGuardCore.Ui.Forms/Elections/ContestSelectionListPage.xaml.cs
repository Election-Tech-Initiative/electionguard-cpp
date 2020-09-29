using ElectionGuardCore.Ui.Elections;
using Xamarin.Forms.Xaml;

namespace ElectionGuardCore.Ui.Forms.Elections
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ContestSelectionListPage
    {
        public ContestSelectionListPage(ContestSelectionListViewModel viewModel)
            :base(viewModel)
        {
            InitializeComponent();
        }
    }
}