#include "azh/type/string.hpp"
#include "azh/utils/logger.hpp"
#include "azh/test/unit_test.hpp"

int main() {
    aDebug()<< "Test for azh::sdk::type\n";
    aDebug()<< "===============================\n\n";
    
    azh::sdk::test::test_runner::getInstance().runAllTests();
    
    return 0;
}