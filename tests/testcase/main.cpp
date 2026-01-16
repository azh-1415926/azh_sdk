#include "azh/type/string.hpp"
#include "azh/utils/logger.hpp"
#include "azh/test/unit_test.hpp"

int main() {
    aDebug()<< "Custom C++ Unit Test Framework\n";
    aDebug()<< "===============================\n\n";
    
    azh::sdk::test::test_runner::getInstance().runAllTests();
    
    return 0;
}