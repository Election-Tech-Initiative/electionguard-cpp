﻿using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Autofac;
using ElectionGuardCore.Ui.Forms.Elections;
using Xamarin.Forms;

namespace ElectionGuardCore.Ui.Forms.Services
{
    public class NavigationService : INavigationService
    {
        private INavigation _navigation;
        private IContainer _container;

        private readonly Dictionary<string, Type> _pageMappings = new Dictionary<string, Type>
        {
            { NavigationPaths.RootPage, typeof(ActiveContestPage) },
            { NavigationPaths.ContestSelectionListPage, typeof(ContestSelectionListPage) },
            { NavigationPaths.ReviewSelectionPage, typeof(ReviewSelectionPage) },
            { NavigationPaths.SelectionVerificationPage, typeof(SelectionVerificationPage) },
            { NavigationPaths.SelectionSubmittedPage, typeof(SelectionSubmittedPage) }
        };

        public async Task Push(string path, object parameter)
        {
            await _navigation.PushAsync(ResolvePage(path, parameter));
        }

        public async Task PopToRoot()
        {
            await _navigation.PopToRootAsync();
        }

        internal void SetNavigation(INavigation navigation)
        {
            _navigation = navigation;
        }

        internal void SetContainer(IContainer container)
        {
            _container = container;
        }

        internal PageBase GetDefaultPage()
        {
            return ResolvePage(NavigationPaths.RootPage, null);
        }

        private PageBase ResolvePage(string path, object parameter)
        {
            var pageType = _pageMappings[path];
            var page = (PageBase)_container.Resolve(pageType);
            page.ViewModel.Parameter = parameter;

            return page;
        }
    }
}
