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
  public
    ref class EncryptionDevice : ManagedInstance<electionguard::EncryptionDevice>
    {
      public:
        EncryptionDevice(uint64_t deviceUuid, uint64_t sessionUuid, uint64_t launchCode,
                         String ^ location)
        {
            std::string _location;
            Utilities::MarshalString(location, _location);
            this->_instance =
              new electionguard::EncryptionDevice(deviceUuid, sessionUuid, launchCode, _location);
        }

      public:
        property ElementModQ ^
          Hash {
              ElementModQ ^ get() {
                  auto unmanaged = this->_instance->getHash();
                  return gcnew ElementModQ(std::move(unmanaged));
              }
          }

          property uint64_t ^
          Timestamp { uint64_t ^ get() { return this->_instance->getTimestamp(); } }
    };
  public
    ref class EncryptionMediator : ManagedInstance<electionguard::EncryptionMediator>
    {
      public:
        EncryptionMediator(InternalManifest ^ internalManifest, CiphertextElectionContext ^ context,
                           EncryptionDevice ^ encryptionDevice)
            : ManagedInstance(new electionguard::EncryptionMediator(
                *internalManifest->_instance, *context->_instance, *encryptionDevice->_instance))
        {
        }

        CiphertextBallot ^
          Encrypt(PlaintextBallot ^ ballot) { return Encrypt(ballot, true); }

          CiphertextBallot
          ^ Encrypt(PlaintextBallot ^ ballot, bool shouldVerifyProofs) {
                auto ciphertext = this->_instance->encrypt(*ballot->_instance, shouldVerifyProofs);
                return gcnew CiphertextBallot(move(ciphertext));
            }
    };

  public
    ref struct Encrypt {
        CiphertextBallotSelection ^
          Selection(PlaintextBallotSelection ^ selection, SelectionDescription ^ description,
                    ElementModP ^ elGamalPublicKey, ElementModQ ^ cryptoExtendedBaseHash,
                    ElementModQ ^ nonceSeed, bool isPlaceholder, bool shouldVerifyProofs) {
              auto ciphertext = electionguard::encryptSelection(
                *selection->_instance, *description->_instance, *elGamalPublicKey->_instance,
                *cryptoExtendedBaseHash->_instance, *nonceSeed->_instance, isPlaceholder,
                shouldVerifyProofs);
              return gcnew CiphertextBallotSelection(std::move(ciphertext));
          }

          CiphertextBallotContest
          ^
          Contest(PlaintextBallotContest ^ contest,
                  ContestDescriptionWithPlaceholders ^ description, ElementModP ^ elGamalPublicKey,
                  ElementModQ ^ cryptoExtendedBaseHash, ElementModQ ^ nonceSeed,
                  bool shouldVerifyProofs) {
              auto ciphertext = electionguard::encryptContest(
                *contest->_instance, *description->_instance, *elGamalPublicKey->_instance,
                *cryptoExtendedBaseHash->_instance, *nonceSeed->_instance, shouldVerifyProofs);
              return gcnew CiphertextBallotContest(std::move(ciphertext));
          }

          CiphertextBallot
          ^
          Ballot(PlaintextBallot ^ ballot, InternalManifest ^ internalManifest,
                 CiphertextElectionContext ^ context, ElementModQ ^ ballotCodeSeed) {
              auto ciphertext = electionguard::encryptBallot(
                *ballot->_instance, *internalManifest->_instance, *context->_instance,
                *ballotCodeSeed->_instance);

              return gcnew CiphertextBallot(std::move(ciphertext));
          }

          CiphertextBallot
          ^
          Ballot(PlaintextBallot ^ ballot, InternalManifest ^ internalManifest,
                 CiphertextElectionContext ^ context, ElementModQ ^ ballotCodeSeed,
                 ElementModQ ^ nonce, uint64_t timestamp, bool shouldVerifyProofs) {
              auto ciphertext = electionguard::encryptBallot(
                *ballot->_instance, *internalManifest->_instance, *context->_instance,
                *ballotCodeSeed->_instance,
                std::make_unique<electionguard::ElementModQ>(*nonce->_instance), timestamp,
                shouldVerifyProofs);

              return gcnew CiphertextBallot(std::move(ciphertext));
          }
    };
} // namespace ElectionGuardInterop
