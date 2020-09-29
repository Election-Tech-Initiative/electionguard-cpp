using ElectionGuardCore.Ui.Elections;
using Xamarin.Forms;
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

        private ContestSelectionListViewModel ContestSelectionListViewModel =>
            ViewModel as ContestSelectionListViewModel;

        private void ListView_OnItemTapped(object sender, ItemTappedEventArgs e)
        {
            var tappedCandidate = (ContestSelectionListViewModel.CandidateViewModel)e.Item;
            ContestSelectionListViewModel?.SelectCandidateCommand.Execute(tappedCandidate);
        }
    }
}