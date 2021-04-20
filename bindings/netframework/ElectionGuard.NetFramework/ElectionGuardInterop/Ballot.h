#pragma once

#include "ElGamal.h"
#include "ChaumPedersen.h"
#include "ManagedInstance.h"
#include "Utilities.h"

#include "ballot.hpp"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ElectionGuardInterop
{
    public ref struct ExtendedData : ManagedInstance<electionguard::ExtendedData> {
    public:
        ExtendedData(String^ value, uint64_t length) : ManagedInstance() {
            std::string _value;
            Utilities::MarshalString(value, _value);

            this->_instance = new electionguard::ExtendedData(_value, length);
        }
    };

    public ref class PlaintextBallotSelection
        : ManagedInstance<electionguard::PlaintextBallotSelection>
    {
    public:
        PlaintextBallotSelection(String ^ objectId, uint64_t vote)
            : PlaintextBallotSelection(objectId, vote, false, nullptr)
        {
        }
        PlaintextBallotSelection(String ^ objectId, uint64_t vote, bool isPlaceholder)
            : PlaintextBallotSelection(
                objectId, vote, isPlaceholder, nullptr)
        {
        }
        PlaintextBallotSelection(
            String ^ objectId, uint64_t vote, bool isPlaceholder, ExtendedData ^ extendedData)
        {
            std::string _objectId;
            Utilities::MarshalString(objectId, _objectId);

            if (extendedData != nullptr) {
                this->_instance = new electionguard::PlaintextBallotSelection(
                  _objectId, vote, isPlaceholder,
                  std::make_unique<electionguard::ExtendedData>(*extendedData->_instance));
                return;
            }
            this->_instance = new electionguard::PlaintextBallotSelection(
              _objectId, vote, isPlaceholder);
        }
    internal: 
        PlaintextBallotSelection(std::unique_ptr<electionguard::PlaintextBallotSelection> other)
        {
            this->_instance = other.release();
        }

    public:
        property String ^ ObjectId {
            String ^ get() {
                auto value = this->_instance->getObjectId();
                return gcnew String(value.c_str());
            }
        }

        property uint64_t ^
          Vote {
              uint64_t ^ get() {
                  return this->_instance->getVote();
              }
          }

        property bool ^
          IsPlaceholder { bool ^ get() { return this->_instance->getIsPlaceholder(); } }

        bool IsValid(String ^ expectedObjectId)
        {
            std::string _expectedObjectId;
            Utilities::MarshalString(expectedObjectId, _expectedObjectId);
            return this->_instance->isValid(_expectedObjectId);
        }
    };

    public ref class CiphertextBallotSelection
        : ManagedInstance<electionguard::CiphertextBallotSelection>
    {
      public:
        CiphertextBallotSelection(
            String ^ objectId, ElementModQ ^ descriptionHash, ElGamalCiphertext ^ ciphertext, 
            bool isPlaceholder, ElementModQ ^ nonce, ElementModQ ^ cryptoHash, 
            DisjunctiveChaumPedersenProof ^ proof, ElGamalCiphertext ^ extendedData)
        {
            std::string _objectId;
            Utilities::MarshalString(objectId, _objectId);

            this->_instance = new electionguard::CiphertextBallotSelection(
              _objectId, *descriptionHash->_instance,
              std::make_unique<electionguard::ElGamalCiphertext>(*ciphertext->_instance),
              isPlaceholder, std::make_unique<electionguard::ElementModQ>(*nonce->_instance),
              std::make_unique<electionguard::ElementModQ>(*cryptoHash->_instance),
              std::make_unique<electionguard::DisjunctiveChaumPedersenProof>(*proof->_instance),
              std::make_unique<electionguard::ElGamalCiphertext>(*extendedData->_instance));
        }
      internal: 
          CiphertextBallotSelection(
                     std::unique_ptr<electionguard::CiphertextBallotSelection> other)
        {
            this->_instance = other.release();
        }

      public:
        property String ^ ObjectId {
            String ^ get() {
                auto value = this->_instance->getObjectId();
                return gcnew String(value.c_str());
            }
        }
    };

    public ref class PlaintextBallot : ManagedInstance<electionguard::PlaintextBallot>
    {
      public:
        PlaintextBallot(String ^ json) {
            std::string _json;
            Utilities::MarshalString(json, _json);
            this->_instance = electionguard::PlaintextBallot::fromJson(_json).release();
        }
      internal: 
        PlaintextBallot(std::unique_ptr<electionguard::PlaintextBallot> other)
        {
            this->_instance = other.release();
        }

      public:
        property String ^ ObjectId {
            String ^ get() {
                auto value = this->_instance->getObjectId();
                return gcnew String(value.c_str());
            }
        } 
        
        property String ^
          Json {
              String ^ get() {
                  auto value = this->_instance->toJson();
                  return gcnew String(value.c_str());
              }
          }
    };

    public
    ref class CiphertextBallot : ManagedInstance<electionguard::CiphertextBallot>
    {
      public:
        CiphertextBallot(String ^ json)
        {
            std::string _json;
            Utilities::MarshalString(json, _json);
            this->_instance = electionguard::CiphertextBallot::fromJson(_json).release();
        }
        internal: 
        CiphertextBallot(std::unique_ptr<electionguard::CiphertextBallot> other)
        {
            this->_instance = other.release();
        }

      public:
        property String ^ ObjectId {
            String ^ get() {
                auto value = this->_instance->getObjectId();
                return gcnew String(value.c_str());
            }
        }

        property String ^
          Json {
              String ^ get() {
                  auto value = this->_instance->toJson();
                  return gcnew String(value.c_str());
              }
          }
    };
    } // namespace ElectionGuardInterop
