using System;
using System.Windows.Input;

namespace ElectionGuardCore.Ui
{
    public class RelayCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;

        private readonly Func<object, bool> _canExecute;
        private readonly Action<object> _execute;

        public RelayCommand(Action<object> execute, Func<object, bool> canExecute = null)
        {
            if (execute == null)
            {
                throw new ArgumentNullException(nameof(execute));
            }

            _execute = execute;
            _canExecute = canExecute;
        }

        public bool CanExecute(object parameter = null) => _canExecute?.Invoke(parameter) ?? true;

        public void Execute(object parameter = null)
        {
            _execute(parameter);    // TODO handle async actions
        }
    }
}
