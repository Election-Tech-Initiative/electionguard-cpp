using System.Threading.Tasks;

namespace ElectionGuardCore.Ui.Elections
{
    public class ContestSelectionListViewModel : ViewModelBase
    {
        public override string Title => "Vote";

        public override Task Load()
        {
            return Task.CompletedTask;  // TODO implement
        }
    }
}
