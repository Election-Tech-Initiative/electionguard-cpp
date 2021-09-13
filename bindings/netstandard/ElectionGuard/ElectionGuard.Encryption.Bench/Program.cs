using ElectionGuard.Bench;
using System;

namespace ElectionGuard.Encryption.Bench
{
    class Program
    {
        static void Main(string[] args)
        {
            BenchEncrypt.Bench_Encrypt_Ballot_Simple();
        }
    }
}
