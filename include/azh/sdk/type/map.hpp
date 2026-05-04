#pragma once

#include "b_tree.hpp"

namespace azh::sdk::type
{
    template <class T1, class T2>
    class map
    {
        typedef T1 _key_type;
        typedef T2 _base_type;

        std::hash<_key_type> m_hash_private;
        b_tree<size_t, _base_type> m_data_private;

    public:
        map() = default;
        map(const map &m) : m_data_private(m.m_data_private) {}
        ~map() = default;

        bool insert(const _key_type& key,const _base_type& data)
        {
            size_t hkey=m_hash_private(key);
            if(m_data_private.contains(hkey))
                m_data_private.erase(hkey);
            return m_data_private.insert(hkey,data);
        }

        bool contains(const _key_type& key) const
        {
            return m_data_private.contains(m_hash_private(key));
        }

        const _base_type& at(const _key_type& key) const
        {
            return m_data_private.search(m_hash_private(key));
        }

        const _base_type& value(const _key_type& key) const
        {
            return m_data_private.search(m_hash_private(key));
        }

        bool erase(const _key_type& key)
        {
            return m_data_private.erase(m_hash_private(key));
        }
    };
}