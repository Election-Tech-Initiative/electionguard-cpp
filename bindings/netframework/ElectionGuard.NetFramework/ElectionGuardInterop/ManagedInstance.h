#pragma once

using namespace System;

namespace ElectionGuardInterop
{
    template <class T>
    public ref class ManagedInstance
    {
    public:
        ManagedInstance() {}
        ManagedInstance(T *instance, bool owned) : _instance(instance), _owned(owned) {}
        ManagedInstance(const T *instance) : _instance(const_cast<T*>(instance)), _owned(false) {}
        ManagedInstance(T *instance) : _instance(instance) {}

    public:
        virtual ~ManagedInstance()
        {
            if (_instance != nullptr && _owned) {
                delete _instance;
            }
        }
        !ManagedInstance()
        {
            if (_instance != nullptr && _owned) {
                delete _instance;
            }
        }

    internal:
        T *_instance;
        bool _owned = true;
    };
} // namespace ElectionGuardInterop
