#pragma once

#include <algorithm>
#include <typeinfo>
#include "engine/core/Util/LogManager.h"
#include "engine/core/Memory/MemAllocDef.h"

namespace Echo
{
	/**s
	 * Anyʵ��,�ο�boost, http://www.boost.org
	 */
    class any
    {
		// ��Ԫ����
		template<typename ValueType> friend ValueType * any_cast(any*);

    public:
		any() : content(0) {}    
        template<typename ValueType> any(const ValueType & value) : content(new holder<ValueType>(value)){}
        any(const any & other) : content(other.content ? other.content->clone() : 0){}
        ~any(){ EchoSafeDelete(content, placeholder);}

    public:
		// swap
        any & swap(any & rhs)
        {
            std::swap(content, rhs.content);
            return *this;
        }

		// ��������� "="
        template<typename ValueType> any & operator=(const ValueType & rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

		// ���������"="
        any & operator=(any rhs)
        {
            rhs.swap(*this);
            return *this;
        }

    public:
		// �Ƿ�Ϊ��
        bool empty() const
        {
            return !content;
        }

		// ��ȡ������Ϣ
        const std::type_info & type() const
        {
            return content ? content->type() : typeid(void);
        }

    private: 
        class placeholder
        {
        public:
            virtual ~placeholder(){}

			// ��������
            virtual const std::type_info & type() const = 0;

			// ���ƺ���
            virtual placeholder * clone() const = 0;
        };

        template<typename ValueType>
		class holder : public placeholder
        {
        public:
            holder(const ValueType & value) : held(value){}

			// ����ֵ����
            virtual const std::type_info & type() const
            {
                return typeid(ValueType);
            }

			// ����
            virtual placeholder * clone() const
            {
                return new holder(held);
            }

		private:
			// ��������� "="
			holder& operator=(const holder &);

        public:
            ValueType held;
        };

	private:
        placeholder * content;			// �洢����
    };

	// ת��ʧ��
    class bad_any_cast : public std::bad_cast
    {
    public:
        virtual const char * what() const throw()
        {
            return "Echo::bad_any_cast: failed conversion using Echo::any_cast";
        }
    };

    template<typename ValueType>
    ValueType* any_cast(any * operand)
    {
        return operand && operand->type() == typeid(ValueType) ? &static_cast<any::holder<ValueType> *>(operand->content)->held : 0;
    }

    template<typename ValueType>
    inline const ValueType * any_cast(const any * operand)
    {
        return any_cast<ValueType>(const_cast<any *>(operand));
    }

    template<typename ValueType>
    ValueType any_cast(any& operand)
    {
        //typedef typename remove_reference<ValueType>::type nonref;

        ValueType* result = any_cast<ValueType>(&operand);
        if(!result)
		{
			EchoLogError( "any_cast failed,so terrible!");
		}

        return *result;
    }
}
