#include "type.h"
#include "azh/utils/logger.hpp"
#include "azh/test/unit_test.hpp"

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
}

TEST(azh_type, queue, "Test queue")
{
    queue<int> q;
    q << 1 << 3 << 4 << 7 << 8 << 9 << 10 << 0;

    ASSERT_EQUAL(8, q.size());
    ASSERT_EQUAL(1, q.front());
}

TEST(azh_type, stack, "Test stack")
{
    stack<int> s;
    s << 1 << 3 << 4 << 7 << 8 << 9 << 10 << 0;

    ASSERT_EQUAL(8, s.size());
    ASSERT_EQUAL(0, s.top());
}

TEST(azh_type, btree, "Test btree")
{
    btree<int,std::string> b;
    b.insert(45,"hello");
    b.insert(4,"world");
    b.insert(5,"nihao");
    b.insert(11,"hi");
    b.insert(47,"sorry");
    b.insert(17,"!");
    b.insert(67,",");
    b.insert(97,".");
    b.insert(57,"?");

    ASSERT_EQUAL(9,b.size());
    ASSERT_EQUAL("hello",b.search(45));
    ASSERT_EQUAL("world",b.search(4));
    ASSERT_EQUAL("nihao",b.search(5));
    ASSERT_EQUAL("hi",b.search(11));
    ASSERT_EQUAL("sorry",b.search(47));
    ASSERT_EQUAL("!",b.search(17));
    ASSERT_EQUAL(",",b.search(67));
    ASSERT_EQUAL(".",b.search(97));
    ASSERT_EQUAL("?",b.search(57));
}

TEST(azh_type, varient, "Test varient")
{
    varient v1 = 3;
    varient v2 = 3.1;
    varient v3 = "hello";

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
}

TEST(azh_type, varientlist, "Test varientlist")
{
    varientlist l;
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
}