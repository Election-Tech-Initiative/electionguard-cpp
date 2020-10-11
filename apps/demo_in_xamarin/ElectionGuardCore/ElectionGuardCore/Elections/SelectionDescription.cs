using Newtonsoft.Json;

namespace ElectionGuardCore.Elections
{
    public class SelectionDescription
    {
        public string ObjectId { get; set; }
        public string CandidateId { get; set; }
        public ulong SequenceOrder { get; set; }

        [JsonIgnore]
        public Candidate Candidate { get; set; }

        [JsonIgnore]
        public bool IsSelected { get; set; }
    }
}
