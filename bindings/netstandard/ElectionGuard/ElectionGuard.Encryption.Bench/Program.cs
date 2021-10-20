using ElectionGuard.Bench;
using System;

namespace ElectionGuard.Encryption.Bench
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("benchmarking");

            BenchEncrypt.Bench_Encrypt_Ballot_Simple();
            BenchManifest.Bench_Can_Construct_Internationalized_Text();

            Console.WriteLine("done! press any key to exit");

            Console.ReadKey();
        }
    }
}
