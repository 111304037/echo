#pragma once

#include "Skeleton.h"
#include "Engine/core/Resource/ResourceManager.h"

namespace Echo
{
	class Bone;
	class AnimBlender;
	class DataStream;
	class AnimState;
	class Animation;
	class AnimSystem : public Resource
	{
	public:
		AnimSystem(const String& name, bool isManual);
		virtual~AnimSystem();

		// ����
		void tick(i32 elapsedTime, bool isSync=false);

		// ���Ŷ���
		void playAnimation(AnimState* animState, ui32 blendTime = 200);

		// ֹͣ���ж���
		void stopAllAnimation() {}

		// ��ȡָ��������ǰλ��
		Vector3 getBonePos(const Bone* bone);

		// �Ƿ���ͣ
		bool isPause() const { return m_isPause; }

		// ��ͣ
		void setPause(bool isPause) { m_isPause = isPause; }

	public:
		// ���ùǼ�
		void setSkeleton(const char* skeleton, AnimBlender* externalBlender = NULL);

		// ��ȡ�Ǽ�
		Skeleton* getSkeleton() { return m_skeleton; }

		// ��ȡ�����
		AnimBlender* getBlender() { return m_blenderExternal ? m_blenderExternal : m_blender; }

		// ��Ӷ���
		void addAnimation(const char* animation);

		// ��ȡ����״̬
		AnimState* getAnimState(const char* animName);

		// ��ȡ����״̬��������
		AnimState* getAnimStateNotCreate(const char* animName);

		// ��ȡ����
		const vector<Animation*>::type& getAnimations() { return m_animations; }

		// �ж϶����Ƿ����
		bool isAnimationExist(const char* fileName);

		// ��ն���
		void cleanAnimations();

		// ����
		void save(const char* filePath);

	protected:
		// ��Դ��ؽӿ�ʵ��
		virtual size_t calculateSize() const;
		virtual bool prepareImpl(Echo::DataStream* stream) { return true; }
		virtual bool loadImpl();
		virtual void unloadImpl();

		// ���Ϸ���
		bool checkup();

	protected:
		Skeleton*					m_skeleton;			// ��Ӧ�Ǽ�
		vector<Animation*>::type	m_animations;		// �ɲ��Ŷ���
		vector<AnimState*>::type	m_animStates;		// ����״̬
		AnimBlender*				m_blender;			// ���������
		AnimBlender*				m_blenderExternal;	// �ⲿ���������
		bool						m_isPause;			// ��ͣ
	};


	/**
	* ����ϵͳ����
	*/
	class AnimSystemManager : public ResourceManager
	{
		__DeclareSingleton(AnimSystemManager);

	public:
		AnimSystemManager() 
		{
			__ConstructSingleton;
		}
		virtual ~AnimSystemManager() 
		{
			__DestructSingleton;
		}

		// ��������ϵͳ
		AnimSystem* createAnimSystem(const String& name, bool isManual = false);

	protected:
		// ʵ��
		virtual Resource* createImpl(const String& name, bool isManual);
	};
}