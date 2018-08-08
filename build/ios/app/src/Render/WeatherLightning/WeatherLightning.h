#pragma once

#include "Frame/Example.h"
#include "Engine/Core.h"
/************************************************************************/
/*           ����                                                       */
/************************************************************************/
namespace Examples
{
	class WeatherLightning : public Example
	{
	public:
		WeatherLightning();
		virtual ~WeatherLightning();

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
		LORD::String	sceneName;  // ��������
	};
}