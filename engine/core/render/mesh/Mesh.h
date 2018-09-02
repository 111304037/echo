#pragma once

#include "engine/core/geom/AABB.h"
#include "MeshVertexData.h"

namespace Echo
{
	/**
	* Mesh 2013-11-6
	*/
	class GPUBuffer;
	class Mesh
	{
	public:
		enum TopologyType
		{
			// A list of points, 1 vertex per point
			TT_POINTLIST,
			// A list of lines, 2 vertices per line
			TT_LINELIST,
			// A strip of connected lines, 1 vertex per line plus 1 start vertex
			TT_LINESTRIP,
			// A list of triangles, 3 vertices per triangle
			TT_TRIANGLELIST,
			// A strip of triangles, 3 vertices for the first triangle, and 1 per triangle after that 
			TT_TRIANGLESTRIP,
		};

	public:
		// ����
		static Mesh* create(bool isDynamicVertexBuffer, bool isDynamicIndicesBuffer);

		// �ͷ�
		void release();

		// ��ȡ����
		const String& getName() const { return m_name; }

		// ͼԪ����
		TopologyType getTopologyType() { return m_topologyType; }

		// ��ȡ������Ϣ
		MeshVertexData& getVertexData() { return m_vertData; }

		// ��ȡ�����ʽ��С
		ui32 getVertexStride() const { return m_vertData.getVertexStride(); }

		// ��ȡ��������
		ui32 getVertexCount() const { return m_vertData.getVertexCount(); }

		// ��ȡ��������
		const MeshVertexData& getVertices() const { return m_vertData; }

		// ��ȡ���㻺��
		GPUBuffer* getVertexBuffer() const;

		// ��ȡ��������
		GPUBuffer* getIndexBuffer() const;

		// ��ȡ������
		ui32 getFaceCount() const;

		// ��ȡ��������
		ui32 getIndexCount() const;

		// ��ȡ������ʽ��С
		ui32 getIndexStride() const;

		// start vertex and index
		void setStartVertex(ui32 startVert) { m_startVert = startVert; }
		void setStartIndex(ui32 startIdx) { m_startIdx = startIdx; }
		ui32 getStartVertex() const { return m_startVert; }
		ui32 getStartIndex() const { return m_startIdx; }

		// ��ȡ��������
		Word* getIndices() const;

		// is valid
		bool isValid() const { return getFaceCount() > 0; }

		// �Ƿ�Ϊ��Ƥ
		bool isSkin() const { return isVertexUsage(VS_BLENDINDICES); }

		// �ж϶����ʽ���Ƿ���ָ�����͵�����
		bool isVertexUsage(VertexSemantic semantic) const { return m_vertData.isVertexUsage(semantic); }

		// ��ȡ�����ʽ
		const VertexElementList& getVertexElements() const;

		// ������������
		void generateTangentData(bool useNormalMap);

		// ��ȡ���ذ�Χ��
		const AABB& getLocalBox() const { return m_box; }

		// ��ȡ��Ӱ��Ĺ�������
		ui32 getBoneNum() const { return static_cast<ui32>(m_boneIdxs.size()); }

		// ��ȡ��Ӱ��Ĺ�������
		ui32 getBoneIdx(int idx) { return m_boneIdxs[idx]; }

		// set primitive type
		void setTopologyType(TopologyType type) { m_topologyType = type; }

		// update indices data
		void updateIndices(ui32 indicesCount, ui32 indicesStride, const void* indices);

		// update vertex data
		void updateVertexs(const MeshVertexFormat& format, ui32 vertCount, const Byte* vertices, const AABB& box);
		void updateVertexs(const MeshVertexData& vertexData, const AABB& box);

		// �������
		void clear();

	protected:
		Mesh(bool isDynamicVertexBuffer, bool isDynamicIndicesBuffer);
		~Mesh();

		// get memory usage
		ui32 getMemeoryUsage() const;

		// ������������
		void buildTangentData();

		// ������������������
		bool buildBuffer();

		// �½����㻺��
		void buildVertexBuffer();

		// ������������
		void buildIndexBuffer();

	protected:
		String						m_name;						// ����
		TopologyType				m_topologyType;				// ͼԪ����
		AABB						m_box;						// ��Χ��
		ui32						m_startVert;
		ui32						m_startIdx;
		ui32						m_idxCount;					// ��������
		ui32						m_idxStride;				// ������ʽ��С
		Byte*						m_indices;					// ��������
		MeshVertexData				m_vertData;					// ��������
		bool						m_isDynamicVertexBuffer;	// �Ƿ�֧�ֶ�̬����
		GPUBuffer*					m_vertexBuffer;				// ���㻺��
		bool						m_isDynamicIndicesBuffer;	// ��������
		GPUBuffer*					m_indexBuffer;				// ��������
		vector<ui32>::type			m_boneIdxs;					// ��������(mesh only use a part of bones of a skeleton)
	};
}

