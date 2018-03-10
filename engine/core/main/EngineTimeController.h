#pragma once

#include "engine/core/Memory/MemManager.h"

namespace Echo
{
	/**
	 * ʱ�������
	 */
	class EngineTimeController
	{
	public:
		enum SlowDownType
		{
			ET_SOUND = 0x00000001,
			ET_UI = 0x00000002,
			ET_SCENE = 0x00000004
		};
		// ֡
		struct Key
		{
			float	m_time;		// ʱ��
			float	m_speed;	// ����
		};
		typedef vector<Key>::type Keys;

	public:
		// ��ȡʵ��
		static EngineTimeController* instance();

		// ����
		static void destroy();

		// ��ȡ��ǰʱ�䱶��
		float getSpeed(int t = ET_UI | ET_SOUND | ET_SCENE);

		// ��������
		void setKeys( const char* keys);

		void setExclusive(ui32 flag);

		// ����(��)
		void update( float elapsed);

		// ���
		void clear();

	private:
		EngineTimeController();
		~EngineTimeController() {}

	private:
		float	m_elapsed;		// ����ʱ��
		Keys	m_keys;			// ֡��
		ui32	m_keyIdx;		// ��ǰ֡
		float	m_speed;		// ��ǰ����
		int		m_type;
	};
}