using System;
using System.Collections.Generic;
using System.Diagnostics;

using ElectionGuardInterop;
using System;

namespace ElectionGuard.Bench
{
    class Program
    {
        static void Main(string[] args)
        {
            BenchEncrypt.Bench_Encrypt_Ballot_Simple();
        }
    }
}
