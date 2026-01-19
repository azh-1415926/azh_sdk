#pragma once

#include "queue.hpp"
#include "pair.hpp"
#include "vector.hpp"

namespace azh::sdk::type
{
    /*
        btree 模板类的前置声明，方便 btnode 将其设为友元类
        三个模板参数分别为
            _key_type 为 B 树中关键字的类型
            _base_type 为 B 树中数据的存储类型
            _degree 为 B 树的阶数，当前为 3 阶 B 树，即 2-3 树
    */
    template <class _key_type, class _base_type, size_t _degree = 3>
    class btree;

    /* btnode 为 B 树的节点，模板参数同前面 btree 一样，就不设默认参数了 */

    template <class _key_type, class _base_type, size_t _degree = 3>
    class btnode
    {
        /* 将 btree 模板类设为友元类，方便 btree 访问当前类的私有成员 */
        friend class btree<_key_type, _base_type, _degree>;

        /* m_key_size_private 为当前节点的关键字数量 */
        size_t m_key_size_private;
        /* keys、data 用于存放关键字、数据 */
        _key_type **m_keys_ptr_private;
        _base_type **m_data_ptr_private;
        /* parent 为父节点，默认为空 */
        btnode *m_parent_private;
        // /* child 为所有子节点 */
        btnode **m_children_private;

    public:
        std::string toString() const
        {
            std::string str;
            for (size_t i = 0; i < m_key_size_private; i++)
            {
                str += to_string(key(i + 1));
                str += "-";
                str += to_string(value(i + 1));
                if (i != m_key_size_private - 1)
                    str += ",";
            }

            return str;
        }

    private:
        btnode(btnode *parent = nullptr)
            : m_parent_private(parent), m_key_size_private(0)
              /* 下标为 0 位置的关键字和数据弃用，故所需空间多一 */
              ,
              m_keys_ptr_private(new _key_type *[_degree + 1]), m_data_ptr_private(new _base_type *[_degree + 1])
              /* 由于有左右孩子节点，所以子节点为关键字总数+1，由于实际上关键字只有 _degree 个，故子节点有 _degree+1 个 */
              ,
              m_children_private(new btnode *[_degree + 1])
        {
            /* 子节点全部设为 nullptr */
            for (size_t i = 0; i <= _degree; i++)
                m_children_private[i] = nullptr;

            for (size_t i = 0; i < _degree + 1; i++)
            {
                m_keys_ptr_private[i] = nullptr;
                m_data_ptr_private[i] = nullptr;
            }
        }

        ~btnode()
        {
            /* 释放空间 */
            for (size_t i = 0; i < m_key_size_private; i++)
            {
                delete m_keys_ptr_private[i + 1];
                delete m_data_ptr_private[i + 1];
            }

            delete[] m_keys_ptr_private;
            delete[] m_data_ptr_private;
            delete[] m_children_private;
        }

        _key_type &key(size_t i) { return *(m_keys_ptr_private[i]); }
        _base_type &value(size_t i) { return *(m_data_ptr_private[i]); }
        const _key_type &key(size_t i) const { return *(m_keys_ptr_private[i]); }
        const _base_type &value(size_t i) const { return *(m_data_ptr_private[i]); }

        void removeKey(size_t i)
        {
            if (m_keys_ptr_private[i])
            {
                delete m_keys_ptr_private[i];
            }

            m_keys_ptr_private[i] = nullptr;
        }
        void removeValue(size_t i)
        {
            if (m_data_ptr_private[i])
            {
                delete m_data_ptr_private[i];
            }

            m_data_ptr_private[i] = nullptr;
        }

        static btnode *copyNode(btnode *dest, btnode *src)
        {
            dest->m_key_size_private = src->m_key_size_private;
            for (size_t i = 1; i <= dest->m_key_size_private; i++)
            {
                dest->m_keys_ptr_private[i] = new _key_type(src->key(i));
                dest->m_data_ptr_private[i] = new _base_type(src->value(i));
            }

            return dest;
        }

        static void moveNode(btnode *dest, btnode *src)
        {
            dest->m_key_size_private = src->m_key_size_private;
            dest->m_children_private[0] = src->m_children_private[0];
            if (src->m_children_private[0] != nullptr)
                src->m_children_private[0]->m_parent_private = dest;
            for (size_t i = 1; i <= dest->m_key_size_private; i++)
            {
                dest->m_keys_ptr_private[i] = src->m_keys_ptr_private[i];
                dest->m_data_ptr_private[i] = src->m_data_ptr_private[i];
                dest->m_children_private[i] = src->m_children_private[i];
                if (src->m_children_private[i] != nullptr)
                    src->m_children_private[i]->m_parent_private = dest;
            }

            /* clear node src */
            src->m_children_private[0] = nullptr;
            for (size_t i = 1; i < src->m_key_size_private; i++)
            {
                src->m_keys_ptr_private[i] = nullptr;
                src->m_data_ptr_private[i] = nullptr;
                src->m_children_private[i] = nullptr;
            }
            src->m_key_size_private = 0;
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
            if (!m_keys_ptr_private[i])
                m_keys_ptr_private[i] = key;
        }

        void setData(size_t i, _base_type *data)
        {
            if (!m_data_ptr_private[i])
                m_data_ptr_private[i] = data;
        }

        void insertData(size_t index, _key_type* k, _base_type* data)
        {
            /* 往 index 下标插入 key、value，将 index 后的数据往后挪一位，使得 index 位置空闲 */
            size_t end_pos = m_key_size_private + 1;

            removeKey(end_pos);
            removeValue(end_pos);
            for (size_t i = 0; i < end_pos - index; i++)
            {
                m_keys_ptr_private[end_pos - i] = m_keys_ptr_private[end_pos - i - 1];
                m_data_ptr_private[end_pos - i] = m_data_ptr_private[end_pos - i - 1];
            }
            /* 将新关键字、数据插入 index 位置，关键字数量++ */
            m_keys_ptr_private[index] = k;
            m_data_ptr_private[index] = data;
            m_key_size_private++;
        }

        /* 查找当前节点中大于等于指定关键字 k 的最大下标 */
        size_t searchIndex(const _key_type &k) const
        {
            /* 因为下标 0 弃用，从 1 开始检索，当 i 未超出关键字数量且小于 k 的值时，i++ */
            size_t i = 1;
            while (i < m_key_size_private && k > key(i))
                i++;
            return i;
        }

        /* 插入数据到指定下标位置，且将关键字数量加一 */
        void insertData(size_t index, const _key_type &k, const _base_type &data)
        {
            /* 往 index 下标插入 key、value，将 index 后的数据往后挪一位，使得 index 位置空闲 */
            size_t end_pos = m_key_size_private + 1;

            removeKey(end_pos);
            removeValue(end_pos);
            for (size_t i = 0; i < end_pos - index; i++)
            {
                m_keys_ptr_private[end_pos - i] = m_keys_ptr_private[end_pos - i - 1];
                m_data_ptr_private[end_pos - i] = m_data_ptr_private[end_pos - i - 1];
            }
            /* 将新关键字、数据插入 index 位置，关键字数量++ */
            m_keys_ptr_private[index] = new _key_type(k);
            m_data_ptr_private[index] = new _base_type(data);
            m_key_size_private++;
        }

        /* 将节点 node 插入当前节点下标 index 位置的右子节点处，关键字数量将不变 */
        void insertChild(size_t index, btnode *n)
        {
            /*
                这个函数会根据当前节点中的关键字总数，将 node 插入 index 位置的右子节点中
                将所有子节点往后挪一位，使得 index 的右子节点位置空闲，再插入 node，同时将 node 的父节点设为当前节点（若 node 不为空）
                    需要注意的是将从最后一个关键字的右子节点开始挪动
                    且将覆盖最后一个关键字的右子节点
                    但 index 若为最后一个关键字的下标，则不挪动，直接插入 node
                故需要提前插入关键字及数据，随后再插入子节点，不然无法生效
            */
            size_t end_pos = m_key_size_private;

            for (size_t i = 0; i < end_pos - index; i++)
                m_children_private[end_pos - i] = m_children_private[end_pos - i - 1];
            m_children_private[index] = n;
            if (n != nullptr)
                n->m_parent_private = this;
        }

        /* 删除指定下标位置的数据，关键字数量将减一 */
        void deleteData(size_t index)
        {
            /* 将 index 位置的数据赋值给 data，然后将 index 位置后的所有数据往前移，将会覆盖 index 位置的数据 */
            size_t end_pos = m_key_size_private;

            removeKey(index);
            removeValue(index);

            for (size_t i = index; i < end_pos; i++)
            {
                m_keys_ptr_private[i] = m_keys_ptr_private[i + 1];
                m_data_ptr_private[i] = m_data_ptr_private[i + 1];
            }
            m_keys_ptr_private[end_pos] = nullptr;
            m_data_ptr_private[end_pos] = nullptr;
            /* 成功覆盖后，将关键字总数-- */
            m_key_size_private--;
        }

        /* 删除指定下标位置的子节点指针，关键字数量将不变，需要手动释放其返回删除的子节点 */
        btnode *deleteBTNode(size_t index)
        {
            /*
                删除 index 位置的子节点指针
                    将最后面的子节点指针往前挪动
                    直到覆盖 index 位置的子节点指针
                    并将最后位置的子节点指针赋值为 nullptr
                返回子节点指针，需要注意的是需要手动释放该指针
            */
            size_t end_pos = m_key_size_private;
            btnode *child = m_children_private[index];
            if (child != nullptr)
                child->m_parent_private = nullptr;
            for (size_t i = index; i < end_pos; i++)
                m_children_private[index] = m_children_private[index + 1];
            m_children_private[end_pos] = nullptr;
            return child;
        }
    };

    /* btree 提供简易的接口，传入关键字类型、数据类型、阶数（默认为 3）来初始化该模板类 */
    template <class T1, class T2, size_t _degree>
    class btree
    {
        typedef T1 _key_type;
        typedef T2 _base_type;
        using node = btnode<T1, T2, _degree>;

        /* m_root_private 为该 B 树的根节点 */
        node *m_root_private;
        size_t m_data_size_private;
        static const size_t m_s_max_key_private = _degree - 1;
        static const size_t m_s_min_key_private = (_degree - 1) / 2;
        static const size_t m_s_mid_key_private = (_degree + 1) / 2;

    public:
        /* 构造 B 树，即创建根节点 */
        btree() : m_root_private(new node), m_data_size_private(0) {}

        btree(const btree &t) : m_data_size_private(t.m_data_size_private)
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

                    while (i <= curr_src_node_ptr->m_key_size_private)
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

        ~btree()
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
                    while (i <= curr_node_ptr->m_key_size_private)
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
            std::string str = "btree(";

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
                while (i <= curr_node_ptr->m_key_size_private)
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

        /* 返回当前 B 树的阶数 */
        inline size_t order() const { return _degree; }
        inline size_t size() const { return m_data_size_private; }

        /* 查找 key 对应的数据 */
        const _base_type &search(const _key_type &k) const
        {
            size_t i = 1;
            node *n = searchBTNode(k);
            assert(n != nullptr);
            i = n->searchIndex(k);
            return n->value(i);
        }

        /* 创建关键字为 key、数据为 data 的节点，并插入到 B 树中 */
        bool insert(const _key_type &key, const _base_type &data)
        {
            size_t i = 1;
            node *preNode = nullptr;
            node *curr_node_ptr = m_root_private;
            if (m_data_size_private > 0)
            {
                while (curr_node_ptr)
                {
                    i = curr_node_ptr->searchIndex(key);
                    if (key > curr_node_ptr->key(i))
                    {
                        preNode = curr_node_ptr;
                        curr_node_ptr = curr_node_ptr->m_children_private[i];
                    }
                    else if (key == curr_node_ptr->key(i))
                    {
                        return false;
                    }
                    else if (key < curr_node_ptr->key(i))
                    {
                        preNode = curr_node_ptr;
                        curr_node_ptr = curr_node_ptr->m_children_private[i - 1];
                    }
                }
            }

            if (curr_node_ptr == nullptr)
                curr_node_ptr = preNode;
            i = curr_node_ptr->searchIndex(key);
            if (i == curr_node_ptr->m_key_size_private && key > curr_node_ptr->key(i))
                i++;

            curr_node_ptr->insertData(i, key, data);
            if (curr_node_ptr->m_key_size_private > m_s_max_key_private)
            {
                splitBTNode(curr_node_ptr);
            }

            m_data_size_private++;
            return true;
        }

        /* 插入 pair */
        inline bool insert(const pair<_key_type, _base_type> &p)
        {
            return insert(p.first, p.second);
        }

        /* 插入 vector，返回插入失败的数组 */
        inline vector<pair<_key_type, _base_type>> insert(const vector<pair<_key_type, _base_type>> &vec)
        {
            vector<pair<_key_type, _base_type>> failed_vec;
            for (int i = 0; i < vec.size(); i++)
            {
                const pair<_key_type, _base_type> &pair = vec[i];
                if (!insert(pair.first, pair.second))
                    failed_vec.push_back(pair);
            }
            return failed_vec;
        }

        /* 在 B 树中删除关键字为 key 的节点 */
        bool erase(const _key_type &key)
        {
            assert(m_root_private != nullptr);

            if(m_data_size_private<1)
                return false;

            // need to alter
            size_t i = 1;
            node *parent = nullptr;
            node *curr_node_ptr = m_root_private;
            //_base_type data;
            curr_node_ptr = searchBTNode(key);
            if (curr_node_ptr == nullptr)
            {
                return false;
            }
            i = curr_node_ptr->searchIndex(key);
            // save data
            // data=curr_node_ptr->data[i];
            // this node is leaf node
            if (curr_node_ptr->m_children_private[0] == nullptr)
            {
                // key greater than min
                if (curr_node_ptr->m_key_size_private > m_s_min_key_private)
                {
                    curr_node_ptr->deleteData(i);
                    // key equal to min
                }
                else
                {
                    parent = curr_node_ptr->m_parent_private;
                    // leaf node is m_root_private
                    if (parent == nullptr)
                    {
                        curr_node_ptr->deleteData(1);

                        m_data_size_private--;
                        return true;
                    }
                    // normal node
                    size_t j = parent->searchIndex(curr_node_ptr->key(i));
                    if (curr_node_ptr->key(i) < parent->key(j))
                        j--;
                    curr_node_ptr->deleteData(i);
                    balanceBTNode(parent, j);
                }
            }
            else
            {
                // get precursor key-data
                node *target = curr_node_ptr->m_children_private[i - 1];
                parent = target->m_parent_private;
                size_t j = parent->searchIndex(target->key(target->m_key_size_private));
                if (target->key(target->m_key_size_private) < parent->key(j))
                    j--;
                while (target->m_children_private[target->m_key_size_private] != nullptr)
                    target = target->m_children_private[target->m_key_size_private];
                curr_node_ptr->key(i) = target->key(target->m_key_size_private);
                curr_node_ptr->value(i) = target->value(target->m_key_size_private);
                target->deleteData(target->m_key_size_private);
                if (target->m_key_size_private < m_s_min_key_private)
                {
                    if(j<1)
                        j++;
                    balanceBTNode(target->m_parent_private, j);
                }
            }

            m_data_size_private--;
            return true;
        }

        btree &operator<<(const pair<_key_type, _base_type> &p)
        {
            insert(p.first, p.second);

            return *this;
        }

    private:
        /* 根据 key 在全局查找指定节点，若不存在该关键字则返回 nullptr */
        node *searchBTNode(const _key_type &key) const
        {
            size_t i = 1;
            node *curr_node_ptr = m_root_private;
            while (curr_node_ptr)
            {
                /* i 下标位置的关键字大于等于 key
                    当 key 大于 i 位置关键字时当往右子节点继续查找
                    当 key 等于 i 位置关键字时退出循环，表明查找成功
                    当 key 小于 i 位置关键字时当往左子节点继续查找
                */
                i = curr_node_ptr->searchIndex(key);
                // i range is [1,m_key_size_private]
                if (key > curr_node_ptr->key(i))
                    curr_node_ptr = curr_node_ptr->m_children_private[i]; /* right */
                else if (key == curr_node_ptr->key(i))
                    break; /* find it */
                else if (key < curr_node_ptr->key(i))
                    curr_node_ptr = curr_node_ptr->m_children_private[i - 1]; /* left */
            }
            return curr_node_ptr;
        }

        /* 分裂指定节点，传入的 node 仍然为分裂后的根节点 */
        void splitBTNode(node *n)
        {
            node *parent = n->m_parent_private;
            node *r_child = nullptr;
            // right child
            r_child = new node;
            r_child->insertChild(0, n->m_children_private[m_s_mid_key_private]);
            n->m_children_private[m_s_mid_key_private] = nullptr;
            for (size_t i = 1; i <= n->m_key_size_private - m_s_mid_key_private; i++)
            {
                // n->key(m_s_mid_key_private+i)=0;
                // n->value(m_s_mid_key_private+i)=nullptr;
                r_child->insertData(i, n->key(m_s_mid_key_private + i), n->value(m_s_mid_key_private + i));
                r_child->insertChild(i, n->m_children_private[m_s_mid_key_private + i]);
                n->m_children_private[m_s_mid_key_private + i] = nullptr;
            }

            if (parent == nullptr)
            {
                node *l_child = nullptr;
                // left child
                l_child = new node;
                l_child->insertChild(0, n->m_children_private[0]);
                n->m_children_private[0] = nullptr;
                for (size_t i = 1; i < m_s_mid_key_private; i++)
                {
                    l_child->insertData(i, n->key(i), n->value(i));
                    l_child->insertChild(i, n->m_children_private[i]);
                    n->m_children_private[i] = nullptr;
                }
                n->m_key_size_private = 0;
                n->insertChild(0, l_child);
                n->insertData(1, n->key(m_s_mid_key_private), n->value(m_s_mid_key_private));
                n->insertChild(1, r_child);
            }
            else
            {
                size_t index = 1;
                index = parent->searchIndex(n->key(m_s_mid_key_private));
                n->m_key_size_private = m_s_mid_key_private - 1;
                if (n->key(m_s_mid_key_private) > parent->key(index))
                    index++;
                parent->insertData(index, n->key(m_s_mid_key_private), n->value(m_s_mid_key_private));
                // l_child is n;
                parent->insertChild(index, r_child);
                if (parent->m_key_size_private > m_s_max_key_private)
                    splitBTNode(parent);
            }
        }

        /* 将父节点中下标位置为 index 的关键字左移到 index 关键字对应的左子节点，到其 rm_pos 位置停下（即覆盖 rm_pos 位置的关键字、数据），若左子节点为空，则将父节点的第一个关键字覆盖，不处理左子节点 */
        void moveLeft(node *parent, size_t index, node *l_child, size_t rm_pos)
        {
            node *r_child = parent->m_children_private[index];
            if (!(parent && r_child))
                return;

            if (l_child != nullptr)
            {
                // left child move to left
                l_child->m_key_size_private++;
                l_child->setKey(l_child->m_key_size_private, parent->moveKey(index));
                l_child->setData(l_child->m_key_size_private, parent->moveData(index));
            }
            l_child->insertChild(l_child->m_key_size_private, r_child->deleteBTNode(0));
            // l_child->m_key_size_private++;
            // parent move to left
            parent->setKey(index, r_child->moveKey(1));
            parent->setData(index, r_child->moveData(1));
            // right child move to left
            r_child->deleteData(1);
        }

        /* 同理，将父节点中下标位置为 index 的关键字右移到 index 关键字对应的右子节点，到其 rm_pos 位置停下，若右子节点为空，则将父节点的最后一个关键字覆盖，不处理右子节点 */
        void moveRight(node *parent, size_t index, node *r_child, size_t rm_pos)
        {
            node *l_child = parent->m_children_private[index - 1];
            if (!(parent && l_child))
                return;

            if (r_child != nullptr)
            {
                r_child->insertData(1,parent->moveKey(index),parent->moveData(index));
            }
            r_child->insertChild(0, l_child->deleteBTNode(l_child->m_key_size_private));
            // parent move to left
            parent->setKey(index,l_child->moveKey(l_child->m_key_size_private));
            parent->setData(index,l_child->moveData(l_child->m_key_size_private));
            l_child->deleteData(l_child->m_key_size_private);
        }

        /* 合并父节点中指定下标位置的左右子节点 */
        void combineBTNode(node *parent, size_t index)
        {
            if (!parent || index < 1)
                return;
            node *l_child = parent->m_children_private[index - 1];
            node *r_child = parent->m_children_private[index];
            if (!(l_child && r_child))
                return;

            if (l_child->m_key_size_private < m_s_min_key_private)
            {
                // parent key move to left node
                l_child->insertData(l_child->m_key_size_private + 1, parent->key(index), parent->value(index));
                // right node's child move to left node
                l_child->insertChild(l_child->m_key_size_private, r_child->m_children_private[0]);
                // right children move to left node
                for (size_t i = 1; i <= r_child->m_key_size_private; i++)
                {
                    l_child->insertData(l_child->m_key_size_private + 1, r_child->key(i), r_child->value(i));
                    l_child->insertChild(l_child->m_key_size_private, r_child->m_children_private[i]);
                }
                delete parent->deleteBTNode(index);
            }
            else if (r_child->m_key_size_private < m_s_min_key_private)
            {
                // parent key move to right node
                r_child->insertData(1, parent->key(index), parent->value(index));
                r_child->insertChild(0, l_child->m_children_private[0]);
                for (size_t i = 1; i <= l_child->m_key_size_private; i++)
                {
                    r_child->insertData(1, l_child->key(i), l_child->value(i));
                    r_child->insertChild(i, l_child->m_children_private[i]);
                }
                delete parent->deleteBTNode(index - 1);
            }
            /* root node */
            if (parent->m_parent_private == nullptr)
            {
                parent->deleteData(index);
                if (parent->m_key_size_private == 0)
                {
                    node *n = parent->m_children_private[0];
                    // copy l_child to parent
                    node::moveNode(parent, n);
                    delete n;
                }
            }
            else
            {
                // update node
                node *n = parent;
                parent = n->m_parent_private;
                size_t j = parent->searchIndex(n->key(index));
                if (n->key(index) < parent->key(j))
                    j--;
                n->deleteData(index);
                if (n->m_key_size_private < m_s_min_key_private)
                {
                    balanceBTNode(parent, j);
                }
            }
        }

        /* 平衡父节点中指定下标位置的左右子节点 */
        void balanceBTNode(node *parent, size_t index)
        {
            if (parent == nullptr)
                return;
            // btnode* parent=node->m_parent_private;
            node *n = parent->m_children_private[index];
            // move to left
            if (index + 1 <= parent->m_key_size_private && parent->m_children_private[index + 1]->m_key_size_private > m_s_min_key_private)
            {
                // right node is rich node
                moveLeft(parent, index + 1, n, n->m_key_size_private + 1);
            }
            // move to right
            else if (index >= 1 && parent->m_children_private[index - 1]->m_key_size_private > m_s_min_key_private)
            {
                // left node is rich node
                moveRight(parent, index, n, n->m_key_size_private + 1);
            }
            // right and left are not rich node
            else
            {
                if (index >= 1)
                {
                    // node is r_child
                }
                else if (index + 1 <= parent->m_key_size_private)
                {
                    // node is l_child
                    index++;
                }
                combineBTNode(parent, index);
            }
        }
    };
}