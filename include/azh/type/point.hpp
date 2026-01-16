#pragma once

#include "string.hpp"

namespace azh::sdk::type
{
    template <class T = double>
    class point
    {
        typedef T _base_type;

        _base_type *m_data_private;

    public:
        point() : point(0, 0)
        {
        }

        point(const _base_type &x, const _base_type &y) : m_data_private(new _base_type[2])
        {
            m_data_private[0] = x;
            m_data_private[1] = y;
        }

        point(const point &p) : m_data_private(new _base_type[2])
        {
            m_data_private[0] = p.m_data_private[0];
            m_data_private[1] = p.m_data_private[1];
        }

        point &operator=(const point &p)
        {
            m_data_private[0] = p.m_data_private[0];
            m_data_private[1] = p.m_data_private[1];

            return *this;
        }

        ~point()
        {
            delete[] m_data_private;
        }

        inline size_t size() const { return 2; }

        inline _base_type &x() { return m_data_private[0]; }
        inline _base_type &y() { return m_data_private[1]; }

        std::string toString() const { return "point(" + to_string(m_data_private[0]) + "," + to_string(m_data_private[1]) + ")"; }
    };
}