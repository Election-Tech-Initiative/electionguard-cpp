using System.Collections.Generic;

namespace ElectionGuardCore.Elections
{
    public class BallotStyle
    {
        public string ObjectId { get; set; }
        public List<string> GeopoliticalUnitIds { get; set; }
        public List<string> PartyIds { get; set; }
        public string ImageUri { get; set; }
    }
}
