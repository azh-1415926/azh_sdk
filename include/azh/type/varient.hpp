#pragma once

#include <utility>
#include <stdexcept>
#include "string.hpp"

namespace azh::sdk::type
{
    class varient;

    namespace detail
    {
        class varient_base
        {
        public:
            virtual ~varient_base() = default;
            virtual std::unique_ptr<varient_base> clone() const = 0;
            virtual const std::type_info& getType() const noexcept = 0;
            virtual std::string toString() const = 0;
        };

        template<typename T>
        class varient_holder : public varient_base
        {
            typedef T _base_type;
            _base_type m_value_private;
            
        public:
            varient_holder(const T& value) : m_value_private(value) {}
            varient_holder(T&& value) : m_value_private(std::move(value)) {}
            
            std::unique_ptr<varient_base> clone() const override
            {
                return std::make_unique<varient_holder<T>>(m_value_private);
            }
            
            const std::type_info& getType() const noexcept override
            {
                return typeid(T);
            }
            
            std::string toString() const override
            {
                return to_string(m_value_private);
            }
            
            T& getValue() { return m_value_private; }
            const T& getValue() const { return m_value_private; }
        };
    }

    class varient
    {
        std::unique_ptr<detail::varient_base> m_data_private;
        
    public:
        varient() = default;
        
        varient(const varient& v)
        {
            if (v.m_data_private)
            {
                m_data_private = v.m_data_private->clone();
            }
        }
        
        varient(varient&& v) noexcept = default;
        
        template<typename T>
        varient(const T& value)
            : m_data_private(std::make_unique<detail::varient_holder<std::decay_t<T>>>(value)) {}
        
        varient(const char* str)
            : m_data_private(std::make_unique<detail::varient_holder<std::string>>(std::string(str))) {}
        
        varient(char* str)
            : m_data_private(std::make_unique<detail::varient_holder<std::string>>(std::string(str))) {}
        
        ~varient() = default;
        
        varient& operator=(const varient& v)
        {
            if (this != &v)
            {
                if (v.m_data_private)
                {
                    m_data_private = v.m_data_private->clone();
                }
                else
                {
                    m_data_private.reset();
                }
            }
            return *this;
        }
        
        varient& operator=(varient&& v) noexcept = default;
        
        template<typename T>
        varient& operator=(const T& value)
        {
            m_data_private = std::make_unique<detail::varient_holder<std::decay_t<T>>>(value);
            return *this;
        }
        
        varient& operator=(const char* str)
        {
            m_data_private = std::make_unique<detail::varient_holder<std::string>>(std::string(str));
            return *this;
        }
        
        varient& operator=(char* str)
        {
            m_data_private = std::make_unique<detail::varient_holder<std::string>>(std::string(str));
            return *this;
        }
        
        bool empty() const noexcept
        {
            return !m_data_private;
        }
        
        bool hasValue() const noexcept
        {
            return !empty();
        }
        
        const std::type_info& getType() const
        {
            if (empty())
            {
                throw std::runtime_error("varient is empty");
            }
            return m_data_private->getType();
        }
        
        template<typename T>
        T& getValue()
        {
            if (empty())
            {
                m_data_private = std::make_unique<detail::varient_holder<T>>(T());
            }
            
            auto* holder = dynamic_cast<detail::varient_holder<T>*>(m_data_private.get());
            if (!holder)
            {
                throw std::bad_cast();
            }
            
            return holder->getValue();
        }
        
        template<typename T>
        const T& getValue() const
        {
            if (empty())
            {
                throw std::runtime_error("varient is empty");
            }
            
            auto* holder = dynamic_cast<const detail::varient_holder<T>*>(m_data_private.get());
            if (!holder)
            {
                throw std::bad_cast();
            }
            
            return holder->getValue();
        }
        
        template<typename T>
        T* getIf() noexcept
        {
            if (empty()) return nullptr;
            auto* holder = dynamic_cast<detail::varient_holder<T>*>(m_data_private.get());
            return holder ? &holder->getValue() : nullptr;
        }
        
        template<typename T>
        const T* getIf() const noexcept
        {
            if (empty()) return nullptr;
            auto* holder = dynamic_cast<const detail::varient_holder<T>*>(m_data_private.get());
            return holder ? &holder->getValue() : nullptr;
        }
        
        template<typename T>
        bool holdsAlternative() const noexcept
        {
            if (empty()) return false;
            return m_data_private->getType() == typeid(T);
        }
        
        void reset() noexcept
        {
            m_data_private.reset();
        }
        
        void clear() noexcept
        {
            reset();
        }
        
        std::string toString() const
        {
            if (empty())
            {
                return "varient(empty)";
            }
            
            std::ostringstream oss;
            oss << "varient(" << m_data_private->toString() << ")";
            return oss.str();
        }
        
        operator std::string() const
        {
            return toString();
        }
        
        friend std::ostream& operator<<(std::ostream& os, const varient& v)
        {
            os << v.toString();
            return os;
        }
        
        bool operator==(const varient& v) const
        {
            if (empty() && v.empty()) return true;
            if (empty() != v.empty()) return false;
            if (getType() != v.getType()) return false;
            
            return m_data_private->toString() == v.m_data_private->toString();
        }
        
        bool operator!=(const varient& v) const
        {
            return !(*this == v);
        }
    };
}