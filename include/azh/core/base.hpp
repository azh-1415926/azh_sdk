#pragma once

#include "azh/utils/logger.hpp"
#include "define.hpp"

namespace azh::sdk::core
{
    class _base
    {
    public:
        DECLARE_CLASS_NAME(_base)
        DECLARE_CLASS_CONSTRUCT_DEFAULT(_base)

        ~_base()
        {
        }

        virtual void test()
        {
            utils::logger::getInstance()->debug("test for base");
        }

    protected:
        virtual void _init_()
        {
            CLASS_WARNING_ON_VIRTUAL_FUNC_NOT_OVERRIDE()
            
            // __PRETTY_FUNCTION__;
        }

    private:
    };

    class _demo : public _base
    {
    public:
        DECLARE_CLASS_NAME(_demo)
        DECLARE_CLASS_CONSTRUCT_DEFAULT(_demo)

        ~_demo()
        {
        }

        virtual void test() override
        {
            utils::logger::getInstance()->debug("test for demo");
        }

    protected:
    };
}