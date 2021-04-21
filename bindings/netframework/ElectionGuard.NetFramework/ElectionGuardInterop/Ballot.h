#pragma once

#include "ChaumPedersen.h"
#include "ElGamal.h"
#include "ManagedInstance.h"
#include "Utilities.h"
#include "ballot.hpp"

#include <iterator>

using namespace System;
using namespace System::Collections;
using namespace System::Runtime::InteropServices;

namespace ElectionGuardInterop
{
  public
    enum class BallotBoxState { cast = 1, spoiled = 2, unknown = 999 };

  public
    ref struct ExtendedData : ManagedInstance<electionguard::ExtendedData> {
      public:
        ExtendedData(String ^ value, uint64_t length) : ManagedInstance()
        {
            std::string _value;
            Utilities::MarshalString(value, _value);

            this->_instance = new electionguard::ExtendedData(_value, length);
        }
    };

  public
    ref class PlaintextBallotSelection : ManagedInstance<electionguard::PlaintextBallotSelection>
    {
      public:
        PlaintextBallotSelection(String ^ objectId, uint64_t vote)
            : PlaintextBallotSelection(objectId, vote, false, nullptr)
        {
        }
        PlaintextBallotSelection(String ^ objectId, uint64_t vote, bool isPlaceholder)
            : PlaintextBallotSelection(objectId, vote, isPlaceholder, nullptr)
        {
        }
        PlaintextBallotSelection(String ^ objectId, uint64_t vote, bool isPlaceholder,
                                 ExtendedData ^ extendedData)
        {
            std::string _objectId;
            Utilities::MarshalString(objectId, _objectId);

            if (extendedData != nullptr) {
                this->_instance = new electionguard::PlaintextBallotSelection(
                  _objectId, vote, isPlaceholder,
                  std::make_unique<electionguard::ExtendedData>(*extendedData->_instance));
                return;
            }
            this->_instance =
              new electionguard::PlaintextBallotSelection(_objectId, vote, isPlaceholder);
        }
        internal : PlaintextBallotSelection(
                     std::unique_ptr<electionguard::PlaintextBallotSelection> other)
        {
            this->_instance = other.release();
        }

        PlaintextBallotSelection(electionguard::PlaintextBallotSelection *unowned)
            : ManagedInstance(unowned, false)
        {
        }

      public:
        property String ^
          ObjectId {
              String ^ get() {
                  auto value = this->_instance->getObjectId();
                  return gcnew String(value.c_str());
              }
          }

          property uint64_t ^
          Vote { uint64_t ^ get() { return this->_instance->getVote(); } }

          property bool ^
          IsPlaceholder { bool ^ get() { return this->_instance->getIsPlaceholder(); } }

          bool IsValid(String ^ expectedObjectId)
        {
            std::string _expectedObjectId;
            Utilities::MarshalString(expectedObjectId, _expectedObjectId);
            return this->_instance->isValid(_expectedObjectId);
        }
    };

  public
    ref class CiphertextBallotSelection : ManagedInstance<electionguard::CiphertextBallotSelection>
    {
      public:
        CiphertextBallotSelection(String ^ objectId, ElementModQ ^ descriptionHash,
                                  ElGamalCiphertext ^ ciphertext, bool isPlaceholder,
                                  ElementModQ ^ nonce, ElementModQ ^ cryptoHash,
                                  DisjunctiveChaumPedersenProof ^ proof,
                                  ElGamalCiphertext ^ extendedData)
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
        internal : CiphertextBallotSelection(
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

  public
    ref class PlaintextBallotContest : ManagedInstance<electionguard::PlaintextBallotContest>
    {
      public:
        PlaintextBallotContest(String ^ objectId, array<PlaintextBallotSelection ^> ^ selections)
        {
            std::string _objectId;
            Utilities::MarshalString(objectId, _objectId);

            std::vector<std::unique_ptr<electionguard::PlaintextBallotSelection>> elements;

            for each (auto item in selections) {
                elements.push_back(item->_instance->clone());
            }

            this->_instance =
              new electionguard::PlaintextBallotContest(_objectId, std::move(elements));
        }
        internal
            : PlaintextBallotContest(std::unique_ptr<electionguard::PlaintextBallotContest> other)
        {
            this->_instance = other.release();
        }

        PlaintextBallotContest(electionguard::PlaintextBallotContest *unowned)
            : ManagedInstance(unowned, false)
        {
        }

      public:
        property String ^
          ObjectId {
              String ^ get() {
                  auto value = this->_instance->getObjectId();
                  return gcnew String(value.c_str());
              }
          }

          property array<PlaintextBallotSelection ^> ^
          Selections {
              array<PlaintextBallotSelection ^> ^ get() {
                  auto value = this->_instance->getSelections();
                  auto elements = gcnew array<PlaintextBallotSelection ^>(value.size());

                  int index = 0;
                  for (const auto &item : value) {
                      elements[index] = gcnew PlaintextBallotSelection(&item.get());
                      index++;
                  }

                  return elements;
              }
          }
    };

  public
    ref class PlaintextBallot : ManagedInstance<electionguard::PlaintextBallot>
    {
      public:
        PlaintextBallot(String ^ json)
        {
            std::string _json;
            Utilities::MarshalString(json, _json);
            this->_instance = electionguard::PlaintextBallot::fromJson(_json).release();
        }
        internal : PlaintextBallot(std::unique_ptr<electionguard::PlaintextBallot> other)
        {
            this->_instance = other.release();
        }

      public:
        property String ^
          ObjectId {
              String ^ get() {
                  auto value = this->_instance->getObjectId();
                  return gcnew String(value.c_str());
              }
          }

          property array<PlaintextBallotContest ^> ^
          Contests {
              array<PlaintextBallotContest ^> ^ get() {
                  auto value = this->_instance->getContests();
                  auto elements = gcnew array<PlaintextBallotContest ^>(value.size());

                  int index = 0;
                  for (const auto &item : value) {
                      elements[index] = gcnew PlaintextBallotContest(&item.get());
                      index++;
                  }

                  return elements;
              }
          }

          property array<Byte> ^
          Bson {
              array<Byte> ^ get() {
                  auto value = this->_instance->toBson();

                  array<Byte> ^ byteArray = gcnew array<Byte>(value.size());
                  Marshal::Copy((IntPtr)value.data(), byteArray, 0, value.size());
                  return byteArray;
              }
          }

          property String ^
          Json {
              String ^ get() {
                  auto value = this->_instance->toJson();
                  return gcnew String(value.c_str());
              }
          }

          static PlaintextBallot ^
          FromBson(array<Byte> ^ data) {
              auto data_ = Utilities::MarshalByteArray(data);
              auto value = electionguard::PlaintextBallot::fromBson(items);
              return gcnew PlaintextBallot(move(value));
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
        internal : CiphertextBallot(std::unique_ptr<electionguard::CiphertextBallot> other)
        {
            this->_instance = other.release();
        }

      public:
        property String ^
          ObjectId {
              String ^ get() {
                  auto value = this->_instance->getObjectId();
                  return gcnew String(value.c_str());
              }
          }

          property String ^
          StyleId {
              String ^ get() {
                  auto value = this->_instance->getStyleId();
                  return gcnew String(value.c_str());
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
          BallotCodeSeed {
              ElementModQ ^ get() {
                  auto value = this->_instance->getBallotCodeSeed();
                  return gcnew ElementModQ(value);
              }
          }

          property ElementModQ ^
          BallotCode {
              ElementModQ ^ get() {
                  auto value = this->_instance->getBallotCode();
                  return gcnew ElementModQ(value);
              }
          }

          property ElementModQ ^
          Nonce {
              ElementModQ ^ get() {
                  auto value = this->_instance->getNonce();
                  return gcnew ElementModQ(value);
              }
          }

          property ElementModQ ^
          CryptoHash {
              ElementModQ ^ get() {
                  auto value = this->_instance->getCryptoHash();
                  return gcnew ElementModQ(value);
              }
          }

          property array<Byte> ^
          Bson {
              array<Byte> ^ get() {
                  auto value = this->_instance->toBson();

                  array<Byte> ^ byteArray = gcnew array<Byte>(value.size());
                  Marshal::Copy((IntPtr)value.data(), byteArray, 0, value.size());
                  return byteArray;
              }
          }

          property String ^
          Json {
              String ^ get() {
                  auto value = this->_instance->toJson();
                  return gcnew String(value.c_str());
              }
          }

          bool IsValidencryption(ElementModQ ^ manifestHash, ElementModP ^ elGamalPublicKey,
                                 ElementModQ ^ cryptoExtendedBaseHash)
        {
            return this->_instance->isValidEncryption(*manifestHash->_instance,
                                                      *elGamalPublicKey->_instance,
                                                      *cryptoExtendedBaseHash->_instance);
        }

        static CiphertextBallot ^ FromBson(array<Byte> ^ data) {
            auto data_ = Utilities::MarshalByteArray(data);
            auto value = electionguard::CiphertextBallot::fromBson(data_);
            return gcnew CiphertextBallot(move(value));
        }
    };

  public
    ref class SubmittedBallot : ManagedInstance<electionguard::SubmittedBallot>
    {
      public:
        SubmittedBallot(String ^ json)
        {
            std::string _json;
            Utilities::MarshalString(json, _json);
            this->_instance = electionguard::SubmittedBallot::fromJson(_json).release();
        }
        internal : SubmittedBallot(std::unique_ptr<electionguard::SubmittedBallot> other)
        {
            this->_instance = other.release();
        }

      public:
        static SubmittedBallot ^
          From(CiphertextBallot ^ ballot, BallotBoxState state) {
              auto value = electionguard::SubmittedBallot::from(
                *ballot->_instance, static_cast<electionguard::BallotBoxState>(state));

              return gcnew SubmittedBallot(move(value));
          }

          property String
          ^
          ObjectId {
              String ^ get() {
                  auto value = this->_instance->getObjectId();
                  return gcnew String(value.c_str());
              }
          }

          property BallotBoxState State
        {
            BallotBoxState get()
            {
                auto value = this->_instance->getState();
                switch (value) {
                    case electionguard::BallotBoxState::cast:
                        return BallotBoxState::cast;
                    case electionguard::BallotBoxState::spoiled:
                        return BallotBoxState::spoiled;
                    case electionguard::BallotBoxState::unknown:
                    default:
                        return BallotBoxState::unknown;
                }
            }
        }

        property String ^ Json {
            String ^ get() {
                auto value = this->_instance->toJson();
                return gcnew String(value.c_str());
            }
        }
    };
} // namespace ElectionGuardInterop
