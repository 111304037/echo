#pragma once

#include "Frame/Example.h"
#include "Engine/Core.h"

namespace LORD
{
	class ActorManager;
}

namespace Examples
{
	class MultiThreadSkeleton : public Example
	{
	public:
		MultiThreadSkeleton();
		virtual ~MultiThreadSkeleton();

		virtual const LORD::String getHelp();

		virtual bool initialize();

		virtual void tick(LORD::ui32 elapsedTime);

		virtual void destroy();

	private:
		LORD::Scene*	m_scene;	// ����
		LORD::Camera*	m_camera;   // �������
		//LORD::ActorManager* m_actorMgr;
	};
}