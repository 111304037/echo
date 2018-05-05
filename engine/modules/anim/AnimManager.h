#pragma once

#include "engine/core/Resource/ResourceManager.h"

namespace Echo
{
	class Animation;
	class AnimBlender;
	class Skeleton;
	class AnimManager : public ResourceManager
	{
	public:
		virtual ~AnimManager();

		// instance
		static AnimManager* instance();

		// ��������
		Animation* createAnim(const String& name, bool isManual=false);

		// �������������
		AnimBlender* createAnimBlender(Skeleton* pSkel);

		// ����ID��ȡ���������
		AnimBlender* getAnimBlender(ui32 id);

		// ���ٶ��������
		void destroyAnimBlenders(AnimBlender** blenders, int num);

	protected:
		Resource*	createImpl(const String& name, bool isManual);

	private:
		AnimManager();
		
	private:
		std::map<ui32, AnimBlender*>	 m_animBlenders;		// �������
	};
}
