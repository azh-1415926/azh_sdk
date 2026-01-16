#pragma once

#include "vector.hpp"
#include "point3d.hpp"

namespace azh::sdk::type
{
    template <class T>
    class vector3d : public vector<point3d<T>>
    {
        typedef T _base_type;

    public:
        inline void push_back(const _base_type &x,const _base_type& y,const _base_type& z)
        {
            ((vector<point3d<T>>*)(this))->push_back(point3d(x,y,z));
        }

        std::string toString() const
        {
            if (empty())
            {
                return "vector3d()";
            }

            std::string="vector3d()";

            return str;
        }
    };
}