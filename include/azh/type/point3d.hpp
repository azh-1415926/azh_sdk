#pragma once

#include "string.hpp"

namespace azh::sdk::type
{
    template <class T = double>
    class point3d
    {
        typedef T _base_type;

        _base_type *m_data_private;

    public:
        point3d() : point3d(0, 0, 0)
        {
        }

        point3d(const _base_type &x, const _base_type &y, const _base_type &z) : m_data_private(new _base_type[3])
        {
            m_data_private[0] = x;
            m_data_private[1] = y;
            m_data_private[2] = z;
        }

        point3d(const point3d &p) : m_data_private(new _base_type[3])
        {
            m_data_private[0] = p.m_data_private[0];
            m_data_private[1] = p.m_data_private[1];
            m_data_private[2] = p.m_data_private[2];
        }

        point3d &operator=(const point3d &p)
        {
            m_data_private[0] = p.m_data_private[0];
            m_data_private[1] = p.m_data_private[1];
            m_data_private[2] = p.m_data_private[2];

            return *this;
        }

        ~point3d()
        {
            delete[] m_data_private;
        }

        inline size_t size() const { return 3; }

        inline _base_type &x() { return m_data_private[0]; }
        inline _base_type &y() { return m_data_private[1]; }
        inline _base_type &z() { return m_data_private[2]; }

        std::string toString() const { return "point3d(" + to_string(m_data_private[0]) + "," + to_string(m_data_private[1]) + "," + std::to_string(m_data_private[2]) + ")"; }
    };
}