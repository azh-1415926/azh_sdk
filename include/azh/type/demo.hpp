#pragma once

#include "string.hpp"

namespace azh::sdk::type
{
    template <class T>
    class demo
    {
        typedef T _base_type;

        _base_type *m_data_private;
        size_t m_data_size_private;

    public:
        using value_type = _base_type;

        explicit demo() : m_data_size_private(0) {}

        demo(const demo &dem) : m_data_size_private(dem.m_data_size_private)
        {
            m_data_private = new _base_type[m_data_size_private];
            for (size_t i = 0; i < m_data_size_private; i++)
                m_data_private[i] = dem.m_data_private[i];
        }

        ~demo() { delete[] m_data_private; }

        inline bool empty() const { return m_data_size_private == 0; }
        inline size_t size() const { return m_data_size_private; }

        inline void push_back(const _base_type &data) {}

        inline void pop_back() {}

        demo &operator=(const demo &dem)
        {
            return *this;
        }

        std::string toString() const
        {
            if (empty())
            {
                return "demo()";
            }

            std::string str = "demo(";
            str += ")";

            return str;
        }

        class iterator
        {
        private:
            demo *m_vec_private;
            size_t m_i_private;

        public:
            iterator(demo *dem = nullptr, size_t i = 0)
                : m_vec_private(dem), m_i_private(i) {}

            iterator(const iterator &it)
                : m_vec_private(it.m_vec_private), m_i_private(it.m_i_private) {}

            iterator &operator=(const iterator &it)
            {
                m_vec_private = it.m_vec_private;
                m_i_private = it.m_i_private;
            }

            iterator &operator++()
            {
                m_i_private++;
                return *this;
            }

            iterator operator++(int)
            {
                iterator it = *this;
                m_i_private++;
                return it;
            }

            iterator &operator--()
            {
                m_i_private--;
                return *this;
            }

            iterator operator--(int)
            {
                iterator it = *this;
                m_i_private--;
                return it;
            }

            bool operator!=(const iterator &it) { return !(m_vec_private == it.m_vec_private && m_i_private == it.m_i_private); }

            // _base_type &operator*() { return (*m_vec_private)[m_i_private]; }

            int operator-(const iterator &it)
            {
                assert(m_vec_private == it.m_vec_private);

                return m_i_private - it.m_i_private;
            }

            iterator operator-(int i) const { return iterator(m_vec_private, m_i_private - i); }
            iterator operator+(int i) const { return iterator(m_vec_private, m_i_private + i); }
        };

        inline iterator begin() { return iterator(this, 0); }
        inline iterator end() { return iterator(this, m_data_size_private); }
    };
}
