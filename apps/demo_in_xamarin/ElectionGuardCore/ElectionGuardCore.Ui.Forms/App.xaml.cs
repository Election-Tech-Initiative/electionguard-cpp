using System.IO;
using System.Reflection;
using Autofac;
using Autofac.Features.ResolveAnything;
using ElectionGuardCore.Elections;
using ElectionGuardCore.Encryption;
using ElectionGuardCore.Ui.Forms.Services;
using Newtonsoft.Json;
using Xamarin.Forms;

namespace ElectionGuardCore.Ui.Forms
{
    public partial class App
    {
        public App()
        {
            InitializeComponent();

            var appSettings = LoadAppSettings();

            var navigationService = new NavigationService();
            var container = CreateContainer(appSettings, navigationService);
            navigationService.SetContainer(container);  // need to set nav service dependencies after creating container

            var navigationPage = new NavigationPage(navigationService.GetDefaultPage());
            navigationPage.BarTextColor = Color.FromHex(Colors.LightGreen);
            navigationService.SetNavigation(navigationPage.Navigation);

            MainPage = navigationPage;
        }

        private IContainer CreateContainer(AppSettings appSettings, INavigationService navigationService)
        {
            var builder = new ContainerBuilder();
            builder.RegisterInstance(appSettings).AsSelf().SingleInstance();

            builder.RegisterInstance(navigationService).As<INavigationService>().SingleInstance();
            builder.RegisterType<AlertService>().As<IAlertService>().SingleInstance();
            builder.RegisterType<EncryptionService>().As<IEncryptionService>().SingleInstance();
            builder.RegisterType<ClipboardService>().As<IClipboardService>().SingleInstance();

            // uncomment the desired IElectionService implementation
            if (appSettings.UseMockApi)
            {
                builder.RegisterType<MockElectionService>().As<IElectionService>().SingleInstance();
            } else
            {
                builder.RegisterType<ApiElectionService>().As<IElectionService>().SingleInstance();
            }

            builder.RegisterSource(new AnyConcreteTypeNotAlreadyRegisteredSource());

            return builder.Build();
        }

        protected override void OnStart() { }

        protected override void OnSleep() { }

        protected override void OnResume() { }

        private AppSettings LoadAppSettings()
        {
            var assembly = typeof(App).GetTypeInfo().Assembly;
            using (var stream = assembly.GetManifestResourceStream($"{assembly.GetName().Name}.appsettings.json"))
            {
                using (var reader = new StreamReader(stream))
                {
                    var json = reader.ReadToEnd();
                    return JsonConvert.DeserializeObject<AppSettings>(json);
                }
            }
        }
    }
}
