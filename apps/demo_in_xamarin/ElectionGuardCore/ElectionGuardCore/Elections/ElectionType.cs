using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using Newtonsoft.Json.Serialization;

namespace ElectionGuardCore.Elections
{
    [JsonConverter(typeof(StringEnumConverter), typeof(SnakeCaseNamingStrategy))]
    public enum ElectionType
    {
        Unknown = 0,
        General = 1,
        PartisanPrimaryClosed = 2,
        PartisanPrimaryOpen = 3,
        Primary = 4,
        Runoff = 5,
        Special = 6,
        Other = 7
    }
}
