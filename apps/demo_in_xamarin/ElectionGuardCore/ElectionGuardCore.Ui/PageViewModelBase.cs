using System.Threading.Tasks;

namespace ElectionGuardCore.Ui
{
    public abstract class PageViewModelBase : ViewModelBase
    {
        public abstract string Title { get; }

        public object Parameter { get; set; }

        public abstract Task Load();
    }
}
