#pragma once

#include <exception>

#include "queue.hpp"

namespace azh::sdk::type
{
    /*
        b_tree's forward declaration
        T1 -> key type
        T2 -> data type
        _degree -> max_key
    */
    template <class T1, class T2, size_t _degree = 3>
    class b_tree;

    template <class T1, class T2, size_t _degree = 3>
    class b_node
    {
        typedef T1 _key_type;
        typedef T2 _base_type;

        /* set b_tree as friend class，access private member variable */
        friend class b_tree<_key_type, _base_type, _degree>;

        /* m_key_size_private is current node's key size */
        size_t m_key_size_private;
        /* storage key&data */
        _key_type **m_keys_ptr_private;
        _base_type **m_data_ptr_private;
        /* parent is nullptr by default */
        b_node *m_parent_private;
        /* children */
        b_node **m_children_private;

    public:
        std::string toString() const
        {
            std::string str;
            for (size_t i = 0; i < m_key_size_private; i++)
            {
                str += to_string(key(i));
                str += "-";
                str += to_string(value(i));
                if (i != m_key_size_private - 1)
                    str += ",";
            }

            return str;
        }

    private:
        b_node(b_node *parent = nullptr)
            : m_parent_private(parent), m_key_size_private(0), m_keys_ptr_private(new _key_type *[_degree]), m_data_ptr_private(new _base_type *[_degree]), m_children_private(new b_node *[_degree + 1])
        {
            for (size_t i = 0; i < _degree; i++)
            {
                m_keys_ptr_private[i] = nullptr;
                m_data_ptr_private[i] = nullptr;
            }

            /* init children */
            for (size_t i = 0; i <= _degree; i++)
                m_children_private[i] = nullptr;
        }

        ~b_node()
        {
            /* free */
            for (size_t i = 0; i < m_key_size_private; i++)
            {
                if (m_keys_ptr_private[i])
                    delete m_keys_ptr_private[i];
                if (m_data_ptr_private[i])
                    delete m_data_ptr_private[i];
            }

            delete[] m_keys_ptr_private;
            delete[] m_data_ptr_private;
            delete[] m_children_private;
        }

        _key_type &key(size_t i)
        {
            if (i >= m_key_size_private)
                throw std::out_of_range("b_node : out of range by key : " + std::to_string(i));
            return *(m_keys_ptr_private[i]);
        }
        _base_type &value(size_t i)
        {
            if (i >= m_key_size_private)
                throw std::out_of_range("b_node : out of range by value : " + std::to_string(i));
            return *(m_data_ptr_private[i]);
        }
        const _key_type &key(size_t i) const
        {
            if (i >= m_key_size_private)
                throw std::out_of_range("b_node : out of range by key : " + std::to_string(i));
            return *(m_keys_ptr_private[i]);
        }
        const _base_type &value(size_t i) const
        {
            if (i >= m_key_size_private)
                throw std::out_of_range("b_node : out of range by value : " + std::to_string(i));
            return *(m_data_ptr_private[i]);
        }

        size_t size() const { return m_key_size_private; }
        b_node *parent() { return m_parent_private; }
        void setParent(b_node *n) { m_parent_private = n; }

        /* find index who is greater than k, return value range is [0,size()] */
        size_t searchIndex(const _key_type &k) const
        {
            size_t i = 0;
            while (i < m_key_size_private && k > key(i))
                i++;
            return i;
        }

        void freeKey(size_t i)
        {
            if (i >= _degree)
                throw std::out_of_range("b_node : out of range in func freeKey's i : " + std::to_string(i));
            if (m_keys_ptr_private[i])
            {
                delete m_keys_ptr_private[i];
            }

            m_keys_ptr_private[i] = nullptr;
        }
        void freeData(size_t i)
        {
            if (i >= _degree)
                throw std::out_of_range("b_node : out of range in func freeData's i : " + std::to_string(i));
            if (m_data_ptr_private[i])
            {
                delete m_data_ptr_private[i];
            }

            m_data_ptr_private[i] = nullptr;
        }

        static b_node *copyNode(b_node *dest, b_node *src)
        {
            dest->m_key_size_private = src->m_key_size_private;
            for (size_t i = 0; i < dest->m_key_size_private; i++)
            {
                dest->m_keys_ptr_private[i] = new _key_type(src->key(i));
                dest->m_data_ptr_private[i] = new _base_type(src->value(i));
            }

            return dest;
        }

        static void moveNode(b_node *dest, b_node *src)
        {
            dest->m_key_size_private = src->m_key_size_private;
            src->m_key_size_private = 0;
            for (size_t i = 0; i <= dest->m_key_size_private; i++)
            {
                if (i < dest->m_key_size_private)
                {
                    dest->m_keys_ptr_private[i] = src->m_keys_ptr_private[i];
                    dest->m_data_ptr_private[i] = src->m_data_ptr_private[i];
                }

                dest->m_children_private[i] = src->m_children_private[i];
                src->m_children_private[i] = nullptr;

                if (dest->m_children_private[i] != nullptr)
                    dest->m_children_private[i]->setParent(dest);
            }
        }

        _key_type *moveKey(size_t i)
        {
            _key_type *key = m_keys_ptr_private[i];
            m_keys_ptr_private[i] = nullptr;
            return key;
        }

        _base_type *moveData(size_t i)
        {
            _base_type *data = m_data_ptr_private[i];
            m_data_ptr_private[i] = nullptr;
            return data;
        }

        void setKey(size_t i, _key_type *key)
        {
            if (i >= m_key_size_private)
                throw std::out_of_range("b_node : out of range in func setKey's i : " + std::to_string(i));
            if (!m_keys_ptr_private[i])
                m_keys_ptr_private[i] = key;
        }

        void setData(size_t i, _base_type *data)
        {
            if (i >= m_key_size_private)
                throw std::out_of_range("b_node : out of range in func setData's i : " + std::to_string(i));
            if (!m_data_ptr_private[i])
                m_data_ptr_private[i] = data;
        }

        /* insert key&data to index, move the key&data after the index backward, size++ */
        void insertData(size_t index, _key_type *k, _base_type *data)
        {
            size_t end_pos = m_key_size_private;

            freeKey(end_pos);
            freeData(end_pos);
            for (size_t i = 0; i < end_pos - index; i++)
            {
                m_keys_ptr_private[end_pos - i] = m_keys_ptr_private[end_pos - i - 1];
                m_data_ptr_private[end_pos - i] = m_data_ptr_private[end_pos - i - 1];
            }
            m_keys_ptr_private[index] = k;
            m_data_ptr_private[index] = data;
            m_key_size_private++;
        }

        /* insert key&data to index, move the key&data after the index backward, size++ */
        void insertData(size_t index, const _key_type &k, const _base_type &data)
        {
            size_t end_pos = m_key_size_private;

            freeKey(end_pos);
            freeData(end_pos);
            for (size_t i = 0; i < end_pos - index; i++)
            {
                m_keys_ptr_private[end_pos - i] = m_keys_ptr_private[end_pos - i - 1];
                m_data_ptr_private[end_pos - i] = m_data_ptr_private[end_pos - i - 1];
            }
            m_keys_ptr_private[index] = new _key_type(k);
            m_data_ptr_private[index] = new _base_type(data);
            m_key_size_private++;
        }

        /* insert child to index, set child's parent to n, move the children after the index backward, size unchanged */
        void insertChild(size_t index, b_node *n)
        {
            size_t end_pos = m_key_size_private;

            for (size_t i = 0; i < end_pos - index; i++)
                m_children_private[end_pos - i] = m_children_private[end_pos - i - 1];
            m_children_private[index] = n;
            if (n != nullptr)
                n->setParent(this);
        }

        /* delete key&data at index, move the key&data after the index forward, size-- */
        void deleteData(size_t index)
        {
            size_t end_pos = m_key_size_private - 1;

            freeKey(index);
            freeData(index);

            for (size_t i = index; i < end_pos; i++)
            {
                m_keys_ptr_private[i] = m_keys_ptr_private[i + 1];
                m_data_ptr_private[i] = m_data_ptr_private[i + 1];
            }
            m_keys_ptr_private[end_pos] = nullptr;
            m_data_ptr_private[end_pos] = nullptr;
            m_key_size_private--;
        }

        /* delete child at index, move the children after the index forward, size unchanged, need to free manually */
        b_node *deleteChild(size_t index)
        {
            size_t end_pos = m_key_size_private;
            b_node *child = m_children_private[index];

            for (size_t i = index; i < end_pos; i++)
                m_children_private[i] = m_children_private[i + 1];
            m_children_private[end_pos] = nullptr;
            return child;
        }
    };

    /* b_tree */
    template <class T1, class T2, size_t _degree>
    class b_tree
    {
        typedef T1 _key_type;
        typedef T2 _base_type;
        using node = b_node<_key_type, _base_type, _degree>;

        /* m_root_private is root node */
        node *m_root_private;
        size_t m_data_size_private;
        static const size_t m_s_max_key_private = _degree - 1;
        static const size_t m_s_min_key_private = (_degree - 1) / 2;
        static const size_t m_s_mid_key_private = (_degree + 1) / 2;

    public:
        b_tree() : m_root_private(new node), m_data_size_private(0) {}
        b_tree(const b_tree &t) : m_data_size_private(t.m_data_size_private)
        {
            /* copy root */
            m_root_private = new node;
            m_root_private = node::copyNode(m_root_private, t.m_root_private);

            queue<node *> dest_q;
            queue<node *> src_q;
            node *curr_dest_node_ptr = nullptr;
            node *curr_src_node_ptr = nullptr;

            dest_q.enqueue(m_root_private);
            src_q.enqueue(t.m_root_private);
            size_t i = 1;
            while (!src_q.empty())
            {
                curr_dest_node_ptr = dest_q.front();
                curr_src_node_ptr = src_q.front();

                dest_q.dequeue();
                src_q.dequeue();

                if (curr_src_node_ptr->m_children_private[0] != nullptr)
                {
                    node *next_dest_node_ptr = new node(curr_dest_node_ptr);
                    curr_dest_node_ptr->m_children_private[0] = next_dest_node_ptr;

                    node *next_src_node_ptr = curr_src_node_ptr->m_children_private[0];
                    next_dest_node_ptr = node::copyNode(next_dest_node_ptr, next_src_node_ptr);

                    dest_q.enqueue(next_dest_node_ptr);
                    src_q.enqueue(next_src_node_ptr);

                    while (i <= curr_src_node_ptr->size())
                    {
                        if (curr_src_node_ptr->m_children_private[i] != nullptr)
                        {
                            /* copy key/data to dest node */
                            next_dest_node_ptr = new node(curr_dest_node_ptr);
                            curr_dest_node_ptr->m_children_private[i] = next_dest_node_ptr;

                            next_src_node_ptr = curr_src_node_ptr->m_children_private[i];
                            next_dest_node_ptr = node::copyNode(next_dest_node_ptr, next_src_node_ptr);

                            dest_q.enqueue(next_dest_node_ptr);
                            src_q.enqueue(next_src_node_ptr);
                        }
                        i++;
                    }

                    i = 1;
                }
            }
        }

        ~b_tree()
        {
            queue<node *> q;
            node *curr_node_ptr = nullptr;
            q.enqueue(m_root_private);
            size_t i = 1;
            while (!q.empty())
            {
                curr_node_ptr = q.front();
                q.dequeue();
                if (curr_node_ptr->m_children_private[0] != nullptr)
                {
                    q.enqueue(curr_node_ptr->m_children_private[0]);
                    while (i <= curr_node_ptr->size())
                    {
                        if (curr_node_ptr->m_children_private[i] != nullptr)
                            q.enqueue(curr_node_ptr->m_children_private[i]);
                        i++;
                    }
                }
                delete curr_node_ptr;
                i = 1;
            }
        }

        std::string toString() const
        {
            std::string str = "b_tree(";

            queue<node *> q;
            node *curr_node_ptr = nullptr;
            q.enqueue(m_root_private);
            size_t i = 1;
            while (!q.empty())
            {
                curr_node_ptr = q.front();
                q.dequeue();
                if (curr_node_ptr->m_children_private[0] != nullptr)
                {
                    q.enqueue(curr_node_ptr->m_children_private[0]);
                }
                str += "<";
                str += to_string(*curr_node_ptr);
                while (i <= curr_node_ptr->size())
                {
                    if (curr_node_ptr->m_children_private[i] != nullptr)
                        q.enqueue(curr_node_ptr->m_children_private[i]);
                    i++;
                }
                str += ">";
                if (!q.empty())
                    str += ",";
                i = 1;
            }

            str += ")";

            return str;
        }

        /* return b_tree's degree */
        inline size_t order() const { return _degree; }
        /* return data size */
        inline size_t size() const { return m_data_size_private; }

        /* find k in b_tree, return data */
        const _base_type &search(const _key_type &k) const
        {
            size_t i = 0;
            node *n = searchBTNode(k);
            if (!n)
                throw std::out_of_range("b_tree : can not search k : " + to_string(k));
            i = n->searchIndex(k);
            return n->value(i);
        }

        /* insert key&data into b_tree */
        bool insert(const _key_type &key, const _base_type &data)
        {
            size_t i = 0;
            node *pre_node_ptr = nullptr;
            node *curr_node_ptr = m_root_private;

            /* has data */
            if (m_data_size_private > 0)
            {
                curr_node_ptr = searchBTNode(key, true);
            }

            i = curr_node_ptr->searchIndex(key);
            if (i < curr_node_ptr->size() && key > curr_node_ptr->key(i))
                i++;

            curr_node_ptr->insertData(i, key, data);

            /* keys size is more than max size, split */
            if (curr_node_ptr->size() > m_s_max_key_private)
            {
                splitBTNode(curr_node_ptr);
            }

            m_data_size_private++;
            return true;
        }

        /* erase key&data in b_tree */
        bool erase(const _key_type &key)
        {
            if (!m_root_private)
                throw std::out_of_range("b_tree : erase error, m_root_private is nullptr");

            /* empty */
            if (m_data_size_private < 1)
                return false;

            node *curr_node_ptr = searchBTNode(key);
            /* not found this key */
            if (curr_node_ptr == nullptr)
                return false;

            size_t i = curr_node_ptr->searchIndex(key);
            eraseInNode(curr_node_ptr, i);

            m_data_size_private--;
            return true;
        }

    private:
        /* search node by key, if to_ret_nearest_node is true and not find key in all node, then return nearse node */
        node *searchBTNode(const _key_type &key, bool to_ret_nearest_node = false) const
        {
            size_t i = 0;
            node *pre_node_ptr = nullptr;
            node *curr_node_ptr = m_root_private;
            while (curr_node_ptr)
            {
                /* i 下标位置的关键字大于等于 key , range -> [0,m_key_size_private-1]
                    当 key 大于 i 位置关键字时当往右子节点继续查找
                    当 key 等于 i 位置关键字时退出循环，表明查找成功
                    当 key 小于 i 位置关键字时当往左子节点继续查找
                */
                i = curr_node_ptr->searchIndex(key);
                if (i > 0 && i >= curr_node_ptr->size())
                    i--;
                /* right */
                if (key > curr_node_ptr->key(i))
                {
                    pre_node_ptr = curr_node_ptr;
                    curr_node_ptr = curr_node_ptr->m_children_private[i + 1];
                }
                /* find it */
                else if (key == curr_node_ptr->key(i))
                {
                    break;
                }
                /* left */
                else if (key < curr_node_ptr->key(i))
                {
                    pre_node_ptr = curr_node_ptr;
                    curr_node_ptr = curr_node_ptr->m_children_private[i];
                }
            }

            if (!to_ret_nearest_node || curr_node_ptr)
            {
                return curr_node_ptr;
            }

            return pre_node_ptr;
        }

        /* split node n, only for insert */
        void splitBTNode(node *n)
        {
            /* if n is root node, only maintain key&data at m_s_mid_key_private-1, else maintain key&data in [0,m_s_mid_key_private-1) */
            node *parent = n->parent();
            node *r_child = nullptr;
            /* new r_child, more than mid size's key&data move to r_child */
            r_child = new node;
            /* pos : m_s_mid_key_private, use n's right node as r_child's left node */
            r_child->insertChild(0, n->m_children_private[m_s_mid_key_private]);
            n->m_children_private[m_s_mid_key_private] = nullptr;
            /* move key&data in [m_s_mid_key_private , n->size()-1], to r_child */
            for (size_t i = 0; i <= n->size() - m_s_mid_key_private - 1; i++)
            {
                r_child->insertData(i, n->moveKey(m_s_mid_key_private + i), n->moveData(m_s_mid_key_private + i));
                r_child->insertChild(i + 1, n->m_children_private[m_s_mid_key_private + i + 1]);
                n->m_children_private[m_s_mid_key_private + i + 1] = nullptr;
            }

            n->m_key_size_private = m_s_mid_key_private;

            /* n is root node, then new l_child, move key&data in [0,m_s_mid_key_private-1) to l_child */
            if (parent == nullptr)
            {
                node *l_child = new node;
                /* pos : 0, use n's left node as l_child's left node */
                l_child->insertChild(0, n->m_children_private[0]);
                n->m_children_private[0] = nullptr;
                /* pos -> [0 , m_s_mid_key_private-1), move key&data to l_child */
                for (size_t i = 0; i < m_s_mid_key_private - 1; i++)
                {
                    l_child->insertData(i, n->moveKey(i), n->moveData(i));
                    l_child->insertChild(i + 1, n->m_children_private[i + 1]);
                    n->m_children_private[i] = nullptr;
                }

                /* use l_child as n's left node, use r_child as n's right node, only maintain pos at m_s_mid_key_private-1 's key&value */
                n->m_key_size_private = 0;
                n->insertChild(0, l_child);
                n->insertData(0, n->moveKey(m_s_mid_key_private - 1), n->moveData(m_s_mid_key_private - 1));
                n->insertChild(1, r_child);
                r_child->setParent(n);
            }
            /* n is not root node, then move key&data at m_s_mid_key_private-1 to parent node */
            else
            {
                size_t index = parent->searchIndex(n->key(m_s_mid_key_private - 1));
                if (index > 0 && index < parent->size() && n->key(m_s_mid_key_private - 1) > parent->key(index))
                    index++;
                /* maintain key&data in [0,m_s_mid_key_private-1), move key&data at m_s_mid_key_private-1 to parent */
                parent->insertData(index, n->moveKey(m_s_mid_key_private - 1), n->moveData(m_s_mid_key_private - 1));
                n->deleteData(m_s_mid_key_private - 1);
                /* insert r_child to parent */
                parent->insertChild(index + 1, r_child);
                /* if parent's key size more than m_s_max_key_private, then split */
                if (parent->size() > m_s_max_key_private)
                    splitBTNode(parent);
            }
        }

        void eraseInNode(node *n, size_t i)
        {
            /* curr_node_ptr is leaf node */
            if (n->m_children_private[0] == nullptr)
            {
                eraseInLeafNode(n, i);
            }
            else
            {
                eraseInNormalNode(n, i);
            }
        }

        void eraseInLeafNode(node *n, size_t i)
        {
            /* key size > min size, then delete data at i */
            if (n->size() > m_s_min_key_private)
            {
                n->deleteData(i);
            }
            /* key size <= min size */
            else
            {
                node *parent = n->parent();
                /* n is root node, and it's leaf node */
                if (parent == nullptr)
                {
                    n->deleteData(i);
                }
                else
                {
                    /* curr_node_ptr is not root node */
                    size_t j = parent->searchIndex(n->key(i));
                    /* j node's key size must less than min */
                    n->deleteData(i);
                    balanceBTNode(parent, j);
                }
            }
        }

        void eraseInNormalNode(node *n, size_t i)
        {
            /* get predecessor or successor node, named target */
            node *target = getSuccessorNode(n, i);
            if (target == nullptr)
                throw std::out_of_range("b_tree : eraseInNormalNode, n's successor node is nullptr");

            node *target_parent = target->parent();
            if (target_parent == nullptr)
                throw std::out_of_range("b_tree : eraseInNormalNode, target_parent node is nullptr");

            size_t j = target_parent->searchIndex(target->key(0));

            delete n->moveKey(i);
            delete n->moveData(i);
            n->setKey(i, target->moveKey(0));
            n->setData(i, target->moveData(0));
            target->deleteData(0);

            if (target->size() < m_s_min_key_private)
            {
                balanceBTNode(target_parent, j);
            }
        }

        node *getSuccessorNode(node *n, size_t i)
        {
            if (!n->m_children_private[i + 1])
            {
                return nullptr;
            }

            n = n->m_children_private[i + 1];
            while (n->m_children_private[0])
                n = n->m_children_private[0];
            return n;
        }

        node *getPredecessorNode(node *n, size_t i)
        {
            if (!n->m_children_private[i])
            {
                return nullptr;
            }

            n = n->m_children_private[i];
            while (n->m_children_private[n->size()])
                n = n->m_children_private[n->size()];
            return n;
        }

        /* 平衡父节点中指定下标位置的左右子节点 */
        void balanceBTNode(node *parent, size_t index)
        {
            if (parent == nullptr || index < 0 || index > parent->size())
                throw std::invalid_argument("b_tree : balanceBTNode, invaild argument");
            node *n = parent->m_children_private[index];
            size_t other_node_i = index;
            /* 0 is mid, -1 is left, 1 is right */
            int node_type = 0;
            bool other_node_is_rich = false;

            if (index + 1 > parent->size() && index != 0)
            {
                /* n is right node */
                node_type = 1;
            }
            else if (index == 0)
            {
                /* n is left node */
                node_type = -1;
            }
            else
            {
                /* n is mid node */
                node_type = 0;
            }

            switch (node_type)
            {
            /* n is left node*/
            case -1:
                other_node_i = index + 1;
                break;
            /* n is mid node*/
            case 0:
                other_node_i = parent->m_children_private[index - 1]->size() < parent->m_children_private[index + 1]->size() ? index + 1 : index - 1;
                break;
            /* n is right node*/
            case 1:
                other_node_i = index - 1;
                break;

            default:
                throw std::invalid_argument("b_tree : balanceBTNode, undefined node_type : " + std::to_string(node_type));
                break;
            }

            if (parent->m_children_private[other_node_i]->size() <= m_s_min_key_private)
                other_node_is_rich = false;
            else
                other_node_is_rich = true;

            node *l_child = index > other_node_i ? parent->m_children_private[index - 1] : n;
            node *r_child = index > other_node_i ? n : parent->m_children_private[index + 1];

            if (!other_node_is_rich)
            {
                if (l_child == n)
                    combineBTNode(parent, index);
                else
                    combineBTNode(parent, index - 1);
            }
            else if (other_node_is_rich && parent->m_children_private[other_node_i]->m_children_private[0] != nullptr)
            {
                if (l_child == n)
                    combineNodePartial(parent, index);
                else
                    combineNodePartial(parent, index - 1);
            }
            else
            {
                if (node_type <= 0 && other_node_i > index)
                {
                    /* r_child is rich node */
                    moveLeft(parent, index, n->m_key_size_private + 1);
                }
                else if (node_type >= 0 && other_node_i < index)
                {
                    /* l_child is rich node */
                    moveRight(parent, index - 1, n->m_key_size_private + 1);
                }
            }
        }

        /* move key&data at index in parent to children[index]'s rm_pos, move children[index+1]'s first key&data to parent's pos at index */
        void moveLeft(node *parent, size_t index, size_t rm_pos)
        {
            node *l_child = parent->m_children_private[index];
            node *r_child = parent->m_children_private[index + 1];
            if (!(parent && r_child && l_child))
                throw std::invalid_argument("b_tree : moveLeft, parent or left or right node is nullptr");

            /* parent's key&data at index move to l_child's end */
            l_child->insertData(l_child->size(), parent->moveKey(index), parent->moveData(index));
            l_child->insertChild(l_child->size(), r_child->deleteChild(0));
            /* r_child's begin key&data move to parent */
            parent->setKey(index, r_child->moveKey(0));
            parent->setData(index, r_child->moveData(0));
            r_child->deleteData(0);
        }

        /* move key&data at index in parent to children[index+1]'s rm_pos, move children[index]'s last key&data to parent's pos at index */
        void moveRight(node *parent, size_t index, size_t rm_pos)
        {
            node *l_child = parent->m_children_private[index];
            node *r_child = parent->m_children_private[index + 1];
            if (!(parent && r_child && l_child))
                throw std::invalid_argument("b_tree : moveRight, parent or left or right node is nullptr");

            /* parent's key&data at index move to r_child's begin */
            r_child->insertData(0, parent->moveKey(index), parent->moveData(index));
            r_child->insertChild(0, l_child->deleteChild(l_child->size() - 1));
            /* l_child's end key&data move to parent */
            parent->setKey(index, l_child->moveKey(l_child->size() - 1));
            parent->setData(index, l_child->moveData(l_child->size() - 1));
            l_child->deleteData(l_child->size() - 1);
        }

        /* combine parent's children[index] and children[index+1] */
        void combineBTNode(node *parent, size_t index)
        {
            if (!parent || index >= parent->size())
                throw std::out_of_range("b_tree : combineBTNode, parent is nullptr or index is out of range");

            node *l_child = parent->m_children_private[index];
            node *r_child = parent->m_children_private[index + 1];
            if (!(l_child && r_child))
                throw std::invalid_argument("b_tree : combineBTNode, left or right node is nullptr");

            /* -1 is left, 0 is none, 1 is right */
            int node_size_less_than_min = 0;
            /* other node's key size must equal to min */
            bool other_node_is_min = false;
            if (l_child->size() < m_s_min_key_private)
            {
                node_size_less_than_min = -1;
                if (r_child->size() == m_s_min_key_private)
                    other_node_is_min = true;
            }
            else if (r_child->size() < m_s_min_key_private)
            {
                node_size_less_than_min = 1;
                if (l_child->size() == m_s_min_key_private)
                    other_node_is_min = true;
            }

            if (node_size_less_than_min == 0 || !other_node_is_min)
                throw std::invalid_argument("b_tree : combineBTNode, the left and right node can not combine");

            /* parent's key at index move to l_child's end */
            l_child->insertData(l_child->size(), parent->moveKey(index), parent->moveData(index));
            /* move r_child's child at 0 to l_child */
            l_child->insertChild(l_child->size(), r_child->m_children_private[0]);
            /* move r_child's key&data&children to l_child */
            for (size_t i = 0; i < r_child->size(); i++)
            {
                l_child->insertData(l_child->size(), r_child->moveKey(i), r_child->moveData(i));
                l_child->insertChild(l_child->size(), r_child->m_children_private[i + 1]);
            }
            /* delete r_child */
            delete parent->deleteChild(index + 1);
            parent->deleteData(index);

            /* root node, parent = children[0] */
            if (parent->parent() == nullptr)
            {
                if (parent->m_key_size_private == 0)
                {
                    node *n = parent->m_children_private[0];
                    /* copy n to parent */
                    node::moveNode(parent, n);
                    delete n;
                }
            }
            /* not root node, but key size is min */
            else if (parent->size() < m_s_min_key_private)
            {
                node *n = parent;
                parent = n->parent();
                size_t j = parent->searchIndex(n->m_children_private[0]->key(0));
                balanceBTNode(parent, j);
            }
        }

        void combineNodePartial(node *parent, size_t index)
        {
            if (!parent || index >= parent->size())
                throw std::out_of_range("b_tree : combineNodePartial, parent is nullptr or index is out of range");

            node *l_child = parent->m_children_private[index];
            node *r_child = parent->m_children_private[index + 1];
            if (!(l_child && r_child))
                throw std::invalid_argument("b_tree : combineNodePartial, left or right node is nullptr");

            /* -1 is left, 0 is none, 1 is right */
            int node_size_less_than_min = 0;
            /* other node's key size must equal to min */
            bool other_node_is_min = false;
            if (l_child->size() < m_s_min_key_private)
            {
                node_size_less_than_min = -1;
                if (r_child->size() <= m_s_min_key_private)
                    other_node_is_min = true;
            }
            else if (r_child->size() < m_s_min_key_private)
            {
                node_size_less_than_min = 1;
                if (l_child->size() <= m_s_min_key_private)
                    other_node_is_min = true;
            }

            if (node_size_less_than_min == 0 || other_node_is_min)
                throw std::invalid_argument("b_tree : combineNodePartial, the left and right node can not combine");

            /* left node is less than min */
            if (node_size_less_than_min == -1)
            {
                /* parent's key at index move to l_child's end */
                l_child->insertData(l_child->size(), parent->moveKey(index), parent->moveData(index));
                /* move r_child's child at 0 to l_child */
                l_child->insertChild(l_child->size(), r_child->m_children_private[0]);
                /* move r_child's key&data to parent */
                parent->setKey(index, r_child->moveKey(0));
                parent->setData(index, r_child->moveData(0));
                /* r_child delete key&data&child at 0 */
                r_child->deleteChild(0);
                r_child->deleteData(0);
            }
            else if (node_size_less_than_min == 1)
            {
                /* parent's key at index move to r_child's begin */
                r_child->insertData(0, parent->moveKey(index), parent->moveData(index));
                /* move l_child's child at 0 to r_child */
                r_child->insertChild(0, l_child->m_children_private[l_child->size()]);
                /* move r_child's key&data to parent */
                parent->setKey(index, l_child->moveKey(l_child->size() - 1));
                parent->setData(index, l_child->moveData(l_child->size() - 1));
                /* l_child delete key&data&child at l_child->size()-1 */
                l_child->deleteChild(l_child->size());
                l_child->deleteData(l_child->size() - 1);
            }

            /* root node, parent = children[0] */
            if (parent->parent() == nullptr)
            {
                if (parent->m_key_size_private == 0)
                {
                    node *n = parent->m_children_private[0];
                    /* copy n to parent */
                    node::moveNode(parent, n);
                    delete n;
                }
            }
            /* not root node, but key size is min */
            else if (parent->size() < m_s_min_key_private)
            {
                node *n = parent;
                parent = n->parent();
                size_t j = parent->searchIndex(n->m_children_private[0]->key(0));
                balanceBTNode(parent, j);
            }
        }
    };
}