using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using ElectionGuardCore.Elections;

namespace ElectionGuardCore.Ui.Forms.Services
{
    public class ApiElectionService : ApiClientBase, IElectionService
    {
        public ApiElectionService(AppSettings appSettings) : base(appSettings) { }

        public async Task<Election> GetElection()
        {
            var elections = await SendRequestForResponse<List<Election>>("/elections", HttpMethod.Get);
            return elections.FirstOrDefault();  // TODO determine which election to use
        }

        public async Task<CiphertextElectionContext> GetCiphertextElectionContext(string electionId)
        {
            return await SendRequestForResponse<CiphertextElectionContext>($"/elections/{electionId}/contest",
                HttpMethod.Get);
        }

        public async Task CastBallot(string electionId, CiphertextBallot ballot)
        {
            await SendRequest($"/elections/{electionId}/ballots/cast", HttpMethod.Post, requestData: ballot);
        }

        public async Task SpoilBallot(string electionId, CiphertextBallot ballot)
        {
            await SendRequest($"/elections/{electionId}/ballots/spoil", HttpMethod.Post, requestData: ballot);
        }

        public Task<bool> HasVoted(string electionId)
        {
            return Task.FromResult(false);  // TODO implement when API is in place
        }
    }
}
