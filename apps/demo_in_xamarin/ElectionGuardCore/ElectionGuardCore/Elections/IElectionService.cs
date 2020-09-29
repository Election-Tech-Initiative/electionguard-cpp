using System.Collections.Generic;
using System.Threading.Tasks;

namespace ElectionGuardCore.Elections
{
    public interface IElectionService
    {
        Task<ElectionDescription> GetElectionDescription();
        IDictionary<string, bool> Votes { get; }
    }
}
