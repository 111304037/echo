#pragma once

#include "Frame/Example.h"

namespace Examples
{
	/**
	 * ���߳�ʾ��
	 */
	class MultiThread : public Example
	{
		MultiThread(const MultiThread& rhs);
		MultiThread& operator=(const MultiThread& rhs);

	public: 
		MultiThread();
		virtual ~MultiThread();

		// ������Ϣ
		virtual const LORD::String getHelp() override;

		// ��ʼ��
		virtual bool initialize() override;

		// ����
		virtual void tick(LORD::ui32 elapsedTime) override;

		// ����
		virtual void destroy() override;

		// �����¼�������FXAA�Ŀ���&&�رգ�
		virtual void keyboardProc(LORD::ui32 keyChar, bool isKeyDown) override;

		// ����¼�
		virtual void mouseLBProc(LORD::i32 xpos, LORD::i32 ypos) override;

		// ����¼�
		virtual void mouseMoveProc(LORD::i32 xpos, LORD::i32 ypos) override;

		// �����¼�
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
		LORD::Vector3 m_cameraPosition; 
		LORD::Vector3 m_cameraLookAt; 
		LORD::Vector3 m_cameraForward; 
		float		  m_cameraRadius; 
		float		  m_horizonAngle;  // ˮƽ��ת�Ƕ�(˳ʱ��)
		float		  m_verticleAngle; // ��ֱ��ת�Ƕ�(˳ʱ��)

		LORD::i32 m_xPos; 

		LORD::DebugDisplayItem* m_Sphere; 
		LORD::SceneNode*		m_SphereNode; 

		bool m_autoSpin; 
	};
}