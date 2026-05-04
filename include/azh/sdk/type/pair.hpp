#pragma once

#include "string.hpp"

namespace azh::sdk::type
{
    template <class T1, class T2 = T1>
    class pair
    {
        typedef T1 _base_type_1;
        typedef T2 _base_type_2;

        _base_type_1 *m_first_private;
        _base_type_2 *m_second_private;

    public:
        pair() : m_first_private(nullptr), m_second_private(nullptr) {}
        pair(const _base_type_1 &v1, const _base_type_2 &v2) : m_first_private(new _base_type_1(v1)), m_second_private(new _base_type_1(v2)) {}
        pair(const pair &p) : m_first_private(new _base_type_1(p.m_first_private)), m_second_private(new _base_type_1(p.m_second_private)) {}
        ~pair()
        {
            if (m_first_private)
                delete m_first_private;

            if (m_second_private)
                delete m_second_private;
        }

        bool empty() { return m_first_private == nullptr && m_second_private == nullptr; }
        inline size_t size() const { return empty() ? 0 : 2; }

        const _base_type_1 &first() const { return *m_first_private; }
        const _base_type_2 &second() const { return *m_second_private; }

        pair &operator=(const pair &p)
        {
            if (!empty())
            {
                m_first_private = p.m_first_private;
                m_second_private = p.m_second_private;
            }
            else
            {
                m_first_private = new _base_type_1(p.m_first_private);
                m_second_private = new _base_type_1(p.m_second_private);
            }

            return *this;
        }

        std::string toString() const { return "pair(" + to_string(*m_first_private) + "," + to_string(*m_second_private) + ")"; }
    };
};