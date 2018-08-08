//
//
//
//
//
//
//

#pragma once

#include "Frame/Example.h"

namespace LORD
{
	class PostProcessRenderStage;
}

namespace Examples
{
	class FXAA : public Example
	{
		FXAA(const FXAA& rhs); 
		FXAA& operator=(const FXAA& rhs); 

	public: 
		FXAA(); 
		virtual ~FXAA(); 

	public: 
		// ������Ϣ
		virtual const LORD::String getHelp();

		// ��ʼ��
		virtual bool initialize();

		// ����
		virtual void tick(LORD::ui32 elapsedTime);

		// ����
		virtual void destroy();

		// �����¼�������FXAA�Ŀ���&&�رգ�
		virtual void keyboardProc(LORD::ui32 keyChar, bool isKeyDown) override;

		// ����¼�
		virtual void mouseLBProc(LORD::i32 xpos, LORD::i32 ypos) override;
		virtual void mouseMoveProc(LORD::i32 xpos, LORD::i32 ypos) override;
		virtual void mouseWhellProc(LORD::i32 parma) override; 

	private: 
		// 
		bool initMainCameraSettings(); 

		//
		bool initSphereWireFrame(); 

		//
		bool initGeometry(); 

		//
		bool rotateCamera(float deltaX, float deltaY); 

	private:
		LORD::Camera* m_camera;        // �������

		LORD::Vector3	 m_lastCameraPosition; 
		LORD::Vector3	 m_lastCameraDirection; 
		float			 m_lastNearClip; 
		float			 m_lastFarClip; 
		float			 m_lastFov; 

		LORD::Vector3 m_cameraPosition; 
		LORD::Vector3 m_cameraLookAt; 

		float		  m_horizonAngle;  // ˮƽ��ת�Ƕ�(˳ʱ��)
		float		  m_verticleAngle; // ��ֱ��ת�Ƕ�(˳ʱ��)

		LORD::i32 m_xPos; 

		LORD::PostProcessRenderStage* m_postRenderStage; 

		LORD::DebugDisplayItem* m_Sphere; 
		LORD::SceneNode*		m_SphereNode; 

		bool m_autoSpin; 
	};
}