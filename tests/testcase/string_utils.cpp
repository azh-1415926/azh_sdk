#include "azh/sdk/type/string.hpp"
#include "azh/sdk/utils/logger.hpp"
#include "azh/sdk/test/unit_test.hpp"
#include "string_utils.h"
#include <vector>
#include <string>

using namespace azh::sdk::test;

TEST_GROUP(string_utils_tests);

TEST(string_utils_tests, SplitBasic, "Test basic string splitting") {
    std::vector<std::string> result = string_utils::split("a,b,c", ',');
    
    ASSERT_EQUAL(3, result.size());
    ASSERT_EQUAL("a", result[0]);
    ASSERT_EQUAL("b", result[1]);
    ASSERT_EQUAL("c", result[2]);
}

TEST(string_utils_tests, SplitEmpty, "Test splitting empty string") {
    std::vector<std::string> result = string_utils::split("", ',');
    
    ASSERT_EQUAL(0, result.size());
}

TEST(string_utils_tests, SplitNoDelimiter, "Test string without delimiter") {
    std::vector<std::string> result = string_utils::split("hello world", ',');
    
    ASSERT_EQUAL(1, result.size());
    ASSERT_EQUAL("hello world", result[0]);
}

// 测试 trim 函数
TEST(string_utils_tests, TrimSpaces, "Test trimming spaces") {
    std::string result = string_utils::trim("  hello  ");
    
    ASSERT_EQUAL("hello", result);
}

TEST(string_utils_tests, TrimTabs, "Test trimming tabs") {
    std::string result = string_utils::trim("\t\thello\t\t");
    
    ASSERT_EQUAL("hello", result);
}

TEST(string_utils_tests, TrimAllSpaces, "Test trimming all spaces") {
    std::string result = string_utils::trim("     ");
    
    ASSERT_EQUAL("", result);
}

// 测试 reverse 函数
TEST(string_utils_tests, ReverseString, "Test string reversal") {
    std::string result = string_utils::reverse("hello");
    
    ASSERT_EQUAL("olleh", result);
}

TEST(string_utils_tests, ReverseEmpty, "Test reversing empty string") {
    std::string result = string_utils::reverse("");
    
    ASSERT_EQUAL("", result);
}

// 测试 contains 函数
TEST(string_utils_tests, ContainsTrue, "Test contains returns true") {
    bool result = string_utils::contains("hello world", "world");
    
    ASSERT_TRUE(result);
}

TEST(string_utils_tests, ContainsFalse, "Test contains returns false") {
    bool result = string_utils::contains("hello world", "test");
    
    ASSERT_FALSE(result);
}

// 测试大小写转换
TEST(string_utils_tests, ToUpperTest, "Test toUpper conversion") {
    std::string result = string_utils::toUpper("Hello World");
    
    ASSERT_EQUAL("HELLO WORLD", result);
}

TEST(string_utils_tests, ToLowerTest, "Test toLower conversion") {
    std::string result = string_utils::toLower("HELLO WORLD");
    
    ASSERT_EQUAL("hello world", result);
}

// 测试异常情况
TEST(string_utils_tests, AssertionTest, "Test assertion failures") {
    int x = 5;
    int y = 5;
    
    ASSERT_EQUAL(x, y);
    ASSERT_NOT_EQUAL(x, 10);
    ASSERT_TRUE(x == 5);
    ASSERT_FALSE(x == 10);
}

// 另一个测试组的例子
TEST_GROUP(MathTests);

TEST(MathTests, AdditionTest, "Test basic addition") {
    int result = 2 + 3;
    ASSERT_EQUAL(5, result);
}

TEST(MathTests, MultiplicationTest, "Test multiplication") {
    int result = 4 * 5;
    ASSERT_EQUAL(20, result);
}