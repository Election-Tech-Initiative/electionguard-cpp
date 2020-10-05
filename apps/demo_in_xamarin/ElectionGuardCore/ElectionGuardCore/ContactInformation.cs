using System.Collections.Generic;

namespace ElectionGuardCore
{
    public class ContactInformation
    {
        public List<string> AddressLine { get; set; }
        public string Name { get; set; }
        public List<AnnotatedString> Email { get; set; }
        public List<AnnotatedString> Phone { get; set; }
    }
}
