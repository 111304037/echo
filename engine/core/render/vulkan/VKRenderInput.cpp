#include "GLES2RenderStd.h"
#include "GLES2Renderer.h"
#include "GLES2RenderInput.h"
#include "GLES2ShaderProgram.h"
#include "GLES2Mapping.h"
#include "GLES2GPUBuffer.h"
#include "GLES2Loader.h"
#include "GLES2RendererProxy.h"
#include <Foundation/Util/AssertX.h>
#include <Foundation/Util/Exception.h>
#include "Render/PixelFormat.h"
#include "Render/RenderThread.h"
#include "Render/RenderTask.h"
#include "GLES2RendererTasks.h"


namespace LORD
{
	extern GLES2Renderer* g_renderer;

	// ���캯��
	GLES2RenderInput::GLES2RenderInput( ShaderProgram* pProgram)
		: RenderInput( pProgram)
		, m_vao( -1)
	{
	}
	
	// ��������
	GLES2RenderInput::~GLES2RenderInput()
	{
	}
	
	// �󶨵�GPU
	void GLES2RenderInput::bind( RenderInput* pre)
	{
		// 1.ȷ���������������Ƿ���Ҫ����
		bool isNeedSetVertexBuffer;
		bool isNeedSetIdxBuffer;
		if (!pre)
		{
			isNeedSetVertexBuffer = true;
			isNeedSetIdxBuffer = m_pIdxBuff ? true : false;
		}
		else
		{
#ifdef LORD_PLATFORM_MAC_IOS
			isNeedSetVertexBuffer = pre->getVertexStreamHash() != m_vertexStreamsHash ? true : false;
#else
			isNeedSetVertexBuffer = true;
#endif
			isNeedSetIdxBuffer = (!m_pIdxBuff || pre->getIndexBuffer() == m_pIdxBuff) ? false : true;
		}

		// �󶨶�����
		if (isNeedSetVertexBuffer)
		{
			GLES2ShaderProgram* pGLES2Program = (GLES2ShaderProgram*)m_program;
			for (int i = (int)(m_vertexStreams.size() - 1); i >= 0; i--)
			{
				const StreamUnit& streamUnit = m_vertexStreams[i];

				((GLES2GPUBuffer*)streamUnit.m_buffer)->bindBuffer();

				size_t declarationSize = streamUnit.m_vertDeclaration.size();
				for (size_t i = 0; i < declarationSize; ++i)
				{
					const VertexDeclaration& declaration = streamUnit.m_vertDeclaration[i];
					if (declaration.m_attribute != -1)
					{
						// Enable the vertex array attributes.

						TRenderTask<GLES2RenderTaskVertexAttribPointer>::CreateTask(declaration.m_attribute, declaration.count, declaration.type, declaration.bNormalize, streamUnit.m_vertStride, declaration.elementOffset);

						g_renderer->enableAttribLocation(declaration.m_attribute);
					}
				}
			}
		}

		// ��������
		if ( isNeedSetIdxBuffer)
		{
			// Bind the index buffer and load the index data into it.
			((GLES2GPUBuffer*)m_pIdxBuff)->bindBuffer();
		}
	}
	
	// ȥ����
	void GLES2RenderInput::unbind()
	{
#ifndef LORD_PLATFORM_MAC_IOS
		// �󶨶�����
  		for (size_t i = 0; i < m_vertexStreams.size(); i++)
  		{
  			const StreamUnit& streamUnit = m_vertexStreams[i];
  			size_t declarationSize = streamUnit.m_vertDeclaration.size();
  			for (size_t i = 0; i < declarationSize; ++i)
  			{
  				const VertexDeclaration& declaration = streamUnit.m_vertDeclaration[i];
 				if (declaration.m_attribute != -1)
 				{
 					// Enable the vertex array attributes.
					g_renderer->disableAttribLocation(declaration.m_attribute);
 				}
  			}
  		}
#endif

		//OGLESDebug(glBindBuffer(GL_ARRAY_BUFFER, 0));

		//if(m_pIdxBuff)
		//	OGLESDebug(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}


	// ���㶥��������
	bool GLES2RenderInput::buildVertStreamDeclaration(StreamUnit* stream)
	{
		uint numVertElms = stream->m_vertElements.size();
		if (numVertElms == 0)
		{
			LordLogError("Vertex elements size error, buildVertStreamDeclaration failed.");
			return false;
		}
			
		stream->m_vertDeclaration.reserve(numVertElms);
		stream->m_vertDeclaration.resize(numVertElms);

		uint elmOffset = 0;
		for (size_t i = 0; i < numVertElms; ++i)
		{
			stream->m_vertDeclaration[i].m_attribute = stream->m_vertElements[i].m_semantic;
			stream->m_vertDeclaration[i].count = PixelUtil::GetChannelCount(stream->m_vertElements[i].m_pixFmt);
			stream->m_vertDeclaration[i].type = GLES2Mapping::MapDataType(stream->m_vertElements[i].m_pixFmt);
			stream->m_vertDeclaration[i].bNormalize = PixelUtil::IsNormalized(stream->m_vertElements[i].m_pixFmt);
			stream->m_vertDeclaration[i].elementOffset = elmOffset;
			elmOffset += PixelUtil::GetPixelSize(stream->m_vertElements[i].m_pixFmt);
		}

		stream->m_vertStride = elmOffset;

		return true;
	}

	// ���ɶ�����Hashֵ(BKDR Hash)
	void GLES2RenderInput::generateVertexStreamHash()
	{
		unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
		unsigned int hash = 0;
		for (int i = (int)(m_vertexStreams.size() - 1); i >= 0; i--)
		{
			const StreamUnit& streamUnit = m_vertexStreams[i];

			hash = hash * seed + (size_t)(streamUnit.m_buffer);

			size_t declarationSize = streamUnit.m_vertDeclaration.size();
			for (size_t i = 0; i < declarationSize; ++i)
			{
				const VertexDeclaration& declaration = streamUnit.m_vertDeclaration[i];
				if (declaration.m_attribute != -1)
				{
					// Enable the vertex array attributes.
					hash = hash * seed + declaration.m_attribute;
					hash = hash * seed + declaration.count;
					hash = hash * seed + declaration.type;
					hash = hash * seed + declaration.bNormalize;
					hash = hash * seed + streamUnit.m_vertStride;
					hash = hash * seed + declaration.elementOffset;
				}
			}
		}

		m_vertexStreamsHash = (hash & 0x7FFFFFFF);
	}
}
