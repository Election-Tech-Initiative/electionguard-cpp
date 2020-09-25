using Xamarin.Forms;

namespace ElectionGuardCore.Ui.Forms
{
    public abstract class PageBase : ContentPage
    {
        private readonly ViewModelBase _viewModel;

        protected PageBase(ViewModelBase viewModel)
        {
            _viewModel = viewModel;
        }

        protected override async void OnAppearing()
        {
            base.OnAppearing();
            await _viewModel.Load();
        }
    }
}