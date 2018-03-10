#pragma once

#include "engine/core/render/render/RenderQueue.h"
#include "engine/core/render/render/Color.h"
#include "Engine/core/Camera/Camera.h"
#include "Engine/core/Camera/CameraShadow.h"
#include "Node.h"
#include "scene.h"
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
		// ���ݲ���ģ�������Ⱦ����
		RenderQueue* addRenderQueue(const String& materialTemplate, const char* stage, const String& macros, bool isAlphaTest);

		// ������Ⱦ�������ƻ�ȡ��Ⱦ����
		RenderQueue* getRenderQueue(const String& strQueueName) const;

		// ����������ȡ��Ⱦ����
		RenderQueue* getRenderQueueByIndex(ui32 nIndex) const;

		// �������ƻ�ȡ��������
		ui8 getRenderQueueIndex(const String& strQueueName) const;

		// ��ȡ��������
		size_t getRenderQueueCount() { return m_RenderQueueGroup.size(); }

		// ��Ⱦ����ִ��(����endQueue)
		void execRenderQueue(const String& startQueue, const String& endQueue, bool includeEndQueue=false);

		// �����Ⱦ����(����endQueue)
		void clearRenderQueue(const String& startQueue, const String& endQueue, bool includeEndQueue = false);

	public:
		void update( float elapsedTime);

		void render();

		// ��ȡ��Ӱ�����
		CameraShadow& getShadowCamera() { EchoAssert( m_pShadowCamera);  return *m_pShadowCamera; }

		// ��Ӱ��Ч����
		float getShadowDistance(){ return m_shadowDistance; }
		void setShadowDistance(float dis){ m_shadowDistance = dis; }

	protected:
		void initRenderQueueGroupManual();
		void initRenderQueueGroup();
		void destroyRenderQueueGroup();

	protected:
		bool				m_isSceneVisible; 
		Camera*			    m_pMainCamera;			// �������
		Camera*				m_p2DCamera;			// 2D�����
		Camera*				m_pGUICamera;			// ���������
		CameraShadow*		m_pShadowCamera;		// ��Ӱͼ�����		
		Vector4				m_LightParam[3];		// ����Դ����,����Դ��ɫ����������ɫ
		Vector4				m_LightParamForActor[3];
		Vector4				m_LightParamForSceneObject[3];
		Vector4				m_FogParam[3][3];			// param, color, mainpos
		Vector4				m_UIFogParam[3];		// UI Fog Param
		Vector4				m_heightFogParam;		// x:start height, y:deep
		Vector4				m_UIheightFogParam;		// x:start height, y:deep
		bool				m_bNeedUpate;
		RenderQueueGroup	m_RenderQueueGroup;
		bool				m_bIsRenderActorHigh;
		bool				m_isUseXRay;
		bool				m_bisComputeActorFog;
		std::map<ui32, Node*>	m_sceneNodes;
		Vector3				m_mainActorPos;
		Vector3				m_lookatPos;
		float				m_shadowDistance;		// ��Ӱ��Ч����
	};
}
