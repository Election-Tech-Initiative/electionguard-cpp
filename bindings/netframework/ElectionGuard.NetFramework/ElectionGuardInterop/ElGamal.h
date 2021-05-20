#pragma once

#include "Group.h"
#include "ManagedInstance.h"
#include "elgamal.hpp"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace ElectionGuardInterop
{
  public
    ref class ElGamalKeyPair : ManagedInstance<electionguard::ElGamalKeyPair>
    {
      public:
        ElGamalKeyPair(ElementModQ ^ secretKey, ElementModP ^ publicKey)
            : ManagedInstance(new electionguard::ElGamalKeyPair(
                std::make_unique<electionguard::ElementModQ>(*secretKey->_instance),
                std::make_unique<electionguard::ElementModP>(*publicKey->_instance)))
        {
        }

        internal : ElGamalKeyPair(std::unique_ptr<electionguard::ElGamalKeyPair> other)
        {
            this->_instance = other.release();
        }
        ElGamalKeyPair(electionguard::ElGamalKeyPair *unowned) : ManagedInstance(unowned, false) {}
        ElGamalKeyPair(const electionguard::ElGamalKeyPair *unowned) : ManagedInstance(unowned) {}

      public:
        property ElementModQ ^
          SecretKey { ElementModQ ^ get() { return gcnew ElementModQ(_instance->getSecretKey()); } }

          property ElementModP ^
          PublicKey { ElementModP ^ get() { return gcnew ElementModP(_instance->getPublicKey()); } }

          static ElGamalKeyPair ^
          FromSecret(ElementModQ ^ secretKey) {
              auto unmanaged = electionguard::ElGamalKeyPair::fromSecret(*secretKey->_instance);
              return gcnew ElGamalKeyPair(move(unmanaged));
          }
    };

  public
    ref class ElGamalCiphertext : ManagedInstance<electionguard::ElGamalCiphertext>
    {
      public:
        ElGamalCiphertext(ElementModP ^ pad, ElementModP ^ data)
            : ManagedInstance(new electionguard::ElGamalCiphertext(
                std::make_unique<electionguard::ElementModP>(*pad->_instance),
                std::make_unique<electionguard::ElementModP>(*data->_instance)))
        {
        }
        internal : ElGamalCiphertext(std::unique_ptr<electionguard::ElGamalCiphertext> other)
        {
            this->_instance = other.release();
        }
        ElGamalCiphertext(electionguard::ElGamalCiphertext *unowned)
            : ManagedInstance(unowned, false)
        {
        }
        ElGamalCiphertext(const electionguard::ElGamalCiphertext *unowned)
            : ManagedInstance(unowned)
        {
        }

      public:
        property ElementModP ^
          Pad { ElementModP ^ get() { return gcnew ElementModP(_instance->getPad()); } }

          property ElementModP ^
          Data { ElementModP ^ get() { return gcnew ElementModP(_instance->getData()); } }

          uint64_t Decrypt(ElementModQ ^ secretKey)
        {
            return _instance->decrypt(*secretKey->_instance);
        }
    };

  public
    ref class ElGamal
    {
      public:
        static ElGamalCiphertext ^
          Encrypt(const uint64_t m, const ElementModQ ^ nonce, const ElementModP ^ publicKey) {
              auto unmanaged =
                electionguard::elgamalEncrypt(m, *nonce->_instance, *publicKey->_instance);
              return gcnew ElGamalCiphertext(move(unmanaged));
          }
    };
} // namespace ElectionGuardInterop
