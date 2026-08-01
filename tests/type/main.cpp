#include "azh/sdk/core/base.hpp"
#include "azh/sdk/type/type.hpp"

#include "azh/sdk/utils/command.hpp"
#include "azh/sdk/utils/exception.hpp"
#include "azh/sdk/utils/sort.hpp"

#include <map>

void test_sort_vector()
{
    azh::sdk::type::vector<int> array = {1, 5, 7, 6, 3, 2, 10, 34, 12, 22, 11, 33};
    aDebug() << "orignal array : " << array;

    #ifndef __APPLE__
    azh::sdk::utils::bubble_sort<int>::sort(array.begin(), array.end());
    aDebug() << "sorted array : " << array;
    #endif
}

void test_variant_vector()
{
    azh::sdk::type::vector<azh::sdk::type::variant> array_v = {1, 5, 7, 6, 3, 2, 10, 34, 12, 22, 11, 33};
    aDebug() << array_v;
}

void test_list()
{
    azh::sdk::type::list<int> l;
    l << 4 << 1 << 0 << 2;
    aDebug() << l;
    aDebug() << "orignal list : " << l;

    #ifndef __APPLE__
    azh::sdk::utils::insertion_sort<int>::sort(l.begin(), l.end());
    aDebug() << "sorted list : " << l;
    #endif
}

void test_queue()
{
    azh::sdk::type::queue<int> q;
    q.enqueue(1);
    q.enqueue(3);
    q.enqueue(4);
    q.enqueue(0);
    q.enqueue(7);
    q << 8 << 2 << 1;

    aDebug() << q;
}

void test_btree()
{
    azh::sdk::type::b_tree<int, int> *b = new azh::sdk::type::b_tree<int, int>;
    b->insert(1, 1);
    aDebug() << *b;
    b->insert(3, 3);
    aDebug() << *b;
    b->insert(0, 0);
    aDebug() << *b;
    b->insert(7, 7);
    aDebug() << *b;
    b->insert(9, 9);
    aDebug() << *b;
    b->insert(6, 6);
    aDebug() << *b;
    b->insert(2, 2);
    aDebug() << *b;
    b->insert(21, 21);
    aDebug() << *b;
    b->insert(51, 51);
    aDebug() << *b;
    b->insert(4, 4);
    aDebug() << *b;
    b->erase(4);
    aDebug() << *b;
    b->erase(1);
    aDebug() << *b;
    b->erase(3);
    aDebug() << *b;
    b->erase(0);
    aDebug() << *b;
    b->erase(2);
    aDebug() << *b;
    b->erase(7);
    aDebug() << *b;
    b->erase(51);
    aDebug() << *b;
    b->erase(6);
    aDebug() << *b;
    b->erase(9);
    aDebug() << *b;
    b->erase(21);
    aDebug() << *b;

    azh::sdk::type::b_tree<int, int> b1(*b);
    delete b;
    aDebug() << b1;
}

void test_stringlist()
{
    azh::sdk::type::stringlist list_s;
    list_s << "test" << "hello" << "12";
    aDebug() << list_s;
}

void test_stack()
{
    azh::sdk::type::stack<int> *s = new azh::sdk::type::stack<int>;
    s->push(1);
    s->push(2);
    s->push(4);
    *s << 5;

    azh::sdk::type::stack<int> s2(*s);
    delete s;

    aDebug() << s2;
}

void test_pair()
{
    azh::sdk::type::pair<int, int> pa = {1, 2};
    aDebug() << pa;
}

void test_variantlist()
{
    azh::sdk::type::variantlist l;
    l << "1" << "test" << 1 << 3414 << 1.2 << '1';
    aDebug() << l;
}

void test_map()
{
    azh::sdk::type::map<std::string, std::string> m;
    m.insert("nihao", "hello");
    aDebug() << m.value("nihao");
    m.insert("nihao", "hello1");
    aDebug() << m.value("nihao");
    aDebug() << m.contains("nihao");
}

void test_map_insert()
{
    azh::sdk::type::map<std::string, size_t> amap;
    std::map<std::string, size_t> map;
    azh::sdk::utils::timer time_of_amap;
    for (size_t i = 0; i < 1000; i++)
    {
        amap.insert(std::to_string(i), i);
    }
    aDebug() << "amap insert cost " << time_of_amap.interval() << " s";

    azh::sdk::utils::timer time_of_map;
    for (size_t i = 0; i < 1000; i++)
    {
        map[std::to_string(i)] = i;
    }
    aDebug() << "map insert cost " << time_of_map.interval() << " s";

    azh::sdk::type::vector<std::string> search_keys;
    
    for (size_t i = 0; i < 1000; i++)
    {
        time_of_amap.refresh();
        if (amap.value(std::to_string(i)) != i)
            throw std::invalid_argument("");

        // aDebug() << "amap search cost " << time_of_amap.interval() << " s";
        double cost_time_of_amap=time_of_amap.interval();

        time_of_map.refresh();
        if (map.at(std::to_string(i)) != i)
            throw std::invalid_argument("");

        double cost_time_of_map=time_of_map.interval();

        // aDebug() << "map search cost " << time_of_map.interval() << " s";
        if(cost_time_of_amap<cost_time_of_map)
            search_keys.push_back(std::to_string(i));
    }

    aDebug()<<search_keys;
    aDebug()<<"size : "<<search_keys.size();
    
    time_of_amap.refresh();
    for (size_t i = 0; i < 1000; i++)
    {
        amap.erase(std::to_string(i));
    }
    aDebug() << "amap erase cost " << time_of_amap.interval() << " s";

    time_of_map.refresh();
    for (size_t i = 0; i < 1000; i++)
    {
        map.erase(std::to_string(i));
    }
    aDebug() << "map erase cost " << time_of_map.interval() << " s";
}

int main()
{
    test_sort_vector();
    test_variant_vector();
    test_list();
    test_queue();
    test_btree();
    test_stringlist();
    test_stack();
    test_pair();
    test_variantlist();
    test_map();
    test_map_insert();

    return 0;
}