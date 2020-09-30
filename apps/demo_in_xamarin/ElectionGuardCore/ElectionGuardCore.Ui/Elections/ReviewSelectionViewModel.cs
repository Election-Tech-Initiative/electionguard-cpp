using System.Threading.Tasks;
using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Ui.Elections
{
    public class ReviewSelectionViewModel : PageViewModelBase
    {
        public override string Title => "Review";

        public override Task Load()
        {
            Args = (ReviewSelectionArgs)Parameter;
            return Task.CompletedTask;
        }

        private ReviewSelectionArgs _args;
        private ReviewSelectionArgs Args
        {
            get => _args;
            set
            {
                _args = value;
                OnPropertyChanged(nameof(CandidateName));
            }
        }

        public string CandidateName => Args?.Selection.BallotName.GetTextValue("en");

        public class ReviewSelectionArgs
        {
            public readonly Candidate Selection;
            public readonly ContestDescription Contest;

            public ReviewSelectionArgs(Candidate selection, ContestDescription contest)
            {
                Selection = selection;
                Contest = contest;
            }
        }
    }
}
