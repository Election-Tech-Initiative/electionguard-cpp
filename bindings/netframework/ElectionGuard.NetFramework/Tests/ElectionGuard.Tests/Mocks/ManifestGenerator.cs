using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ElectionGuardInterop;

namespace ElectionGuard.Tests.Mocks
{
    public class ManifestGenerator
    {
        public static Manifest GetMinimalFakeJeffersonCountyManifest()
        {
            var gpUnits = new List<GeopoliticalUnit>
        {
            new GeopoliticalUnit("jefferson-county", "Jefferson County", ReportingUnitType.county)
        };

            var candidates = new List<Candidate>
        {
            new Candidate("benjamin-franklin", isWriteIn: false),
            new Candidate("john-adams", isWriteIn: false),
            new Candidate("write-in", isWriteIn: true)
        };

            var contests = new List<ContestDescription>
        {
            new ContestDescription(
                objectId: "justice-supreme-court",
                electoralDistrictId: gpUnits[0].ObjectId,
                sequenceOrder: 0,
                VoteVariationType.n_of_m,
                numberElected: 1,
                name: "Justice of the Supreme Court",
                selections: new[]
                {
                    new SelectionDescription(
                        "benjamin-franklin-selection", candidates[0].CandidateId, sequenceOrder: 0),
                    new SelectionDescription(
                        "john-adams-selection", candidates[1].CandidateId, sequenceOrder: 1),
                    new SelectionDescription(
                        "write-in-selection", candidates[2].CandidateId, sequenceOrder: 2)
                })
        };

            var ballotStyles = new List<BallotStyle>
        {
            new BallotStyle(
                "jefferson-county-ballot-style",
                new[] { gpUnits[0].ObjectId })
        };


            return new Manifest(
                "jefferson-county-open-primary",
                ElectionType.primary,
                startDate: DateTime.Now,
                endDate: DateTime.Now.AddDays(1),
                gpUnits.ToArray(),
                (new List<Party>()).ToArray(),
                candidates.ToArray(),
                contests.ToArray(),
                ballotStyles.ToArray()
            );
        }
    }
}
