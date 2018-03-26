#pragma once

#include "engine/core/geom/Box.h"
#include "VertexData.h"


namespace Echo
{
	/**
	* Mesh 2013-11-6
	*/
	class Mesh
	{
	public:
		// Vertex Define
		struct VertexDefine
		{
			bool	m_isUseNormal;
			bool	m_isUseVertexColor;
			bool	m_isUseDiffuseUV;

			VertexDefine()
				: m_isUseNormal(false), m_isUseVertexColor(false), m_isUseDiffuseUV(false)
			{}
		};

	public:
		// ����
		static Mesh* create();

		// �ͷ�
		void release();

		// ��ȡ����
		const String& getName() const { return m_name; }

		// ��ȡ������Ϣ
		VertexData& getVertexData() { return m_vertData; }

		// ��ȡ�����ʽ��С
		ui32 getVertexStride() const { return m_vertData.m_stride; }

		// ��ȡ��������
		ui32 getVertexCount() const { return m_vertData.m_count; }

		// ��ȡ��������
		Byte* getVertices() const { return m_vertData.m_vertices; }

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

		// ��������
		void set(const VertexDefine& format,ui32 vertCount, const Byte* vertices, ui32 indicesCount, const ui16* indices, const Box& box);

		// �������
		void clear();

	protected:
		Mesh();
		~Mesh();

		// ��������
		void insertVertexData(const RenderInput::VertexElement& element, void* templateData);

		// �Ƴ�����
		void removeVertexData(RenderInput::VertexSemantic semantic);

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
		String					m_name;					// ����
		Box						m_box;					// ��Χ��
		ui32					m_idxCount;				// ��������
		ui32					m_idxStride;			// ������ʽ��С
		Byte*					m_indices;				// ��������
		VertexData				m_vertData;				// ��������
		GPUBuffer*				m_vertexBuffer;			// ���㻺��
		GPUBuffer*				m_indexBuffer;			// ��������
		vector<ui32>::type		m_boneIdxs;				// ��������(mesh only use a part of bones of a skeleton)
	};
}

