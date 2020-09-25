using ElectionGuardCore.Elections;
using ElectionGuardCore.Ui.Elections;
using ElectionGuardCore.Ui.Forms.Elections;
using ElectionGuardCore.Ui.Forms.Services;
using Ninject.Modules;

namespace ElectionGuardCore.Ui.Forms.NinjectModules
{
    internal class ElectionsModule : NinjectModule
    {
        public override void Load()
        {
            Bind<IElectionService>().To<MockElectionService>().InSingletonScope();
            Bind<ActiveContestViewModel>().ToSelf().InSingletonScope();
            Bind<ActiveContestViewPage>().ToSelf();
        }
    }
}
