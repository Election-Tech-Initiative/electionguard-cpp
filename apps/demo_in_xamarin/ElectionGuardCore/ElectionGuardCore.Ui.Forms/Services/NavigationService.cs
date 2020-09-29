using System;
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
            { NavigationPaths.RootPage, typeof(ActiveContestViewPage) },
            { NavigationPaths.VoteOptionListPage, typeof(VoteOptionListPage) }
        };

        public async Task Push(string path)
        {
            await _navigation.PushAsync(ResolvePage(path));
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
            return ResolvePage(NavigationPaths.RootPage);
        }

        private PageBase ResolvePage(string path)
        {
            var pageType = _pageMappings[path];
            return (PageBase)_container.Resolve(pageType);
        }
    }
}
