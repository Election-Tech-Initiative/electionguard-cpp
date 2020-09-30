using System.Threading.Tasks;
using Xamarin.Essentials;

namespace ElectionGuardCore.Ui.Forms.Services
{
    public class ClipboardService : IClipboardService
    {
        public async Task Copy(string text)
        {
            await Clipboard.SetTextAsync(text);
        }
    }
}
