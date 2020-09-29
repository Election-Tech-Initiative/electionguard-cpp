﻿using Autofac;
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

            var navigationService = new NavigationService();
            var container = CreateContainer(navigationService);
            navigationService.SetContainer(container);  // need to set nav service dependencies after creating container

            var navigationPage = new NavigationPage(navigationService.GetDefaultPage());
            navigationPage.BarTextColor = Color.FromHex(Colors.LightGreen);
            NavigationPage.SetBackButtonTitle(this, "Back");
            navigationService.SetNavigation(navigationPage.Navigation);

            MainPage = navigationPage;
        }

        private IContainer CreateContainer(INavigationService navigationService)
        {
            var builder = new ContainerBuilder();
            builder.RegisterInstance(navigationService).As<INavigationService>().SingleInstance();
            builder.RegisterType<MockElectionService>().As<IElectionService>().SingleInstance();

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
