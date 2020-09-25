using ElectionGuardCore.Ui.Forms.Elections;
using ElectionGuardCore.Ui.Forms.NinjectModules;
using Ninject;
using Xamarin.Forms;

namespace ElectionGuardCore.Ui.Forms
{
    public partial class App : Application
    {
        public App()
        {
            InitializeComponent();

            var kernel = CreateKernel();
            MainPage = kernel.Get<ActiveContestViewPage>();
        }

        private IKernel CreateKernel()
        {
            return new StandardKernel(new ElectionsModule());
        }

        protected override void OnStart()
        {
        }

        protected override void OnSleep()
        {
        }

        protected override void OnResume()
        {
        }
    }
}
