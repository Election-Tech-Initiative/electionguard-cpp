using System;
using System.Threading.Tasks;

namespace ElectionGuardCore.Ui
{
    public interface INavigationService
    {
        Task Push(string path, object parameter = null);
        Task PushModal(string path, object parameter = null);
    }
}