using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{

    internal static unsafe class NativeInterface
    {
#if Android
        const string DllName = "libelectionguard.so";
#else
    const string DllName = "__Internal";
#endif

        [DllImport(DllName, EntryPoint = "eg_element_mod_q_create")]
        internal static extern ulong* CreateElementModQ(ulong* element);

        [DllImport(DllName, EntryPoint = "eg_element_mod_q_get")]
        internal static extern int GetElementModQ(ulong* pointer, ulong** element);
    }
}
