#pragma once

#include <sstream>
#include <string>
#include <type_traits>
#include <cassert>

namespace azh::sdk::type
{
    template <typename _type>
    struct _has_to_string
    {
    private:
        template <typename U>
        static auto test(int) -> decltype(std::declval<U>().toString(), std::true_type{});

        template <typename>
        static std::false_type test(...);

    public:
        static constexpr bool value = decltype(test<_type>(0))::value;
    };

    template <typename T, typename U>
    struct decay_equiv : std::is_same<typename std::decay<T>::type, U>::type
    {
    };

    template <typename _type>
    inline typename std::enable_if<!_has_to_string<_type>::value, std::string>::type
    to_string(const _type &t)
    {
        std::ostringstream oss;
        if(decay_equiv<_type, std::string>::value)
        {
            oss << "\""<<t<<"\"";
        }
        else if(decay_equiv<_type, char>::value)
        {
            oss << "\'"<<t<<"\'";
        }
        else
        {
            oss << t;
        }
        
        return oss.str();
    }

    template <typename _type>
    inline typename std::enable_if<_has_to_string<_type>::value, std::string>::type
    to_string(const _type &t)
    {
        std::ostringstream oss;
        // oss << typeid(t).name();
        oss << t.toString();
        return oss.str();
    }
}