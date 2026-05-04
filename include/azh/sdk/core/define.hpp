#pragma once

namespace azh::sdk::core
{
#define CLASS_TO_STR(class_name) (#class_name)
/* warning for no override */
#define CLASS_WARNING_ON_VIRTUAL_FUNC_NOT_OVERRIDE() \
    utils::logger::getInstance()->warning("Please override virtual function : ", __FUNCTION__, ".");
/* declare className func */
#define DECLARE_CLASS_NAME(class_name) \
    virtual const char *className() { return CLASS_TO_STR(class_name); }
/* declare constract func */
#define DECLARE_CLASS_CONSTRUCT_DEFAULT(class_name) \
    class_name()                                    \
    {                                               \
    }

#define DECLARE_CLASS_CONSTRUCT_BY_FUNC(class_name, init_func) \
    class_name()                                               \
    {                                                          \
        init_func();                                           \
    }

#define DECLARE_CLASS_DECONSTRUCT_DEFAULT(class_name) \
    ~class_name()                                     \
    {                                                 \
    }

#define DECLARE_CLASS_DECONSTRUCT_BY_FUNC(class_name, deinit_func) \
    virtual ~class_name()                                          \
    {                                                              \
        deinit_func();                                             \
    }

#define DELETE_CLASS_COPY_CONSTRUCT(class_name) \
    class_name(const class_name &c) = delete;
}