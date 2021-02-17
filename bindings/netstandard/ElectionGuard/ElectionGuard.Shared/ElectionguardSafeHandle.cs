using System;
using Microsoft.Win32.SafeHandles;
using System.Runtime.ConstrainedExecution;

namespace ElectionGuard
{
    public abstract class ElectionguardSafeHandle<T>
        : SafeHandleZeroOrMinusOneIsInvalid
        where T : unmanaged
    {
        public ElectionguardSafeHandle()
            : base(true)
        {

        }

        public unsafe ElectionguardSafeHandle(
            T* handle)
            : base(false)
        {
            SetHandle((IntPtr)handle);
        }

        public unsafe ElectionguardSafeHandle(
            T* handle, bool ownsHandle)
            : base(ownsHandle)
        {
            SetHandle((IntPtr)handle);
        }

        public IntPtr Ptr => handle;

        protected abstract bool Free();

        [ReliabilityContract(Consistency.WillNotCorruptState, Cer.MayFail)]
        protected override bool ReleaseHandle()
        {
            try
            {
                return Free();
            }
            catch(Exception ex)
            {
                Console.WriteLine($"{nameof(T)} ERROR ReleaseHandle: {ex.Message}");
                return false;
            }
        }
    }
}
