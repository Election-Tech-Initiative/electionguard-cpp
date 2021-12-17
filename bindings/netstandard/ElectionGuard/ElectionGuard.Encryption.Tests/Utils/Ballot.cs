using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ElectionGuard.Encryption;

namespace ElectionGuard.Encryption.Tests.Utils
{
    public class BallotGenerator
    {
        public static PlaintextBallotSelection SelectionFrom(SelectionDescription description, bool isAffirmative = false, bool isPlaceholder = false)
        {
            return new PlaintextBallotSelection(description.ObjectId, isAffirmative ? 1UL : 0UL, isPlaceholder);
        }

        public static PlaintextBallotContest ContestFrom(ContestDescription contest, uint maxChoices)
        {
            uint choices = 0;
            List<PlaintextBallotSelection> selections = new List<PlaintextBallotSelection>();

            for (ulong i = 0; i < contest.SelectionsSize; i++)
            {
                if (choices < maxChoices)
                {
                    ++choices;
                    selections.Add(SelectionFrom(contest.GetSelectionAtIndex(i), true));
                }
                else
                {
                    selections.Add(SelectionFrom(contest.GetSelectionAtIndex(i), false));
                }
            }

            return new PlaintextBallotContest(contest.ObjectId, selections.ToArray());
        }

        public static PlaintextBallot GetFakeBallot(InternalManifest manifest, uint maxChoices = 1)
        {
            string ballotId = "ballot-id-123";

            List<PlaintextBallotContest> contests = new List<PlaintextBallotContest>();
            for (ulong i = 0; i < manifest.ContestsSize; i++)
            {
                contests.Add(ContestFrom(manifest.GetContestAtIndex(i), maxChoices));
            }

            return new PlaintextBallot(ballotId, manifest.GetBallotStyleAtIndex(0).ObjectId, contests.ToArray());
        }
    }
}
