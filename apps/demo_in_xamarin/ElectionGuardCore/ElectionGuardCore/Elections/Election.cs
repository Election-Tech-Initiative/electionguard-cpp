namespace ElectionGuardCore.Elections
{
    public class Election
    {
        public string Id { get; set; }
        public ElectionDescription ElectionDescription { get; set; }
        public ElectionState State { get; set; }
    }
}
