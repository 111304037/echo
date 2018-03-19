#pragma once

#include "engine/core/render/render/RenderQueue.h"
#include "engine/core/render/render/Color.h"
#include "Engine/core/Camera/Camera.h"
#include "Engine/core/Camera/CameraShadow.h"
#include "Node.h"
#include "Engine/core/Render/TextureRes.h"
#include "engine/core/Util/Singleton.h"

namespace Echo
{
	/**
	 * ����������
	 */
	class SceneManager
	{	
		__DeclareSingleton(SceneManager);

	public:
		SceneManager();
		virtual ~SceneManager();

		// ��ʼ��
		bool initialize();

		// ����
		void destroy();

	public:
		// ��ȡ�������
		Camera* getMainCamera() const { return m_pMainCamera; }

		// �����������
		void setMainCamera(Camera* camera) { m_pMainCamera = camera; }

		// ��ȡ2D�����
		Camera* get2DCamera() const { return m_p2DCamera; }

		// ��ȡ���������
		Camera* getGUICamera() const { return m_pGUICamera; }

		// ���ý��������
		void setGUICamera(Camera* pCamera) { m_pGUICamera = pCamera; }

	public:
		void update( float elapsedTime);

		// ��ȡ��Ӱ�����
		CameraShadow& getShadowCamera() { EchoAssert( m_pShadowCamera);  return *m_pShadowCamera; }

		// ��Ӱ��Ч����
		float getShadowDistance(){ return m_shadowDistance; }
		void setShadowDistance(float dis){ m_shadowDistance = dis; }

	protected:
		Camera*			    m_pMainCamera;			// �������
		Camera*				m_p2DCamera;			// 2D�����
		Camera*				m_pGUICamera;			// ���������
		CameraShadow*		m_pShadowCamera;		// ��Ӱͼ�����		
		bool				m_bNeedUpate;
		float				m_shadowDistance;		// ��Ӱ��Ч����

		Node*				m_invisibleRoot;		// invisible root node
	};
}
