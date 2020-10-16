using System.Collections.Generic;
using System.Threading.Tasks;
using ElectionGuardCore.Elections;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using Newtonsoft.Json.Serialization;

namespace ElectionGuardCore.Ui.Forms.Services
{
    internal class MockElectionService : IElectionService
    {
        private readonly JsonSerializerSettings _serializerSettings;
        private readonly Dictionary<string, bool> _votes = new Dictionary<string, bool>();

        public MockElectionService()
        {
            _serializerSettings = new JsonSerializerSettings
            {
                ContractResolver = new DefaultContractResolver
                {
                    NamingStrategy = new SnakeCaseNamingStrategy()
                },
                Converters = new List<JsonConverter>
                {
                    new StringEnumConverter(typeof(SnakeCaseNamingStrategy)),
                    new ElementConverter()
                }
            };
        }

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
      ""type"": ""CandidateContest"",
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
      ""vote_variation"": ""one_of_m"",
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
      ""type"": ""county""
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
  ""type"": ""general""
}
";

        private static readonly string SampleCiphertextelectionContextJson = "{\"crypto_base_hash\": \"113133177615765376285011049573550420815271886877763010626891582395189234851926\", \"crypto_extended_base_hash\": \"105453391201168277388902836868037575613019125079321017624607857728655939685293\", \"description_hash\": \"53955750461069851967519202298702206066009470464098563801488249985305233352944\", \"elgamal_public_key\": \"650903852758409990312291004707477534089130033640662303715889879300347470392572690670101560912298594019829500498680782581572320776102549359895131407798036010654305977756399507847597479423671994208260039161369645650200942975974929953888903769695737089030275004229889476017798210970661871162275031836161607571451823712343702817531718010784271941394242942006265819298593036639468800809490092770569604506388439055292605497883778779611729789663664798167171674337068864723339201806682926014651803729389240636842705316934200504855627368696961128633360112017545267132035378493927445218628450907731842309001212988582993449142731558063793504208297748527143100200065294394641695565727310707322384170547753676466622210526941258867623997491196087758801720083186367809900514525632362462844385192386919825231284597310902731851602135234568810844789653646269937313911984366094930582328113744033618007077931478678652204258800943529497120480504301445563522118452831782982792079277539134130956843111167981302599894866033073072191529549456530244798319208372693788385873385381417507913172762961192775063726092027733799628281766673150600650635616539905903227431959490156343811435649089212902563459216275058506452739629763054946580522277008451961937902994906\", \"number_of_guardians\": 5, \"quorum\": 3}";

        public Task<Election> GetElection()
        {
            var electionDescription =
                JsonConvert.DeserializeObject<ElectionDescription>(SampleElectionDescriptionJson, _serializerSettings);
            var election = new Election
            {
                Id = "42",
                ElectionDescription = electionDescription,
                State = ElectionState.Open
            };
            return Task.FromResult(election);
        }

        public Task<CiphertextElectionContext> GetCiphertextElectionContext(string electionId)
        {
            var ciphertextelectionContext =
                JsonConvert.DeserializeObject<CiphertextElectionContext>(SampleCiphertextelectionContextJson,
                    _serializerSettings);
            return Task.FromResult(ciphertextelectionContext);
        }

        public Task CastBallot(string electionId, CiphertextBallot ballot)
        {
            _votes[electionId] = true;
            return Task.CompletedTask;
        }

        public Task SpoilBallot(string electionId, CiphertextBallot ballot)
        {
            _votes[electionId] = false;
            return Task.CompletedTask;
        }

        public Task<bool> HasVoted(string electionId)
        {
            var hasVoted = _votes.ContainsKey(electionId) && _votes[electionId];
            return Task.FromResult(hasVoted);
        }
    }
}
