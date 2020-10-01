using System.Threading.Tasks;

namespace ElectionGuardCore.Ui
{
    public interface IClipboardService
    {
        Task Copy(string text);
    }
}
