//#ifndef __ECHO_GLES2SHADERPROGRAM_H__
//#define __ECHO_GLES2SHADERPROGRAM_H__
//
//#include "GLES2Shader.h"
//#include "render/ShaderProgram.h"
//#include "render/Renderable.h"
//#include <engine/core/util/Array.hpp>
//
//namespace Echo
//{
//	class GLES2ShaderProgramGPUProxy;
//	class GLES2ShaderProgram : public ShaderProgram
//	{
//	public:
//		typedef array<GLint, RenderInput::VS_MAX> AttribLocationArray;
//	public:
//		GLES2ShaderProgram( ShaderProgramRes* material);
//		~GLES2ShaderProgram();
//
//		// ����shader
//		virtual bool attachShader(Shader* pShader) override;
//
//		// �Ƴ�shader
//		virtual Shader*	detachShader(Shader::ShaderType type) override;
//
//		// ����shader
//		virtual bool linkShaders() override;
//
//		// Ӧ�ñ���
//		virtual void bindUniforms() override;
//
//		// ��
//		virtual void bind() override;
//
//		// ���
//		virtual void unbind() override;
//
//		// �󶨼���������
//		virtual void bindRenderInput(RenderInput* renderInput) override;
//
//		// ��ȡ������������λ��
//		i32 getAtrribLocation(RenderInput::VertexSemantic vertexSemantic) override;
//
//		// ��ȡ������
//		const String& getMaterialName();
//	
//	private:
//		RenderInput*		m_preRenderInput;				// ������������
//		AttribLocationArray	m_attribLocationMapping;		// ��������λ��ӳ��
//
//#ifdef ECHO_RENDER_THREAD
//		GLES2ShaderProgramGPUProxy* m_gpu_proxy;
//#else
//		GLuint			m_hProgram;
//#endif
//	};
//
//}
//
//#endif
