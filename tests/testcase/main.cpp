#include "azh/sdk/type/string.hpp"
#include "azh/sdk/utils/logger.hpp"
#include "azh/sdk/test/unit_test.hpp"

int main() {
    aDebug()<< "Custom C++ Unit Test Framework\n";
    aDebug()<< "===============================\n\n";
    
    azh::sdk::test::test_runner::getInstance().runAllTests();
    
    return 0;
}