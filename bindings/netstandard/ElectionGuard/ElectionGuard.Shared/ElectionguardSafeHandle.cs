using System;
using Microsoft.Win32.SafeHandles;
using System.Runtime.ConstrainedExecution;

namespace ElectionGuard
{
    public abstract class ElectionguardSafeHandle<T>
        : SafeHandleZeroOrMinusOneIsInvalid
        where T : unmanaged
    {
        // Objects constructed with the default constructor
        // own the context
        public ElectionguardSafeHandle()
            : base(ownsHandle: true)
        {

        }

        // Objects constructed from a structure pointer
        // do not own the context
        public unsafe ElectionguardSafeHandle(
            T* handle)
            : base(ownsHandle: false)
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
