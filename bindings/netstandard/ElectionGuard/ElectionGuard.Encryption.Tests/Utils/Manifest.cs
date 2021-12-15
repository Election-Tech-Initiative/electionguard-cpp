using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ElectionGuard.Encryption.Tests.Utils
{
    public class ManifestGenerator
    {
        /// <summary>
        /// A test manifest that is loaded from a json file
        /// </summary>
        public static Manifest GetJeffersonCountyManifest_FromFile()
        {
            var path = Path.Combine(AppContext.BaseDirectory, "data", "election_manifest_jefferson_county.json");
            var text = File.ReadAllText(path);
            return new Manifest(text);
        }

        public static Manifest GetManifestFromFile(string version, string sample)
        {
            var path = Path.Combine(version, "sample", sample, "manifest.json");
            return GetManifestFromFile(path);
        }

        public static Manifest GetManifestFromFile(string filepath)
        {
            var path = Path.Combine(AppContext.BaseDirectory, "data", filepath);
            var text = File.ReadAllText(path);
            return new Manifest(text);
        }

        /// <summary>
        /// A minimal test manifest demonstrating a single contest with two choices and a writein
        /// </summary>
        public static Manifest GetJeffersonCountyManifest_Minimal()
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
                startDate: DateTime.UtcNow,
                endDate: DateTime.UtcNow.AddDays(1),
                gpUnits.ToArray(),
                (new List<Party>()).ToArray(),
                candidates.ToArray(),
                contests.ToArray(),
                ballotStyles.ToArray()
            );
        }

        /// <summary>
        /// A test manifest demonstrating multiple ballot styles.  
        /// In the ElectionGuard data structure each contest belongs 
        /// to a Geopolitical Unit, and then each Geopolitical Unit is part of
        /// a ballot style.  This example also adds some more metadata 
        /// such as language transaltions for certain objects
        /// </summary>
        public static Manifest GetJeffersonCountyManifest_MultipleBallotStyles()
        {
            var gpUnits = new List<GeopoliticalUnit>
            {
                new GeopoliticalUnit("jefferson-county", "Jefferson County", ReportingUnitType.county),
                new GeopoliticalUnit(
                    "jefferson-county-school-district-1", "Jefferson County School District 1", ReportingUnitType.school)
            };

            var parties = new List<Party>
            {
                new Party("federalist-party"),
                new Party("whig-party")
            };

            var candidates = new List<Candidate>
            {
                new Candidate("benjamin-franklin", isWriteIn: false),
                new Candidate("john-adams", isWriteIn: false),
                // The write-in candidate can be reused for all contests
                new Candidate("write-in", isWriteIn: true),
                new Candidate("referendum-pineapple-affirmative", new InternationalizedText (
                    new []{ 
                        // Adding language info isn't requred for the library to function
                        // but is useful for providing what data was displayed to users
                        // since it is included in the manifest hash.
                        new Language("Pineapple should be banned on pizza", "en"),
                        // TODO: #176: new Language("La piña debe prohibirse en la pizza", "es"),
                }), parties[0].ObjectId, "some-image-uri-for-logo", isWriteIn: false),
                new Candidate("referendum-pineapple-negative", new InternationalizedText (
                    new []{
                        new Language("Pineapple should not be banned on pizza", "en"),
                    // new Language("La piña no debe prohibirse en la pizza", "es"),
                }), parties[1].ObjectId, "some-image-uri-for-logo", isWriteIn: false)
            };

            var contests = new List<ContestDescription>
            {
                // Justice Supreme Court
                new ContestDescription(
                    objectId: "justice-supreme-court",
                    electoralDistrictId: gpUnits[0].ObjectId,  // jefferson-county
                    sequenceOrder: 0,
                    VoteVariationType.n_of_m,
                    numberElected: 1,
                    name: "Justice of the Supreme Court",
                    selections: new[]
                    {
                        new SelectionDescription(
                            "benjamin-franklin-selection",
                            candidates[0].CandidateId,          // benjamin-franklin
                            sequenceOrder: 0),
                        new SelectionDescription(
                            "john-adams-selection",
                            candidates[1].CandidateId,          // john-adams
                            sequenceOrder: 1),
                        new SelectionDescription(
                            "write-in-selection",
                            candidates[2].CandidateId,          // write-in
                            sequenceOrder: 2)
                    }),

                // Pineapple Referendum
                new ContestDescription(
                    objectId: "referendum-pineapple",
                    electoralDistrictId: gpUnits[1].ObjectId, // jefferson-county-school-district-1 
                    sequenceOrder: 1,
                    VoteVariationType.n_of_m,
                    numberElected: 1,
                    votesAllowed: 1,
                    name: "The Pineapple Question",
                    ballotTitle: new InternationalizedText (
                        new []{
                            new Language("Should pineapple be banned on pizza?", "en"),
                        // TODO: #176: new Language("¿Debería prohibirse la piña en la pizza?", "es"),
                        }),
                    ballotSubtitle: new InternationalizedText (
                        new []{
                            new Language("The township considers this issue to be very important", "en"),
                        // TODO: #176: new Language("El municipio considera que esta cuestión es muy importante", "es"),
                        }),
                    selections: new[]
                    {
                        new SelectionDescription(
                            "referendum-pineapple-affirmative-selection",
                            // Use Linq to lookup the value
                            candidates.First(
                                i => i.ObjectId == "referendum-pineapple-affirmative"
                            ).CandidateId,
                            sequenceOrder: 0),
                        new SelectionDescription(
                            "referendum-pineapple-negative-selection",
                            // Or just use the known string
                            "referendum-pineapple-negative",
                            sequenceOrder: 1)
                    })
            };

            var ballotStyles = new List<BallotStyle>
            {
                new BallotStyle(
                    "jefferson-county-ballot-style",
                    new[] { gpUnits[0].ObjectId }),
                new BallotStyle(
                    "jefferson-county-school-district-1",
                    new[] {
                        gpUnits[0].ObjectId,
                        gpUnits[1].ObjectId,
                    })
            };

            return new Manifest(
                "jefferson-county-open-primary",
                ElectionType.primary,
                startDate: DateTime.UtcNow,
                endDate: DateTime.UtcNow.AddDays(1),
                gpUnits.ToArray(),
                parties.ToArray(),
                candidates.ToArray(),
                contests.ToArray(),
                ballotStyles.ToArray()
            );
        }
    }
}
