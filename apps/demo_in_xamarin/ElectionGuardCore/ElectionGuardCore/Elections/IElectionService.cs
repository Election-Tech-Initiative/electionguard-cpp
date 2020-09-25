using System.Threading.Tasks;
using ElectionGuardCore.Elections.Models;

namespace ElectionGuardCore.Elections
{
    public interface IElectionService
    {
        Task<ElectionDescription> GetElectionDescription();
    }
}
