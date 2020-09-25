using System;
using System.Collections.Generic;
using Autofac;
using ElectionGuardCore.Ui.Forms.Elections;

namespace ElectionGuardCore.Ui.Forms.Services
{
    internal class NavigationService
    {
        public static readonly string RootPage = "/";

        private readonly IContainer _container;
        private readonly Dictionary<string, Type> _pageMappings = new Dictionary<string, Type>
        {
            { RootPage, typeof(ActiveContestViewPage) }
        };

        public NavigationService(IContainer container)
        {
            _container = container;
        }

        public PageBase GetDefaultPage()
        {
            return ResolvePage(RootPage);
        }

        private PageBase ResolvePage(string path)
        {
            var pageType = _pageMappings[path];
            return (PageBase) _container.Resolve(pageType);
        }
    }
}
