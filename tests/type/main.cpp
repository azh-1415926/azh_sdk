#include "azh/core/base.hpp"
#include "azh/type/type.hpp"

#include "azh/utils/command.hpp"
#include "azh/utils/exception.hpp"
#include "azh/utils/sort.hpp"

using aDebug = azh::sdk::utils::aDebug;

void test_sort_vector()
{
    azh::sdk::type::vector<int> array = {1, 5, 7, 6, 3, 2, 10, 34, 12, 22, 11, 33};
    aDebug()<<"orignal array : "<<array;

    azh::sdk::utils::merge_sort_r<azh::sdk::type::vector<int>>::sort(array.begin(), array.end());
    aDebug()<<"sorted array : "<<array;
}

void test_varient_vector()
{
    azh::sdk::type::vector<azh::sdk::type::varient> array_v = {1, 5, 7, 6, 3, 2, 10, 34, 12, 22, 11, 33};
    aDebug()<<array_v;
}

void test_list()
{
    azh::sdk::type::list<int> l;
    l<<4<<1<<0<<2;
    aDebug()<<l;
    aDebug()<<"orignal list : "<<l;

    azh::sdk::utils::merge_sort_r<azh::sdk::type::list<int>>::sort(l.begin(), l.end());
    aDebug()<<"sorted list : "<<l;
}

void test_queue()
{
    azh::sdk::type::queue<int> q;
    q.enqueue(1);
    q.enqueue(3);
    q.enqueue(4);
    q.enqueue(0);
    q.enqueue(7);
    q<<8<<2<<1;

    aDebug()<<q;
}

void test_btree()
{
    azh::sdk::type::btree<int, int> *b = new azh::sdk::type::btree<int, int>;
    b->insert(1, 1);
    b->insert(3, 3);
    b->insert(0, 0);
    b->insert(7, 7);
    b->insert(9, 9);
    b->insert(6, 6);
    b->insert(2, 2);
    b->insert(21, 21);
    b->insert(51, 51);
    b->insert(4, 4);
    // (*b)<<azh::sdk::type::pair<int,int>(4,4);

    azh::sdk::type::btree<int, int> b1(*b);
    delete b;
    aDebug()<<b1;
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
    *s<<5;

    azh::sdk::type::stack<int> s2(*s);
    delete s;

    aDebug() << s2;
}

void test_pair()
{
    azh::sdk::type::pair<int,int> pa={1,2};
    aDebug()<<pa;
}

void test_varientlist()
{
    azh::sdk::type::varientlist l;
    l<<"1"<<"test"<<1<<3414<<1.2<<'1';
    aDebug()<<l;
}

int main()
{
    test_sort_vector();
    test_varient_vector();
    test_list();
    test_queue();
    test_btree();
    test_stringlist();
    test_stack();
    test_pair();
    test_varientlist();

    return 0;
}