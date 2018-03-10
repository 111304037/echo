#include "engine/core/Math/Matrix4.h"
#include "Engine/modules/Anim/Animation.h"
#include "Engine/modules/Anim/AnimBlender.h"
#include "Engine/modules/Anim/AnimManager.h"

namespace Echo
{
	__ImplementSingleton(AnimManager);

	// ���캯��
	AnimManager::AnimManager()
	{
		__ConstructSingleton;
	}

	// ��������
	AnimManager::~AnimManager()
	{
		__DestructSingleton;
		// subclasses should unregister with resource group manager
	}

	Animation* AnimManager::createAnim(const String& name, bool isManual)
	{
		Animation* pAnimation = (Animation*)ResourceManager::createResource(name, isManual);
		return pAnimation;
	}

	Resource* AnimManager::createImpl(const String& name, bool isManual)
	{
		return EchoNew(Animation(name, isManual));
	}

	// �������������
	AnimBlender* AnimManager::createAnimBlender(Skeleton* pSkel)
	{
		AnimBlender* blender = EchoNew(AnimBlender(pSkel));
		m_animBlenders[blender->getIdentifier()] = blender;

		return blender;
	}

	// ����ID��ȡ���������
	AnimBlender* AnimManager::getAnimBlender(ui32 id)
	{
		std::map<ui32, AnimBlender*>::iterator it = m_animBlenders.find(id);
		if (it != m_animBlenders.end())
			return it->second;

		return NULL;
	}

	// ���ٶ��������
	void AnimManager::destroyAnimBlenders(AnimBlender** blenders, int num)
	{
		for (int i = 0; i < num; i++)
		{
			AnimBlender*& blender = blenders[i];
			if (blender)
			{
				std::map<ui32, AnimBlender*>::iterator it = m_animBlenders.find(blender->getIdentifier());
				EchoAssert(it != m_animBlenders.end());
				m_animBlenders.erase(it);

				EchoSafeDelete(blender, AnimBlender);
			}
		}
	}
}