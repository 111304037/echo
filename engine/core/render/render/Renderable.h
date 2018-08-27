#pragma once

#include <engine/core/util/Array.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "RenderState.h"
#include "ShaderProgramRes.h"

namespace Echo
{
	static const size_t MAX_TEXTURE_SAMPLER = 8;
	static const int	SHADER_TEXTURE_SLOT0= 0;	// Ĭ��ʹ�������
	static const int	SHADER_TEXTURE_SLOT1= 1;	// Ĭ��ʹ�������
	static const int	SHADER_TEXTURE_SLOT2= 2;	// Ĭ��ʹ�������
	static const int	SHADER_TEXTURE_SLOT3= 3;	// Ĭ��ʹ�������
	static const int	SHADER_TEXTURE_SLOT4= 4;	// Ĭ��ʹ�������
	static const int	SHADER_TEXTURE_SLOT5= 5;	// Ĭ��ʹ�������
	static const int	SHADER_TEXTURE_SLOT6= 6;	// Ĭ��ʹ�������
	static const int	SHADER_TEXTURE_SLOT7= 7;	// Ĭ��ʹ�������

	/**
	 * ��С����Ⱦ���װ
	 */
	class Render;
	class Mesh;
	class Material;
	class Renderable
	{
		friend class Renderer;
		typedef array<TextureSampler, MAX_TEXTURE_SAMPLER> MaxTextureArray;
	public:
		// ��ɫ������
		struct ShaderParam
		{
			ui32			physicsIndex;
			ShaderParamType stype;
			const void*		pData;
			ui32			ParamsLength;  // shader constance register num.
		};

	public:
		// ��ȡΨһ��ʶ��
		ui32 getIdentifier() const { return m_identifier; }

		// ��������
		static Renderable* create(Mesh* mesh, Material* matInst, Render* node);

		// release
		void release();

		// set mesh
		Mesh* getMesh() { return m_mesh; }
		void setMesh(Mesh* mesh);

		// ��ʼ����
		void beginShaderParams(size_t paramNum);

		// ����������
		void endShaderParams();

		// ���ò���
		void setShaderParam(size_t physicsIndex, ShaderParamType type, const void* param, size_t num=1);
		
		// �޸Ĳ���
		void modifyShaderParam(ui32 physics, ShaderParamType type, void* param, size_t num=1);

		// ��������
		void setTexture( ui32 stage, Texture* texture, const SamplerState* state);
		void setTexture( ui32 stage, ui32 globalTexture, const SamplerState* state);

		// ��ȡ����
		Texture* getTexture(ui32 stage) const { return m_textures[stage].m_texture; }

		// ִ����Ⱦ
		virtual void render();
		
		// ������Ⱦ����
		void setRenderStage(const String& renderStage) { m_renderStage = renderStage; }

		// ��ȡ��Ⱦ����
		const String& getRenderStage(void){ return m_renderStage; }

		// �ύ����Ⱦ����
		void submitToRenderQueue();

		// �Ƿ����Լ�����Ⱦ״̬
		bool isHaveRenderState( ) { return m_bRenderState; }

		// ������Ⱦ״̬
		void setBlendState( BlendState* state );

		// ���ù�դ��״̬
		void setRasterizerState( RasterizerState* state );

		// �������ģ��״̬
		void setDepthStencilState(DepthStencilState* state);

		// ��ȡ��Ⱦ״̬
		BlendState*	getBlendState() { return m_pBlendState; }

		// ��ȡ��դ��״̬
		RasterizerState* getRasterizerState() { return m_pRasterizerState; }

		// ��ȡ���ģ��״̬
		DepthStencilState* getDepthStencilState() { return m_pDepthStencil; }

		// set node
		void setNode( Render* node) { m_node = node; }

		// get node
		Render* getNode() { return m_node; }

		// get shader
		ShaderProgram* getShader();

	protected:
		// �����������������
		void bindTextures();

		// �󶨲���
		void bindShaderParams();

		// ����Ⱦ״̬
		void bindRenderState();

		// link shader and program
		virtual void link() = 0;

	protected:
		Renderable( const String& renderStage, ShaderProgramRes* shader, int identifier);
		virtual ~Renderable();

	public:
		ui32									m_identifier;
		Render*									m_node;
		String									m_renderStage;
		ShaderProgramResPtr						m_shaderProgram;		// material
		Mesh*									m_mesh;
		MaxTextureArray							m_textures;				// now only one texture sampler.
		vector<ShaderParam>::type				m_shaderParams;			// third shader params
		size_t									m_SParamWriteIndex;

		bool									m_bRenderState;
		BlendState*								m_pBlendState;
		RasterizerState*						m_pRasterizerState;
		DepthStencilState*						m_pDepthStencil;
	};
	typedef ui32 RenderableID;
}
