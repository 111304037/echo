#pragma once

#include "RenderState.h"
#include "ShaderProgram.h"
#include "RenderInput.h"
#include "Renderable.h"
#include <utility>

namespace Echo
{
	/**
	 * ����
	 */
	class Renderer;
	class Material : public PtrMonitor<Material>
	{
		typedef map<String, const SamplerState*>::type SamplerStateMap;
		typedef vector<std::pair<String, String> >::type SeparatedSamplerArray;
		typedef vector<String>::type TextureSamplerStateArray;
	public:
		Material();
		~Material();

		// ��ȡ����
		const String& getName() { return m_name; }

		// �ͷ�����״̬
		void free();

		// load and parse by file
		bool loadFromFile(const String& filename, const String& macros);
		bool loadFromContent(const char* content, const String& macros);
		bool loadShaderFrom(void* node, const String& macros);
		bool loadBlendState(void* pNode);
		bool loadMacro(void * pNode);
		bool loadRasterizerState(void* pNode);
		bool loadDepthStencilState(void* pNode);
		
		//bool loadSamplerState(void* pNode, int stage);
		bool loadSamplerState_Ext( void* pNode );
		bool loadTexture_Ext( void* pNode );

		// create manual
		void createBlendState(BlendState::BlendDesc& desc);
		void createDepthState(DepthStencilState::DepthStencilDesc& desc);
		void createRasterizerState(RasterizerState::RasterizerDesc& desc);

		// ������ɫ��
		bool createShaderProgram( const String& vsContent, const String& psContent);

		// ��ȡ���״̬
		BlendState* getBlendState() const { return m_pBlendState; }

		// ��ȡ���ģ��״̬
		DepthStencilState* getDepthState() const { return m_pDepthState; }

		// ��ȡ��դ��״̬
		RasterizerState* getRasterizerState() const { return m_pRasterizerState; }

		// ��ȡ����״̬
		const SamplerState* getSamplerState(int stage) const;

		// ��ȡ��ɫ��
		ShaderProgram* getShaderProgram() const { return m_pShaderProgram; }

		// ��shader
		void activeShader();

		// ���ö���
		void setQueue(RenderQueue* queue) { m_queue = queue; }

		bool			hasMacro		(const char* const macro) const;

		// ��ȡ���ʿ�ѡ�궨���б�
		static StringArray getEnabledMacros(const String& matFileName, bool withEnabled = false);


		struct DefaultUniform
		{
			i32 count;
			ShaderParamType type;
			ui32 sizeInByte;
			void* value;

			~DefaultUniform();
		};
		bool					loadDefaultUniform(void* pNode);
		const DefaultUniform*	getDefaultUniformValue(const String& name);
		void*					createDefaultUniformValue(const String& strType, const i32 count, const String& strValue, ui32& outSize, ShaderParamType& outType);

	private:
		// ��ȡ����״̬
		const SamplerState* getSamplerStateByTexStage(int stage);

		// �����ݼ���
		bool loadFromContent(char* content, const String& macros);

	private:
		String				m_name;
		Shader::ShaderDesc	m_shaderDesc;							// ����ʹ�õĺ궨��
		RenderQueue*		m_queue;								// ��ǰ������������Ⱦ����
		BlendState*			m_pBlendState;
		DepthStencilState*	m_pDepthState;
		RasterizerState*	m_pRasterizerState;
		const SamplerState*	m_pSamplerState[MAX_TEXTURE_SAMPLER];
		SamplerStateMap		m_mapSamplerState;
		TextureSamplerStateArray	m_arrTexSamplerState;
		ShaderProgram*		m_pShaderProgram;
		typedef map<String, DefaultUniform*>::type MapDefaultUniforms;
		MapDefaultUniforms	m_defaultUniforms;
	};
}
