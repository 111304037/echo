#pragma once

#include "engine/core/scene/render_node.h"
#include "engine/core/render/base/mesh/mesh.h"
#include "engine/core/render/base/shader/material.h"
#include "engine/core/render/base/proxy/render_proxy.h"
#include "engine/core/render/base/image/image.h"
#include "terrain_tile.h"

namespace Echo
{
    class Terrain : public Render
    {
        ECHO_CLASS(Terrain, Render)
        
    public:
        // Vertex Format
        struct VertexFormat
        {
            Vector3        m_position;
            Vector3        m_normal;
            Dword		   m_color;
            Vector2        m_uv;
            Dword          m_layerIndices;
            Vector4        m_layerWeights;
        };
        typedef vector<VertexFormat>::type  VertexArray;
        typedef vector<ui32>::type          IndiceArray;
        
    public:
        Terrain();
        virtual ~Terrain();
        
        // font res path
        void setDataPath(const ResourcePath& path);
        const ResourcePath& getDataPath() { return m_dataPath; }

        // set height
        void setHeight(i32 minX, i32 minY, i32 width, i32 height, vector<float>::type& heightData);
        
        // width height
        i32 getColumns() const { return m_columns; }
        i32 getRows() const { return m_rows; }

		// height range
		float getHeightRange() const { return m_heightRange; }
		void setHeightRange(float range);

		// grid spacing
		i32 getGridSpacing() const { return m_gridSpacing; }
		void setGridSpacing(i32 gridSpacing);
        
        // material
        Material* getMaterial() const { return m_material; }
        void setMaterial( Object* material);
        
        // get height
        float getHeight(i32 x, i32 z);
        
        // get normal
        Vector3 getNormal(i32 x, i32 z);

        // get weight
        float getWeight(i32 x, i32 z, i32 index);
        
    protected:
        // build drawable
        void buildRenderable();
        void clearRenderable();
        
        // update
        virtual void updateInternal(float elapsedTime) override;
        
        // update vertex buffer
        void updateMeshBuffer();
        
        // build mesh data by drawables data
        void buildMeshData(VertexArray& oVertices, IndiceArray& oIndices);
        
        // clear
        void clear();
        
    private:
        bool                    m_isRenderableDirty = true;
        ResourcePath            m_dataPath = ResourcePath("", "");
        ImagePtr                m_heightmapImage = nullptr;
        vector<Image*>::type    m_layerImages;
		float					m_heightRange = 256.f;
		i32						m_gridSpacing = 1;
        MeshPtr                 m_mesh;
        MaterialPtr             m_material;
        RenderProxyPtr          m_renderable = nullptr;
        i32                     m_columns = 0;
        i32                     m_rows = 0;
        vector<float>::type     m_heightData;
		//TerrainTiles			m_tiles;
    };
}
