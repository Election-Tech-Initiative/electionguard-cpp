using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ElectionGuard.Encryption.Bench
{
    public abstract class Fixture
    {
        public Fixture()
        {

        }

        public virtual void SetUp()
        {

        }

        public virtual void TearDown()
        {

        }

        public abstract void Run();

        public virtual void Run(Action executor)
        {
            SetUp();
            BenchmarkCase(executor);
            TearDown();
        }

        protected virtual void BenchmarkCase(Action executor)
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            executor();
            sw.Stop();
            Console.WriteLine("Elapsed={0}", sw.Elapsed);
        }
    }
}
