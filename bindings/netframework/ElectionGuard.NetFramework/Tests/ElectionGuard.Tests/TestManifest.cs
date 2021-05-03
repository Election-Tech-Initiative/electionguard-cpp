using NUnit.Framework;

using ElectionGuardInterop;
using ElectionGuard.Tests.Mocks;

namespace ElectionGuard.Tests
{
    [TestFixture]
    class TestManifest
    {
        [Test]
        public void Test_Can_Construct_InternalManifest_From_Manifest_Minimal()
        {
            // Get a simple manifest that shows the bare minimum data required
            var manifest = ManifestGenerator.GetJeffersonCountyManifest_Minimal();
            var internalManifest = new InternalManifest(manifest);

            Assert.That(manifest.CryptoHash.Hex == internalManifest.ManifestHash.Hex);
            Assert.That(manifest.IsValid());
        }

        [Test]
        public void Test_Can_Construct_InternalManifest_From_Manifest_MultipleBallotStyles()
        {
            // Get a slightly more complex manifest that shows including multiple ballot styles
            var manifest = ManifestGenerator.GetJeffersonCountyManifest_MultipleBallotStyles();
            var internalManifest = new InternalManifest(manifest);

            Assert.That(manifest.CryptoHash.Hex == internalManifest.ManifestHash.Hex);
            Assert.That(manifest.IsValid());
        }

        [Test]
        public void Test_Can_Serialize_Manifest_Minimal()
        {
            var manifest = ManifestGenerator.GetJeffersonCountyManifest_Minimal();
            var json = manifest.Json;

            var result = new Manifest(json);

            Assert.That(manifest.CryptoHash.Hex == result.CryptoHash.Hex);
            Assert.That(result.IsValid());
        }

        [Test]
        public void Test_Can_Serialize_Manifest_MultipleBallotStyles()
        {
            var manifest = ManifestGenerator.GetJeffersonCountyManifest_MultipleBallotStyles();
            var json = manifest.Json;

            var result = new Manifest(json);

            Assert.That(manifest.CryptoHash.Hex == result.CryptoHash.Hex);
            Assert.That(result.IsValid());
        }
    }
}
