#pragma once

#include "azh/sdk/utils/logger.hpp"
#include "azh/sdk/utils/timer.hpp"
#include "azh/sdk/utils/unique_time_stamp.hpp"
#include "define.hpp"

namespace azh::sdk::core
{
    class _base
    {
        utils::unique_time_stamp m_time_stamp_private;

    public:
        DECLARE_CLASS_NAME(_base)
        DECLARE_CLASS_CONSTRUCT_BY_FUNC(_base, _init_)
        DELETE_CLASS_COPY_CONSTRUCT(_base)
        DECLARE_CLASS_DECONSTRUCT_BY_FUNC(_base, _deinit_)

        const utils::unique_time_stamp& timestamp() const { return m_time_stamp_private; }
        
        virtual std::string toString() const
        {
            CLASS_WARNING_ON_VIRTUAL_FUNC_NOT_OVERRIDE()
            return "[ class _base ]";
        }

    private:
        void _init_()
        {
            // utils::logger::getInstance()->debug("_base init");
        }

        void _deinit_()
        {
            // utils::logger::getInstance()->debug("_base deinit");
        }
    };
}