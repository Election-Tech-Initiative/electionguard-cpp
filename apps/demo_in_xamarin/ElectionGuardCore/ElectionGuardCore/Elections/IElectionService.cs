using System.Threading.Tasks;

namespace ElectionGuardCore.Elections
{
    public interface IElectionService
    {
        Task<ElectionDescription> GetElectionDescription();
    }
}
