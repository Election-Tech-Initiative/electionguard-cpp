using System.Collections.Generic;
using System.Threading.Tasks;

namespace ElectionGuardCore.Elections
{
    public interface IElectionService
    {
        Task<ElectionDescription> GetElectionDescription();
        Task<CiphertextElectionContext> GetCiphertextElectionContext();
        IDictionary<string, bool> Votes { get; }    // TODO replace w/ real API operation
    }
}
