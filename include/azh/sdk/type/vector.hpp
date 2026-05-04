#pragma once

#include "string.hpp"

namespace azh::sdk::type
{
    template <class T>
    class vector
    {
        typedef T _base_type;

    protected:
        _base_type **m_data_ptr_private;
        size_t m_data_size_private;
        size_t m_vec_size_private;
        size_t m_vec_increment_size_private;

    public:
        using value_type = _base_type;

        explicit vector(size_t capacity = 10)
            : m_data_size_private(0), m_vec_size_private(10), m_vec_increment_size_private(20)
        {
            m_data_ptr_private = new _base_type *[m_vec_size_private];

            for (size_t i = m_data_size_private; i < m_vec_size_private; i++)
            {
                m_data_ptr_private[i] = nullptr;
            }
        }

        template <class _type>
        vector(const std::initializer_list<_type> &list)
            : m_data_size_private(0), m_vec_size_private(list.size()), m_vec_increment_size_private(20)
        {
            m_data_ptr_private = new _base_type *[m_vec_size_private];
            size_t i = 0;
            for (auto it = list.begin(); it != list.end(); it++)
            {
                m_data_ptr_private[i] = new _base_type(*it);
                i++;
            }
            for (; i < m_vec_size_private; i++)
            {
                m_data_ptr_private[i] = nullptr;
            }

            m_data_size_private = m_vec_size_private;
        }

        vector(const vector &vec)
            : m_data_size_private(vec.m_data_size_private), m_vec_size_private(vec.m_vec_size_private), m_vec_increment_size_private(vec.m_vec_increment_size_private)
        {
            m_data_ptr_private = new _base_type *[m_vec_size_private];
            for (size_t i = 0; i < m_data_size_private; i++)
            {
                m_data_ptr_private[i] = new _base_type(*(vec.m_data_ptr_private[i]));
            }

            for (size_t i = m_data_size_private; i < m_vec_size_private; i++)
            {
                m_data_ptr_private[i] = nullptr;
            }
        }

        ~vector()
        {
            for (size_t i = 0; i < m_data_size_private; i++)
            {
                delete m_data_ptr_private[i];
            }

            delete[] m_data_ptr_private;
        }

        inline bool empty() const { return m_data_size_private == 0; }
        inline size_t size() const { return m_data_size_private; }
        inline size_t capacity() const { return m_vec_size_private; }
        inline size_t increments() const { return m_vec_increment_size_private; }
        inline void setIncrements(size_t increments) { m_vec_increment_size_private = increments; }

        _base_type &at(size_t i) { return *(m_data_ptr_private[i]); }
        const _base_type &at(size_t i) const { return *(m_data_ptr_private[i]); }

        void reserve(size_t n)
        {
            if (n < m_data_size_private)
                return;
            _base_type **tmp = new _base_type *[n];
            for (size_t i = 0; i < m_data_size_private; i++)
            {
                tmp[i] = m_data_ptr_private[i];
            }

            for (size_t i = m_data_size_private; i < n; i++)
            {
                tmp[i] = nullptr;
            }

            delete[] m_data_ptr_private;
            m_data_ptr_private = tmp;
            m_vec_size_private = n;
        }

        inline void push_back(const _base_type &data)
        {
            if (m_data_size_private == m_vec_size_private)
                reserve(m_vec_size_private + m_vec_increment_size_private);

            if (m_data_ptr_private[m_data_size_private])
            {
                *(m_data_ptr_private[m_data_size_private]) = data;
            }
            else
            {
                m_data_ptr_private[m_data_size_private] = new _base_type(data);
            }

            m_data_size_private++;
        }

        inline void pop_back()
        {
            if(m_data_size_private>0&&m_data_ptr_private[m_data_size_private-1])
            {
                delete m_data_ptr_private[m_data_size_private-1];
                m_data_size_private--;
            }
        }

        _base_type &operator[](size_t i) { return *(m_data_ptr_private[i]); }
        const _base_type &operator[](size_t i) const { return *(m_data_ptr_private[i]); }

        vector &operator=(const vector &vec)
        {
            m_data_size_private = vec.m_data_size_private;
            m_vec_size_private = vec.m_vec_size_private;
            m_vec_increment_size_private = vec.m_vec_increment_size_private;

            for (size_t i = 0; i < m_data_size_private; i++)
                delete m_data_ptr_private[i];
            delete[] m_data_ptr_private;
            m_data_ptr_private = new _base_type *[m_vec_size_private];

            for (size_t i = 0; i < m_data_size_private; i++)
            {
                m_data_ptr_private[i] = new _base_type(*(vec.m_data_ptr_private[i]));
            }

            for (size_t i = m_data_size_private; i < m_vec_size_private; i++)
            {
                m_data_ptr_private[i] = nullptr;
            }

            return *this;
        }

        vector &operator<<(const _base_type &t)
        {
            push_back(t);

            return *this;
        }

        std::string toString() const
        {
            if (empty())
            {
                return "vector()";
            }

            std::string str = "vector(";

            for (size_t i = 0; i < m_data_size_private; i++)
            {
                str += to_string((*this)[i]);
                if (i != m_data_size_private - 1)
                    str += ",";
            }

            str += ")";

            return str;
        }

        class iterator
        {
        private:
            vector *m_vec_private;
            size_t m_i_private;

        public:
            iterator(vector *vec = nullptr, size_t i = 0)
                : m_vec_private(vec), m_i_private(i) {}

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

            _base_type &operator*() { return (*m_vec_private)[m_i_private]; }

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
