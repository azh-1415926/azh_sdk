#include "type.h"
#include "azh/sdk/utils/logger.hpp"
#include "azh/sdk/test/unit_test.hpp"

using namespace azh::sdk::test;
using namespace azh::sdk::type;

TEST_GROUP(azh_type);

TEST(azh_type, vec, "Test vector")
{
    vector<int> array = {1, 3, 4, 7, 8, 9, 10, 0};

    ASSERT_EQUAL(8, array.size());
    ASSERT_EQUAL(1, array[0]);
    ASSERT_EQUAL(3, array[1]);
    ASSERT_EQUAL(4, array[2]);
    ASSERT_EQUAL(7, array[3]);
    ASSERT_EQUAL(8, array[4]);
    ASSERT_EQUAL(9, array[5]);
    ASSERT_EQUAL(10, array[6]);
    ASSERT_EQUAL(0, array[7]);

    array.pop_back();
    array.pop_back();
    array.pop_back();
    array.pop_back();
    array.pop_back();
}

TEST(azh_type, list, "Test list")
{
    list<int> l = {1, 3, 4, 7, 8, 9, 10, 0};

    ASSERT_EQUAL(8, l.size());
    ASSERT_EQUAL(1, l[0]);
    ASSERT_EQUAL(3, l[1]);
    ASSERT_EQUAL(4, l[2]);
    ASSERT_EQUAL(7, l[3]);
    ASSERT_EQUAL(8, l[4]);
    ASSERT_EQUAL(9, l[5]);
    ASSERT_EQUAL(10, l[6]);
    ASSERT_EQUAL(0, l[7]);

    l.pop_back();
    ASSERT_EQUAL(7, l.size());
    l.push_back(66);
    ASSERT_EQUAL(66, l[7]);
    l.insert(22,2);
    ASSERT_EQUAL(9, l.size());
    
    ASSERT_EQUAL(1, l[0]);
    ASSERT_EQUAL(3, l[1]);
    ASSERT_EQUAL(22, l[2]);
    ASSERT_EQUAL(4, l[3]);
    ASSERT_EQUAL(7, l[4]);
    ASSERT_EQUAL(8, l[5]);
    ASSERT_EQUAL(9, l[6]);
    ASSERT_EQUAL(10, l[7]);
    ASSERT_EQUAL(66, l[8]);
}

TEST(azh_type, queue, "Test queue")
{
    queue<int> q;
    q << 1 << 3 << 4 << 7 << 8 << 9 << 10 << 0;

    ASSERT_EQUAL(8, q.size());
    ASSERT_EQUAL(1, q.front());

    q.dequeue();
    ASSERT_EQUAL(3, q.front());

    q.dequeue();
    ASSERT_EQUAL(4, q.front());

    q.dequeue();
    ASSERT_EQUAL(7, q.front());

    q.dequeue();
    ASSERT_EQUAL(8, q.front());

    q.dequeue();
    ASSERT_EQUAL(9, q.front());

    q.dequeue();
    ASSERT_EQUAL(10, q.front());

    q.dequeue();
    ASSERT_EQUAL(0, q.front());

    q.dequeue();
    ASSERT_EQUAL(0, q.size());
}

TEST(azh_type, stack, "Test stack")
{
    stack<int> s;
    s << 1 << 3 << 4 << 7 << 8 << 9 << 10 << 0;

    ASSERT_EQUAL(8, s.size());
    ASSERT_EQUAL(0, s.top());

    s.pop();
    ASSERT_EQUAL(10, s.top());

    s.pop();
    ASSERT_EQUAL(9, s.top());

    s.pop();
    ASSERT_EQUAL(8, s.top());

    s.pop();
    ASSERT_EQUAL(7, s.top());

    s.pop();
    ASSERT_EQUAL(4, s.top());

    s.pop();
    ASSERT_EQUAL(3, s.top());

    s.pop();
    ASSERT_EQUAL(1, s.top());

    s.pop();
    ASSERT_EQUAL(0, s.size());
}

TEST(azh_type, b_tree, "Test b_tree")
{
    b_tree<int, std::string> b;
    ASSERT_EQUAL(1, b.level());
    b.insert(45, "hello");
    ASSERT_EQUAL(1, b.level());
    b.insert(4, "world");
    ASSERT_EQUAL(1, b.level());
    b.insert(5, "nihao");
    ASSERT_EQUAL(2, b.level());
    b.insert(11, "hi");
    ASSERT_EQUAL(2, b.level());
    b.insert(47, "sorry");
    ASSERT_EQUAL(2, b.level());
    b.insert(17, "!");
    ASSERT_EQUAL(2, b.level());
    b.insert(67, ",");
    ASSERT_EQUAL(2, b.level());
    b.insert(97, ".");
    ASSERT_EQUAL(3, b.level());
    b.insert(57, "?");
    ASSERT_EQUAL(3, b.level());

    ASSERT_EQUAL(9, b.size());
    ASSERT_EQUAL("hello", b.search(45));
    ASSERT_EQUAL("world", b.search(4));
    ASSERT_EQUAL("nihao", b.search(5));
    ASSERT_EQUAL("hi", b.search(11));
    ASSERT_EQUAL("sorry", b.search(47));
    ASSERT_EQUAL("!", b.search(17));
    ASSERT_EQUAL(",", b.search(67));
    ASSERT_EQUAL(".", b.search(97));
    ASSERT_EQUAL("?", b.search(57));

    b.erase(11);
    ASSERT_EQUAL(8, b.size());
    
    b.insert(11, "hi azh");
    ASSERT_EQUAL(3, b.level());
    b.insert(12, "hi azh");
    ASSERT_EQUAL(3, b.level());
    b.insert(13, "hi azh");
    b.insert(14, "hi azh");
    b.insert(15, "hi azh");
    b.insert(16, "hi azh");
    b.insert(18, "hi azh");
    ASSERT_EQUAL(3, b.level());
    b.insert(19, "hi azh");
    ASSERT_EQUAL(4, b.level());
    b.insert(20, "hi azh");
    b.insert(21, "hi azh");
    b.insert(22, "hi azh");
    ASSERT_EQUAL("hello", b.search(45));
    ASSERT_EQUAL("world", b.search(4));
    ASSERT_EQUAL("nihao", b.search(5));
    ASSERT_EQUAL("hi azh", b.search(11));
    ASSERT_EQUAL("sorry", b.search(47));
    ASSERT_EQUAL("!", b.search(17));
    ASSERT_EQUAL(",", b.search(67));
    ASSERT_EQUAL(".", b.search(97));
    ASSERT_EQUAL("?", b.search(57));
    
    b.erase(11);
    ASSERT_EQUAL(3, b.level());
    b.erase(45);
    b.erase(4);
    b.erase(5);
    b.erase(47);
    b.erase(17);
    b.erase(97);
    ASSERT_EQUAL(3, b.level());
    b.erase(67);
    b.erase(57);
    b.erase(16);
    b.erase(14);
    b.erase(20);
    b.erase(18);
    b.erase(22);
    b.erase(15);
    b.erase(12);
    ASSERT_EQUAL(2, b.level());
    b.erase(19);
    ASSERT_EQUAL(1, b.level());
    b.erase(13);
    b.erase(21);
    ASSERT_EQUAL(1, b.level());
    
    ASSERT_EQUAL(0, b.size());
}

TEST(azh_type, variant, "Test variant")
{
    variant v1 = 3;
    variant v2 = 3.1;
    variant v3 = "hello";

    ASSERT_EQUAL(3, v1.getValue<int>());
    ASSERT_EQUAL(3.1, v2.getValue<double>());
    ASSERT_EQUAL("hello", v3.getValue<std::string>());
}

TEST(azh_type, stringlist, "Test stringlist")
{
    stringlist l;
    l << "1" << "test" << "hello" << "world" << "2" << "2.3" << "4.5" << "100";

    ASSERT_EQUAL(8, l.size());
    ASSERT_EQUAL("1", l[0]);
    ASSERT_EQUAL("test", l[1]);
    ASSERT_EQUAL("hello", l[2]);
    ASSERT_EQUAL("world", l[3]);
    ASSERT_EQUAL("2", l[4]);
    ASSERT_EQUAL("2.3", l[5]);
    ASSERT_EQUAL("4.5", l[6]);
    ASSERT_EQUAL("100", l[7]);
    l.pop_back();
    l.pop_back();
    l.pop_back();
    l.pop_back();
    l.pop_back();
}

TEST(azh_type, variantlist, "Test variantlist")
{
    variantlist l;
    l << 1 << "test" << "hello" << "world" << 2 << 2.3 << 4.5 << 100;

    ASSERT_EQUAL(8, l.size());
    ASSERT_EQUAL(1, l[0].getValue<int>());
    ASSERT_EQUAL("test", l[1].getValue<std::string>());
    ASSERT_EQUAL("hello", l[2].getValue<std::string>());
    ASSERT_EQUAL("world", l[3].getValue<std::string>());
    ASSERT_EQUAL(2, l[4].getValue<int>());
    ASSERT_EQUAL(2.3, l[5].getValue<double>());
    ASSERT_EQUAL(4.5, l[6].getValue<double>());
    ASSERT_EQUAL(100, l[7].getValue<int>());

    l.pop_back();
    l.pop_back();
    l.pop_back();
    l.pop_back();
    l.pop_back();
}