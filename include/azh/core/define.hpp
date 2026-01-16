#pragma once

namespace azh::sdk::core
{
#define CLASS_TO_STR(class_name) (#class_name)
#define CLASS_WARNING_ON_VIRTUAL_FUNC_NOT_OVERRIDE() \
    utils::logger::getInstance()->warning("Please override virtual function : ", __FUNCTION__,".");

#define DECLARE_CLASS_NAME(class_name) \
    static const char* className() { return CLASS_TO_STR(class_name); }
#define DECLARE_CLASS_CONSTRUCT_DEFAULT(class_name) \
    class_name() \
    { \
        _init_(); \
    }
}