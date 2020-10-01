using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using Newtonsoft.Json.Serialization;

namespace ElectionGuardCore.Elections
{
    [JsonConverter(typeof(StringEnumConverter), typeof(SnakeCaseNamingStrategy))]
    public enum ReportingUnitType
    {
        Unknown = 0,
        BallotBatch = 1,
        BallotStyleArea = 2,
        Borough = 3,
        City = 4,
        CityCouncil = 5,
        CombinedPrecinct = 6,
        Congressional = 7,
        Country = 8,
        County = 9,
        CountyCouncil = 10,
        DropBox = 11,
        Judicial = 12,
        Municipality = 13,
        PollingPlace = 14,
        Precinct = 15,
        School = 16,
        Special = 17,
        SplitPrecinct = 18,
        State = 19,
        StateHouse = 20,
        StateSenate = 21,
        Township = 22,
        Utility = 23,
        Village = 24,
        VoteCenter = 25,
        Ward = 26,
        Water = 27,
        Other = 28
    }
}
