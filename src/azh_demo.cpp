#include "azh/azh_demo.h"

#define AZH_VAR_TO_STR(a) (#a)

azh::demo::demo()
{
    azh::sdk::utils::logger::getInstance()->debug("class ",AZH_VAR_TO_STR(demo)," construct.");
    _init();
}

azh::demo::demo(const demo &d)
{
    azh::sdk::utils::logger::getInstance()->debug("class ",AZH_VAR_TO_STR(demo)," copy_construct.");
}

azh::demo::~demo()
{
    azh::sdk::utils::logger::getInstance()->debug("class ",AZH_VAR_TO_STR(demo)," deconstruct.");
}

azh::demo azh::demo::operator=(const demo &d)
{
    return demo();
}

void azh::demo::_init()
{
    azh::sdk::utils::logger::getInstance()->debug("class ",AZH_VAR_TO_STR(demo)," init.");
}
