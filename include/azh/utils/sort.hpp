#ifndef SORT_H
#define SORT_H

#include <iostream>
#include <functional>

namespace azh::sdk::utils
{
    template <class T>
    class _sort_base
    {
    protected:
        /* container or base_type */
        typedef T _base_type;
        /* container* or base_type*, is the default iter */
        typedef _base_type *_base_type_iterator;

    protected:
        // /* _type_iter is _base_type::iterator or _base_type*, default is _base_type::iterator */
        // template <class _type_iter = typename _base_type::iterator>
        // static void swap(_type_iter t1, _type_iter t2)
        // {
        //     /* use _get_compare_type_by_iter get real type by iter type */
        //     using _type = typename _get_compare_type_by_iter<_type_iter>::type;

        //     _type *tmp = new _type(*t1);
        //     *t1 = *t2;
        //     *t2 = *tmp;
        // }

        /* _type_iter is _base_type::iterator or _base_type*, default is _base_type::iterator */
        template <class _type_iter = _base_type_iterator>
        static int compare(_type_iter t1, _type_iter t2)
        {
            if (*t1 > *t2)
            {
                return 1;
            }
            else if (*t1 == *t2)
            {
                return 0;
            }

            return -1;
        }

        // /* get real type from container */
        // template <typename _type_iter, typename = void>
        // struct _get_compare_type_by_iter
        // {
        //     using type = typename _base_type::value_type;
        // };

        // /* get real type from base_type* */
        // template <typename _type_iter>
        // struct _get_compare_type_by_iter<_type_iter,
        //                                  std::enable_if_t<std::is_same<_type_iter, _base_type *>::value>>
        // {
        //     using type = _base_type;
        // };

        /* use real type to compare */
        template <typename _type>
        static int _compare(void *t1, void *t2)
        {
            if (*static_cast<_type *>(t1) > *static_cast<_type *>(t2))
            {
                return 1;
            }
            else if (*static_cast<_type *>(t1) == *static_cast<_type *>(t2))
            {
                return 0;
            }

            return -1;
        }

        // /* get real type, call func _compare<real_type>(t1,t2) */
        // template <typename _type = _base_type>
        // static int compare(void *t1, void *t2)
        // {
        //     using compare_type = typename _get_compare_type_by_iter<_type>::type;
        //     return _compare<compare_type>(t1, t2);
        // }

    private:
    };

    /* bubble_sort */
    template <class T>
    class bubble_sort : public _sort_base<T>
    {
    public:
        template <class _type_iter = typename T::iterator>
        static void sort(_type_iter begin, _type_iter end)
        {
            int len = end - begin;
            assert(len > 0);
            for (int i = 0; i < len - 1; i++)
            {
                for (int j = 0; j < len - 1 - i; j++)
                {
                    if (compare(begin + j, begin + j + 1) > 0)
                        swap(begin + j, begin + j + 1);
                }
            }
        }

    private:
        bubble_sort() = delete;
    };

    /* selection_sort */
    template <class T>
    class selection_sort : public _sort_base<T>
    {
    public:
        template <class _type_iter = typename T::iterator>
        static void sort(_type_iter begin, _type_iter end)
        {
            int len = end - begin;
            assert(len > 0);
            for (int i = 0; i < len - 1; i++)
            {
                int min = i;
                for (int j = i + 1; j < len; j++)
                {
                    if (compare(begin + min, begin + j) > 0)
                        min = j;
                }
                if (i != min)
                    swap(begin + i, begin + min);
            }
        }

    private:
        selection_sort() = delete;
    };

    /* insertion_sort */
    template <class T>
    class insertion_sort : public _sort_base<T>
    {
    public:
        template <class _type_iter = typename T::iterator>
        static void sort(_type_iter begin, _type_iter end)
        {
            // using _type =  _get_compare_type_by_iter<_type_iter>::type;
            // size_t byte_sizes = sizeof(char) * (sizeof(*begin));

            // int len = end - begin;
            // assert(len > 0);

            // // void *tmp = new char[byte_sizes];
            // _type *tmp = new _type;
            // for (int i = 1; i < len; i++)
            // {
            //     int j;
            //     // memcpy(tmp, &(*(begin + i)), byte_sizes);
            //     *tmp = *(begin + i);
            //     for (j = i; j > 0 && compare(&*(begin + j - 1), tmp) > 0; j--)
            //     {
            //         // swap(begin + j, begin + j - 1);
            //         _type *__tmp = new _type(*(begin + j));
            //         *(begin + j) = *(begin + j - 1);
            //         *(begin + j - 1) = *__tmp;
            //         delete __tmp;
            //     }

            //     // swap(&*(begin + j), tmp,byte_sizes);

            //     /* swap */
            //     _type *_tmp = new _type(*(begin + j));
            //     *(begin + j) = (_type)(*tmp);
            //     *tmp = *_tmp;
            //     delete _tmp;
            // }
            // delete tmp;
        }

    private:
        insertion_sort() = delete;
    };

    /* shell_sort */
    template <class T>
    class shell_sort : public _sort_base<T>
    {
    public:
        template <class _type_iter = typename T::iterator>
        static void sort(_type_iter begin, _type_iter end)
        {
            // using _type = typename _get_compare_type_by_iter<_type_iter>::type;

            // int len = end - begin;
            // assert(len > 0);

            // _type *tmp = new _type;
            // for (int gap = len >> 1; gap > 0; gap >>= 1)
            // {
            //     for (int i = gap; i < len; i++)
            //     {
            //         int j;
            //         *tmp = *(begin + i);
            //         for (j = i - gap; j >= 0 && (compare(&*(begin + j), tmp) > 0); j -= gap)
            //         {
            //             swap(begin + j + gap, begin + j);
            //             _type *_tmp = new _type(*(begin + j + gap));
            //             *(begin + j + gap) = *(begin + j);
            //             *(begin + j) = *_tmp;
            //             delete _tmp;
            //         }

            //         _type *_tmp = new _type(*(begin + j + gap));
            //         *(begin + j + gap) = *tmp;
            //         *tmp = *(begin + j + gap);
            //         delete _tmp;
            //     }
            // }
            // delete tmp;
        }

    private:
        shell_sort() = delete;
    };

    /* merge_sort */
    template <class T>
    class merge_sort : public _sort_base<T>
    {
    public:
        template <class _type_iter = typename T::iterator>
        static void sort(_type_iter begin, _type_iter end)
        {
            // using _type = typename _get_compare_type_by_iter<_type_iter>::type;
            // int len = end - begin;

            // assert(len > 0);

            // _type *tmp = new _type[len];
            // for (int merge_size = 1; merge_size < len; merge_size <<= 1)
            // {
                
            //     for (int left = 0; left < len;)
            //     {
            //         int mid = left + merge_size - 1;
            //         int right = (mid + merge_size) < (len - 1) ? (mid + merge_size) : (len - 1);
            //         if (mid >= len)
            //             break;
            //         /* 归并开始 */
            //         int i = left;
            //         int p1 = left;
            //         int p2 = mid + 1;
            //         while (p1 <= mid && p2 <= right)
            //         {
            //             if (compare(begin + p1, begin + p2) <= 0)
            //             {
            //                 *(tmp + i) = *(begin + p1);
            //                 i++;
            //                 p1++;
            //             }
            //             else
            //             {
            //                 *(tmp + i) = *(begin + p2);
            //                 i++;
            //                 p2++;
            //             }
            //         }
            //         while (p1 <= mid)
            //         {
            //             *(tmp + i) = *(begin + p1);
            //             i++;
            //             p1++;
            //         }
            //         while (p2 <= right)
            //         {
            //             *(tmp + i) = *(begin + p2);
            //             i++;
            //             p2++;
            //         }
            //         for (int j = 0; j < right + 1 - left; j++)
            //             *(begin + left + j) = *(tmp + left + j);
            //         /* 归并结束 */
            //         left = right + 1;
            //     }
            //     if (merge_size > len / 2)
            //         break;
            // }

            // delete[] tmp;
        }

    private:
        merge_sort() = delete;
    };

    /* merge_sort_r */
    template <class T>
    class merge_sort_r : public _sort_base<T>
    {
    public:
        template <class _type_iter = typename T::iterator>
        static void sort(_type_iter begin, _type_iter end)
        {
            // using _type = typename _get_compare_type_by_iter<_type_iter>::type;
            // int len = end - begin;

            // assert(len > 0);

            // _type *tmp = new _type[len];
            // static std::function<void(int, int)> _mergeRecursive = [begin, tmp](int left, int right)
            // {
            //     if (left == right)
            //         return;
            //     int mid = left + ((right - left) >> 1);
            //     _mergeRecursive(left, mid);
            //     _mergeRecursive(mid + 1, right);
            //     /* 归并开始 */
            //     int i = left;
            //     int p1 = left;
            //     int p2 = mid + 1;
            //     while (p1 <= mid && p2 <= right)
            //     {
            //         if (compare(begin + p1, begin + p2) <= 0)
            //         {
            //             *(tmp + i) = *(begin + p1);
            //             i++;
            //             p1++;
            //         }
            //         else
            //         {
            //             *(tmp + i) = *(begin + p2);
            //             i++;
            //             p2++;
            //         }
            //     }
            //     while (p1 <= mid)
            //     {
            //         *(tmp + i) = *(begin + p1);
            //         i++;
            //         p1++;
            //     }
            //     while (p2 <= right)
            //     {
            //         *(tmp + i) = *(begin + p2);
            //         i++;
            //         p2++;
            //     }
            //     for (size_t i = 0; i < right + 1 - left; i++)
            //         *(begin + left + i) = *(tmp + left + i);
            //     /* 归并结束 */
            // };
            // _mergeRecursive(0, len - 1);

            // delete[] tmp;
        }

    private:
        merge_sort_r() = delete;
    };

    /* quick_sort */
    template <class T>
    class quick_sort : public _sort_base<T>
    {
    public:
        template <class _type_iter = typename T::iterator>
        static void sort(_type_iter begin, _type_iter end)
        {
            int len = end - begin;
            assert(len > 0);

            int *l_stack = new int[len / 2];
            int *r_stack = new int[len / 2];
            int p = -1;
            int pivot, flag;
            // push 0-len-1
            p++;
            l_stack[p] = 0;
            r_stack[p] = len - 1;
            while (p >= 0)
            {
                pivot = r_stack[p];
                int l = l_stack[p];
                int r = pivot - 1;
                while (l < r)
                {
                    // compare to left
                    flag = compare(begin + l, begin + pivot);
                    if (flag <= 0)
                    {
                        l++;
                        continue;
                    }
                    // compare to right
                    while (l < r && flag >= 0)
                    {
                        flag = compare(begin + r, begin + pivot);
                        if (flag >= 0)
                            r--;
                    }
                    swap(begin + l, begin + r);
                }
                flag = compare(begin + l, begin + pivot);
                if (l >= r && flag > 0)
                    swap(begin + l, begin + pivot);
                else
                    l++;
                if (l > l_stack[p] + 1 && l < r_stack[p] - 1)
                {
                    r_stack[p + 1] = r_stack[p];
                    r_stack[p] = l - 1;
                    l_stack[p + 1] = l + 1;
                    p++;
                }
                else if (l > l_stack[p] + 1)
                    r_stack[p] = l - 1;
                else if (l < r_stack[p] - 1)
                    l_stack[p] = l + 1;
                else
                    p--;
            }
            delete[] l_stack;
            delete[] r_stack;
        }

    private:
        quick_sort() = delete;
    };

    /* quick_sort_r */
    template <class T>
    class quick_sort_r : public _sort_base<T>
    {
    public:
        template <class _type_iter = typename T::iterator>
        static void sort(_type_iter begin, _type_iter end)
        {
            int len = end - begin;
            assert(len > 0);

            static std::function<void(int, int)> _quick_recursive = [begin](int left, int right)
            {
                if (left >= right)
                    return;
                int pivot = right;
                int l = left;
                int r = right - 1;
                int flag;
                while (l < r)
                {
                    // compare to left
                    flag = compare(begin + l, begin + pivot);
                    if (flag <= 0)
                    {
                        l++;
                        continue;
                    }
                    // compare to right
                    while (l < r && flag >= 0)
                    {
                        flag = compare(begin + r, begin + pivot);
                        if (flag >= 0)
                            r--;
                    }
                    swap(begin + l, begin + r);
                }
                flag = compare(begin + l, begin + pivot);
                if (l >= r && flag > 0)
                    swap(begin + l, begin + pivot);
                else
                    l++;
                if (left < l - 1)
                    _quick_recursive(left, l - 1);
                if (l + 1 < right)
                    _quick_recursive(l + 1, right);
            };
            _quick_recursive(0, len - 1);
        }

    private:
        quick_sort_r() = delete;
    };

    /* heap_sort */
    template <class T>
    class heap_sort : public _sort_base<T>
    {
    public:
        template <class _type_iter = typename T::iterator>
        static void sort(_type_iter begin, _type_iter end)
        {
            int len = end - begin;
            assert(len > 0);
            static std::function<void(int, int)> heapify = [begin](int index, int heap_size)
            {
                if (heap_size <= 1)
                    return;

                int pre = index;
                int curr = index * 2 + 1;
                int flag;
                while (curr <= heap_size)
                {
                    if (curr + 1 <= heap_size)
                    {
                        flag = compare(begin + curr + 1, begin + curr);
                        if (flag > 0)
                            curr++;
                    }
                    flag = compare(begin + pre, begin + curr);
                    if (flag > 0)
                        return;
                    else
                    {
                        swap(begin + pre, begin + curr);
                        pre = curr;
                        curr = pre * 2 + 1;
                    }
                }
            };
            for (int i = len / 2 - 1; i >= 0; i--)
                heapify(i, len - 1);
            for (int i = len - 1; i > 0; i--)
            {
                swap(begin, begin + i);
                heapify(0, i - 1);
            }
        }

    private:
        heap_sort() = delete;
    };
}

#endif /* SORT_H */