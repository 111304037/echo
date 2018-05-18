#pragma once

#include "engine/core/geom/Box.h"
#include "MeshVertexData.h"

namespace Echo
{
	/**
	* Mesh 2013-11-6
	*/
	class Mesh
	{
	public:
		// ����
		static Mesh* create(bool isDynamicVertexBuffer, bool isDynamicIndicesBuffer);

		// �ͷ�
		void release();

		// ��ȡ����
		const String& getName() const { return m_name; }

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

		// ��ȡ��������
		Word* getIndices() const;

		// �Ƿ�Ϊ��Ƥ
		bool isSkin() const { return isVertexUsage(RenderInput::VS_BLENDINDICES); }

		// �ж϶����ʽ���Ƿ���ָ�����͵�����
		bool isVertexUsage(RenderInput::VertexSemantic semantic) const { return m_vertData.isVertexUsage(semantic); }

		// ��ȡ�����ʽ
		const RenderInput::VertexElementList& getVertexElements() const;

		// ������������
		void generateTangentData(bool useNormalMap);

		// ��ȡ���ذ�Χ��
		const Box& getLocalBox() const { return m_box; }

		// ��ȡ��Ӱ��Ĺ�������
		ui32 getBoneNum() const { return static_cast<ui32>(m_boneIdxs.size()); }

		// ��ȡ��Ӱ��Ĺ�������
		ui32 getBoneIdx(int idx) { return m_boneIdxs[idx]; }

		// update indices data
		void updateIndices(ui32 indicesCount, const ui16* indices);
		void updateIndices(ui32 indicesCount, const ui32* indices);

		// update vertex data
		void updateVertexs(const MeshVertexFormat& format, ui32 vertCount, const Byte* vertices, const Box& box);
		void updateVertexs(const MeshVertexData& vertexData, const Box& box);

		// �������
		void clear();

	protected:
		Mesh(bool isDynamicVertexBuffer, bool isDynamicIndicesBuffer);
		~Mesh();

		// ��ȡ�ڴ�ռ�ô�С
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
		String					m_name;						// ����
		Box						m_box;						// ��Χ��
		ui32					m_idxCount;					// ��������
		ui32					m_idxStride;				// ������ʽ��С
		Byte*					m_indices;					// ��������
		MeshVertexData			m_vertData;					// ��������
		bool					m_isDynamicVertexBuffer;	// �Ƿ�֧�ֶ�̬����
		GPUBuffer*				m_vertexBuffer;				// ���㻺��
		bool					m_isDynamicIndicesBuffer;	// ��������
		GPUBuffer*				m_indexBuffer;				// ��������
		vector<ui32>::type		m_boneIdxs;					// ��������(mesh only use a part of bones of a skeleton)
	};
}

