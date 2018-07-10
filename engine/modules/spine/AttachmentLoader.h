#pragma once

#include <spine/spine.h>
#include <Render/GPUBuffer.h>
#include <Render/RenderInput.h>
#include <Render/Renderable.h>
#include "engine/core/render/TextureRes.h"

extern "C"
{
	typedef struct EchoAttachmentLoader
	{
		spAttachmentLoader		 super;
		spAtlasAttachmentLoader* atlasAttachmentLoader;
	} EchoAttachmentLoader;

	/* The EchoAttachmentLoader must not be disposed until after the skeleton data has been disposed. */
	EchoAttachmentLoader* EchoAttachmentLoader_create(spAtlas* atlas);
}

namespace Echo
{
	struct SpineVertexFormat
	{
		Vector3	m_position;		// λ��
		ui32	m_diffuse;		// ��ɫֵ
		Vector2	m_uv;			// UV����
	};

	/**
	* Spine AttachmentVertices
	*/
	class AttachmentVertices
	{
	public:
		AttachmentVertices(TextureRes* texture, int verticesCount, ui16* triangles, int indicesCount);
		virtual ~AttachmentVertices();

		// render
		void submitToRenderQueue(Node* node);

	public:
		TextureResPtr					m_texture;			// ����
		vector<SpineVertexFormat>::type	m_verticesData;		// ��������
		vector<ui16>::type				m_indicesData;		// ��������
		Matrix4							m_matWVP;			// ����۲�ͶӰ����
		Mesh*							m_mesh;				// Geometry Data for render
		Material*						m_material;			// Material
		Renderable*						m_renderable;
	};
}
