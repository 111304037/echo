#pragma once

#include "Frame/Example.h"
#include "Engine/Core.h"
#include "Engine/Camera/CameraAnimation.h"
/************************************************************************/
/*           VR��Ⱦģʽ��������                                            */
/************************************************************************/

namespace Examples
{
	class VRModeRoaming : public Example
	{
	public:
		VRModeRoaming();
		virtual ~VRModeRoaming();

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
		LORD::String	m_sceneName;  // ��������
		LORD::CameraAnimation* pCamAnim;
	};
}