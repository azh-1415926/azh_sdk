#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <chrono>
#include <sstream>
#include <exception>
#include <memory>

namespace azh::sdk::test
{
    enum class test_result
    {
        PASSED,
        FAILED,
        ERROR
    };

    // 测试用例信息
    struct test_case
    {
        std::string name;
        std::string description;
        std::function<void()> test_func;
        test_result result;
        std::string err_msg;
        std::chrono::microseconds duration;

        test_case(const std::string &n, const std::string &desc, std::function<void()> func)
            : name(n), description(desc), test_func(func), result(test_result::PASSED) {}
    };

    class assertion_exception;

    class test_group
    {
    private:
        std::string name;
        std::vector<test_case> testCases;
        int passedCount;
        int failedCount;
        int errorCount;

    public:
        test_group(const std::string &groupName)
            : name(groupName), passedCount(0), failedCount(0), errorCount(0) {}

        void addTest(const std::string &testName, const std::string &description,
                     std::function<void()> testFunc)
        {
            testCases.emplace_back(testName, description, testFunc);
        }

        void runAll()
        {
            aDebug() << "=== Running Test Group: " << name << " ===";

            for (auto &testCase : testCases)
            {
                runSingleTest(testCase);
            }

            printSummary();
        }

        void runSingleTest(test_case &testCase)
        {
            auto start = std::chrono::high_resolution_clock::now();

            try
            {
                testCase.test_func();
                testCase.result = test_result::PASSED;
                passedCount++;
            }
            // catch (const assertion_exception &e)
            // {
            //     testCase.result = test_result::FAILED;
            //     testCase.err_msg = e.what();
            //     failedCount++;
            // }
            catch (const std::exception &e)
            {
                testCase.result = test_result::ERROR;
                testCase.err_msg = "Unexpected exception: " + std::string(e.what());
                errorCount++;
            }
            catch (...)
            {
                testCase.result = test_result::ERROR;
                testCase.err_msg = "Unknown exception";
                errorCount++;
            }

            auto end = std::chrono::high_resolution_clock::now();
            testCase.duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            printTestResult(testCase);
        }

        void printTestResult(const test_case &testCase)
        {
            aDebug() << "  ";

            switch (testCase.result)
            {
            case test_result::PASSED:
                aDebug() << "\033[32m[PASS]\033[0m ";
                break;
            case test_result::FAILED:
                aDebug() << "\033[31m[FAIL]\033[0m ";
                break;
            case test_result::ERROR:
                aDebug() << "\033[33m[ERROR]\033[0m ";
                break;
            }

            aDebug() << testCase.name << " (" << testCase.duration.count() << " 1e-6s)";

            if (testCase.result != test_result::PASSED)
            {
                aDebug() << "    " << testCase.err_msg;
            }
        }

        void printSummary()
        {
            aDebug() << "=== Summary for " << name << " ===";
            aDebug() << "Total: " << testCases.size() << " tests";
            aDebug() << "Passed: \033[32m" << passedCount << "\033[0m";
            aDebug() << "Failed: \033[31m" << failedCount << "\033[0m";
            aDebug() << "Errors: \033[33m" << errorCount << "\033[0m";
            aDebug() << "====================";
        }

        int getTotalTests() const { return testCases.size(); }
        int getPassedCount() const { return passedCount; }
        int getFailedCount() const { return failedCount; }
        int getErrorCount() const { return errorCount; }
    };

    class assertion_exception : public std::exception
    {
    private:
        std::string message;
        std::string file;
        int line;

    public:
        assertion_exception(const std::string &msg, const std::string &f, int l)
            : message(msg), file(f), line(l) {}

        const char *what() const noexcept override
        {
            static std::string fullMsg;
            fullMsg = file + ":" + std::to_string(line) + " - " + message;
            return fullMsg.c_str();
        }
    };

    // 主测试运行器
    class test_runner
    {
    private:
        std::vector<test_group> testGroups;
        int totalPassed;
        int totalFailed;
        int totalErrors;

        test_runner() : totalPassed(0), totalFailed(0), totalErrors(0) {}

    public:
        static test_runner &getInstance()
        {
            static std::unique_ptr<test_runner>instance = nullptr;

            if (!instance)
            {
                instance.reset(new test_runner());
            }
            return *instance;
        }

        test_group &createTestGroup(const std::string &name)
        {
            testGroups.emplace_back(name);
            return testGroups.back();
        }

        void runAllTests()
        {
            aDebug() << "========== STARTING ALL TESTS ==========";

            for (auto &group : testGroups)
            {
                group.runAll();
                totalPassed += group.getPassedCount();
                totalFailed += group.getFailedCount();
                totalErrors += group.getErrorCount();
            }

            printFinalSummary();
        }

        void printFinalSummary()
        {
            aDebug() << "========== FINAL SUMMARY ==========";
            aDebug() << "Total Groups: " << testGroups.size() << "";
            aDebug() << "Total Tests: " << (totalPassed + totalFailed + totalErrors) << "";
            aDebug() << "Passed: \033[32m" << totalPassed << "\033[0m";
            aDebug() << "Failed: \033[31m" << totalFailed << "\033[0m";
            aDebug() << "Errors: \033[33m" << totalErrors << "\033[0m";

            if (totalFailed == 0 && totalErrors == 0)
            {
                aDebug() << "\033[32mALL TESTS PASSED!\033[0m";
            }
            else
            {
                aDebug() << "\033[31mSOME TESTS FAILED!\033[0m";
            }

            aDebug() << "===================================";
        }
    };

#define ASSERT_TRUE(condition)                                                                          \
    do                                                                                                  \
    {                                                                                                   \
        if (!(condition))                                                                               \
        {                                                                                               \
            throw assertion_exception("Assertion failed: " #condition " is false", __FILE__, __LINE__); \
        }                                                                                               \
    } while (0)

#define ASSERT_FALSE(condition)                                                                        \
    do                                                                                                 \
    {                                                                                                  \
        if (condition)                                                                                 \
        {                                                                                              \
            throw assertion_exception("Assertion failed: " #condition " is true", __FILE__, __LINE__); \
        }                                                                                              \
    } while (0)

#define ASSERT_EQUAL(expected, actual)                                     \
    do                                                                     \
    {                                                                      \
        auto exp = (expected);                                             \
        auto act = (actual);                                               \
        if (exp != act)                                                    \
        {                                                                  \
            std::stringstream ss;                                          \
            ss << "Assertion failed: Expected " << exp << ", got " << act; \
            throw assertion_exception(ss.str(), __FILE__, __LINE__);       \
        }                                                                  \
    } while (0)

#define ASSERT_NOT_EQUAL(notExpected, actual)                                       \
    do                                                                              \
    {                                                                               \
        auto notExp = (notExpected);                                                \
        auto act = (actual);                                                        \
        if (notExp == act)                                                          \
        {                                                                           \
            std::stringstream ss;                                                   \
            ss << "Assertion failed: Values should not be equal, both are " << act; \
            throw assertion_exception(ss.str(), __FILE__, __LINE__);                \
        }                                                                           \
    } while (0)

#define ASSERT_THROWS(statement, exceptionType)                                                              \
    do                                                                                                       \
    {                                                                                                        \
        bool threwCorrectException = false;                                                                  \
        try                                                                                                  \
        {                                                                                                    \
            statement;                                                                                       \
        }                                                                                                    \
        catch (const exceptionType &)                                                                        \
        {                                                                                                    \
            threwCorrectException = true;                                                                    \
        }                                                                                                    \
        catch (...)                                                                                          \
        {                                                                                                    \
            throw assertion_exception("Wrong exception type thrown", __FILE__, __LINE__);                    \
        }                                                                                                    \
        if (!threwCorrectException)                                                                          \
        {                                                                                                    \
            throw assertion_exception("Expected exception not thrown: " #exceptionType, __FILE__, __LINE__); \
        }                                                                                                    \
    } while (0)

#define ASSERT_NO_THROW(statement)                                                     \
    do                                                                                 \
    {                                                                                  \
        try                                                                            \
        {                                                                              \
            statement;                                                                 \
        }                                                                              \
        catch (const std::exception &e)                                                \
        {                                                                              \
            std::stringstream ss;                                                      \
            ss << "Unexpected exception thrown: " << e.what();                         \
            throw assertion_exception(ss.str(), __FILE__, __LINE__);                   \
        }                                                                              \
        catch (...)                                                                    \
        {                                                                              \
            throw assertion_exception("Unknown exception thrown", __FILE__, __LINE__); \
        }                                                                              \
    } while (0)

#define ASSERT_NULL(ptr)                                                          \
    do                                                                            \
    {                                                                             \
        if ((ptr) != nullptr)                                                     \
        {                                                                         \
            throw assertion_exception("Pointer is not null", __FILE__, __LINE__); \
        }                                                                         \
    } while (0)

#define ASSERT_NOT_NULL(ptr)                                                  \
    do                                                                        \
    {                                                                         \
        if ((ptr) == nullptr)                                                 \
        {                                                                     \
            throw assertion_exception("Pointer is null", __FILE__, __LINE__); \
        }                                                                     \
    } while (0)

// 测试注册宏
#define TEST_GROUP(groupName) \
    static auto &testGroup##groupName = test_runner::getInstance().createTestGroup(#groupName)

#define TEST(groupName, testName, description)                                                       \
    void test_##groupName##_##testName();                                                            \
    namespace                                                                                        \
    {                                                                                                \
        struct RegisterTest_##groupName##_##testName                                                 \
        {                                                                                            \
            RegisterTest_##groupName##_##testName()                                                  \
            {                                                                                        \
                testGroup##groupName.addTest(#testName, description, test_##groupName##_##testName); \
            }                                                                                        \
        } registerTest_##groupName##_##testName;                                                     \
    }                                                                                                \
    void test_##groupName##_##testName()
}