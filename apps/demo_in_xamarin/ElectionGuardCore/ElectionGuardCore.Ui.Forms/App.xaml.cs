using Autofac;
using Autofac.Features.ResolveAnything;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Ui.Forms.Services;
using Xamarin.Forms;

namespace ElectionGuardCore.Ui.Forms
{
    public partial class App
    {
        public App()
        {
            InitializeComponent();

            var navigationService = new NavigationService(CreateContainer());

            MainPage = new NavigationPage(navigationService.GetDefaultPage());
        }

        private IContainer CreateContainer()
        {
            var builder = new ContainerBuilder();
            builder.RegisterType<MockElectionService>().As<IElectionService>().InstancePerLifetimeScope();

            builder.RegisterSource(new AnyConcreteTypeNotAlreadyRegisteredSource());

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
