#include "gltf_mesh.h"
#include "engine/core/util/LogManager.h"
#include "engine/core/scene/NodeTree.h"
#include "render/renderer.h"
#include "render/Material.h"
#include "engine/core/script/lua/luaex.h"
#include "engine/core/main/Root.h"
#include "engine/core/render/material/generalmaterial.h"

namespace Echo
{
	static void csmLogFunc(const char* message)
	{
		EchoLogError(message);
	}

	GltfMesh::GltfMesh()
		: m_textureRes("", ".png")
		, m_mesh(nullptr)
		, m_materialInst(nullptr)
		, m_renderable(nullptr)
		, m_meshIdx(-1)
		, m_primitiveIdx(-1)
	{
	}

	GltfMesh::~GltfMesh()
	{
		clear();
	}

	void GltfMesh::bindMethods()
	{
		CLASS_BIND_METHOD(GltfMesh, getTextureRes, DEF_METHOD("getTextureRes"));
		CLASS_BIND_METHOD(GltfMesh, setTextureRes, DEF_METHOD("setTextureRes"));

		CLASS_REGISTER_PROPERTY(GltfMesh, "Texture", Variant::Type_ResourcePath, "getTextureRes", "setTextureRes");
	}

	// set texture res path
	void GltfMesh::setTextureRes(const ResourcePath& path)
	{
		if (m_textureRes.setPath(path.getPath()))
		{
			clearRenderable();
			buildRenderable();
		}
	}

	// set geometry data
	void GltfMesh::setGeometryData(GltfRes* asset, int meshIdx, int primitiveIdx)
	{
		m_asset = asset;
		m_meshIdx = meshIdx;
		m_primitiveIdx = primitiveIdx;
		if (m_meshIdx != -1)
		{
			clearRenderable();
			buildRenderable();
		}
	}

	// build drawable
	void GltfMesh::buildRenderable()
	{
		if (!m_textureRes.getPath().empty())
		{
			// material
			m_materialInst = MaterialInst::create();
			m_materialInst->setOfficialMaterialContent( GeneralMaterial::getContent());
			m_materialInst->setRenderStage("Transparent");
			m_materialInst->applyLoadedData();

			m_materialInst->setTexture(0, m_textureRes.getPath());

			// mesh
			Mesh::VertexDefine define;
			VertexArray	vertices;
			IndiceArray	indices;
			buildMeshData( vertices, indices, &define);

			m_mesh = Mesh::create(true, true);
			m_mesh->set(define, vertices.size(), (const Byte*)vertices.data(), indices.size(), indices.data(), m_localAABB);

			m_renderable = Renderable::create(m_mesh, m_materialInst, this);
		}
	}

	// update per frame
	void GltfMesh::update()
	{
		if (m_renderable)
		{
			m_matWVP = getWorldMatrix() * NodeTree::instance()->get2DCamera()->getViewProjMatrix();;
			m_renderable->submitToRenderQueue();
		}
	}

	// build mesh data by drawables data
	void GltfMesh::buildMeshData(VertexArray& oVertices, IndiceArray& oIndices, Mesh::VertexDefine* attributes)
	{
		GltfMeshInfo& meshInfo = m_asset->m_meshes[m_meshIdx];
		GltfPrimitive& primitive = meshInfo.m_primitives[m_primitiveIdx];
		if (primitive.m_indices != -1)
		{
			// parse attributes
			if (attributes)
			{

			}

		}

		TextureRes*	texture = m_materialInst->getTexture(0);

		float hw = texture ? texture->getWidth() * 0.5f : 50.f;
		float hh = texture ? texture->getHeight() * 0.5f : 50.f;

		// vertices
		oVertices.push_back(VertexFormat(Vector3(-hw, -hh, 0.f), Vector2(0.f, 1.f)));
		oVertices.push_back(VertexFormat(Vector3(-hw, hh, 0.f), Vector2(0.f, 0.f)));
		oVertices.push_back(VertexFormat(Vector3(hw, hh, 0.f), Vector2(1.f, 0.f)));
		oVertices.push_back(VertexFormat(Vector3(hw, -hh, 0.f), Vector2(1.f, 1.f)));

		// indices
		oIndices.push_back(0);
		oIndices.push_back(1);
		oIndices.push_back(2);
		oIndices.push_back(0);
		oIndices.push_back(2);
		oIndices.push_back(3);
	}

	// update vertex buffer
	void GltfMesh::updateMeshBuffer()
	{
		VertexArray	vertices;
		IndiceArray	indices;
		buildMeshData( vertices, indices, nullptr);

		m_mesh->updateIndices(indices.size(), indices.data());
		m_mesh->updateVertexs(vertices.size(), (const Byte*)vertices.data(), m_localAABB);
	}

	// ��ȡȫ�ֱ���ֵ
	void* GltfMesh::getGlobalUniformValue(const String& name)
	{
		if (name == "matWVP")
			return (void*)(&m_matWVP);

		return nullptr;
	}

	void GltfMesh::clear()
	{
		clearRenderable();
	}

	void GltfMesh::clearRenderable()
	{
		EchoSafeRelease(m_renderable);
		EchoSafeRelease(m_materialInst);
		EchoSafeRelease(m_mesh);
	}
}