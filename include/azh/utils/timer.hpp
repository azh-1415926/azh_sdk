#pragma once

#include <chrono>

namespace azh::sdk::utils
{
    class timer
    {
    private:
        using milliseconds_ratio = std::ratio<1, 1000>;
        using duration_millis = std::chrono::duration<double, milliseconds_ratio>;

        std::chrono::system_clock::time_point m_start_timepoint_private;
        // std::chrono::_V2::system_clock::time_point m_start_timepoint_private;

    public:
        timer()
        {
            m_start_timepoint_private = std::chrono::system_clock::now();
        }

        void refresh()
        {
            m_start_timepoint_private = std::chrono::system_clock::now();
        }

        double interval()
        {
            duration_millis duration = std::chrono::system_clock::now() - m_start_timepoint_private;
            return duration.count();
        }

        double intervalThenRefresh()
        {
            duration_millis duration = std::chrono::system_clock::now() - m_start_timepoint_private;
            m_start_timepoint_private = std::chrono::system_clock::now();
            return duration.count();
        }

        timer(const timer &t) : m_start_timepoint_private(t.m_start_timepoint_private) {}
        timer operator=(const timer &t)
        {
            timer timer;
            timer.m_start_timepoint_private = t.m_start_timepoint_private;
            return timer;
        }
        double operator-(const timer &t)
        {
            duration_millis duration = m_start_timepoint_private - t.m_start_timepoint_private;
            return duration.count();
        }
    };
}