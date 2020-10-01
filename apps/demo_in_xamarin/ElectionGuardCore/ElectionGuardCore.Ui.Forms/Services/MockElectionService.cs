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

        private static readonly string SampleCiphertextelectionContextJson = "{\"crypto_base_hash\":\"30646075209226194318764201276176181549191373179751202907953635677105303212019\",\"crypto_extended_base_hash\":\"97125355090260421788811922826693853528586871831509203257916648484828683653587\",\"description_hash\":\"72693218853007076690216337059328285868949668173962526436942281008218813401456\",\"elgamal_public_key\":\"89247119125444089994171855388429729655256357880769999846278201885764608823492278858970138524703594393056226971028438981839110758629413721900466327767829784249719082899229533458917266254493572288484994844286737451612620699657148055972358746562605294138600951036089225002453438692109461881482564236998811561120367677436230207836531226624205610045400781612753873438877067018402683166483052212777096066762502248333992013336380640677245002644808854444968002026230965680152886485374464975096536999587576114964811304066429369894871409583366767944588797053313802225943818502304257643804805128612702993960673646346663074186843366987184785346532507273431781551311346181232896111276005625186367149848014312194073773461356773547824268624341139786168266185713193384799397512669173499223341623642606759113336516922889901521634089585360946193305224578519717433436327192854287165817253859197285310041645677060011516799758373122019991295775783590371245362865935426565915874608685809741726261318304495165328018400619843018099650173946995259003412111819369577250702531681738994632256152239218833211679984512823530094320397560993458708331336292028814787018272866257327624550676878150446065227873452747336990015802388885928452470908592602600925002384899\",\"number_of_guardians\":5,\"quorum\":3}";

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

        public Task<CiphertextElectionContext> GetCiphertextElectionContext()
        {
            var ciphertextelectionContext = JsonConvert.DeserializeObject<CiphertextElectionContext>(SampleCiphertextelectionContextJson,
                new JsonSerializerSettings
                {
                    ContractResolver = new DefaultContractResolver
                    {
                        NamingStrategy = new SnakeCaseNamingStrategy()
                    }
                });
            return Task.FromResult(ciphertextelectionContext);
        }

        public IDictionary<string, bool> Votes { get; } = new Dictionary<string, bool>();   // TODO for demo purposes only
    }
}
