using ElectionGuardCore.Ui.Elections;
using Xamarin.Forms.Xaml;

namespace ElectionGuardCore.Ui.Forms.Elections
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ActiveContestViewPage : PageBase
    {
        public ActiveContestViewPage(ActiveContestViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }
    }
}