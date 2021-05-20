#pragma once

#include <chrono>
#include <codecvt>
#include <iterator>
#include <locale>
#include <string>
#include <vcclr.h>
#include <vector>

using namespace System;

namespace ElectionGuardInterop
{

    ref struct Utilities {
        static void MarshalString(System::String ^ s, std::string &os)
        {
            using namespace System::Runtime::InteropServices;
            pin_ptr<const wchar_t> str = PtrToStringChars(s);

            const wchar_t *chars = str;
            os = Utilities::wideStringToString(std::wstring(chars));
        }

        static void MarshalStringAnsi(System::String ^ s, std::string &os)
        {
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

        using ticks =
          std::chrono::duration<std::int64_t, std::ratio_multiply<std::ratio<100>, std::nano>>;

        static std::chrono::system_clock::time_point ToNativeTime(DateTime dateTime)
        {
            using namespace std::chrono;
            return system_clock::time_point{ticks{dateTime.Ticks - 621355968000000000}};
        }

        static std::wstring stringToWideString(const std::string &str)
        {
            using convert_typeX = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_typeX, wchar_t> converterX;

            return converterX.from_bytes(str);
        }

        static std::string wideStringToString(const std::wstring &wstr)
        {
            using convert_typeX = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convert_typeX, wchar_t> converterX;

            return converterX.to_bytes(wstr);
        }
    };
} // namespace ElectionGuardInterop
