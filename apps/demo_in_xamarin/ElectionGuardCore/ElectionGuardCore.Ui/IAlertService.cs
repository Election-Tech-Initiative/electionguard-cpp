using System.Threading.Tasks;

namespace ElectionGuardCore.Ui
{
    public interface IAlertService
    {
        Task<bool> Alert(string title, string message, string accept, string cancel);
    }
}
