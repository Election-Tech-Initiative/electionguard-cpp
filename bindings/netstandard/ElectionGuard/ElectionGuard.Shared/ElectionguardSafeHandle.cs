using System;
using Microsoft.Win32.SafeHandles;

namespace ElectionGuard
{
    public class ElectionguardSafeHandle : SafeHandleZeroOrMinusOneIsInvalid
    {
        public ElectionguardSafeHandle() : base(true) { }

        public IntPtr Ptr => this.handle;

        protected override bool ReleaseHandle()
        {
            //MyMathFuncsWrapper.DisposeMyMathFuncs(handle);
            return true;
        }
    }
}
