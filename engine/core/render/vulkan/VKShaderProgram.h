#ifndef __LORD_GLES2SHADERPROGRAM_H__
#define __LORD_GLES2SHADERPROGRAM_H__

#include "GLES2Shader.h"
#include "Render/ShaderProgram.h"
#include "Render/Renderable.h"
#include <Foundation/Util/Array.hpp>

namespace LORD
{
	class GLES2ShaderProgramGPUProxy;

	/**
	 * ��ɫ������
	 */
	class LORD_GLES2RENDER_API GLES2ShaderProgram: public ShaderProgram
	{
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
	
	private:
		RenderInput*		m_preRenderInput;				// ������������
		GLES2ShaderProgramGPUProxy* m_gpu_proxy;
	};

}

#endif
