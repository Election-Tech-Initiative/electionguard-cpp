using System;
using System.Collections.Generic;
using ElectionGuardCore.Models;

namespace ElectionGuardCore.Elections
{
    public class ElectionDescription
    {
        public string ElectionScopeId { get; set; }
        public InternationalizedText Name { get; set; }
        public ElectionType? Type { get; set; }
        public ContactInformation ContactInformation { get; set; }
        public DateTime? StartDate { get; set; }
        public DateTime? EndDate { get; set; }
        public List<Candidate> Candidates { get; set; }
        public List<Party> Parties { get; set; }
        public List<ContestDescription> Contests { get; set; }
        public List<BallotStyle> BallotStyles { get; set; }
    }
}