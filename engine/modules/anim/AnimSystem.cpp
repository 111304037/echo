#include "engine/core/Math/Quaternion.h"
#include "engine/core/Math/Vector4.h"
#include "engine/core/Util/Singleton.h"
#include "Bone.h"
#include "Animation.h"
#include "AnimBlender.h"
#include "AnimState.h"
#include "AnimManager.h"
#include "SkeletonManager.h"
#include "AnimSystem.h"
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <rapidxml/rapidxml_utils.hpp>

using namespace rapidxml;

namespace Echo
{
	// ���캯��
	AnimSystem::AnimSystem(const String& name, bool isManual)
		: Resource(name, RT_AnimationSystem, isManual)
		, m_skeleton(NULL)
		, m_blender(NULL)
		, m_blenderExternal(NULL)
		, m_isPause(false)
	{
	}

	// ��������
	AnimSystem::~AnimSystem()
	{
		if (m_blender)
			AnimManager::instance()->destroyAnimBlenders(&m_blender, 1);

		// ��ն���
		for (size_t i = 0; i < m_animations.size(); i++)
			AnimManager::instance()->releaseResource(m_animations[i]);
		m_animations.clear();

		EchoSafeDeleteContainer(m_animStates, AnimState);

		if (m_skeleton)
		{
			SkeletonManager::instance()->releaseResource(m_skeleton);
		}
	}

	// ����
	void AnimSystem::tick(i32 elapsedTime, bool isSync)
	{
		if (!m_isPause)
		{
			if (getBlender())
				getBlender()->frameMove(elapsedTime, isSync);
		}
	}

	// ���Ŷ���
	void AnimSystem::playAnimation(AnimState* animState, ui32 blendTime)
	{
		if (getBlender() && animState && animState->getLength() > 0)
		{
			getBlender()->blend(animState, blendTime);
		}
	}

	// ��ȡָ��������ǰλ��
	Vector3 AnimSystem::getBonePos(const Bone* bone)
	{
		return getBlender()->getAnimPosition(bone->getId());
	}

	// �����ڴ�ռ�ô�С
	size_t AnimSystem::calculateSize() const
	{
		// δʵ��
		return 0;
	}

	// ��Դ����
	bool AnimSystem::loadImpl()
	{
		try
		{
			MemoryReader memReader(mName.c_str());
			xml_document<> doc;
			doc.parse<0>(memReader.getData<char*>());

			// ��ȡ���ڵ�
			xml_node<>* rootnode = doc.first_node("animsys"); EchoAssert(rootnode);
			if (rootnode)
			{
				// �Ǽ���Ϣ
				xml_attribute<>* skeletonAtt = rootnode->first_attribute("skeleton");
				if (skeletonAtt)
				{
					String skeleton = skeletonAtt->value();
					if (!skeleton.empty())
					{
						setSkeleton(skeleton.c_str());
					}
				}

				// ������Ϣ
				xml_attribute<>* animationsAtt = rootnode->first_attribute("anims");
				if (animationsAtt)
				{
					StringArray anims = StringUtil::Split(animationsAtt->value(), ";");
					for (size_t i = 0; i < anims.size(); i++)
					{
						addAnimation(anims[i].c_str());
					}
				}
			}
		}
		catch (...)
		{
			EchoLogError("Load AnimSystem [%s] failed", mName.c_str());
			return false;
		}

		return true;
	}

	// ��Դж��
	void AnimSystem::unloadImpl()
	{

	}

	// ���ùǼ�
	void AnimSystem::setSkeleton(const char* skeleton, AnimBlender* externalBlender)
	{
		if (m_skeleton)
		{
			if (m_skeleton->getName() != skeleton)
			{
				// ��չǼ�
				if (m_skeleton)
					SkeletonManager::instance()->releaseResource(m_skeleton);

				// ��ն���
				cleanAnimations();

				// ���ùǼ�
				m_skeleton = SkeletonManager::instance()->createSkeleton(skeleton);
				m_skeleton->prepareLoad();
			}
		}
		else
		{
			// ��ն���
			cleanAnimations();

			// ���ùǼ�
			m_skeleton = SkeletonManager::instance()->createSkeleton(skeleton);
			m_skeleton->prepareLoad();
		}

		// ���������
		if (m_blender)
			AnimManager::instance()->destroyAnimBlenders(&m_blender, 1);
		m_blender = AnimManager::instance()->createAnimBlender(m_skeleton);

		m_blenderExternal = externalBlender;

		// ƥ����
		checkup();
	}

	// ��Ӷ���
	void AnimSystem::addAnimation(const char* animPath)
	{
		if (!isAnimationExist(animPath))
		{
			Animation* animation = AnimManager::instance()->createAnim(animPath);
			if (animation)
			{
				if (!animation->prepareLoad())
				{
					AnimManager::instance()->releaseResource(animation);
					return;
				}

				m_animations.push_back(animation);
			}
		}
		else
		{
			EchoLogWarning("the animation [%s] is existed in animsystem [%s]", animPath, mName.c_str());
		}

		// ƥ����
		checkup();
	}

	// ���Ϸ���
	bool AnimSystem::checkup()
	{
		if (m_skeleton)
		{
			for (size_t i = 0; i < m_animations.size(); i++)
			{
				if (m_skeleton->getBoneCount() != m_animations[i]->getBoneCount())
				{
					EchoLogError("skeleton [%s] can not used by animation [%s]", m_skeleton->getName().c_str(), m_animations[i]->getName().c_str());
					return false;
				}
			}
		}

		return true;
	}

	// �ж϶����Ƿ����
	bool AnimSystem::isAnimationExist(const char* fileName)
	{
		for (size_t i = 0; i < m_animations.size(); i++)
		{
			if (m_animations[i]->getName() == fileName)
				return true;
		}

		return false;
	}

	// ��ն���
	void AnimSystem::cleanAnimations()
	{
		// ��ն���
		for (size_t i = 0; i < m_animations.size(); i++)
			AnimManager::instance()->releaseResource(m_animations[i]);

		m_animations.clear();
	}

	// ��ȡ����״̬
	AnimState* AnimSystem::getAnimState(const char* animName)
	{
		// �Ƿ��Ѵ���
		for (size_t i = 0; i < m_animStates.size(); i++)
		{
			if (m_animStates[i]->getAnim()->getName() == animName)
			{
				m_animStates[i]->reset();
				return m_animStates[i];
			}
		}

		// �´���
		Animation* animation = dynamic_cast<Animation*>(AnimManager::instance()->getByName(animName));
		if (animation)
		{
			AnimState* animState = EchoNew(AnimState(animation));
			animState->reset();
			m_animStates.push_back(animState);

			return animState;
		}
		else
		{
			EchoLogError("The animation resource [%d] has not been loaded", animName);

			return NULL;
		}
	}

	AnimState* AnimSystem::getAnimStateNotCreate(const char* animName)
	{
		for (size_t i = 0; i < m_animStates.size(); i++)
		{
			if (m_animStates[i]->getAnim()->getName() == animName)
			{
				return m_animStates[i];
			}
		}

		return NULL;
	}

	// ����
	void AnimSystem::save(const char* filePath)
	{
#ifdef ECHO_PLATFORM_WINDOWS
		xml_document<> doc;
		xml_node<>* xmlnode = doc.allocate_node(node_pi, doc.allocate_string("xml version='1.0' encoding='utf-8'"));
		xml_node<>* rootnode = doc.allocate_node(node_element, "animsys");

		doc.append_node(xmlnode);
		doc.append_node(rootnode);

		// �洢�Ǽ�����
		if (m_skeleton)
		{
			xml_attribute<>* propertySkeleton = doc.allocate_attribute("skeleton", m_skeleton->getName().c_str());
			rootnode->append_attribute(propertySkeleton);
		}

		// ���涯����Ϣ
		String animations;
		if (!m_animations.empty())
		{
			// ��֯�����洢����
			for (size_t i = 0; i < m_animations.size(); i++)
				animations += m_animations[i]->getName() + ";";

			// ��ӵ�������
			xml_attribute<>* propertyanims = doc.allocate_attribute("anims", animations.c_str());
			rootnode->append_attribute(propertyanims);
		}

		std::ofstream out(filePath);
		out << doc;
		out.close();
#endif
	}

	__ImplementSingleton(AnimSystemManager);

	// ��������ϵͳ
	AnimSystem* AnimSystemManager::createAnimSystem(const String& name, bool isManual)
	{
		AnimSystem* animSystem = ECHO_DOWN_CAST<AnimSystem*>(ResourceManager::createResource(name, isManual)); EchoAssert(animSystem);

		return animSystem;
	}

	// ʵ��
	Resource* AnimSystemManager::createImpl(const String& name, bool isManual)
	{
		return EchoNew(AnimSystem(name, isManual));
	}
}