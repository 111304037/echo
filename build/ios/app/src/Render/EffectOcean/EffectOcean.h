#pragma once

#include "Frame/Example.h"
#include "Engine/Core.h"
/************************************************************************/
/*           ��Ч����                                                   */
/************************************************************************/
namespace Examples
{
	class EffectOcean : public Example
	{
	public:
		EffectOcean();
		virtual ~EffectOcean();

		// ������Ϣ
		virtual const LORD::String getHelp();

		// ��ʼ��
		virtual bool initialize();

		// ����
		virtual void tick(LORD::ui32 elapsedTime);

		// ����
		virtual void destroy();

	private:
		LORD::Scene*	m_scene;	// ����
		LORD::Camera*	m_camera;   // �������
	};
}