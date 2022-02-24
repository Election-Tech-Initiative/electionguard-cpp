using System;
using System.Collections.Generic;

namespace ElectionGuard.Encryption.Bench
{
    class Program
    {
        static readonly List<Fixture> fixtures =
        new List<Fixture>{
            new BenchEncrypt(),
            new BenchManifest()
        };

        static void Main(string[] args)
        {
            Console.WriteLine("------ benchmarking ------");

            foreach(var fixture in fixtures)
            {
                fixture.Run();
            }
        }
    }
}
