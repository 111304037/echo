#pragma once

#include "engine/core/geom/AABB.h"
#include "engine/core/render/base/image/PixelFormat.h"

namespace Echo
{
	enum VertexSemantic
	{
		VS_UNKNOWN = -1,
		VS_POSITION,                // Position (Vector3)
		VS_NORMAL,                  // Normal (Vector3)
		VS_COLOR,                   // Diffuse & specular color (Dword)
		VS_TEXCOORD0,               // Texture coordinates 0(Vector2)
		VS_TEXCOORD1,               // Texture coordinates 1(Vector2)
		VS_BLENDINDICES,            // Blending indices    (Dword)
		VS_BLENDWEIGHTS,            // Blending weights(Vector4)
		VS_TANGENT,                 // Tangent (X axis if normal is Z)
		VS_BINORMAL,
		VS_MAX
	};

	struct VertexElement
	{
		VertexSemantic		m_semantic;		// Vertex Semantic
		PixelFormat			m_pixFmt;		// Vertex pixel format

		VertexElement(VertexSemantic semantic = VS_UNKNOWN, PixelFormat pixFmt = PF_UNKNOWN)
			: m_semantic(semantic)
			, m_pixFmt(pixFmt)
		{}
	};
	typedef vector<VertexElement>::type	VertexElementList;

	struct MeshVertexFormat
	{
		bool		        m_isUseNormal;
		bool		        m_isUseVertexColor;
		bool		        m_isUseUV;
		bool		        m_isUseLightmapUV;
		bool		        m_isUseBoneData;
		bool		        m_isUseTangentBinormal;
		ui32		        m_stride;
		Byte		        m_posOffset;
		Byte		        m_normalOffset;
		Byte		        m_colorOffset;
		Byte		        m_uv0Offset;
		Byte		        m_uv1Offset;
		Byte		        m_boneIndicesOffset;
		Byte		        m_boneWeightsOffset;
		Byte		        m_tangentOffset;
		VertexElementList	m_vertexElements;

		MeshVertexFormat();

		// Build
		void build();

		// Is used
		bool isVertexUsage(VertexSemantic semantic) const;

		// reset
		void reset();
	};

	// vertex data for mesh
	class MeshVertexData
	{
    public:
        typedef vector<Byte>::type ByteArray;
        
	public:
		MeshVertexData();

		// set
		void set(const MeshVertexFormat& format, ui32 count);

		// get format
		const MeshVertexFormat& getFormat() const { return m_format; }

		// vertex info
		ui32 getVertexStride() const;
		ui32 getVertexCount() const;
		ui32 getByteSize() const { return m_count * m_format.m_stride; }

		// ��ȡ��������
		Byte* getVertices();

		// ��ȡ�����ַ
		Byte* getVertice(int idx);

		// �ж϶����ʽ���Ƿ���ָ�����͵�����
		bool isVertexUsage(VertexSemantic semantic) const;

		// ��ȡ����λ������
		Vector3& getPosition(Word index);
		void setPosition(int idx, const Vector3& pos);

		// ��ȡ���㷨������
		const Vector3& getNormal(Word index);
		void setNormal(int idx, const Vector3& normal);

		// ��ȡ������ɫ����
		Dword& getColor(Word index);
		void setColor(i32 idx, Dword color);

		// ��ȡ����UV����0
		const Vector2& getUV0(Word index);
		void setUV0(int idx, const Vector2& uv0);

		// ��ȡ����UV����1
		const Vector2& getUV1(Word index);

		 // set skin weight
		void setJoint(int idx, Dword weight);

		// set skin joint
		void setWeight(int idx, const Vector4& joint);

		// ��ȡ����
		Vector3& getTangent(Word index);

		// ��ȡ��Χ��
		const AABB& getAABB() const { return m_aabb; }

		//״̬�ÿ�
		void reset();

	private:
		ui32				m_count;
		MeshVertexFormat	m_format;
		ByteArray			m_vertices;
		AABB				m_aabb;
	};
}
