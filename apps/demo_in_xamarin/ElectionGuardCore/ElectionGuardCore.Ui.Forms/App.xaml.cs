using Autofac;
using Autofac.Features.ResolveAnything;
using ElectionGuardCore.Elections;
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
            builder.RegisterSource(new AnyConcreteTypeNotAlreadyRegisteredSource());

            builder.RegisterType<MockElectionService>().As<IElectionService>().InstancePerLifetimeScope();

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
