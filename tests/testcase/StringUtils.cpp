#include "azh/type/string.hpp"
#include "azh/utils/logger.hpp"
#include "azh/test/unit_test.hpp"
#include "StringUtils.h"
#include <vector>
#include <string>

using namespace azh::sdk::test;

TEST_GROUP(StringUtilsTests);

TEST(StringUtilsTests, SplitBasic, "Test basic string splitting") {
    std::vector<std::string> result = StringUtils::split("a,b,c", ',');
    
    ASSERT_EQUAL(3, result.size());
    ASSERT_EQUAL("a", result[0]);
    ASSERT_EQUAL("b", result[1]);
    ASSERT_EQUAL("c", result[2]);
}

TEST(StringUtilsTests, SplitEmpty, "Test splitting empty string") {
    std::vector<std::string> result = StringUtils::split("", ',');
    
    ASSERT_EQUAL(0, result.size());
}

TEST(StringUtilsTests, SplitNoDelimiter, "Test string without delimiter") {
    std::vector<std::string> result = StringUtils::split("hello world", ',');
    
    ASSERT_EQUAL(1, result.size());
    ASSERT_EQUAL("hello world", result[0]);
}

// 测试 trim 函数
TEST(StringUtilsTests, TrimSpaces, "Test trimming spaces") {
    std::string result = StringUtils::trim("  hello  ");
    
    ASSERT_EQUAL("hello", result);
}

TEST(StringUtilsTests, TrimTabs, "Test trimming tabs") {
    std::string result = StringUtils::trim("\t\thello\t\t");
    
    ASSERT_EQUAL("hello", result);
}

TEST(StringUtilsTests, TrimAllSpaces, "Test trimming all spaces") {
    std::string result = StringUtils::trim("     ");
    
    ASSERT_EQUAL("", result);
}

// 测试 reverse 函数
TEST(StringUtilsTests, ReverseString, "Test string reversal") {
    std::string result = StringUtils::reverse("hello");
    
    ASSERT_EQUAL("olleh", result);
}

TEST(StringUtilsTests, ReverseEmpty, "Test reversing empty string") {
    std::string result = StringUtils::reverse("");
    
    ASSERT_EQUAL("", result);
}

// 测试 contains 函数
TEST(StringUtilsTests, ContainsTrue, "Test contains returns true") {
    bool result = StringUtils::contains("hello world", "world");
    
    ASSERT_TRUE(result);
}

TEST(StringUtilsTests, ContainsFalse, "Test contains returns false") {
    bool result = StringUtils::contains("hello world", "test");
    
    ASSERT_FALSE(result);
}

// 测试大小写转换
TEST(StringUtilsTests, ToUpperTest, "Test toUpper conversion") {
    std::string result = StringUtils::toUpper("Hello World");
    
    ASSERT_EQUAL("HELLO WORLD", result);
}

TEST(StringUtilsTests, ToLowerTest, "Test toLower conversion") {
    std::string result = StringUtils::toLower("HELLO WORLD");
    
    ASSERT_EQUAL("hello world", result);
}

// 测试异常情况
TEST(StringUtilsTests, AssertionTest, "Test assertion failures") {
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