using System.Threading.Tasks;

namespace ElectionGuardCore.Ui
{
    public interface IAlertService
    {
        Task<bool> Prompt(string title, string message, string accept, string cancel);
    }
}
