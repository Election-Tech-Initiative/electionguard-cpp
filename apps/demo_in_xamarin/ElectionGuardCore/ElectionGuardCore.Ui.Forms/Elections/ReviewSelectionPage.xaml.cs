using ElectionGuardCore.Ui.Elections;
using Xamarin.Forms.Xaml;

namespace ElectionGuardCore.Ui.Forms.Elections
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ReviewSelectionPage
    {
        public ReviewSelectionPage(ReviewSelectionViewModel viewModel)
            : base(viewModel)
        {
            InitializeComponent();
        }
    }
}