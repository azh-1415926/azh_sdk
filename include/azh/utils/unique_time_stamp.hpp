#pragma once

#include <mutex>
#include <string>

#include "timer.hpp"
#include "logger.hpp"

namespace azh::sdk::utils
{
    static std::mutex unique_time_stamp_mutex;
    class counter
    {
        long long m_last_time_stamp;
        long long m_count_private;

        counter() : m_count_private(0), m_last_time_stamp(0) {}

    public:
        static counter &getInstance()
        {
            static counter c;
            return c;
        }

        long long count(long long time_stamp)
        {
            if (time_stamp != m_last_time_stamp)
            {
                m_last_time_stamp = time_stamp;
                m_count_private = 0;
            }

            return m_count_private++;
        }
    };

    class unique_time_stamp
    {
        long long m_unix_stamp_private;
        long long m_count_private;

    public:
        unique_time_stamp()
        {
            m_unix_stamp_private = timer::sinceEpoch();
            unique_time_stamp_mutex.lock();
            m_count_private = getCount();
            unique_time_stamp_mutex.unlock();
            logger::getInstance()->debug("stamp : ", m_unix_stamp_private, ",count : ", m_count_private);
        }

        explicit unique_time_stamp(size_t i) : m_unix_stamp_private(i), m_count_private(i) {}

        unique_time_stamp(const unique_time_stamp &u) : m_unix_stamp_private(u.m_unix_stamp_private), m_count_private(u.m_count_private) { }
        ~unique_time_stamp() = default;

        long long stamp() { return m_unix_stamp_private; }
        long long count() { return m_count_private; }

        unique_time_stamp &operator=(const unique_time_stamp &u)
        {
            m_unix_stamp_private = u.m_unix_stamp_private;
            m_count_private = u.m_count_private;
            return *this;
        }

        bool operator>(const unique_time_stamp &u) const { return m_unix_stamp_private > u.m_unix_stamp_private || (m_unix_stamp_private == u.m_unix_stamp_private && m_count_private > u.m_count_private); }
        bool operator<(const unique_time_stamp &u) const { return m_unix_stamp_private < u.m_unix_stamp_private || (m_unix_stamp_private == u.m_unix_stamp_private && m_count_private < u.m_count_private); }
        bool operator==(const unique_time_stamp &u) const { return m_unix_stamp_private == u.m_unix_stamp_private && m_count_private == u.m_count_private; }
        bool operator!=(const unique_time_stamp &u) const { return m_unix_stamp_private != u.m_unix_stamp_private || m_count_private != u.m_count_private; }

        std::string toString() const
        {
            std::string str = "time_stamp(";
            str += std::to_string(m_unix_stamp_private);
            str += ",";
            str += std::to_string(m_count_private);
            str += ")";
            return str;
        }

    private:
        long long getCount()
        {
            return counter::getInstance().count(m_unix_stamp_private);
        }
    };
}