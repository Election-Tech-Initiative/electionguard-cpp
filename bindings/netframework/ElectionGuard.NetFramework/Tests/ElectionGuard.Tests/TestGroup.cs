using NUnit.Framework;
using System;

using ElectionGuardInterop;

namespace ElectionGuard.Tests
{
    [TestFixture]
    public class TestGroup
    {
        [Test]
        public void Test_Can_Invoke_Group()
        {
            var data = new ulong[4] { 5, 0, 0, 0 };
            var element = new ElementModQ(data);

            Assert.That(element.Hex == "05");
        }
    }
}
