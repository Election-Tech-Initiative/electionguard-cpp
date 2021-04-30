using NUnit.Framework;

using ElectionGuardInterop;
using ElectionGuard.Tests.Mocks;

namespace ElectionGuard.Tests
{
    [TestFixture]
    class TestManifest
    {
        [Test]
        public void Test_Can_Construct_InternalManifest_From_Manifest()
        {
            var manifest = ManifestGenerator.GetMinimalFakeJeffersonCountyManifest();
            var internalManifest = new InternalManifest(manifest);

            Assert.That(manifest.CryptoHash.Hex == internalManifest.ManifestHash.Hex);
            Assert.That(manifest.IsValid());
        }
    }
}
