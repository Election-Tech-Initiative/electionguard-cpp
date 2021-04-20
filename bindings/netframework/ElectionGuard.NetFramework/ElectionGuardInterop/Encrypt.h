#pragma once

#include "Ballot.h"
#include "Election.h"
#include "ManagedInstance.h"
#include "Manifest.h"
#include "Utilities.h"

#include "encrypt.hpp"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ElectionGuardInterop
{
  public ref class EncryptionDevice: ManagedInstance<electionguard::EncryptionDevice>
    {
    public:
        EncryptionDevice(uint64_t deviceUuid, uint64_t sessionUuid, uint64_t launchCode, String ^ location)
        {
            std::string _location;
            Utilities::MarshalString(location, _location);
            this->_instance = new electionguard::EncryptionDevice(
                deviceUuid, sessionUuid, launchCode, _location);
        }
    };
  public ref class EncryptionMediator : ManagedInstance<electionguard::EncryptionMediator>
    {
      public:
        EncryptionMediator(InternalManifest ^ internalManifest, CiphertextElectionContext ^ context, EncryptionDevice ^ encryptionDevice)
            : ManagedInstance(new electionguard::EncryptionMediator(
                *internalManifest->_instance, *context->_instance, *encryptionDevice->_instance))
        {
        
        }

        CiphertextBallot ^
          Encrypt(PlaintextBallot ^ ballot) { 
            return Encrypt(ballot, true);
          }
        
        CiphertextBallot^ Encrypt(PlaintextBallot^ ballot, bool shouldVerifyProofs) {
            auto ciphertext = this->_instance->encrypt(*ballot->_instance, shouldVerifyProofs);
            return gcnew CiphertextBallot(move(ciphertext));
        }
    };
} // namespace ElectionGuardInterop
