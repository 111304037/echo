#pragma once

#include "GLES2Shader.h"
#include "GLES2Renderable.h"
#include "render/ShaderProgram.h"
#include "render/Renderable.h"
#include "engine/core/render/mesh/Mesh.h"
#include <engine/core/util/Array.hpp>

namespace Echo
{
	class GLES2ShaderProgramGPUProxy;
	class GLES2ShaderProgram : public ShaderProgram
	{
	public:
		typedef array<GLint, VS_MAX> AttribLocationArray;
	public:
		GLES2ShaderProgram( ShaderProgramRes* material);
		~GLES2ShaderProgram();

		// ����shader
		virtual bool attachShader(Shader* pShader) override;

		// �Ƴ�shader
		virtual Shader*	detachShader(Shader::ShaderType type) override;

		// ����shader
		virtual bool linkShaders() override;

		// Ӧ�ñ���
		virtual void bindUniforms() override;

		// ��
		virtual void bind() override;

		// ���
		virtual void unbind() override;

		// �󶨼���������
		virtual void bindRenderable(Renderable* renderable) override;

		// ��ȡ������������λ��
		i32 getAtrribLocation(VertexSemantic vertexSemantic) override;

		// ��ȡ������
		const String& getMaterialName();
	
	private:
		GLES2Renderable*	m_preRenderable;				// ������������
		AttribLocationArray	m_attribLocationMapping;		// ��������λ��ӳ��

#ifdef ECHO_RENDER_THREAD
		GLES2ShaderProgramGPUProxy* m_gpu_proxy;
#else
		GLuint			m_hProgram;
#endif
	};

}
