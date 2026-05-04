#pragma once

#include <string>
#include <sstream>

namespace azh::sdk::type
{
    template <typename _type>
    struct _has_to_string;

    template <typename _type>
    typename std::enable_if<!_has_to_string<_type>::value, std::string>::type
    to_string(const _type &t);

    template <typename _type>
    inline typename std::enable_if<_has_to_string<_type>::value, std::string>::type
    to_string(const _type &t);
}

namespace azh::sdk::utils
{
    class string
    {
    public:
        static int firstOf(const std::string &s, const std::string &match)
        {
            if (match.size() > s.size())
                return -1;

            if (match.size() == s.size() && match == s)
            {
                return 0;
            }

            int index = 0;

            for (; index < s.size() - match.size(); index++)
            {
                if (s.substr(index, match.size()) == match)
                    break;
            }

            if (index == s.size() - match.size() - 1 && s.substr(index, match.size()) != match)
            {
                return -1;
            }

            return index;
        }

        static int lastOf(const std::string &s, const std::string &match)
        {
            if (match.size() > s.size())
                return -1;

            if (match.size() == s.size() && match == s)
            {
                return 0;
            }

            int index = s.size() - match.size();

            for (; index >= 0; index--)
            {
                std::string substr = s.substr(index, match.size());
                if (substr == match)
                    break;
            }

            return index;
        }

        static int indexOf(const std::string &s, const std::string &match)
        {
            return firstOf(s, match);
        }

        static std::string subtract(const std::string &s1, const std::string &s2)
        {
            int index = lastOf(s1, s2);
            if (index == -1)
            {
                return s1;
            }

            std::string result(s1.begin(), s1.begin() + index);
            result.append(s1.begin() + index + s2.size(), s1.end());

            return result;
        }

    private:
    };

    template <typename Head>
    void stringbuilder_r(std::ostream &stream, Head &&head)
    {
        stream << head;
    }

    template <typename Head, typename... Tail>
    void stringbuilder_r(std::ostream &stream, Head &&head, Tail &&...tail)
    {
        stringbuilder_r(stream, std::forward<Head>(head));
        stringbuilder_r(stream, std::forward<Tail>(tail)...);
    }

    template <typename... Args>
    std::string stringbuilder(Args &&...args)
    {
        // std::stringstream ss;
        std::ostringstream ss;
        stringbuilder_r(ss, std::forward<Args>(args)...);
        return ss.str();
    }

    template <typename T>
    class string_ostreamable
    {
    public:
        ~string_ostreamable()
        {
            static_assert(
                std::is_same<decltype(std::declval<const T>().toString()), std::string>::value,
                "string_ostreamable depends on the method T::toString() const");
        }

    private:
        const T &cast() const { return static_cast<const T &>(*this); }

        friend inline std::ostream &operator<<(std::ostream &os, const string_ostreamable &t)
        {
            return os << t.cast().toString();
        }
    };
}