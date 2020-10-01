using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows.Input;

namespace ElectionGuardCore.Ui.Elections
{
    public class SelectionSubmittedViewModel : PageViewModelBase
    {
        private readonly INavigationService _navigationService;

        public SelectionSubmittedViewModel(INavigationService navigationService)
        {
            _navigationService = navigationService;

            CloseCommand = new RelayCommand(Close);
        }

        public override string Title => "Thank You";

        public override Task Load()
        {
            Args = (SelectionSubmittedArgs)Parameter;
            return Task.CompletedTask;
        }

        private SelectionSubmittedArgs _args;
        private SelectionSubmittedArgs Args
        {
            get => _args;
            set
            {
                _args = value;
                OnPropertyChanged(nameof(Message));
                OnPropertyChanged(nameof(CanReVote));
            }
        }

        public string Message
        {
            get
            {
                if (Args == null)
                {
                    return null;
                }
                return Args.VoteCast ? "Your vote has been counted!" : "Your vote has been spoiled.";
            }
        }

        public bool CanReVote
        {
            get
            {
                if (Args == null)
                {
                    return false;
                }
                return !Args.VoteCast;
            }
        }

        public ICommand CloseCommand { get; }

        private async void Close(object parameter)
        {
            var tasks = new List<Task>
            {
                _navigationService.PopModal(),
                _navigationService.PopToRoot()
            };
            await Task.WhenAll(tasks);
        }

        public class SelectionSubmittedArgs
        {
            public readonly bool VoteCast;

            public SelectionSubmittedArgs(bool voteCast)
            {
                VoteCast = voteCast;
            }
        }
    }
}
