#pragma once

#include "azh/sdk/utils/logger.hpp"

namespace azh
{
    class demo
    {
    public:
        demo();
        demo(const demo &d);
        ~demo();

        demo operator=(const demo &d);

    private:
        void _init();
    };
}