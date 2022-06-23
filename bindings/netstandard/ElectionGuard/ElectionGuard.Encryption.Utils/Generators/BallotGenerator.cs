using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace ElectionGuard.Encryption.Utils
{
    public class BallotGenerator
    {
        public static PlaintextBallotSelection SelectionFrom(SelectionDescription description, bool isAffirmative = false, bool isPlaceholder = false)
        {
            return new PlaintextBallotSelection(description.ObjectId, isAffirmative ? 1UL : 0UL, isPlaceholder);
        }

        public static PlaintextBallotContest ContestFrom(ContestDescription contest, Func<string, int, bool> voteResultFunc)
        {
            List<PlaintextBallotSelection> selections = new List<PlaintextBallotSelection>();

            for (ulong i = 0; i < contest.SelectionsSize; i++)
            {
                var isAffirmative = voteResultFunc(contest.ObjectId, (int)i);
                selections.Add(SelectionFrom(contest.GetSelectionAtIndex(i), isAffirmative));
            }

            return new PlaintextBallotContest(contest.ObjectId, selections.ToArray());
        }

        public static PlaintextBallot GetFakeBallotWithContest(InternalManifest manifest, string contestId, int numberOfSelections, string styleId)
        {
            return GetFakeBallotInternal(manifest, styleId, (currentContestId, selectionId) =>
            {
                if (currentContestId == contestId)
                    return selectionId < numberOfSelections;
                return selectionId == 0;
            });
        }

        public static PlaintextBallot GetFakeBallot(InternalManifest manifest, uint maxChoices = 1)
        {
            var styleId = manifest.GetBallotStyleAtIndex(0).ObjectId;
            return GetFakeBallotInternal(manifest, styleId, (_, selectionId) => selectionId < maxChoices);
        }

        public static PlaintextBallot GetFakeBallotInternal(InternalManifest manifest, string styleId,
            Func<string, int, bool> voteResultFunc)
        {
            List<PlaintextBallotContest> contests = new List<PlaintextBallotContest>();
            for (ulong i = 0; i < manifest.ContestsSize; i++)
            {
                contests.Add(ContestFrom(manifest.GetContestAtIndex(i), voteResultFunc));
            }

            const string ballotId = "ballot-id-123";
            return new PlaintextBallot(ballotId, styleId, contests.ToArray());
        }
    }
}
