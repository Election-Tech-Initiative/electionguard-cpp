using Autofac;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Ui.Elections;
using ElectionGuardCore.Ui.Forms.Elections;
using ElectionGuardCore.Ui.Forms.Services;

namespace ElectionGuardCore.Ui.Forms
{
    public partial class App
    {
        public App()
        {
            InitializeComponent();

            var container = CreateContainer();
            MainPage = container.Resolve<ActiveContestViewPage>();
        }

        private IContainer CreateContainer()
        {
            var builder = new ContainerBuilder();
            builder.RegisterType<MockElectionService>().As<IElectionService>().InstancePerLifetimeScope();
            builder.RegisterType<ActiveContestViewModel>().AsSelf().InstancePerLifetimeScope();
            builder.RegisterType<ActiveContestViewPage>().AsSelf();

            return builder.Build();
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
