#pragma once

#include <exception>
#include "string.hpp"

namespace azh::sdk::utils
{
    class exception : public std::exception
    {
        std::string m_msg_private;
        
    public:
        static void eof(const std::string &msg = "")
        {
            static std::string prefix = "Unexpected end of stream";
            if (msg.empty())
            {
                throw exception(prefix);
            }
            throw exception(prefix, ": ", msg);
        }

        static void not_yet_impl(const std::string &msg = "")
        {
            throw exception("Not yet implemented: ", msg, ".");
        }

        template <typename... Args>
        explicit exception(Args &&...args)
            : m_msg_private(stringbuilder(std::forward<Args>(args)...)) {}

        explicit exception(std::string msg) : m_msg_private(std::move(msg)) {}

        explicit exception(const char *msg, const std::exception &) : m_msg_private(msg) {}

        exception(const exception &) = default;
        exception &operator=(const exception &) = default;
        exception(exception &&) = default;
        exception &operator=(exception &&) = default;

        const char *what() const noexcept override { return m_msg_private.c_str(); }
    };
}