using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NativeInternalElectionDescription = NativeInterface.InternalElectionDescription.InternalElectionDescriptionType;
    using NativeCiphertextElectionContext = NativeInterface.CiphertextElectionContext.CiphertextElectionType;

    public class InternalElectionDescription
    {
        internal unsafe NativeInternalElectionDescription* Handle;

        public unsafe InternalElectionDescription(string json)
        {
            Handle = NativeInterface.InternalElectionDescription.FromJson(json);
        }

        public unsafe string ToJson()
        {
            _ = NativeInterface.InternalElectionDescription.ToJson(Handle, out IntPtr pointer);
            var json = Marshal.PtrToStringAnsi(pointer);
            return json;
        }
    }

    public class CiphertextElectionContext
    {
        internal unsafe NativeCiphertextElectionContext* Handle;

        public unsafe CiphertextElectionContext(string json)
        {
            Handle = NativeInterface.CiphertextElectionContext.FromJson(json);           
        }

        public unsafe string ToJson()
        {
            _ = NativeInterface.CiphertextElectionContext.ToJson(Handle, out IntPtr pointer);
            var json = Marshal.PtrToStringAnsi(pointer);
            return json;
        }
    }
}
