using ElectionGuardCore.Ui.Elections;
using Xamarin.Forms.Xaml;

namespace ElectionGuardCore.Ui.Forms.Elections
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class VoteOptionListPage
    {
        public VoteOptionListPage(VoteOptionListViewModel viewModel)
            :base(viewModel)
        {
            InitializeComponent();
        }
    }
}