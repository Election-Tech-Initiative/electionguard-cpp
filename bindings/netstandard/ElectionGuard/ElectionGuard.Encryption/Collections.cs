using System;
using System.Runtime.InteropServices;

namespace ElectionGuard
{
    using NativeLinkedList = NativeInterface.LinkedList.LinkedListHandle;

    /// <Summary>
    /// A singly-linked list data structure composed of strings for keys and strings for values.
    /// </Summary>
    public class LinkedList : DisposableBase
    {
        /// <Summary>
        /// Get the count of elements in the list.
        /// </Summary>
        public unsafe ulong Count
        {
            get
            {
                return NativeInterface.LinkedList.GetCount(Handle);
            }
        }

        internal unsafe NativeLinkedList Handle;

        unsafe internal LinkedList(NativeLinkedList handle)
        {
            Handle = handle;
        }

        /// <summary>
        /// Default constructor
        /// </summary>
        unsafe public LinkedList()
        {
            var status = NativeInterface.LinkedList.New(out Handle);
            status.ThrowIfError();
        }

        /// <summary>
        /// Append the key value pair to the end of the linked list
        /// </summary>
        public unsafe void Append(string key, string value)
        {
            var status = NativeInterface.LinkedList.Append(Handle, key, value);
            status.ThrowIfError();
        }

        /// <summary>
        /// Delete the last value in the list
        /// </summary>
        public unsafe void DeleteLast()
        {
            var status = NativeInterface.LinkedList.DeleteLast(Handle);
            status.ThrowIfError();
        }

        /// <summary>
        /// Get the element at the designated position
        /// </summary>
        public unsafe Tuple<string, string> GetElementAt(ulong position)
        {
            var status = NativeInterface.LinkedList.GetElementAt(
                    Handle, position, out IntPtr key, out IntPtr value);
            status.ThrowIfError();
            return Tuple.Create(Marshal.PtrToStringAnsi(key), Marshal.PtrToStringAnsi(value));
        }

        /// <summary>
        /// Get the value at the designated position
        /// </summary>
        public unsafe string GetValueAt(ulong position)
        {
            var status = NativeInterface.LinkedList.GetValueAt(
                    Handle, position, out IntPtr value);
            status.ThrowIfError();
            return Marshal.PtrToStringAnsi(value);
        }

        public unsafe string this[string searchKey]
        {
            get
            {
                var count = NativeInterface.LinkedList.GetCount(Handle);
                for (ulong position = 0; position < count; position++)
                {
                    var status = NativeInterface.LinkedList.GetElementAt(
                            Handle, position, out IntPtr key, out IntPtr value);
                    status.ThrowIfError();
                    if (Marshal.PtrToStringAnsi(key) == searchKey)
                        return Marshal.PtrToStringAnsi(value);
                }
                return null;
            }
        }

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
        protected override unsafe void DisposeUnmanaged()
#pragma warning restore CS1591 // Missing XML comment for publicly visible type or member
        {
            base.DisposeUnmanaged();

            if (Handle == null || Handle.IsInvalid) return;
            Handle.Dispose();
            Handle = null;
        }
    }
}
