#ifndef __ECHO_GLES2SHADERPROGRAM_H__
#define __ECHO_GLES2SHADERPROGRAM_H__

#include "GLES2Shader.h"
#include "Render/ShaderProgram.h"
#include "Render/Renderable.h"
#include <engine/core/Util/Array.hpp>

namespace Echo
{
	class GLES2ShaderProgramGPUProxy;

	/**
	 * ��ɫ������
	 */
	class ECHO_GLES2RENDER_API GLES2ShaderProgram: public ShaderProgram
	{
	public:
		typedef array<GLint, RenderInput::VS_MAX> AttribLocationArray;
	public:
		GLES2ShaderProgram( Material* material);
		~GLES2ShaderProgram();

		// ����shader
		bool attachShader(Shader* pShader);

		// �Ƴ�shader
		Shader*	detachShader(Shader::ShaderType type);

		// ����shader
		bool linkShaders();

		// Ӧ�ñ���
		virtual void bindUniforms();

		// ��
		void bind();

		// ���
		void unbind();

		// �󶨼���������
		virtual void bindRenderInput(RenderInput* renderInput);

		// ��ȡ������������λ��
		i32 getAtrribLocation(RenderInput::VertexSemantic vertexSemantic) override;

		// ��ȡ������
		const String& getMaterialName();
	
	private:
		RenderInput*		m_preRenderInput;				// ������������
		AttribLocationArray	m_attribLocationMapping;		// ��������λ��ӳ��

#ifdef ECHO_RENDER_THREAD
		GLES2ShaderProgramGPUProxy* m_gpu_proxy;
#else
		GLuint			m_hProgram;
#endif
	};

}

#endif
