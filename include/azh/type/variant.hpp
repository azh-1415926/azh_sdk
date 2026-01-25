#pragma once

#include <utility>
#include <stdexcept>
#include <memory>
#include "string.hpp"

namespace azh::sdk::type
{
    class variant;

    namespace detail
    {
        class variant_base
        {
        public:
            virtual ~variant_base() = default;
            virtual std::unique_ptr<variant_base> clone() const = 0;
            virtual const std::type_info& getType() const noexcept = 0;
            virtual std::string toString() const = 0;
        };

        template<typename T>
        class variant_holder : public variant_base
        {
            typedef T _base_type;
            _base_type m_value_private;
            
        public:
            variant_holder(const T& value) : m_value_private(value) {}
            variant_holder(T&& value) : m_value_private(std::move(value)) {}
            
            std::unique_ptr<variant_base> clone() const override
            {
                return std::make_unique<variant_holder<T>>(m_value_private);
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

    class variant
    {
        std::unique_ptr<detail::variant_base> m_data_private;
        
    public:
        variant() = default;
        
        variant(const variant& v)
        {
            if (v.m_data_private)
            {
                m_data_private = v.m_data_private->clone();
            }
        }
        
        variant(variant&& v) noexcept = default;
        
        template<typename T>
        variant(const T& value)
            : m_data_private(std::make_unique<detail::variant_holder<std::decay_t<T>>>(value)) {}
        
        variant(const char* str)
            : m_data_private(std::make_unique<detail::variant_holder<std::string>>(std::string(str))) {}
        
        variant(char* str)
            : m_data_private(std::make_unique<detail::variant_holder<std::string>>(std::string(str))) {}
        
        ~variant() = default;
        
        variant& operator=(const variant& v)
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
        
        variant& operator=(variant&& v) noexcept = default;
        
        template<typename T>
        variant& operator=(const T& value)
        {
            m_data_private = std::make_unique<detail::variant_holder<std::decay_t<T>>>(value);
            return *this;
        }
        
        variant& operator=(const char* str)
        {
            m_data_private = std::make_unique<detail::variant_holder<std::string>>(std::string(str));
            return *this;
        }
        
        variant& operator=(char* str)
        {
            m_data_private = std::make_unique<detail::variant_holder<std::string>>(std::string(str));
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
                throw std::runtime_error("variant is empty");
            }
            return m_data_private->getType();
        }
        
        template<typename T>
        T& getValue()
        {
            if (empty())
            {
                m_data_private = std::make_unique<detail::variant_holder<T>>(T());
            }
            
            auto* holder = dynamic_cast<detail::variant_holder<T>*>(m_data_private.get());
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
                throw std::runtime_error("variant is empty");
            }
            
            auto* holder = dynamic_cast<const detail::variant_holder<T>*>(m_data_private.get());
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
            auto* holder = dynamic_cast<detail::variant_holder<T>*>(m_data_private.get());
            return holder ? &holder->getValue() : nullptr;
        }
        
        template<typename T>
        const T* getIf() const noexcept
        {
            if (empty()) return nullptr;
            auto* holder = dynamic_cast<const detail::variant_holder<T>*>(m_data_private.get());
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
                return "variant(empty)";
            }
            
            std::ostringstream oss;
            oss << "variant(" << m_data_private->toString() << ")";
            return oss.str();
        }
        
        operator std::string() const
        {
            return toString();
        }
        
        friend std::ostream& operator<<(std::ostream& os, const variant& v)
        {
            os << v.toString();
            return os;
        }
        
        bool operator==(const variant& v) const
        {
            if (empty() && v.empty()) return true;
            if (empty() != v.empty()) return false;
            if (getType() != v.getType()) return false;
            
            return m_data_private->toString() == v.m_data_private->toString();
        }
        
        bool operator!=(const variant& v) const
        {
            return !(*this == v);
        }
    };
}