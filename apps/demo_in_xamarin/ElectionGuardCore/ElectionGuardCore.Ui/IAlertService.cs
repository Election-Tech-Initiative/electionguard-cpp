using System;
using System.Threading.Tasks;

namespace ElectionGuardCore.Ui
{
    public interface IAlertService
    {
        Task Alert(string title, string message, string close);
        Task<bool> Alert(string title, string message, string accept, string cancel);
        Task ShowBusy(string label, Action busyAction, Action onComplete);
    }
}
