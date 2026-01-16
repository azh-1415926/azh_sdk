#pragma once

#include "string.hpp"

namespace azh::sdk::type
{
    template <class T>
    class stack
    {
        typedef T _base_type;
        struct node
        {
        public:
            _base_type *data_ptr;
            node *next;
            explicit node(node *n = nullptr) : data_ptr(nullptr), next(n) {}
            explicit node(const _base_type &d, node *n = nullptr) : data_ptr(new _base_type(d)), next(n) {}
            node(const node &d) : data_ptr(), next(n) {}
            ~node()
            {
                if (data_ptr)
                {
                    delete data_ptr;
                }
            }

            bool empty() { return data_ptr == nullptr; }
            bool has_next() { return next != nullptr; }
        };

        node *m_root_private;
        size_t m_data_size_private;

    public:
        stack() : m_root_private(new node), m_data_size_private(0)
        {
        }

        stack(const stack &s) : m_root_private(new node), m_data_size_private(0)
        {
            append(s);
        }

        template <class _type>
        stack(const std::initializer_list<_type> &list) : m_root_private(new node), m_data_size_private(0)
        {
            size_t i = 0;
            for (auto it = list.begin(); it != list.end(); it++)
            {
                push(*it);
                i++;
            }
        }

        ~stack()
        {
            while (m_root_private->next)
            {
                node *tmp_ptr = m_root_private;
                m_root_private = m_root_private->next;
                delete tmp_ptr;
            }
        }

        inline bool empty() const { return !m_root_private->has_next(); }

        inline _base_type &top() const
        {
            assert(!empty());
            return *(m_root_private->data_ptr);
        }

        void push(const _base_type &data)
        {
            m_root_private = new node(data, m_root_private);
            m_data_size_private++;
        }

        void pop()
        {
            if (empty())
                return;

            node *tmp_ptr = m_root_private;
            m_root_private = m_root_private->next;
            delete tmp_ptr;
            m_data_size_private--;
        }

        void append(const stack &s)
        {
            node *dest_ptr = m_root_private;
            node *src_ptr = s.m_root_private;
            while (src_ptr->next)
            {
                dest_ptr->data_ptr = new _base_type(*(src_ptr->data_ptr));
                dest_ptr->next = new node;
                dest_ptr = dest_ptr->next;
                m_data_size_private++;

                src_ptr = src_ptr->next;
            }
        }

        stack &operator<<(const _base_type &t)
        {
            push(t);

            return *this;
        }

        std::string toString() const
        {
            if (!m_root_private->has_next())
                return "stack()";

            std::string str = "stack(";

            for (size_t i = 0; i < m_data_size_private; i++)
            {
                str += to_string(*at(i));
                if (i != m_data_size_private - 1)
                {
                    str += "->";
                }
            }

            str += ")";

            return str;
        }

    public:
        class iterator
        {
        private:
            stack *m_stack_private;
            size_t m_i_private;

        public:
            iterator(stack *q = nullptr, size_t i = 0)
                : m_stack_private(q), m_i_private(i) {}

            iterator(const iterator &it)
                : m_stack_private(it.m_stack_private), m_i_private(it.m_i_private) {}

            iterator &operator=(const iterator &it)
            {
                m_stack_private = it.m_stack_private;
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

            bool operator!=(const iterator &it) { return !(m_stack_private == it.m_stack_private && m_i_private == it.m_i_private); }

            _base_type &operator*() { return *((*m_stack_private).at(m_i_private)); }

            int operator-(const iterator &it)
            {
                assert(m_stack_private == it.m_stack_private);

                return m_i_private - it.m_i_private;
            }

            iterator operator-(int i) const { return iterator(m_stack_private, m_i_private - i); }
            iterator operator+(int i) const { return iterator(m_stack_private, m_i_private + i); }
        };

        inline iterator begin() { return iterator(this, 0); }
        inline iterator end() { return iterator(this, m_data_size_private); }

    private:
        _base_type *at(size_t pos)
        {
            if (pos >= m_data_size_private && m_data_size_private != 0)
                return nullptr;

            node *node_ptr = m_root_private;
            size_t i = 0;
            while (i++ < m_data_size_private-pos-1 && node_ptr)
                node_ptr = node_ptr->next;

            return node_ptr->data_ptr;
        }

        _base_type *at(size_t pos) const
        {
            if (pos >= m_data_size_private && m_data_size_private != 0)
                return nullptr;

            node *node_ptr = m_root_private;
            size_t i = 0;
            while (i++ < m_data_size_private-pos-1 && node_ptr)
                node_ptr = node_ptr->next;

            return node_ptr->data_ptr;
        }
    };
}