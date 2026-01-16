#pragma once

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <mutex>

#include "string.hpp"

namespace azh::sdk::utils
{
#define REGISTER_LOGGER_TYPE(L_TYPE, MSG)        \
    template <typename T, typename... Args>      \
    void L_TYPE(const T &t, const Args &...args) \
    {                                            \
        m_logger_mutex_private.lock();           \
        m_out_private << "[ ";                   \
        printCurrentTime();                      \
        m_out_private << " ]";                   \
        m_out_private << MSG;                    \
        print(t, args...);                       \
        m_out_private << std::flush;             \
        m_logger_mutex_private.unlock();         \
    }                                            \
    void L_TYPE()                                \
    {                                            \
        m_out_private << "[ ";                   \
        printCurrentTime();                      \
        m_out_private << " ]";                   \
        m_out_private << MSG;                    \
    }

    inline bool str_has_same_prefix(const std::string &s1, const std::string &s2, char delimiter)
    {
        int i1 = s1.find_first_of(delimiter);
        int i2 = s2.find_first_of(delimiter);

        if (i1 == std::string::npos || i2 == std::string::npos || i1 != i2)
        {
            return false;
        }

        std::string p1(s1.begin(), s1.begin() + i1);
        std::string p2(s2.begin(), s2.begin() + i2);

        if (p1 != p2)
        {
            return false;
        }

        return true;
    }

    class _logger
    {
        /* log file stream */
        std::ofstream m_logfile_stream_private;
        /* use to print log */
        std::ostream &m_out_private;
        /* mutex of current logger */
        std::mutex m_logger_mutex_private;
        /* current log file path */
        std::string m_log_filepath_private;
        /* use stdout */
        bool m_used_stdout_private;

    public:
        _logger() : m_out_private(std::cout), m_used_stdout_private(true) {}
        _logger(const std::string &out_file) : m_out_private(m_logfile_stream_private), m_used_stdout_private(false)
        {
            m_logfile_stream_private.open(out_file, std::ios::out);
        }

        _logger(const _logger &l) : m_out_private(l.m_used_stdout_private ? std::cout : m_logfile_stream_private), m_log_filepath_private(l.m_log_filepath_private), m_used_stdout_private(l.m_used_stdout_private)
        {
            if (l.m_logfile_stream_private.is_open())
            {
                m_logfile_stream_private.open(m_log_filepath_private, std::ios::out);
            }
        }

        ~_logger()
        {
            if (m_logfile_stream_private.is_open())
            {
                m_logfile_stream_private.close();
            }
        }

        _logger &operator=(const _logger &l) = delete;

    private:
        void printCurrentTime()
        {
            time_t now;
            time(&now);
            tm p = *localtime(&now);

            m_out_private << p.tm_year + 1900 << "-";
            m_out_private << (p.tm_mon + 1) / 10 << (p.tm_mon + 1) % 10 << "-";
            m_out_private << p.tm_mday / 10 << p.tm_mday % 10 << " ";
            m_out_private << p.tm_hour / 10 << p.tm_hour % 10 << ":";
            m_out_private << p.tm_min / 10 << p.tm_min % 10 << ":";
            m_out_private << p.tm_sec / 10 << p.tm_sec % 10;
        }

    public:
        template <typename T, typename... Args>
        void print(const T &t)
        {
            m_out_private << t << "\n";
        }

        template <typename T, typename... Args>
        void print(const T &t, const Args &...args)
        {
            m_out_private << t;
            print(args...);
        }

        template <typename T>
        _logger &operator<<(const T &t)
        {
            m_logger_mutex_private.lock();

            m_out_private << t;
            m_out_private << std::flush;

            m_logger_mutex_private.unlock();

            return *this;
        }

        void open(const std::string &filepath)
        {
            if (m_used_stdout_private)
            {
                this->error("Current logger print to stdout, can not open file : ", filepath);
                return;
            }

            if (str_has_same_prefix(filepath, m_log_filepath_private, '-'))
            {
                return;
            }

            m_log_filepath_private = filepath;

            if (m_logfile_stream_private.is_open())
                m_logfile_stream_private.close();

            m_logfile_stream_private.open(m_log_filepath_private);
        }

        REGISTER_LOGGER_TYPE(debug, "[ DEBUG   ] ")
        REGISTER_LOGGER_TYPE(info, "[ INFO    ] ")
        REGISTER_LOGGER_TYPE(warning, "[ WARNING ] ")
        REGISTER_LOGGER_TYPE(error, "[ ERROR   ] ")
        REGISTER_LOGGER_TYPE(fatal, "[ FATAL   ] ")
    };

    class logger
    {
    private:
        logger() {}

    public:
        static _logger *getInstance()
        {
            static logger l;

            return l.getLoggerSingleInstance();
        }

        static _logger *getInstance(const std::string &log_name)
        {
            static logger l;
            if (log_name.empty())
            {
                return nullptr;
            }

            return l.getLoggerSingleInstance(log_name);
        }

    private:
        std::string getCurrentLogFileName(const std::string &out_prefix)
        {
            time_t now;
            time(&now);
            tm p = *localtime(&now);

            std::string str = out_prefix;
            str += "-";
            str += std::to_string(p.tm_year + 1900);
            str += std::to_string((p.tm_mon + 1) / 10) + std::to_string((p.tm_mon + 1) % 10);
            str += std::to_string(p.tm_mday / 10) + std::to_string(p.tm_mday % 10) + "_";
            str += std::to_string(p.tm_hour / 10) + std::to_string(p.tm_hour % 10);
            str += std::to_string(p.tm_min / 10) + std::to_string(p.tm_min % 10);
            str += std::to_string(p.tm_sec / 10) + std::to_string(p.tm_sec % 10);
            str += ".log";

            return str;
        }

        _logger *getLoggerSingleInstance()
        {
            static _logger logger;
            return &logger;
        }

        _logger *getLoggerSingleInstance(const std::string &log_name)
        {
            std::string log_filepath = getCurrentLogFileName(log_name);
            static _logger logger(log_filepath);
            logger.open(log_filepath);
            return &logger;
        }
    };

#define REGISTER_LOGGER_WRAPPER_PRINT_TYPE(type) \
    logger_wrapper &operator<<(const type &t)    \
    {                                            \
        *m_logger_private << t;                  \
        return *this;                            \
    }

    class logger_wrapper
    {
        _logger *m_logger_private;

    public:
        explicit logger_wrapper(const std::string &log_name = "", int type = 0) : m_logger_private(nullptr)
        {
            if (log_name.empty())
                m_logger_private = logger::getInstance();
            else
                m_logger_private = logger::getInstance(log_name);

            switch (type)
            {
            case 0:
                m_logger_private->debug();
                break;

            case 1:
                m_logger_private->info();
                break;

            case 2:
                m_logger_private->warning();
                break;

            case 3:
                m_logger_private->error();
                break;

            case 4:
                m_logger_private->fatal();
                break;

            default:
                break;
            }
        }
        ~logger_wrapper()
        {
            *m_logger_private << "\n";
        }

        logger_wrapper(const logger_wrapper &l) = delete;
        logger_wrapper &operator=(const logger_wrapper &l) = delete;

        template <class _type>
        logger_wrapper &operator<<(const _type &t)
        {
            *m_logger_private << azh::sdk::type::to_string(t);
            return *this;
        }
    };

    using aDebug = logger_wrapper;
}