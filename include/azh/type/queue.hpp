#pragma once

#include "string.hpp"

namespace azh::sdk::type
{
    template <class T>
    class queue
    {
        typedef T _base_type;
        struct node
        {
        public:
            _base_type *data_ptr;
            node *next;
            explicit node(node *n = nullptr) : data_ptr(nullptr), next(n) {}
            explicit node(const _base_type &d, node *n = nullptr) : data_ptr(new _base_type(d)), next(n) {}
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

        node *m_head_node_private;
        size_t m_data_size_private;

    public:
        queue() : m_head_node_private(new node), m_data_size_private(0) {}
        queue(const queue &q) : m_head_node_private(new node), m_data_size_private(0)
        {
            append(std::forward<queue>(q));
        }

        ~queue()
        {
            node *node_ptr = m_head_node_private;
            node *tmp_ptr = nullptr;
            while (node_ptr)
            {
                tmp_ptr = node_ptr;
                node_ptr = node_ptr->next;
                delete tmp_ptr;
            }
        }

        inline bool empty() const { return !m_head_node_private->has_next(); }
        inline size_t size() const { return m_data_size_private; }

        inline _base_type front() const
        {
            assert(!empty());

            return *(m_head_node_private->next->data_ptr);
        };

        void enqueue(const _base_type &data)
        {
            node *node_ptr = m_head_node_private;
            while (node_ptr && node_ptr->next)
                node_ptr = node_ptr->next;

            node_ptr->next = new node(data);
            m_data_size_private++;
        }

        void dequeue()
        {
            if (!m_head_node_private->has_next())
                return;

            node *node_ptr = m_head_node_private->next;
            m_head_node_private->next = node_ptr->next;
            delete node_ptr;
            m_data_size_private--;
        }

        queue &append(queue &q)
        {
            m_data_size_private = m_data_size_private + q.m_data_size_private;
            node *node_ptr = m_head_node_private;
            while (node_ptr->next)
                node_ptr = node_ptr->next;

            for (int i = 0; i < q.m_data_size_private; ++i)
            {
                node_ptr->next = new node(q.at(i));
                node_ptr = node_ptr->next;
            }
            return *this;
        }

        queue &operator=(const queue &q)
        {
            if (!empty())
            {
                node *node_ptr = m_head_node_private->next;
                node *tmp_ptr = nullptr;
                while (node_ptr)
                {
                    tmp_ptr = node_ptr;
                    node_ptr = node_ptr->next;
                    delete tmp_ptr;
                }

                m_head_node_private->next = nullptr;
            }

            node *dest_node_ptr = m_head_node_private;
            node *source_node_ptr = q.m_head_node_private->next;
            while (source_node_ptr)
            {
                dest_node_ptr->next = new node(*(source_node_ptr->data_ptr));
                source_node_ptr = source_node_ptr->next;
            }

            return *this;
        }

        queue &operator<<(const _base_type &t)
        {
            enqueue(t);

            return *this;
        }

        std::string toString() const
        {
            if (!m_head_node_private->has_next())
                return "queue()";

            std::string str = "queue(";

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

        class iterator
        {
        private:
            queue *m_queue_private;
            size_t m_i_private;

        public:
            iterator(queue *q = nullptr, size_t i = 0)
                : m_queue_private(q), m_i_private(i) {}

            iterator(const iterator &it)
                : m_queue_private(it.m_queue_private), m_i_private(it.m_i_private) {}

            iterator &operator=(const iterator &it)
            {
                m_queue_private = it.m_queue_private;
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

            bool operator!=(const iterator &it) { return !(m_queue_private == it.m_queue_private && m_i_private == it.m_i_private); }

            _base_type &operator*() { return *((*m_queue_private).at(m_i_private)); }

            int operator-(const iterator &it)
            {
                assert(m_queue_private == it.m_queue_private);

                return m_i_private - it.m_i_private;
            }

            iterator operator-(int i) const { return iterator(m_queue_private, m_i_private - i); }
            iterator operator+(int i) const { return iterator(m_queue_private, m_i_private + i); }
        };

        inline iterator begin() { return iterator(this, 0); }
        inline iterator end() { return iterator(this, m_data_size_private); }

    private:
        _base_type *at(size_t pos)
        {
            if (pos >= m_data_size_private && m_data_size_private != 0)
                return nullptr;

            node *node_ptr = m_head_node_private->next;
            size_t i = 0;
            while (i++ < pos && node_ptr)
                node_ptr = node_ptr->next;

            if (i < pos && node_ptr == NULL)
            {
                return nullptr;
            }

            return node_ptr->data_ptr;
        }

        _base_type *at(size_t pos) const
        {
            if (pos >= m_data_size_private && m_data_size_private != 0)
                return nullptr;

            node *node_ptr = m_head_node_private->next;
            size_t i = 0;
            while (i++ < pos && node_ptr)
                node_ptr = node_ptr->next;

            if (i < pos && node_ptr == NULL)
            {
                return nullptr;
            }

            return node_ptr->data_ptr;
        }
    };
}