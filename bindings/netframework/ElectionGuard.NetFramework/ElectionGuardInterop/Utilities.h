#pragma once

#include <iterator>
#include <string>
#include <vector>

using namespace System;

namespace ElectionGuardInterop
{
    ref struct Utilities
    {
        static void MarshalString(System::String^ s, std::string& os) {
            using namespace System::Runtime::InteropServices;
            const char *chars = (const char *)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
            os = chars;
            Marshal::FreeHGlobal(System::IntPtr((void *)chars));
        }

        static std::vector<uint8_t> MarshalByteArray(array<Byte> ^ bytes)
        {
            pin_ptr<Byte> p = &bytes[0];
            Byte *byte = p;

            std::vector<uint8_t> native;
            native.reserve(bytes->Length);
            std::copy(byte, byte + bytes->Length / sizeof(uint8_t), std::back_inserter(native));
            return native;
        }
    };
}
