using Newtonsoft.Json;

namespace ElectionGuardCore.Elections
{
    public class Party
    {
        public string ObjectId { get; set; }
        public string Abbreviation { get; set; }
        public string Color { get; set; }
        public string Uri { get; set; }
        [JsonProperty("Name")]
        public InternationalizedText BallotName { get; set; }
    }
}
