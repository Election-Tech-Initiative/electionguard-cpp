#pragma once

#include "ManagedInstance.h"
#include "Group.h"
#include "Utilities.h"

#include "election.hpp"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ElectionGuardInterop
{
    public ref class CiphertextElectionContext
        : ManagedInstance<electionguard::CiphertextElectionContext>
    {
    public:
        CiphertextElectionContext(const uint64_t numberOfGuardians, const uint64_t quorum,
                                  ElementModP ^ elGamalPublicKey, ElementModQ ^ commitmentHash,
                                  ElementModQ ^ manifestHash)
        {
            this->_instance =
              electionguard::CiphertextElectionContext::make(
                numberOfGuardians, quorum,
                std::make_unique<electionguard::ElementModP>(*elGamalPublicKey->_instance),
                std::make_unique<electionguard::ElementModQ>(*commitmentHash->_instance),
                std::make_unique<electionguard::ElementModQ>(*manifestHash->_instance))
                .release();
      
        }

        CiphertextElectionContext(const uint64_t numberOfGuardians, const uint64_t quorum,
                                  ElementModP ^ elGamalPublicKey, ElementModQ ^ commitmentHash,
                                  ElementModQ ^ manifestHash, ElementModQ ^ cryptoBaseHash,
                                  ElementModQ ^ cryptoExtendedBaseHash)
            : ManagedInstance(
                new electionguard::CiphertextElectionContext(
                numberOfGuardians, quorum, 
                std::make_unique<electionguard::ElementModP>(*elGamalPublicKey->_instance),
                std::make_unique<electionguard::ElementModQ>(*commitmentHash->_instance),
                std::make_unique<electionguard::ElementModQ>(*manifestHash->_instance),
                std::make_unique<electionguard::ElementModQ>(*cryptoBaseHash->_instance),
                std::make_unique<electionguard::ElementModQ>(*cryptoExtendedBaseHash->_instance)))
        {
        
        }

    internal: 
        CiphertextElectionContext(std::unique_ptr<electionguard::CiphertextElectionContext> other)
        {
            this->_instance = other.release();
        }

    public:
        property uint64_t ^
          NumberOfGuardians {
            uint64_t ^ get() {
                return this->_instance->getNumberOfGuardians();
            }
        }   
        property uint64_t ^
        Quorum {
            uint64_t ^ get() {
               return this->_instance->getQuorum();
            }
        } 
        property ElementModP ^
          ElGamalPublicKey {
              ElementModP ^ get() {
                  auto value = this->_instance->getElGamalPublicKey();
                  return gcnew ElementModP(value);
              }
          }
        property ElementModQ ^
          CommitmentHash {
              ElementModQ ^ get() {
                  auto value = this->_instance->getCommitmentHash();
                  return gcnew ElementModQ(value);
              }
          } 
        property ElementModQ ^
        ManifestHash {
            ElementModQ ^ get() {
                auto value = this->_instance->getManifestHash();
                return gcnew ElementModQ(value);
            }
        } 
        property ElementModQ ^
        CryptoBaseHash {
            ElementModQ ^ get() {
                auto value = this->_instance->getCryptoBaseHash();
                return gcnew ElementModQ(value);
            }
        } 
        property ElementModQ ^
        CryptoExtendedBaseHash {
            ElementModQ ^ get() {
                auto value = this->_instance->getCryptoExtendedBaseHash();
                return gcnew ElementModQ(value);
            }
        }
        property String ^
            Json {
              String ^ get() {
                  auto value = this->_instance->toJson();
                  return gcnew String(value.c_str());
              }
        }

        static CiphertextElectionContext ^
            make(uint64_t numberOfGuardians, uint64_t quorum, ElementModP^ elGamalPublicKey,
                ElementModQ^ commitmentHash, ElementModQ^ manifestHash) {
            auto value = electionguard::CiphertextElectionContext::make(
              numberOfGuardians, quorum, 
              std::make_unique<electionguard::ElementModP>(*elGamalPublicKey->_instance), 
              std::make_unique<electionguard::ElementModQ>(*commitmentHash->_instance),
              std::make_unique<electionguard::ElementModQ>(*manifestHash->_instance));
            return gcnew CiphertextElectionContext(move(value));
        }
        
        static CiphertextElectionContext ^
          FromJson(String ^ json) {
            std::string _json;
            Utilities::MarshalString(json, _json);
            auto unamanaged = electionguard::CiphertextElectionContext::fromJson(_json);
            return gcnew CiphertextElectionContext(move(unamanaged));
          }

    };
} // namespace ElectionGuardInterop
