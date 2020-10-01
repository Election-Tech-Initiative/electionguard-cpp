using System.Collections.Generic;
using System.Threading.Tasks;
using ElectionGuardCore.Elections;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;

namespace ElectionGuardCore.Ui.Forms.Services
{
    internal class MockElectionService : IElectionService
    {
        private static readonly string SampleElectionDescriptionJson = @"
{
  ""ballot_styles"": [
    {
      ""geopolitical_unit_ids"": [
        ""hamilton-county""
      ],
      ""object_id"": ""congress-district-7-hamilton-county""
    }
  ],
  ""candidates"": [
    {
      ""ballot_name"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Joseph Barchi""
          }
        ]
      },
      ""object_id"": ""barchi""
    },
    {
      ""ballot_name"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Adam Cramer""
          }
        ]
      },
      ""object_id"": ""cramer""
    },
    {
      ""ballot_name"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Daniel Court""
          }
        ]
      },
      ""object_id"": ""court""
    },
    {
      ""ballot_name"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Alvin Boone""
          }
        ]
      },
      ""object_id"": ""boone""
    },
    {
      ""ballot_name"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Ashley Hildebrand-McDougall""
          }
        ]
      },
      ""object_id"": ""hildebrand""
    },
    {
      ""ballot_name"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Martin Patterson""
          }
        ]
      },
      ""object_id"": ""patterson""
    },
    {
      ""ballot_name"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Charlene Franz""
          }
        ]
      },
      ""object_id"": ""franz""
    },
    {
      ""ballot_name"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Gerald Harris""
          }
        ]
      },
      ""object_id"": ""harris""
    },
    {
      ""ballot_name"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Linda Bargmann""
          }
        ]
      },
      ""object_id"": ""bargmann""
    },
    {
      ""ballot_name"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Barbara Abcock""
          }
        ]
      },
      ""object_id"": ""abcock""
    }
  ],
  ""contests"": [
    {
      ""@type"": ""CandidateContest"",
      ""ballot_selections"": [
        {
          ""candidate_id"": ""barchi"",
          ""object_id"": ""barchi-selection"",
          ""sequence_order"": 0
        },
        {
          ""candidate_id"": ""cramer"",
          ""object_id"": ""cramer-selection"",
          ""sequence_order"": 1
        },
        {
          ""candidate_id"": ""court"",
          ""object_id"": ""court-selection"",
          ""sequence_order"": 2
        },
        {
          ""candidate_id"": ""boone"",
          ""object_id"": ""boone-selection"",
          ""sequence_order"": 3
        },
        {
          ""candidate_id"": ""hildebrand"",
          ""object_id"": ""hildebrand-selection"",
          ""sequence_order"": 4
        },
        {
          ""candidate_id"": ""patterson"",
          ""object_id"": ""patterson-selection"",
          ""sequence_order"": 5
        },
        {
          ""candidate_id"": ""franz"",
          ""object_id"": ""franz-selection"",
          ""sequence_order"": 6
        },
        {
          ""candidate_id"": ""harris"",
          ""object_id"": ""harris-selection"",
          ""sequence_order"": 7
        },
        {
          ""candidate_id"": ""bargmann"",
          ""object_id"": ""bargmann-selection"",
          ""sequence_order"": 8
        },
        {
          ""candidate_id"": ""abcock"",
          ""object_id"": ""abcock-selection"",
          ""sequence_order"": 9
        }
      ],
      ""ballot_subtitle"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Vote for one""
          },
          {
            ""language"": ""es"",
            ""value"": ""Votar por uno""
          }
        ]
      },
      ""ballot_title"": {
        ""text"": [
          {
            ""language"": ""en"",
            ""value"": ""Hamilton County School Board""
          },
          {
            ""language"": ""es"",
            ""value"": ""Junta Escolar del Condado de Hamilton""
          }
        ]
      },
      ""electoral_district_id"": ""hamilton-county"",
      ""name"": ""Hamilton County School Board"",
      ""number_elected"": 1,
      ""object_id"": ""school-board-contest"",
      ""sequence_order"": 0,
      ""vote_variation"": 1,
      ""votes_allowed"": 1
    }
  ],
  ""election_scope_id"": ""hamilton-county-general-election"",
  ""end_date"": ""2020-03-01T20:00:00-05:00"",
  ""geopolitical_units"": [
    {
      ""contact_information"": {
        ""address_line"": [
          ""1234 Samuel Adams Way"",
          ""Hamilton, Ozark 99999""
        ],
        ""email"": [
          {
            ""annotation"": ""inquiries"",
            ""value"": ""inquiries@hamiltoncounty.gov""
          }
        ],
        ""name"": ""Hamilton County Clerk"",
        ""phone"": [
          {
            ""annotation"": ""domestic"",
            ""value"": ""123-456-7890""
          }
        ]
      },
      ""name"": ""Hamilton County"",
      ""object_id"": ""hamilton-county"",
      ""type"": 9
    }
  ],
  ""name"": {
    ""text"": [
      {
        ""language"": ""en"",
        ""value"": ""Hamiltion County General Election""
      },
      {
        ""language"": ""es"",
        ""value"": ""Elección general del condado de Hamilton""
      }
    ]
  },
  ""parties"": [],
  ""start_date"": ""2020-03-01T08:00:00-05:00"",
  ""type"": 1
}
";

        public Task<ElectionDescription> GetElectionDescription()
        {
            var electionDescription = JsonConvert.DeserializeObject<ElectionDescription>(SampleElectionDescriptionJson,
                new JsonSerializerSettings
                {
                    ContractResolver = new DefaultContractResolver
                    {
                        NamingStrategy = new SnakeCaseNamingStrategy()
                    }
                });
            return Task.FromResult(electionDescription);
        }

        public IDictionary<string, bool> Votes { get; } = new Dictionary<string, bool>();   // TODO for demo purposes only
    }
}
