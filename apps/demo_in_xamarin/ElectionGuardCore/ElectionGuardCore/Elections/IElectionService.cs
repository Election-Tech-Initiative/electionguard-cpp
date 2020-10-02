using System.Threading.Tasks;

namespace ElectionGuardCore.Elections
{
    public interface IElectionService
    {
        Task<Election> GetElection();
        Task<CiphertextElectionContext> GetCiphertextElectionContext(string electionId);
        Task CastBallot(string electionId, CiphertextBallot ballot);
        Task SpoilBallot(string electionId, CiphertextBallot ballot);

        Task<bool> HasVoted(string electionId);     // TODO replace w/ real API operation
    }
}
