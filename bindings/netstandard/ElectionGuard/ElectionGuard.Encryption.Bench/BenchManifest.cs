using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ElectionGuard.Encryption.Bench
{
    class BenchManifest
    {
        public static bool Bench_Can_Construct_Internationalized_Text()
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            // Arrange
            var language_1 = new Language("some words", "en");
            var language_2 = new Language("algunas palabras", "es");
            var languages = new[] { language_1, language_2 };

            // Act
            var subject = new InternationalizedText(languages);

            // Assert
            var actual = subject.GetTextAt(0);
            Console.WriteLine($"actual.Value: {actual.Value}");
            sw.Stop();
            Console.WriteLine("Elapsed={0}", sw.Elapsed);
            return true;
        }
    }
}
