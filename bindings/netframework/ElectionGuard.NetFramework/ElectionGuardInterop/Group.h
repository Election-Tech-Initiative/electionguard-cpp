#pragma once

#include "ManagedInstance.h"
#include "constants.h"
#include "group.hpp"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ElectionGuardInterop
{
  public
    ref class ElementModP : ManagedInstance<electionguard::ElementModP>
    {
      public:
        ElementModP(array<uint64_t> ^ elem)
        {
            pin_ptr<uint64_t> pinned = &elem[0];
            uint64_t array[MAX_P_LEN]{};
            for (int i = 0; i < MAX_P_LEN; i++) {

                array[i] = pinned[i];
            }
            this->_instance = new electionguard::ElementModP(array);
        }
        internal : ElementModP(std::unique_ptr<electionguard::ElementModP> other)
        {
            this->_instance = other.release();
        }
        ElementModP(electionguard::ElementModP *unowned) : ManagedInstance(unowned, false) {}
        ElementModP(const electionguard::ElementModP *unowned) : ManagedInstance(unowned) {}

      public:
        property bool IsInBounds
        {
            bool get() { return _instance->isInBounds(); }
        }

        property array<uint8_t> ^
          Bytes {
              array<uint8_t> ^ get() {
                  auto bytes = _instance->toBytes();
                  array<Byte> ^ byteArray = gcnew array<Byte>(bytes.size() + 2);
                  Marshal::Copy((IntPtr)&bytes, byteArray, 0, bytes.size());
                  return byteArray;
              }
          }

          property String ^
          Hex { String ^ get() { return gcnew String(_instance->toHex().c_str()); } }

          internal : std::unique_ptr<electionguard::ElementModP>
                     clone()
        {
            return _instance->clone();
        }
    };

  public
    ref class ElementModQ : ManagedInstance<electionguard::ElementModQ>
    {
      public:
        ElementModQ(array<uint64_t> ^ elem)
        {
            pin_ptr<uint64_t> pinned = &elem[0];
            uint64_t array[MAX_Q_LEN]{};
            for (int i = 0; i < MAX_Q_LEN; i++) {

                array[i] = pinned[i];
            }
            this->_instance = new electionguard::ElementModQ(array);
        }
        internal : ElementModQ(const uint64_t (&elem)[MAX_Q_LEN])
            : ManagedInstance(new electionguard::ElementModQ(elem))
        {
        }
        ElementModQ(std::unique_ptr<electionguard::ElementModQ> other)
        {
            this->_instance = other.release();
        }
        ElementModQ(electionguard::ElementModQ *unowned) : ManagedInstance(unowned, false) {}
        ElementModQ(const electionguard::ElementModQ *unowned) : ManagedInstance(unowned) {}

      public:
        property bool IsInBounds
        {
            bool get() { return _instance->isInBounds(); }
        }

        property array<uint8_t> ^
          Bytes {
              array<uint8_t> ^ get() {
                  auto bytes = _instance->toBytes();
                  array<Byte> ^ byteArray = gcnew array<Byte>(bytes.size() + 2);
                  Marshal::Copy((IntPtr)&bytes, byteArray, 0, bytes.size());
                  return byteArray;
              }
          }

          property String ^
          Hex { String ^ get() { return gcnew String(_instance->toHex().c_str()); } }

          internal : std::unique_ptr<electionguard::ElementModQ>
                     clone()
        {
            return _instance->clone();
        }
    };

  public
    ref class Constants
    {
      public:
        static property ElementModQ ^
          ONE_MOD_Q { ElementModQ ^ get() { return gcnew ElementModQ(ONE_MOD_Q_ARRAY); } }

          static property ElementModQ ^
          TWO_MOD_Q { ElementModQ ^ get() { return gcnew ElementModQ(TWO_MOD_Q_ARRAY); } }
    };
} // namespace ElectionGuardInterop