using System;
using Microsoft.Win32.SafeHandles;
using System.Runtime.ConstrainedExecution;

namespace ElectionGuard
{
    internal abstract class ElectionguardSafeHandle<T>
        : SafeHandleZeroOrMinusOneIsInvalid
        where T : unmanaged
    {
        // Objects constructed with the default constructor
        // own the context
        internal ElectionguardSafeHandle()
            : base(ownsHandle: true)
        {

        }

        // Objects constructed from a structure pointer
        // do not own the context
        internal unsafe ElectionguardSafeHandle(
            IntPtr handle)
            : base(ownsHandle: false)
        {
            SetHandle(handle);
        }

        internal unsafe ElectionguardSafeHandle(
            IntPtr handle, bool ownsHandle)
            : base(ownsHandle)
        {
            SetHandle(handle);
        }

        internal unsafe ElectionguardSafeHandle(
            T* handle)
            : base(ownsHandle: false)
        {
            SetHandle((IntPtr)handle);
        }

        internal unsafe ElectionguardSafeHandle(
            T* handle, bool ownsHandle)
            : base(ownsHandle)
        {
            SetHandle((IntPtr)handle);
        }

        public unsafe T* TypedPtr => (T*)handle;

        public IntPtr Ptr => handle;

        protected abstract bool Free();

        protected override bool ReleaseHandle()
        {
            try
            {
                var freed = Free();
                if (freed)
                {
                    Close();
                }
                return freed;
            }
            catch (Exception ex)
            {
                throw new ElectionGuardException($"{nameof(T)} ERROR ReleaseHandle: {ex.Message}", ex);
            }
        }
    }
}
