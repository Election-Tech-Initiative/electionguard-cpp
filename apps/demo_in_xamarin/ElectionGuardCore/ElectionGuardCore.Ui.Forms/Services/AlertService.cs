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
    }
}
