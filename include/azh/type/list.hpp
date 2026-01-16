#pragma once

#include "string.hpp"

namespace azh::sdk::type
{
	template <class T>
	class list
	{
		typedef T _base_type;
		struct node
		{
		public:
			_base_type *data_ptr;
			node *next;
			node(node *n = nullptr) : data_ptr(nullptr), next(n) {}
			node(const _base_type &d, node *n = nullptr) : data_ptr(new _base_type(d)), next(n) {}
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
		using value_type = _base_type;

		list() : m_head_node_private(new node), m_data_size_private(0)
		{
		}

		list(const list &l) : m_head_node_private(new node), m_data_size_private(0)
		{
			append(std::forward<list>(l));
		}

		list(_base_type *pArray, size_t size)
			: m_head_node_private(new node)
		{
			node *node_ptr = m_head_node_private;
			for (size_t i = 0; i < size; ++i)
			{
				node_ptr->next = new node(pArray[i]);
				node_ptr = node_ptr->next;
			}
			m_data_size_private = size;
		}

		~list()
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

		inline void push_back(const _base_type &data) { insert(std::forward<_base_type>(data), m_data_size_private); }

		inline void pop_back() { erase(m_data_size_private - 1); }

		int indexOf(const _base_type &data) const
		{
			node *node_ptr = m_head_node_private->next;
			int i = 0;
			while (node_ptr)
			{
				if (*node_ptr->data_ptr == data)
					return i;
				node_ptr = node_ptr->next;
				++i;
			}
			return -1;
		}

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

		bool insert(const _base_type &data, size_t pos)
		{
			if (pos > m_data_size_private && m_data_size_private != 0)
				return false;

			node *node_ptr = m_head_node_private;
			size_t i = 0;

			while (i++ < pos && node_ptr)
				node_ptr = node_ptr->next;

			if (i < pos && node_ptr == NULL)
				return false;

			node *tmp_ptr = node_ptr->next;
			node_ptr->next = new node(data);
			node_ptr = node_ptr->next;
			node_ptr->next = tmp_ptr;
			++m_data_size_private;
			return true;
		}

		void erase(size_t pos)
		{
			if (pos >= m_data_size_private && m_data_size_private != 0)
				return;

			node *node_ptr = m_head_node_private;
			size_t i = 0;
			while (i++ < pos && node_ptr)
				node_ptr = node_ptr->next;

			if (i < pos && node_ptr == nullptr)
			{
				return;
			}

			node *tmp_ptr;
			// _base_type e;
			tmp_ptr = node_ptr->next;
			// e = *(tmp_ptr->data_ptr);
			node_ptr->next = tmp_ptr->next;
			delete tmp_ptr;
			--m_data_size_private;
			// return e;
		}

		list &append(list &l)
		{
			m_data_size_private = m_data_size_private + l.m_data_size_private;
			node *node_ptr = m_head_node_private;
			while (node_ptr->next)
				node_ptr = node_ptr->next;

			for (size_t i = 0; i < l.m_data_size_private; ++i)
			{
				node_ptr->next = new node(l.at(i));
				node_ptr = node_ptr->next;
			}
			return *this;
		}

		list &operator=(const list &l)
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
			node *source_node_ptr = l.m_head_node_private->next;
			while (source_node_ptr)
			{
				dest_node_ptr->next = new node(*(source_node_ptr->data_ptr));
				source_node_ptr = source_node_ptr->next;
			}

			return *this;
		}

		_base_type &operator[](size_t i) { return *at(i); }
		const _base_type &operator[](size_t i) const { return *at(i); }

		list &operator<<(const _base_type &t)
		{
			insert(t, m_data_size_private);

			return *this;
		}

		template <class _type>
		list &operator<<(const _type &t)
		{
			insert(_base_type(t), m_data_size_private);

			return *this;
		}

		std::string toString() const
		{
			if (!m_head_node_private->has_next())
			{
				return "list()";
			}

			std::string str = "list(";

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
			list *m_list_private;
			size_t m_i_private;

		public:
			iterator(list *l = nullptr, size_t i = 0)
				: m_list_private(l), m_i_private(i) {}

			iterator(const iterator &it)
				: m_list_private(it.m_list_private), m_i_private(it.m_i_private) {}

			iterator &operator=(const iterator &it)
			{
				m_list_private = it.m_list_private;
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

			bool operator!=(const iterator &it) { return !(m_list_private == it.m_list_private && m_i_private == it.m_i_private); }

			_base_type &operator*() { return *((*m_list_private).at(m_i_private)); }

			int operator-(const iterator &it)
			{
				assert(m_list_private == it.m_list_private);

				return m_i_private - it.m_i_private;
			}

			iterator operator-(int i) const { return iterator(m_list_private, m_i_private - i); }
			iterator operator+(int i) const { return iterator(m_list_private, m_i_private + i); }
		};

		inline iterator begin() { return iterator(this, 0); }
		inline iterator end() { return iterator(this, m_data_size_private); }
	};
}