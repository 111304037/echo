#ifndef __ECHO_EFFECT_RENDER__
#define __ECHO_EFFECT_RENDER__

#include "EffectHeader.h"
#include "engine/core/render/render/RenderInput.h"
#include "engine/core/render/render/Texture.h"
#include "engine/core/render/TextureRes.h"
#include "Engine/core/Camera/Camera.h"
#include "engine/core/Util/PtrMonitor.h"

namespace Echo
{
	/**
	 * ��Ч��Ⱦ��Ԫ
	 */
	class EffectRenderable : public PtrMonitor<EffectRenderable>
	{
		friend class EffectMaterial;

	public:
		EffectRenderable();
		~EffectRenderable();

		// ����
		void tick( ui32 elapsedTime);

		void createVertexPool(size_t verticesNum);
		void destroyVertexPool();
		void createIndexPool(size_t indicesNum);
		void destroyIndexPool();

		void setMaterialParam(TextureRes* texture, EFFECT_RENDER_GROUP group, EFFECT_BLEND_MODE blendmode, EffectMaterial* pEm = 0);
		void refreshMaterialParam(EffectMaterial* pEm);
		TextureRes* getTexture() const { return m_Texture; }

		// ��ȡ���������
		const TextureSampler& getTextureSampler() { return m_texSampler; }

		TextureRes* getTexture1() const { return m_Texture1; }
		const TextureSampler& getTextureSampler1() { return m_texSampler1; }

		// ��ȡ���ģʽ
		EFFECT_BLEND_MODE getBlendMode() const { return m_BlendMode; }
		EFFECT_RENDER_GROUP getRenderGroup() const { return m_RenderGroup; }

		Vector3* getUV2BlendMode();

		// ��ȡ�Ƿ�д���
		bool isTestDepth() const { return m_isTestDepth; }
		void setTestDepth(bool _val) { m_isTestDepth = _val; }

		// ��ʼ����������
		void clearGeometryData();

		bool getLockPtr(size_t vertexNum, size_t indexNum, ui16& vertexBegin, EffectVertexFmt*& vertexAddr, ui16*& indexAddr);

		// ��������
		void reversePtr(size_t vertexNum, size_t indexNum);

		ui16   getVertexNum() { return static_cast<ui16>(m_vertexBufferData.size()); }
		ui16   getIndexNum() { return static_cast<ui16>(m_indexBufferData.size()); }

		// �Ƿ�ռ�Ť��
		bool isDistortion() const;

		inline void setDistortion( bool _val ){ m_distortion = _val;}

		// �Ƿ�����2��UV
		bool isUseUV2() const { return m_bUseUV2; }

		void setUseUV2(bool _val) { m_bUseUV2 = _val; }

		bool isGlow() const { return m_GlowEnable; }

		void setGlow(bool _val) { m_GlowEnable = _val; }

		bool isUiEffect() const { return m_isUiEffect; }

		void setUiEffect(bool _val) { m_isUiEffect = _val; }

		const Vector4& getDistortionUVScaleOffset() const;

		const float& getDistortionAlphaThreshold() const;

		bool getOnyDistortionLayer() const;

		TextureRes* getDistortionTexturePtr() const;

		const float getDistortionBlendWeight() const;

		const Vector2 getCenterUV() const;

		const EFFECT_DISTORTION_TYPE getDistortionType() const;

		float getPassedU();
		float getPassedV();

		// ��ȡ��������
		const vector<EffectVertexFmt>::type& getVertexBufferData() const { return m_vertexBufferData; }

		// ��ȡ��������
		const vector<ui16>::type& getIndexBufferData() const { return m_indexBufferData; }

		// ��ȡ����ʱ��
		const ui32 getSinceCreateTime(){ return m_timeSinceCreate; }

		// ��������ʱ��
		const void updateSinceCreateTime(ui32 value){ m_timeSinceCreate = value; }

		// ��ȡ����ʱ��
		ui32 getIdleTime() const { return m_idleTime; }

		Vector3* getDissolutionParameter(){return m_DissolutionParameter;}

	protected:
		static const size_t  s_VertexSize;
		typedef ui32 EffectMaterialID;

		vector<EffectVertexFmt>::type m_vertexBufferData;			// ��������
		vector<ui16>::type			  m_indexBufferData;			// ��������

		TextureRes*			m_Texture;
		TextureSampler		m_texSampler;
		EFFECT_RENDER_GROUP m_RenderGroup;
		EFFECT_BLEND_MODE   m_BlendMode;
		bool				m_distortion;
		mutable Vector4		m_distortionUVScaleOffset;
		mutable float		m_distortionAlphaThreshold;
		String				m_distortionTexName;
		TextureRes*			m_distortionTexPtr;
		bool				m_onlyDistortionLayer;
		float				m_distortionBlendWeight;
		Vector2				m_centerUV;
		EFFECT_DISTORTION_TYPE	m_distortionType;

		ui32				m_idleTime;								// ����ʱ��
		bool				m_bUseUV2;								// 2�ףգ�
		TextureRes*			m_Texture1;
		TextureSampler		m_texSampler1;
		int					m_UV2rgbBlendModel;
		int					m_UV2aBlendModel;
		bool				m_GlowEnable;
		bool				m_isUiEffect;
		bool				m_isTestDepth;
		Vector3*			m_DissolutionParameter;

	private:
		ui32 m_timeSinceCreate;	//�Ӵ�����ʼ����������ȥ��ʱ��(����)
	};
	typedef ui32 EffectRenderableID;
}

#endif