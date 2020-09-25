using System.Collections.Generic;

namespace ElectionGuardCore.Models
{
    public class ContactInformation
    {
        public string AddressLine { get; set; }
        public string Name { get; set; }
        public List<AnnotatedString> Email { get; set; }
        public List<AnnotatedString> Phone { get; set; }
    }
}
