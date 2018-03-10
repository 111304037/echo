#ifndef __ECHO_ANIMMANAGER_H__
#define __ECHO_ANIMMANAGER_H__

#include "engine/core/Resource/ResourceManager.h"

namespace Echo
{
	class Animation;
	class AnimBlender;
	class Skeleton;


	class AnimManager : public ResourceManager
	{
		__DeclareSingleton(AnimManager);

	public:
		AnimManager();
		virtual ~AnimManager();

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
		std::map<ui32, AnimBlender*>	 m_animBlenders;		// �������
	};
}

#endif
