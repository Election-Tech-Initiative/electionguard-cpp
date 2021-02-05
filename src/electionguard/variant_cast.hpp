#ifndef __ELECTIONGUARD__CPP_VARIANT_CAST_HPP_INCLUDED__
#define __ELECTIONGUARD__CPP_VARIANT_CAST_HPP_INCLUDED__

#include <electionguard/export.h>
#include <variant>

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)

namespace electionguard
{
    template <class... Args> struct variant_cast_proxy {
        std::variant<Args...> v;

        template <class... ToArgs> operator std::variant<ToArgs...>() const
        {
            return std::visit(
              [](auto &&arg) -> std::variant<ToArgs...> { return arg; }, v);
        }
    };

    template <class... Args>
    auto variant_cast(const std::variant<Args...> &v) -> variant_cast_proxy<Args...>
    {
        return {v};
    }
} // namespace electionguard

#endif /* __ELECTIONGUARD__CPP_VARIANT_CAST_HPP_INCLUDED__ */
