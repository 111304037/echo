#pragma once

#include <engine/core/util/Array.hpp>
#include "interface/ShaderProgram.h"
#include "interface/Renderable.h"
#include "interface/mesh/Mesh.h"
#include "GLESShader.h"
#include "GLESRenderable.h"

namespace Echo
{
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
		GLuint			m_hProgram;
	};

}
