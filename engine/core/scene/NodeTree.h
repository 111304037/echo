#pragma once

#include "Node.h"
#include "engine/core/Util/Singleton.h"
#include "Engine/core/Camera/Camera.h"
#include "Engine/core/Camera/CameraShadow.h"

namespace Echo
{
	class NodeTree
	{	
		__DeclareSingleton(NodeTree);

	public:
		NodeTree();
		virtual ~NodeTree();

		// ��ʼ��
		bool init();

		// ����
		void destroy();

	public:
		// ��ȡ�������
		Camera* get3dCamera() const { return m_3dCamera; }

		// �����������
		void setMainCamera(Camera* camera) { m_3dCamera = camera; }

		// ��ȡ2D�����
		Camera* get2DCamera() const { return m_2dCamera; }

		// ��ȡ���������
		Camera* getGUICamera() const { return m_uiCamera; }

		// ���ý��������
		void setGUICamera(Camera* pCamera) { m_uiCamera = pCamera; }

	public:
		void update( float elapsedTime);

		// ��ȡ��Ӱ�����
		CameraShadow& getShadowCamera() { EchoAssert( m_shadowCamera);  return *m_shadowCamera; }

	protected:
		Camera*			    m_3dCamera;			// �������
		Camera*				m_2dCamera;			// 2D�����
		Camera*				m_uiCamera;			// ���������
		CameraShadow*		m_shadowCamera;		// ��Ӱͼ�����		
		Node*				m_invisibleRoot;	// invisible root node
	};
}
