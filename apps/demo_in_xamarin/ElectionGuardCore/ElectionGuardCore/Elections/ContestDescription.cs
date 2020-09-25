using System.Collections.Generic;
using ElectionGuardCore.Models;

namespace ElectionGuardCore.Elections
{
    public class ContestDescription
    {
        public string Type { get; set; }
        public string ObjectId { get; set; }
        public int SequenceOrder { get; set; }
        public List<SelectionDescription> BallotSelections { get; set; }
        public InternationalizedText BallotTitle { get; set; }
        public InternationalizedText BallotSubtitle { get; set; }
        public VoteVariationType? VoteVariation { get; set; }
        public string ElectoralDistrictId { get; set; }
        public string Name { get; set; }
        public List<string> PrimaryPartyIds { get; set; }
        public int? NumberElected { get; set; }
        public int? VotesAllowed { get; set; }
    }
}
