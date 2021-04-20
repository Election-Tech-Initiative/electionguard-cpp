#pragma once

#include "ElGamal.h"
#include "Group.h"
#include "ManagedInstance.h"
#include "Utilities.h"

#include "chaum_pedersen.hpp"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ElectionGuardInterop
{
    public ref class DisjunctiveChaumPedersenProof
        : ManagedInstance<electionguard::DisjunctiveChaumPedersenProof>
    {
    public:
          DisjunctiveChaumPedersenProof(
              ElementModP ^ proof_zero_pad,
              ElementModP ^ proof_zero_data, 
              ElementModP ^ proof_one_pad,
              ElementModP ^ proof_one_data,
              ElementModQ ^ proof_zero_challenge,
                                    ElementModQ ^ proof_one_challenge, ElementModQ ^ challenge,
                                    ElementModQ ^ proof_zero_response,
                                    ElementModQ ^ proof_one_response)
              : ManagedInstance(new electionguard::DisjunctiveChaumPedersenProof(
                std::make_unique<electionguard::ElementModP>(*proof_zero_pad->_instance),
                std::make_unique<electionguard::ElementModP>(*proof_zero_data->_instance),
                std::make_unique<electionguard::ElementModP>(*proof_one_pad->_instance),
                std::make_unique<electionguard::ElementModP>(*proof_one_data->_instance),
              std::make_unique<electionguard::ElementModQ>(*proof_zero_challenge->_instance),
              std::make_unique<electionguard::ElementModQ>(*proof_one_challenge->_instance),
              std::make_unique<electionguard::ElementModQ>(*challenge->_instance),
              std::make_unique<electionguard::ElementModQ>(*proof_zero_response->_instance),
              std::make_unique<electionguard::ElementModQ>(*proof_one_response->_instance)))
          {

          }
    internal: 
          DisjunctiveChaumPedersenProof(
                     std::unique_ptr<electionguard::DisjunctiveChaumPedersenProof> other)
          {
            this->_instance = other.release();
          }

    public:
          property ElementModP ^ ProofZeroPad {
              ElementModP ^ get() {
                  auto value = this->_instance->getProofZeroPad();
                  return gcnew ElementModP(value);
              }
          } 
          property ElementModP ^
            ProofZeroData {
                ElementModP ^ get() {
                    auto value = this->_instance->getProofZeroData();
                    return gcnew ElementModP(value);
                }
            }
          property ElementModP ^
            ProofOnePad {
                ElementModP ^ get() {
                    auto value = this->_instance->getProofOnePad();
                    return gcnew ElementModP(value);
                }
            } 
          property ElementModP ^
            ProofOneData {
                ElementModP ^ get() {
                    auto value = this->_instance->getProofOneData();
                    return gcnew ElementModP(value);
                }
            } 
          property ElementModQ ^
            ProofZeroChallenge {
            ElementModQ ^ get() {
                    auto value = this->_instance->getProofZeroChallenge();
                return gcnew ElementModQ(value);
                }
            } 
          property ElementModQ ^
            ProofOneChallenge {
            ElementModQ ^ get() {
                    auto value = this->_instance->getProofOneChallenge();
                return gcnew ElementModQ(value);
                }
            } 
          property ElementModQ ^
            Challenge {
            ElementModQ ^ get() {
                    auto value = this->_instance->getChallenge();
                return gcnew ElementModQ(value);
                }
            } 
          property ElementModQ ^
            ProofZeroResponse {
            ElementModQ ^ get() {
                    auto value = this->_instance->getProofZeroResponse();
                return gcnew ElementModQ(value);
                }
            } 
          property ElementModQ ^
            ProofOneResponse {
            ElementModQ ^ get() {
                    auto value = this->_instance->getProofOneResponse();
                return gcnew ElementModQ(value);
                }
            }

          bool IsValid(ElGamalCiphertext ^ message, ElementModP ^ k, ElementModQ ^ q)
          {
              return this->_instance->isValid(*message->_instance, *k->_instance, *q->_instance);
          }
    };

     public ref class ConstantChaumPedersenProof
        : ManagedInstance<electionguard::ConstantChaumPedersenProof>
    {
      public:
        ConstantChaumPedersenProof(ElementModP ^ pad, ElementModP ^ data, ElementModQ ^ challenge,
                                      ElementModQ ^ response,
                                      uint64_t constant)
            : ManagedInstance(new electionguard::ConstantChaumPedersenProof(
                std::make_unique<electionguard::ElementModP>(*pad->_instance),
                std::make_unique<electionguard::ElementModP>(*data->_instance),
                std::make_unique<electionguard::ElementModQ>(*challenge->_instance),
                std::make_unique<electionguard::ElementModQ>(*response->_instance), constant))
        {
        }
        internal: 
            ConstantChaumPedersenProof(
                     std::unique_ptr<electionguard::ConstantChaumPedersenProof> other)
        {
            this->_instance = other.release();
        }

      public:
        property ElementModP ^
          Pad {
              ElementModP ^ get() {
                  auto value = this->_instance->getPad();
                  return gcnew ElementModP(value);
              }
          } 
        
        property ElementModP ^
          Data {
              ElementModP ^ get() {
                  auto value = this->_instance->getData();
                  return gcnew ElementModP(value);
              }
          } 
         
          property ElementModQ ^
          Challenge {
              ElementModQ ^ get() {
                  auto value = this->_instance->getChallenge();
                  return gcnew ElementModQ(value);
              }
          } 
          
          property ElementModQ ^
          Response {
              ElementModQ ^ get() {
                  auto value = this->_instance->getResponse();
                  return gcnew ElementModQ(value);
              }
          }

          bool IsValid(ElGamalCiphertext ^ message, ElementModP ^ k, ElementModQ ^ q)
        {
            return this->_instance->isValid(*message->_instance, *k->_instance, *q->_instance);
        }
    };


} // namespace ElectionGuardInterop

