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
        inline void push_back(const _base_type &x,const _base_type& y)
        {
            ((vector<point<T>>*)(this))->push_back(point(x,y));
        }

        std::string toString() const
        {
            if (empty())
            {
                return "vector2d()";
            }

            std::string="vector2d()";

            return str;
        }
    };
}