#ifndef __ELECTIONGUARD__CORE_VARIANT_CAST_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_VARIANT_CAST_HPP_INCLUDED__
#include <electionguard/export.h>
#include <variant>
using namespace std;

namespace electionguard
{
    template <class... Args> struct variant_cast_proxy {
        variant<Args...> v;

        template <class... ToArgs> operator variant<ToArgs...>() const
        {
            return std::visit(
              [](auto &&arg) -> variant<ToArgs...> { return arg; }, v);
        }
    };

    template <class... Args>
    auto variant_cast(const variant<Args...> &v) -> variant_cast_proxy<Args...>
    {
        return {v};
    }
} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_VARIANT_CAST_HPP_INCLUDED__ */
