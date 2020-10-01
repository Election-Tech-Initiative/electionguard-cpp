using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using Newtonsoft.Json.Serialization;

namespace ElectionGuardCore.Elections
{
    [JsonConverter(typeof(StringEnumConverter), typeof(SnakeCaseNamingStrategy))]
    public enum VoteVariationType
    {
        Unknown = 0,
        OneOfM = 1,
        Approval = 2,
        Borda = 3,
        Cumulative = 4,
        Majority = 5,
        NOfM = 6,
        Plurality = 7,
        Proportional = 8,
        Range = 9,
        Rcv = 10,
        SuperMajority = 11,
        Other = 12
    }
}
