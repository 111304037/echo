#pragma once

#include "Render/RenderInput.h"

namespace Echo
{
	/**
	 * ��Ⱦ����
	 */
	class ECHO_GLES2RENDER_API GLES2RenderInput: public RenderInput
	{
	public:
		GLES2RenderInput( ShaderProgram* pProgram);
		~GLES2RenderInput();

		// �󶨼���������
		virtual void bind( RenderInput* pre);

		// ��󼸺�������
		void unbind();

	private:
		// ������������
		void buildVertexAttributes();

		// ���㶥��������
		virtual bool buildVertStreamDeclaration(StreamUnit* stream);

		// ���ɶ�����Hashֵ(BKDR Hash)
		virtual void generateVertexStreamHash();

	private:
		GLuint			m_vao;							// ��������(OpenGLES 3.0)
	};
}
