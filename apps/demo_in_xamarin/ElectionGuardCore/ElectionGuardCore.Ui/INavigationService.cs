using System.Threading.Tasks;

namespace ElectionGuardCore.Ui
{
    public interface INavigationService
    {
        Task Push(string path, object parameter = null);
    }
}