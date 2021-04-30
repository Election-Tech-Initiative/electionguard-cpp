#pragma once

#include "Group.h"
#include "ManagedInstance.h"
#include "Utilities.h"
#include "manifest.hpp"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ElectionGuardInterop
{
  public
    ref class Manifest : ManagedInstance<electionguard::Manifest>
    {
      public:
        Manifest(String ^ json)
        {
            std::string _json;
            Utilities::MarshalString(json, _json);
            Console::WriteLine(gcnew String(_json.c_str()));
            auto unmanaged = electionguard::Manifest::fromJson(_json);
            this->_instance = unmanaged.release();
        }
        internal : Manifest(std::unique_ptr<electionguard::Manifest> other)
        {
            this->_instance = other.release();
        }

      public:
        property String ^
          Json {
              String ^ get() {
                  auto unmanaged = this->_instance->toJson();
                  return gcnew String(unmanaged.c_str());
              }
          }

          static Manifest ^
          FromJson(String ^ json) {
              std::string _json;
              Utilities::MarshalString(json, _json);
              auto unamanaged = electionguard::Manifest::fromJson(_json);
              return gcnew Manifest(move(unamanaged));
          }
    };

  public
    ref class InternalManifest : ManagedInstance<electionguard::InternalManifest>
    {
      public:
        InternalManifest(Manifest ^ manifest)
            : ManagedInstance(new electionguard::InternalManifest(*manifest->_instance))
        {
        }

        internal : InternalManifest(std::unique_ptr<electionguard::InternalManifest> other)
        {
            this->_instance = other.release();
        }

      public:
        property ElementModQ ^
          ManifestHash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->getManifestHash();
                  return gcnew ElementModQ(unmanaged);
              }
          }

          property String ^
          Json {
              String ^ get() {
                  auto unmanaged = this->_instance->toJson();
                  return gcnew String(unmanaged.c_str());
              }
          }

          static InternalManifest ^
          FromJson(String ^ json) {
              std::string _json;
              Utilities::MarshalString(json, _json);
              auto unmanaged = electionguard::InternalManifest::fromJson(_json);
              return gcnew InternalManifest(move(unmanaged));
          }
    };

} // namespace ElectionGuardInterop
