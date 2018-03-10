#pragma once

#include "engine/core/Base/EchoDef.h"

namespace Echo
{
	/**
	 * �����
	 */
	template<typename T>
	class ObjectPool
	{
	public:
		// ���캯��
		ObjectPool()
		{
			Init();
		}

		// ���캯��
		ObjectPool(ui32 capacity)
		{
			Init();

			Reserve(capacity);
		}

		// ��������
		~ObjectPool()
		{
			Clear();
		}

		// �½�
		T* New()
		{
			if (m_minFreeIdx < m_capacity)
			{
				int idx = m_minFreeIdx;
				m_minFreeIdx = -1;

				for (ui32 i = idx + 1; i < m_capacity; i++)
				{
					if (!m_flags[i])
					{
						m_minFreeIdx = i;
						break;
					}
				}

				m_flags[idx] = true;
				m_size++;
				return &m_objs[idx];
			}

			return NULL;
		}

		// ����
		void Delete(T* obj)
		{
			ui32 idx = obj - m_objs;
			EchoAssert(idx < m_capacity);

			m_flags[idx] = false;
			obj = NULL;
			m_size--;

			if (idx < m_minFreeIdx)
				m_minFreeIdx = idx;
		}

		// ��������
		void Reserve(ui32 capacity)
		{
			EchoAssert(capacity < 65535);

			Clear();

			m_capacity = capacity;
			m_size = 0;
			m_minFreeIdx = 0;

			m_objs = EchoNewArray(T, m_capacity);
			m_flags = EchoAlloc(bool, m_capacity);
			memset(m_flags, false, sizeof(bool)*m_capacity);
		}

		// ���
		void Clear()
		{
			EchoAssert(m_size == 0 && m_minFreeIdx == 0);

			EchoSafeDeleteArray(m_objs, T, m_capacity);
			EchoSafeFree(m_flags);
		}

		// ��ȡ����
		ui32 getCapacity() const { return m_capacity; }

		// ��ȡ��С
		ui32 getSize() const { return m_size; }

	private:
		// ��ʼ��
		void Init()
		{
			m_capacity = 0;
			m_size = 0;
			m_minFreeIdx = 0;
			m_objs = NULL;
			m_flags = NULL;
		}

	private:
		T*		m_objs;			// �ѷ������
		bool*	m_flags;		// �����ʹ��״̬
		ui32	m_capacity;		// ��������
		ui32	m_size;			// ��ǰʹ������
		ui32	m_minFreeIdx;	// ��ǰ���ж����±�(��С)
	};
}