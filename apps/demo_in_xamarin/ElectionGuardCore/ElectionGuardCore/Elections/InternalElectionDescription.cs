using System;
using System.Collections.Generic;

namespace ElectionGuardCore.Elections
{
    public class InternalElectionDescription
    {
        public List<ContestDescription> Contests { get; set; }
        public string DescriptionHash { get; set; }
    }
}
