#pragma once

#include <Foundation/Math/Matrix4.h>
#include <Render/GPUBuffer.h>
#include <Render/RenderInput.h>
#include <Render/Renderable.h>
#include <Engine/Render/TextureRes.h>
#include "spine/AtlasAttachmentLoader.h"

extern "C"
{
	typedef struct EchoAttachmentLoader 
	{
		spAttachmentLoader super;
		spAtlasAttachmentLoader* atlasAttachmentLoader;
	} EchoAttachmentLoader;

	/* The EchoAttachmentLoader must not be disposed until after the skeleton data has been disposed. */
	EchoAttachmentLoader* EchoAttachmentLoader_create(spAtlas* atlas);
}

namespace Spine
{
	/**
	 * �����ʽ����
	 */
	struct VertexFormat
	{
		ECHO::Vector3	m_position;		// λ��
		ECHO::ui32		m_diffuse;		// ��ɫֵ
		ECHO::Vector2	m_uv;			// UV����
	};

	/**
	 * Spine AttachmentVertices
	 */
	class AttachmentVertices 
	{
	public:
		AttachmentVertices( ECHO::TextureRes* texture, int verticesCount, ECHO::ui16* triangles, int indicesCount);
		virtual ~AttachmentVertices();

		// �����ڴ����ݵ�GPU
		void submitToRenderQueue( const ECHO::Matrix4& matWVP);

	private:
		// ��ʼ��
		void init();

	public:
		ECHO::TextureRes*						m_texture;				// ����
		ECHO::vector<VertexFormat>::type		m_verticesData;			// ��������
		ECHO::vector<ECHO::ui16>::type			m_indicesData;			// ��������
		ECHO::GPUBuffer*						m_vertexBuffer;			// ������(��ʾ���Դ������)
		ECHO::GPUBuffer*						m_indexBuffer;			// ������
		ECHO::RenderInput*						m_renderInput;			// ����
		ECHO::RenderInput::VertexElementList	m_verElementLists;		// �����ʽ����
		ECHO::Renderable*						m_renderable;			// ��Ⱦ����
		ECHO::Matrix4							m_matWVP;				// ����۲�ͶӰ����
	};
}