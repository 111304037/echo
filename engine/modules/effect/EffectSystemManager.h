#ifndef __ECHO_EFFECT_SYSTEM_MANAGER_H__
#define __ECHO_EFFECT_SYSTEM_MANAGER_H__

#include "Engine/Core.h"
#include "EffectHeader.h"
#include "EffectController.h"
#include "EffectLayer.h"
#include "engine/core/Util/Singleton.h"
#include "engine/core/Util/StringUtil.h"
#include "engine/core/render/render/Material.h"
#include "engine/core/render/render/RenderState.h"

namespace Echo
{
	class EffectSystemManager;
	class  EffectManualRender
	{
	public:
		virtual void render();
		virtual void beginRender();
	};

	/**
	 * ��Чϵͳ������ 2014-6-20
	 */
	class  EffectSystemManager
	{
		__DeclareSingleton(EffectSystemManager);

		friend class EffectDestroyEvent;
		typedef map<String, EffectSystem*>::type EffectSystemMap;
		typedef map<ui32, EffectSystem*>::type	 EffectSystemIDMap;
		typedef map<ui64, EffectRenderableID>::type RenderableMap;

	public:
		EffectSystemManager();
		~EffectSystemManager();

		// ��ʼ��������������
		void initBuffer();

		// ����
		void tick( i32 elapsedTime);

		EffectLayer* createLayer(EFFECT_LAYER_TYPE layerType);
		EffectLayer* cloneLayer(EffectLayer* layer);
		void destroyLayer(EffectLayer* layer);
		// for editor
		EFFECT_LAYER_TYPE getLayerType(const String& typeName);
		String getLayerTypeString(EFFECT_LAYER_TYPE type);

		EffectKeyFrame* createKeyFrame(void);
		EffectKeyFrame* cloneKeyFrame(EffectKeyFrame* keyFrame);
		void destroyKeyFrame(EffectKeyFrame* keyFrame);

		EffectParticleControllerSet* createParticleControllerSet(void);
		EffectParticleControllerSet* cloneParticleControllerSet(EffectParticleControllerSet* particleControllerSet);
		void destroyParticleControllerSet(EffectParticleControllerSet* particleControllerSet);

		EffectMaterial* createMaterial();
		EffectMaterial* cloneMaterial(EffectMaterial* material);
		void destroyMaterial(EffectMaterial* material);

		EffectController* createController(EFFECT_CONTROL_TYPE controlType);
		EffectController* cloneController(EffectController* controller);
		void destroyController(EffectController* controller);
		void getControllerTypes(StringArray& stringArry);
		// for editor
		EFFECT_CONTROL_TYPE getControllerType(const String& typeName);
		String getControllerTypeString(EFFECT_CONTROL_TYPE type);

		// for editor
		void replaceEffectSystemTemplate(const String& name, EffectSystem* system);
		EffectSystem* createEffectSystemTemplate(const String& name);
		EffectSystem* getEffectSystemTemplate(const String& templateName);
		void destoryEffectSystemTemplate(const String& templateName);
		void destroyAllEffectSystemTemplates(void);

		EffectSystemID createEffectSystem(const String& name, const String& templateName, bool isUIeffect = false);
		EffectSystem* createEffectSystem(const String& name);
		EffectSystemID getEffectSystem(const String& name);
		
		// ����ID��ȡ��Чϵͳ
		EffectSystem* getEffectSystem( EffectSystemID id);
		void destroyEffectSystem( EffectSystemID& effectSystemID);
		void destroyAllEffectSystems();
		void removeEffectSystemFromMap(EffectSystem* effectSystem);

		void createRenderableSet();
		void destroyRenderableSet();

		// ɾ��δʹ�õ�Renderable
		void destroyAllUsedRenderables();

		// ����������Ч�飬���ģʽ�����ʻ�ȡRenderable
		EffectRenderable* getRenderable(TextureRes* texture, EFFECT_RENDER_GROUP rg, EFFECT_BLEND_MODE bm, EffectMaterial* pEm  );
		void beginRender();
		void endRender(const std::vector<EffectRenderable*>* effectRenderables = NULL);
		void endRenderUI(const std::vector<EffectRenderable*>* effectRenderables = NULL);

		void renderDistortionLayer();

		// �Ƿ��пռ�Ť����Ч��
		bool isHaveDistortionLayer();

		// �ϴ�����
		void updateBufferData( const EffectRenderable& effectRenderable);

		void renderGLowEffectLayer();

		void setEnableRender( bool _val ){ mEnableRender = _val; }

		bool getEnableRender() const { return mEnableRender; }

		void setRenderUIEffectOnly(bool _val){ mRenderUIEffectOnly = _val; }

		bool getRenderUIEffectOnly() const { return mRenderUIEffectOnly; }

		// ������Чϵͳ��ʹ�õ������
		void setCamera( Camera* camera) { m_camera = camera; }

		void setUserCamera( Camera* camera) { m_userCamera = camera; }

		// ��ȡ��Ч��ǰʹ�õ������
		Camera* getCamera(EffectRenderable* pRenderable);

		// �����Ƿ���Ť��Ч��
		void setEnableDistortionRender( bool _val);

		// ��ȡ���㻺��
		GPUBuffer* getVertexBuffer() const { return m_vertexBuffer; }
		
		// ��ȡ��������
		GPUBuffer* getIndexBuffer() const { return m_indexBuffer; }

		// ������Ⱦ����
		void setRenderLevel( EFFECT_RENDER_LEVEL level) { m_renderlevelThreshhold = static_cast<EFFECT_RENDER_LEVEL>( ERL_Super-level);}

		// ������Ⱦ����ֵ
		void setRenderLevelThreshhold( EFFECT_RENDER_LEVEL level) { m_renderlevelThreshhold = level; } 

		// ��ȡ��Ⱦ����
		EFFECT_RENDER_LEVEL getRenderLevelThreshhold() const { return m_renderlevelThreshhold; }

		// ��ȡ���ӳ�
		EffectParticlePool* getParticlePool() { return m_particlePool; }

		void setWireFramePattern();

		void closeWireFramePattern();

		Material* getMtlUV2(){ return mMtlUV2; }

	private:
		// ����δʹ�õ�Renderables
		void enlargeUnUsedRenderables();

		void clearUsedRenderablesRenderData();

		void _renderRenderable(EffectRenderable* pRenderable, int uv2_MatIndex, int uv2_TextureIndex, int uv2_TextureIndex1, int uv2_blendModeIndex, int uv2_dissolutionIndex);

		RenderQueue* _getEffectRenderQueue();

	protected:
		EffectSystem* _createSystemImpl(const String& name);
		void _destroySystemImpl(EffectSystem* particleSystem);

	protected:
		Camera*							m_camera;					// ��ǰʹ�������
		Camera*							m_userCamera;					// ��ǰʹ�������
		EFFECT_RENDER_LEVEL				m_renderlevelThreshhold;	// ��Ⱦ����				
		bool							mEnableRender;
		bool							mRenderUIEffectOnly;
		RenderInput::VertexElementList	m_vertexElementList;		// �������ݸ�ʽ
		GPUBuffer*						m_vertexBuffer;				// ���㻺��
		GPUBuffer*						m_indexBuffer;				// ��������
		EffectSystemMap					mEffectSystems;				// ��Чϵͳ����
		EffectSystemIDMap				m_effectSystemIDs;			// �Ѵ�����Ч
		EffectSystemMap					mEffectSystemTemplates;
		RenderableMap					m_usedRenderables;			// ��ǰʹ���е�������Ч��Ⱦ��Ԫ
		RenderInput*					mEffectRenderInput;
		RenderInput*					mEffectUV2RenderInput;
		int								mSPFogParamIndex;
		int								mSPHeightFogParamIndex;
		int								mSPMatIndex;
		int								mSPTextureIndex;
		const SamplerState*				mEffectSamplerState;		// ����״̬
		BlendState*						mEffectAlphaLighting;
		BlendState*						mEffectHighLighting;
		BlendState*						mEffectWireFrameLighting;
		DepthStencilState*				mEffectDepthState;
		DepthStencilState*				mEffectUnWriteDepthState;
		RasterizerState*				mEffectRasterizerState;
		bool							mRenderalbeSetCreated;
		EffectManualRender				mEffectManualRender;
		Material*						mMtlDistortion;
		TextureRes*						mTextureNoise;
		Material*						mMtlUV2;					// ��2��UV����Ч
		bool							m_bRenderGlowOver;
		EffectParticlePool*				m_particlePool;				// ���ӳ�
		bool							bUseWireFrameMode;
		RenderQueue*					mEffectRenderQueue;

	public:
		HashNode<EffectRenderable>**	mEffectRenderablePtr;
		HashNode<EffectMaterial>**		mEffectMaterialPtr;
	};
}
#endif
