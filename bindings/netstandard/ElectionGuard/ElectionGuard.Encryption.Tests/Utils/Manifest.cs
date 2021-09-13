using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ElectionGuard.Encryption.Tests.Utils
{
    public class ManifestGenerator
    {
        public static Manifest GetJeffersonCountyManifest()
        {
            var path = Path.Combine(AppContext.BaseDirectory, "Data", "election_manifest_jefferson_county.json");
            var text = File.ReadAllText(path);
            return new Manifest(text);
        }
    }
}
