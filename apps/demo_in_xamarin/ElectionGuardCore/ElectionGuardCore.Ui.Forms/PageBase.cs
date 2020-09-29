using Xamarin.Forms;

namespace ElectionGuardCore.Ui.Forms
{
    public abstract class PageBase : ContentPage
    {
        public ViewModelBase ViewModel { get; }

        protected PageBase(ViewModelBase viewModel)
        {
            ViewModel = viewModel;
            BindingContext = ViewModel;
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await ViewModel.Load();
        }
    }
}