#pragma once

#include "base.hpp"

#include <azh/sdk/type/map.hpp>

namespace azh::sdk::core
{
    static azh::sdk::type::map<void*,int> s_g_ptr_count;

    template<class T>
    class ptr : public _base
    {
        T* m_ptr;

    public:
        DECLARE_CLASS_NAME(ptr)
        DECLARE_CLASS_CONSTRUCT_BY_FUNC(ptr, _init_)
        DELETE_CLASS_COPY_CONSTRUCT(ptr)
        DECLARE_CLASS_DECONSTRUCT_BY_FUNC(ptr, _deinit_)

        const utils::unique_time_stamp& timestamp() const { return m_time_stamp_private; }
        
        virtual std::string toString() const
        {
            return "[ class ptr ]";
        }

        bool empty() const
        {
            return m_ptr==nullptr;
        }

        void reset(T* _ptr)
        {
            if(empty())
            {
                m_ptr=_ptr;
                return;
            }

            int count;

            if(s_g_ptr_count.contains(m_ptr))
            {
                count=s_g_ptr_count.value(m_ptr);
                if(count>1)
                {
                    s_g_ptr_count.insert(m_ptr,count-1);
                }
                else
                {
                    s_g_ptr_count.erase(m_ptr);
                }
            }

            count=0;
            
            if(s_g_ptr_count.contains(_ptr))
            {
                count=s_g_ptr_count.value(_ptr);   
            }
            s_g_ptr_count.insert(_ptr,count+1);

            m_ptr=_ptr;
        }

        T& operator*()
        {
            if(empty())
            {
                throw std::invalid_argument("ptr is empty");
            }

            return *m_ptr;
        }

    private:
        void _init_()
        {
            // utils::logger::getInstance()->debug("_base init");
            m_ptr=nullptr;
        }

        void _deinit_()
        {
            // utils::logger::getInstance()->debug("_base deinit");
            if(!empty() && s_g_ptr_count.contains(m_ptr))
            {
                int count=s_g_ptr_count.value(m_ptr);
                if(count>1)
                {
                    s_g_ptr_count.insert(m_ptr,count-1);
                }
                else
                {
                    delete m_ptr;
                }
            }
        }
    };
}