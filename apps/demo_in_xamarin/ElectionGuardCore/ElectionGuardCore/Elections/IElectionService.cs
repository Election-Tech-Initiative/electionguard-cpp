using System.Collections.Generic;
using System.Threading.Tasks;

namespace ElectionGuardCore.Elections
{
    public interface IElectionService
    {
        Task<Election> GetElection();
        Task<CiphertextElectionContext> GetCiphertextElectionContext();
        Task CastBallot(string electionId, CiphertextBallot ballot);
        Task SpoilBallot(string electionId, CiphertextBallot ballot);

        IDictionary<string, bool> Votes { get; }    // TODO replace w/ real API operation
    }
}
