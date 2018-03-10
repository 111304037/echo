#include "Render/RenderInput.h"
#include "engine/core/Util/Exception.h"
#include "Render/Renderer.h"
#include "Render/ShaderProgram.h"

namespace Echo
{
	// ���캯��
	RenderInput::RenderInput( ShaderProgram* pProgram)
		: m_topologyType(TT_TRIANGLELIST)
		, m_startVert(0)
		, m_baseVert(0)
		, m_program( pProgram)
		, m_startIdx(0)
		, m_vertCount(0)
		, m_pIdxBuff(NULL)
		, m_idxStride(0)
		, m_idxCount(0)
		, m_supportWireframe(false)
		, m_vertexStreamsHash( 0)
		, m_is_muti_stream(false)
	{
		if(!pProgram)
			EchoException("Shader program couldn't be null, create RenderInput failed.");

		if (!pProgram->isLinked())
		{
			String vsName = pProgram->getShader(Shader::ST_VERTEXSHADER)->getName(); 
			String psName = pProgram->getShader(Shader::ST_PIXELSHADER)->getName(); 

			EchoException("The shader program [%s][%s] has not been linked, create RenderInput failed.", vsName.c_str(), psName.c_str());
		}
	}

	// ��������
	RenderInput::~RenderInput()
	{}

	// �󶨶�����
	bool RenderInput::bindVertexStream(const VertexElementList& vertElements, GPUBuffer* vertexBuffer, int flag)
	{
		if (flag & BS_BEGIN)
			m_vertexStreams.clear();

		StreamUnit unit;
		unit.m_vertElements = vertElements;
		unit.m_buffer	    = vertexBuffer;
		buildVertStreamDeclaration(&unit);

		m_vertexStreams.push_back(unit);

		// ����Hashֵ
		if ( flag & BS_END)
			generateVertexStreamHash();

		return true;
	}

	// ��������
	bool RenderInput::bindIndexStream( GPUBuffer* indexStream, ui32 idxStride)
	{
		m_pIdxBuff = indexStream;
		m_idxStride= idxStride;

		return true;
	}

	// ��󶨵�����������
	void RenderInput::unbindGPUBuffers()
	{
		unbind();

		m_vertexStreams.clear();
		m_vertCount = 0;
		m_pIdxBuff = NULL;
		m_idxStride = 0;
		m_idxCount = 0;
	}
}