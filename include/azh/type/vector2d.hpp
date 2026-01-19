#pragma once

#include "vector.hpp"
#include "point.hpp"

namespace azh::sdk::type
{
    template <class T>
    class vector2d : public vector<point<T>>
    {
        typedef T _base_type;

    public:
        inline void push_back(const _base_type &x, const _base_type &y)
        {
            ((vector<point<T>> *)(this))->push_back(point(x, y));
        }

        std::string toString() const
        {
            if (empty())
            {
                return "vector2d()";
            }

            std::string str = "vector2d(";

            for (size_t i = 0; i < m_data_size_private; i++)
            {
                str += to_string((*this)[i]);
                if (i != m_data_size_private - 1)
                    str += ",";
            }

            str += ")";

            return str;
        }
    };
}