using ElectionGuardCore.Models;

namespace ElectionGuardCore.Elections
{
    public class Candidate
    {
        public string ObjectId { get; set; }
        public InternationalizedText BallotName { get; set; }
        public string PartyId { get; set; }
        public string ImageUri { get; set; }
        public bool? IsWriteIn { get; set; }
    }
}
