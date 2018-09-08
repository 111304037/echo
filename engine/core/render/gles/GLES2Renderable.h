#pragma once

#include "interface/Renderable.h"
#include "interface/mesh/Mesh.h"

namespace Echo
{
	class GLES2Renderable : public Renderable
	{
	public:
		// vertex stream bind state(for multi stream)
		enum BindState
		{
			BS_NORMAL = 0,
			BS_BEGIN = 1 << 0,
			BS_END = 1 << 1,
		};

		// vertex declaration
		struct VertexDeclaration
		{
			i32		m_attribute;
			ui32	count;
			ui32	type;
			bool	bNormalize;
			ui32	elementOffset;
		};
		typedef vector<VertexDeclaration>::type	VertexDeclarationList;

		// ������
		struct StreamUnit
		{
			VertexElementList		m_vertElements;		// �����ʽ����
			VertexDeclarationList	m_vertDeclaration;	// ��������
			ui32					m_vertStride;		// ÿ�����С
			GPUBuffer*				m_buffer;			// ��������

			StreamUnit()
				: m_buffer(NULL)
			{}

			bool isUsedFor(VertexSemantic);
		};

	public:
		GLES2Renderable(const String& renderStage, ShaderProgramRes* shader, int identifier);
		~GLES2Renderable();

		// �󶨼���������
		void bind( Renderable* pre);

		// ��󼸺�������
		void unbind();

	private:
		virtual void link() override;

		// bind vertex stream
		bool bindVertexStream(const VertexElementList& vertElements, GPUBuffer* vertexBuffer, int flag = BS_BEGIN | BS_END);

		// ���㶥��������
		virtual bool buildVertStreamDeclaration(StreamUnit* stream);

		// ���ɶ�����Hashֵ(BKDR Hash)
		virtual void generateVertexStreamHash();

	private:
		vector<StreamUnit>::type	m_vertexStreams;
		unsigned int				m_vertexStreamsHash;
		bool						m_is_muti_stream;
		GLuint						m_vao;					// ��������(OpenGLES 3.0)
	};
}
