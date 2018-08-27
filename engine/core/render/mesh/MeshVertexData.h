#pragma once

#include "core/geom/AABB.h"
#include "core/render/render/PixelFormat.h"

namespace Echo
{
	enum VertexSemantic
	{
		VS_UNKNOWN = -1,
		// Position (Vector3)
		VS_POSITION,
		// Normal (Vector3)
		VS_NORMAL,
		// Diffuse & specular color (Dword)
		VS_COLOR,
		// Texture coordinates 0(Vector2)
		VS_TEXCOORD0,
		// Texture coordinates 1(Vector2)
		VS_TEXCOORD1,
		// Blending indices	(Dword)
		VS_BLENDINDICES,
		// Blending weights(Vector3)
		VS_BLENDWEIGHTS,
		// Tangent (X axis if normal is Z)
		VS_TANGENT,
		VS_BINORMAL,

		// VR Mode
		VS_SCREEN_POS_NDC,
		VS_TAN_EYE_ANGLE_R,
		VS_TAN_EYE_ANGLE_G,
		VS_TAN_EYE_ANGLE_B,

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
		typedef VertexElementList RIVEL;

		bool		m_isUseNormal;			// �Ƿ�ʹ�÷���
		bool		m_isUseVertexColor;		// �Ƿ�ʹ�ö���ɫ
		bool		m_isUseUV;				// �Ƿ�ʹ����������ͼUV 
		bool		m_isUseLightmapUV;		// �Ƿ�ʹ�ù���ͼUV
		bool		m_isUseBoneData;		// �Ƿ�ʹ�ù�������(����Ȩ��)
		bool		m_isUseTangentBinormal;	// �Ƿ�ʹ�������븱��
		ui32		m_stride;				// �����ʽ��С
		Byte		m_posOffset;			// ����λ������ƫ����
		Byte		m_normalOffset;			// ���㷨������ƫ����
		Byte		m_colorOffset;			// ������ɫ����ƫ����
		Byte		m_uv0Offset;			// ������������0ƫ����
		Byte		m_uv1Offset;			// ������������1ƫ����
		Byte		m_boneIndicesOffset;	// �����������ƫ����
		Byte		m_boneWeightsOffset;	// �������Ȩ��ƫ����
		Byte		m_tangentOffset;		// ��������ƫ����
		RIVEL		m_vertexElements;		// �����ʽ

		MeshVertexFormat();

		// build
		void build();

		// �ж϶����ʽ���Ƿ���ָ�����͵�����
		bool isVertexUsage(VertexSemantic semantic) const;

		// reset
		void reset();
	};

	// ��������
	class MeshVertexData
	{
	public:
		typedef vector<Byte>::type ByteArray;

		// ���캯��
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

		// ��ȡ����UV����0
		const Vector2& getUV0(Word index);
		void setUV0(int idx, const Vector2& uv0);

		// ��ȡ����UV����1
		const Vector2& getUV1(Word index);

		// ��ȡ����
		Vector3& getTangent(Word index);

		// ��ȡ��Χ��
		const AABB& getAABB() const { return m_aabb; }

		//״̬�ÿ�
		void reset();

	private:
		ui32				m_count;		// ��������
		MeshVertexFormat	m_format;		// �����ʽ
		ByteArray			m_vertices;		// ��������
		AABB					m_aabb;			// �����Χ��
	};
}
