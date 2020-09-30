using System;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace ElectionGuardCore.Ui.Forms.Services
{
    public class AlertService : IAlertService
    {
        public async Task<bool> Alert(string title, string message, string accept, string cancel)
        {
            return await Application.Current.MainPage.DisplayAlert(title, message, accept, cancel);
        }

        public async Task ShowBusy(string label, Action busyAction, Action onComplete)
        {
            var busyModal = new BusyModal(label);
            var navigation = Application.Current.MainPage.Navigation;
            try
            {
                await navigation.PushModalAsync(busyModal);
                busyAction();
            }
            finally
            {
                await navigation.PopModalAsync();
                onComplete();
            }
        }
    }
}
