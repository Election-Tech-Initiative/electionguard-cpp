using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ElectionGuard.Encryption.Bench
{
    class BenchManifest : Fixture
    {
        public override void Run()
        {
            Bench_Can_Construct_Internationalized_Text();
        }

        public void Bench_Can_Construct_Internationalized_Text()
        {
            Console.WriteLine("Bench_Can_Construct_Internationalized_Text");

            Run(() =>
            {
                var language_1 = new Language("some words", "en");
                var language_2 = new Language("algunas palabras", "es");
                var languages = new[] { language_1, language_2 };
                var subject = new InternationalizedText(languages);

                // Assert
                var actual = subject.GetTextAt(0);
            });
        }
    }
}
