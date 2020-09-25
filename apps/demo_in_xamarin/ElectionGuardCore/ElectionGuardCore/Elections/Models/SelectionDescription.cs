namespace ElectionGuardCore.Elections.Models
{
    public class SelectionDescription
    {
        public string ObjectId { get; set; }
        public string CandidateId { get; set; }
        public ulong SequenceOrder { get; set; }
    }
}
